#include "helper.h"
#include "custom.h"
#include "net_enum.h"
#include <iomanip>

#ifndef OPENCL_LEARNING_NETWORK_H
#define OPENCL_LEARNING_NETWORK_H


#define FORWARD_ERROR_CHECK if(err != CL_SUCCESS) {std::cout << clErrorCode(err) << "\n"; exit(1);};

#define CL_RELEASE(x)                                                          \
  if (x != NULL)                                                               \
    err = clReleaseMemObject(x);                                               \
  x = NULL;

#define CL_KERNEL_ARG(kernel, size, mem)                                       \
  err = clSetKernelArg(kernel, argCounter, size, mem);                         \
  argCounter++;                                                                \
  FORWARD_ERROR_CHECK;

#define CL_CREATE_BUFFER(...) clCreateBuffer(__VA_ARGS__);

#define CL_ENQUEUE_WRITE_BUFFER(...)                                           \
  err = clEnqueueWriteBuffer(__VA_ARGS__);                                     \
  FORWARD_ERROR_CHECK;

#define CL_ENQUEUE_READ_BUFFER(...)                                            \
  err = clEnqueueReadBuffer(__VA_ARGS__);                                      \
  FORWARD_ERROR_CHECK;

#define CL_FINISH(...)                                                         \
  err = clFinish(__VA_ARGS__);                                                 \
  FORWARD_ERROR_CHECK;

#define KERNEL_ENQUEUE(...)                                                    \
  err = clEnqueueNDRangeKernel(__VA_ARGS__);                                   \
  FORWARD_ERROR_CHECK;

#define SET_3D(array, x, y, z)                                                 \
  array[0] = (x);                                                              \
  array[1] = (y);                                                              \
  array[2] = (z);
#define SET_CL_3D_SIZE(data, offset, key)                                      \
  SET_3D(offset, data[key][0].asUInt(), data[key][1].asUInt(),                 \
         data[key][2].asUInt());



typedef struct {
    dType scale;
    int stride;
    int kernelSize;
    int pad;
    int dilation;
    int inputChannel;
    int inputHeight;
    int inputWidth;
    int outputChannel;
    int outputHeight;
    int outputWidth;
    int inputTotalDataNum;
    int outputTotalDataNum;
} NetParam;


typedef struct{
    dType *weight;
    dType *bias;
    int *weightShape; // (outputChannel, inputChannel, kernelSize, kernelSize)
    int *biasShape; // (outputChannel)
    long weight_data_num;
    long bias_data_num;
    int weight_dim_num;
    int bias_dim_num;
} WeightData;


class Layer {
public:
    ~Layer();
    Layer(Json::Value);
    bool forward(oclHardware hardware, oclSoftware software, OpenCLVersion mode, NetLogging log);
    bool freeCLMemory();
    cl_mem weightCL;
    cl_mem biasCL;
    cl_mem outputBufferCL;
    cl_mem inputBufferCL;
    cl_mem paramCL;
    cl_mem phaseCL;
    size_t globalSize[3], localSize[3], offset[3];
    LayerType type;
    string kernelKey;
    WeightData learnedParam;
    NetParam param;
    Layer* next;
    int phase;
    Layer* prev;
    dType *inputBuffer;
    dType *outputBuffer;
    std::string info;
};


class Net{
public:
    Layer **layers;
    int num_layers;
    string name;
    OpenCLVersion mode;
    bool freeCLMemory();
    Net(Json::Value, cmdArg arg);
    ~Net();
    bool forward(oclHardware hardware, oclSoftware software, dType *data, NetLogging log);
    Layer* outputLayer();
};


template<typename T, typename U> U maxLabel(T* arr, U size){
    if(size == 0)
        return -1;
    if(size == 1)
        return 0;
    T max = arr[0];
    U maxIdx = 0;
    for(U i = 1; i<size;i++){
        if(max < arr[i]){
            max = arr[i];
            maxIdx = i;
        }
    }
    return maxIdx;
};
void print2D(dType *fm, int height, int width); //Debugging Utitilties

void softmax(dType *input, dType *output, int size);
#endif
