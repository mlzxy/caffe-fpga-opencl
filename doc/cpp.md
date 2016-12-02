# Source Structure

```shell
├── custom.cpp
├── custom.h
├── fpganet.h
├── helper.cpp
├── helper.h
├── json
│   ├── json-forwards.h
│   └── json.h
├── jsoncpp.cpp
├── main.cpp
├── net_enum.h
├── network.cpp
├── network.h
└── other
    └── mnist_data.h
```

The code consists of four "modules":

- **Helper**: utility function, OpenCL api wrapper, command line argument parser.
- **Network**: class/functions to parser json to network object, and initiate/communicate with kernels.
- **Custom**: callback function that user could modify and customize.
- **Misc**: a small amount of other code.


## Helper

[helper.cpp](../cpp/helper.cpp) / [helper.h](../cpp/helper.h) contains the important entries that wrap opencl api and parse commandline arguments.

```cpp
typedef std::map<std::string, cl_kernel>::iterator itKernelMap;
typedef struct cmdArg{
    cl_device_type deviceType; // CPU or FPGA or GPU
    char mFileName[1024]; // kernel binary/source file
    char mKernelName[128]; //which kernels to compiled
    char platformName[1024]; //which platform to use, apple, xilinx and etc..
    int  deviceID; //deviceID, by default 0, when opencl api return multiple devices
    char network[1024]; // network json file path
    NetLogging networkLoggingLevel; // logging level
    OpenCLVersion  openclVersion; // opencl version 12 or 20 (20 has on-chip cache)
    char info[1024]; //custom information
} cmdArg;
oclSoftware getOclSoftware(const oclHardware &hardware, const char* kernelNames, const char* kernelFileName);
oclHardware getOclHardware(cmdArg arg);
cl_int compileProgram(const oclHardware &hardware, oclSoftware &software);
cmdArg parseCmdArg(int argc, char** argv);
int loadFile2Memory(const char *filename, char **result);
const char *clErrorCode(cl_int code);
typedef bool (*RunOpenCL)(cmdArg arg, oclHardware hardware, oclSoftware software);
double runProgram(int argc, char** argv, RunOpenCL F);
```

the program will first call `runProgram`, which will

1. parse command line arguments into `struct cmdArg`.
2. call opencl api in  `getOclHardware` and `getOclSoftware`, activate device and return two structs `oclHardware` and `oclSoftware`.
3. call `bool (*RunOpenCL)(cmdArg arg, oclHardware hardware, oclSoftware software)` with `cmdArg, oclHardware, oclSoftware`, which is a function defined in [custom.cpp](../cpp/custom.cpp). Like a user callback function.



## Network

[network.cpp](../cpp/network.cpp) / [network.h](,,/cpp/network.h) / [net_enum.h](../cpp/net_enum.h)

- In net_enum.h, some network related enums are defined.

    ```cpp
    enum LayerType {Convolution, Relu, Data, Split, Pooling, Accuracy, SoftmaxWithLoss, Output, Padding};
    enum OpenCLVersion {OCL20, OCL12};
    enum NetLogging {NO, LAYER, NET,DEBUG};
    ```

- In network.cpp, two structs `NetParam` and `WeightData` are defined to contain information for each layer.
    ```cpp
    typedef struct {
    int stride;
    int kernelSize;
    int pad;
    int dilation;
    int inputChannel;
    int inputHeight;
    int inputWidth;
    ...
    } NetParam;
    typedef struct{
    dType \*weight;
    dType \*bias;
    ...
    } WeightData;
    ```

- Two classes `Layer` and `Net` are defined.
    - `Net` contains a linkedlist of `Layer` objects
    - `Layer` contains `NetParam`, `WeightData`, `LayerType` and other related objects for example opencl `cl_mem`.
    - `Net.forward` method is the entry point we use to call network, which is just a wrapper to call `Layer.forward` iteratively.
    - `Layer.forward` is **the very method that really communicate with opencl kernel**.
        1. create `cl_buffer` for input/output feature map (if use on-chip cache, then the input size = 0 except input/output layer)
        2. enqueue `read/kernel exec/write buffer` commands into queue, which kernel to call is determined by `layerType`
        3. assign `current_layer.output_buffer` to `next_layer.input_buffer`

## Custom

[custom.cpp](../cpp/custom.cpp) / [custom.h](../cpp/custom.h)

The user callback function `bool (*RunOpenCL)(cmdArg arg, oclHardware hardware, oclSoftware software)` get executed when
 - command line argument is parsed
 - opencl environment is setup.  

The default callback function in `custom.cpp` does these following things:

1. load network json file
    ```cpp
    int jsonSize = loadFile2Memory(arg.network, &netJson);
    Json::Value netRoot;
    Json::Reader reader =  reader.parse( std::string(netJson), netRoot );
    ```

2. create `Network` object:
    ```cpp
    Net *net = new Net(netRoot, arg);
    ```
3. feed mnist images to network:

    ```cpp
    for(i = 0; i<len; i++) {
        net->forward(hardware, software, mnist_images[i], arg.networkLoggingLevel);
    }
    ```

You can change it up to your own need.

## Misc

### main.cpp        

Just a wrapper to call the real program

```cpp
int main(int argc, char** argv) {
    double time = runProgram(argc, argv, run);
    INFO_LOG << "Total Custom Code Run Time: " << time << endl;
    return 0;
}
```

### json

Download from C++ JSON library https://github.com/open-source-parsers/jsoncpp.

### fpganet.h

Just a more semantical wrapper.

```cpp
#include "helper.h"
#include "network.h"
```

### other

#### mnist_data.h

A large header file where big mnist data array `float mnist_images[300][784]` is defined, lazy person's lazy way to test with mnist.
