/**
 *  Customization
 */
// #define BUFFER_SIZE 100

//////////////////////////////////////////////////////////////////////
/**
 * Don't change anything below if you don't understand.
 */
#include "net.h"

#ifdef BUFFER_SIZE
dType fmCache[2][BUFFER_SIZE];
#define readFmBuffer fmCache[phase == false]; // phase should start with false
#define writeFmBuffer fmCache[phase == true];
#else
#define readFmBuffer inputFeatureMap
#define writeFmBuffer outputFeatureMap
#endif

/**
 * [EASY_WORK_ITEM_3D_OUTPUT_BEGIN
 * Utility to write simple 3D parrallel code for opencl kernel to process 3d
 * feature map, BEGIN
 * Assume to use param, GLOBAL_ID.
 * Parallel code block start, **everything in between will execute per work item
 * by <channelCounter, heightCounter, widthCounter>**]
 * @param  channelCounter [current work item channel id that will be used in
 * code block inside]
 * @param  heightCounter  [current work item height id that will be used in code
 * block inside]
 * @param  widthCounter   [current work item width id that will be used in code
 * block inside]
 * @return                [code generated]
 */
#define EASY_WORK_ITEM_3D_OUTPUT_BEGIN(channelCounter, heightCounter,          \
                                       widthCounter)                           \
  WORK_ITEM_3D_BEGIN(channelCounter, param->outputChannel, GLOBAL_ID_0,        \
                     heightCounter, param->outputHeight, GLOBAL_SIZE_1,        \
                     widthCounter, param->outputWidth, GLOBAL_ID_2)

/**
 * [EASY_WORK_ITEM_3D_OUTPUT_END
 *  Utility to write simple 3D parrallel code for opencl kernel to process 3d
 * feature map, END
 *  Assume to use param, GLOBAL_ID.
 *  Parallel code block end, **everything in between will execute per work item
 * by <channelCounter, heightCounter, widthCounter>**]
 * @param  channelCounter [current work item channel id that will be used in
 * code block inside]
 * @param  heightCounter  [current work item height id that will be used in code
 * block inside]
 * @param  widthCounter   [current work item width id that will be used in code
 * block inside]
 * @return                [code generated]
 */
#define EASY_WORK_ITEM_3D_OUTPUT_END(channelCounter, heightCounter,            \
                                     widthCounter)                             \
  WORK_ITEM_3D_END(channelCounter, GLOBAL_SIZE_0, heightCounter,               \
                   GLOBAL_SIZE_1, widthCounter, GLOBAL_SIZE_2)

/**
 * shorter macro for output of feature map in parallel code block.
 */
#define OUTPUT_ELM                                                             \
  ELM(writeFmBuffer, channelCounter, param->outputHeight, heightCounter,       \
      param->outputWidth, widthCounter)

/**
 * [dataLayer
 *  data layer to transfer data from input feature map to (output|cache), with
 * scale]
 * @param inputFeatureMap  [input feature map]
 * @param outputFeatureMap [output feature map (won't use if in FPGA optimized
 * setting)]
 * @param param            [network parameter]
 * @param phase            [phase id, for ping pong buffer switching]
 */
__kernel void dataLayer(__global dType *inputFeatureMap,
                        __global dType *outputFeatureMap,
                        __global NetParam *param, bool phase) {
  LOAD_DATA_SCALE(inputFeatureMap, writeFmBuffer, param->inputTotalDataNum,
                  GLOBAL_ID, GLOBAL_SIZE, param->scale);
}

/**
 * [paddingLayer pad the feature map, used param.{pad} before convolution layer
 * and pooling layer]
 * @param inputFeatureMap  [input feature map]
 * @param outputFeatureMap [output feature map (won't use if in FPGA optimized
 * setting)]
 * @param param            [network parameter]
 * @param phase            [phase id, for ping pong buffer switching]
 */
__kernel void paddingLayer(__global dType *inputFeatureMap,
                           __global dType *outputFeatureMap,
                           __global NetParam *param, bool phase) {
  LOAD_DATA_PAD(readFmBuffer, writeFmBuffer, param->inputChannel, GLOBAL_SIZE_0,
                GLOBAL_ID_0, param->inputHeight, GLOBAL_SIZE_1, GLOBAL_ID_1,
                param->inputWidth, GLOBAL_SIZE_2, GLOBAL_ID_2, param->pad);
}

