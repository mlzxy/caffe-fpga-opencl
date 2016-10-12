# How to Use

## Requirements

- `SDaccel` correct installed and your FPGA configured through PCIe, [installing guide](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug1020-sdaccel-installation-guide.pdf). Linux machine required.

- CPU based `OpenCL` installed for debugging

- `OpenCV` libraries.

- `Caffe & Pycaffe` installed

- (nvidia library if you also want to try gpu)

I recommend you debugging/coding in OSX, it has `opencl` and `opencv` installed by default I think. Run FPGA programs in Linux.



## Demo

The default [cpp/RunOpenCL/net/net.cpp](../cpp/RunOpenCL/net/net.cpp) and everything are a mnist classification conv network.

- Run CPU Demo:
    - open Clion
    - open this folder in Clion
    - create new running configuration, set
        - working directory as source root
        - program parameters: `-f ./kernels/net/net.cl  -k dataLayer,outputLayer,convLayer,paddingLayer,reluLayer,poolingLayer  -n ./convertor/output/mnist.json -d cpu`


- Run FPGA Demo:
    - open [cpp/RunOpenCL/net/net.tcl](../cpp/RunOpenCL/net/net.tcl)
    - change `absoluteRoot` variable to your path name
    - `source ./build/bash/runBuildTcl.sh`
    - `runBuildTCL net`



## How-to Steps

The steps to convert and run a trained caffe model on your opencl enabled fpga:

1. convert your model to json file using python scripts
    ```shell
        python ./convertor/src/convertor/ --model_path  /home/xy0/Desktop/convertor/data/mnist/model.caffemodel  --prototxt_path  /home/xy0/Desktop/convertor/data/mnist/train.prototxt  --output_dir  /home/xy0/Desktop/convertor/output  --name mnist
    ```
    - output
        - [network.json](../convertor/output/mnist.json): contains structure/weight/parameters
        - [network-config.json](convertor/output/mnist-config.json): contains globalSize/localSize configuration for each layer, which you can change to tweak performance.




2. modify [cpp/RunOpenCL/net/net.cpp](../cpp/RunOpenCL/net/net.cpp) for your network.
    - Read in correct dataset(or webcam)
    - call `net.forward` method


3. Building
    - **CPU:**
        - use `CMakeLists.txt` to build for CPU (I use [CLion](https://www.jetbrains.com/clion/) to automate that and debugging.)
    - **GPU:**
        - If you want to use GPU, take an example of this [Makefile](build/resources/archive/RunOpenCL/pipelineDemo/Makefile.gpu) and create your own. (rememeber to set correct `LD_LIBRARY_PATH`)
    - **FPGA:**
        1. Modify your tcl building script in [cpp/RunOpenCL/net/net.tcl](../cpp/RunOpenCL/net/net.tcl)
        2. `source ./build/bash/runBuildTcl.sh && runBuildTCL net` to run this in `sdaccel`
        3. or equivalent `cd ./build/ && mkdir -p output && sdaccel ../../cpp/RunOpenCL/net/net.tcl`




## Custom Setting

Go to [cpp/custom.h](../cpp/custom.h) and head of [kernels/net/net.cl](../kernels/net/net.cl).

- dataType to use (`float` by default)
- opencl compilation option
- memory optimization
- etc...
