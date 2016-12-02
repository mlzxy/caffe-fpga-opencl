# Source Structure

```shell
└── net
    └── net.cl
```


## Configuration Macro

1. Buffer Size, if use on chip buffer, it should be the largest buffer needed for all those layers
    ```cpp
    #define BUFFER_SIZE 15680
    ```

2. whether
    - print information during execution
    - transfer feature map back to host memory even cache
    ```cpp
    #define DEBUG
    ```

3. Data type
    ```cpp
    // Data type for weight/bias/feature map
    typedef float dType;

    // Data type for Bool
    typedef int BOOL;
    ```


## On chip buffer Implementation

On chip buffer is implemented in this way, so it could be easily switched by the `BUFFER_SIZE` macro.  

```cpp
#define readFmBufferId phase[0] == 0
#define writeFmBufferId phase[0] == 1

#ifdef BUFFER_SIZE
global dType fmCache[2][BUFFER_SIZE];
#define readFmBuffer fmCache[readFmBufferId]  // read==0
#define writeFmBuffer fmCache[writeFmBufferId] // write==1
#else
#define readFmBuffer inputFeatureMap
#define writeFmBuffer outputFeatureMap
#endif
```
`phase` is the ping pong id.

## Some utility Macros

They have doc inside the source code, here I just list them:

- `GLOBAL_SIZE_0, GROUP_ID_2 and etc...` wrapper for opencl api


- `ELM, WELM` wrapper to access the feature map and filter array, for less typing.

- `WORK_ITEM_BEGIN, WORK_ITEM_END` special macro, code enclosed between will get execute per work item.


- `WORK_ITEM_3D_BEGIN, WORK_ITEM_3D_END; EASY_WORK_ITEM_3D_OUTPUT_BEGIN, EASY_WORK_ITEM_3D_OUTPUT_END` special macro, code enclosed between will get execute per work item with awareness of where they are in the 3D feature map.

- `LOAD_DATA_*` macro to copy data, with scale/pad support.


## Kernel code

All layers kernel code are based on above utility macros, and you will find the kernel code is very minimal because of the help from those macros.  For example, if you are familiar with [how 2d convolution works](http://cs231n.stanford.edu/slides/winter1516_lecture7.pdf), code belong will be very relevant and understandable. 


```OpenCL
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
                        __global const NetParam *param, __global BOOL *phase) {
  DEBUG_PRINT_INFO("convLayer");
  __private int dilatedKernelSize =
      (param->kernelSize - 1) * param->dilation + 1;
  __private dType result;
  EASY_WORK_ITEM_3D_OUTPUT_BEGIN(channelCounter, heightCounter, widthCounter);
  result = 0;
  for (int c = 0; c < param->inputChannel; c++)
    for (int i = 0; i < param->kernelSize; i++)
      for (int j = 0; j < param->kernelSize; j++) {
        result += (ELM(readFmBuffer, c, param->inputHeight,
                       heightCounter * param->stride + i * param->dilation,
                       param->inputWidth,
                       widthCounter * param->stride + j * param->dilation) *
                   WELM(weight, channelCounter, param->inputChannel, c,
                        param->kernelSize, i, j));
      }

  result += bias[channelCounter];
  OUTPUT_ELM = result;
  EASY_WORK_ITEM_3D_OUTPUT_END(channelCounter, heightCounter, widthCounter);
  DEBUG_DUMP_DATA_TO_CPU;
}
```