/**
 * [poolingLayer pooling layer, use param.{stride, kernelSize}]
 * @param inputFeatureMap  [input feature map]
 * @param outputFeatureMap [output feature map (won't use if in FPGA optimized
 * setting)]
 * @param param            [network parameter]
 * @param phase            [phase id, for ping pong buffer switching]
 */
__kernel void poolingLayer(__global dType *inputFeatureMap,
                           __global dType *outputFeatureMap,
                           __global NetParam *param, bool phase) {
  __private dType maxValue = MIN_NUM, _temp;
  EASY_WORK_ITEM_3D_OUTPUT_BEGIN(channelCounter, heightCounter, widthCounter);
  for (int i = 0; i < param->kernelSize; i++)
    for (int j = 0; j < param->kernelSize; j++) {
      _temp = ELM(readFmBuffer, channelCounter, param->inputHeight,
                  heightCounter * param->stride + i, param->inputWidth,
                  widthCounter * param->stride + j);
      maxValue = maxValue > _temp ? maxValue : _temp;
    }
  OUTPUT_ELM = maxValue;
  EASY_WORK_ITEM_3D_OUTPUT_END(channelCounter, heightCounter, widthCounter);
}

/**
 * [reluLayer relu layer, give all data relu activation]
 * @param inputFeatureMap  [input feature map]
 * @param outputFeatureMap [output feature map (won't use if in FPGA optimized
 * setting)]
 * @param param            [network parameter]
 * @param phase            [phase id, for ping pong buffer switching]
 */
__kernel void reluLayer(__global dType *inputFeatureMap,
                        __global dType *outputFeatureMap,
                        __global NetParam *param, bool phase) {
  WORK_ITEM_BEGIN(reluCounter, param->inputTotalDataNum, GLOBAL_ID)
  writeFmBuffer[reluCounter] = RELU(readFmBuffer[reluCounter]);
  WORK_ITEM_END(reluCounter, GLOBAL_SIZE)
}

/**
 * [convLayer conv layer, process all data in inputFeatureMap or inner cache]
 * @param inputFeatureMap  [input feature map]
 * @param outputFeatureMap [output feature map (won't use if in FPGA optimized
 * setting)]
 * @param weight           [weight memory]
 * @param bias             [bias memory]
 * @param param            [network parameter]
 * @param phase            [phase id, for ping pong buffer switching]
 */
__kernel void convLayer(__global dType *inputFeatureMap,
                        __global dType *outputFeatureMap,
                        __global dType *weight, __global dType *bias,
                        __global const NetParam *param, bool phase) {

  __private int dilatedKernelSize =
      (param->kernelSize - 1) * param->dilation + 1;
  __private dType result = 0;
  EASY_WORK_ITEM_3D_OUTPUT_BEGIN(channelCounter, heightCounter, widthCounter);
  for (int c = 0; c < param->inputChannel; c++)
    for (int i = 0; i < param->kernelSize; i++)
      for (int j = 0; j < param->kernelSize; j++) {
        result += (ELM(readFmBuffer, c, param->inputHeight,
                       heightCounter * param->stride + i * param->dilation,
                       param->inputWidth,
                       widthCounter * param->stride + j * param->dilation) *
                   WELM(weight, channelCounter, param->outputChannel, c,
                        param->kernelSize, i, j));
      }

  result += bias[channelCounter];
  OUTPUT_ELM = result;
  EASY_WORK_ITEM_3D_OUTPUT_END(channelCounter, heightCounter, widthCounter);
}


/**
 * [output layer
 *  layer to copy data from (input|cache) to output]
 * @param inputFeatureMap  [input feature map]
 * @param outputFeatureMap [output feature map (won't use if in FPGA optimized
 * setting)]
 * @param param            [network parameter]
 * @param phase            [phase id, for ping pong buffer switching]
 */
__kernel void outputLayer(__global dType *inputFeatureMap,
                          __global dType *outputFeatureMap,
                          __global NetParam *param, bool phase) {
  LOAD_DATA(inputFeatureMap, writeFmBuffer, param->inputTotalDataNum, GLOBAL_ID,
            GLOBAL_SIZE);
}


/**
 * This code should be finished like in one hour.
 * I could even skip the python code, and go direct with c++ code.
 * cpp code shoud implement this
 *  - every layer has a output buffer, and next pointer and a enqueue method.
 *  - net has a forward method
 *      - which enqueue all
 *      - and clFinish in FPGA case
 *      - or alternately in other case
 *      - or three mode
 *
 * Since we have padding layer, the input/output shape should be carefully
 * designed.
 */
