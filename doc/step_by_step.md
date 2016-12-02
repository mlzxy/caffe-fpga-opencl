# Step by Step of a MNIST Demo

## 0. clone the code

```shell
git clone https://github.com/BenBBear/caffe-fpga-opencl
cd caffe-fpga-opencl
```

## a. Train your own network


1. Go to the data folder

    ```shell
    cd ./convertor/data/mnist
    ```

2. change `snapshot_prefix` to be the path on your OS

    ```shell
    vim solver.prototxt
    ```

3. change `Datalayer.data_param.source` to be the path on your OS

    ```shell
    vim train.prototxt
    ```

4. train the network

    ```shell
    caffe train --solver=solver.prototxt
    ```

5. when finished, named the final `[snapshot].caffemodel` to be `model.caffemodel` under the `convertor/data/mnist` folder.

## b. Convert Caffe Model into JSON

1. go to python convertor tool folder

    ```shell
    cd ./convertor/src
    ```

2. convert the `caffemodel` into json
    ```shell
    python convert.py  -m  ../data/mnist/model.caffemodel  -p  ../data/mnist/train.prototxt  -o  ../output  --name mnist
    ```

3. this will generate
    - [output/mnist.json](../convertor/output/mnist.json)
    - [output/mnist-config.json](../convertor/output/mnist-config.json)

You only need the `mnist.json` afterwards, but **the `mnist-config.json` is used to define `OpenCL globalSize/localSize` for each layer**.

## c. Add OpenCL Work-Group-Item Configuration

`mnist-config.json` looks like:

```json
{
"layers": [
    {
        "config": {
            "global_size": [
                1,
                1,
                1
            ],
            "local_size": [
                1,
                1,
                1
            ],
            "offset": [
                0,
                0,
                0
            ]
        },
        "input_channel": 1,
        "input_height": 28,
        "input_width": 28,
        "output_channel": 1,
        "output_height": 28,
        "output_width": 28,
        "type": "Data"
    }, ...]
}    
```

1. Customize `globalSize/localSize` for each layer
2. Add Configuration `mnist-config.json` information to `mnist.json` by

    ```shell
    python applyConfig.py -n ../output/mnist.json -c ../output/mnist-config.json  
    ```

3. this will insert configuration info to `mnist.json`


## d. Edit/Read some CPP code

Open
- [cpp/custom.h](../cpp/custom.h)
- [cpp/custom.cpp](../cpp/custom.cpp)

You won't need edit them because they are already written. But you could see

1. how the JSON file is readed and parsed into network.
3. how the network get executed.



## e. Run the CPU demo in your mac laptop, using Makefile provided.

1. load some environment variable
    ```shell
    cd /path/to/repo
    source env.sh
    ```

2. copy the `Makefile` template into output folder
    ```shell
    cd ./build/output
    cp ../resources/Makefile_templates/Makefile.mac.cpu  ./Makefile  
    ```

3. `make` and `run`
    ```shell    
    make
    ./network_cpu.exe -h # to see help on program parameter
    make run
    ```




## f. Run the Demo in your own laptop with CPU with CLion

1. Open CLion, and open this source folder in CLion

2. Make sure CLion can parse `CMakeLists.txt` correctly (you will get warnings if not).

3. Click **Edit Configurations** -> **Add New** and set options:
    - Working Directory: Top source folder
    - Program Parameters:
    ```
    -f ./kernels/net/net.cl  -k dataLayer,outputLayer,convLayer,paddingLayer,reluLayer,poolingLayer  -n ./convertor/output/mnist.json -d cpu -l layer -i 30
    ```

4. Click Run, and enter `y` and `<enter>` in the console to confirm.

5. Result will be like below. **If error happens, please check the console logging output, which is very detailed.**

    ![](./images/cpu-demo.png)


6. Here are the explanations of the program parameters above:
    ```man
      -f <kernel_file>   kernel source/binary file path, required

      -k <kernel_name1,kernel_name2>  kernels to compile, required

      -n <network file>  network json file that converted by python script, required

      -d <cpu|gpu|fpga> device type, default cpu in apple, otherwise fpga

      -l <no|layer|net|debug> network logging level, debug will print feature map

      -c <0|1|2> device index to choose, choose first available device by default

      -v <12|20> opencl version

      -p <Xilinx|Apple|'NVIDIA CUDA'> platform to choose, by default Apple in mac, Xilinx otherwise.

      -i <extra information> parse it in your own custom run function, here means the number of mnist images used

      -h <print help info>
    ```



## g. Run the demo with GPU [Optional]

Tested on `Centos6` and its `Makefile` provided, other OS should be similar

1. setup environment variables and copy makefile to output folder
    ```shell
    cd /path/to/repo
    source env.sh
    cd ./build/output
    cp ../resources/Makefile_templates/Makefile.centos6.gpu  ./Makefile  
    ```

2. modify the `Makefile`, make sure to change `OPENCL_*`, `OPENCV_*`, `AV_LIB` to the correct values in your OS.
    ```shell
    vim ./Makefile  
    ```

3. `make && make run`


## h. Run the demo with FPGA

1. make sure you have these correct environment variable for FPGA.
    - `SDACCEL_HOME`: where is `Sdaccel` installed
    - `FPGA`: the device name of your fpga
    ```shell
    cd /path/to/repo
    vim env.sh
    ```
2. copy `tcl building script` to `output`

    ```shell
    cd ./build/output
    cp ../resources/Makefile_templates/Makefile.fgpa.tcl ./net.tcl
    ```

3. compile by `sdaccel ./net.tcl`

4. set the correct `LD_LIBRARY_PATH` for FPGA by running this on bash

    ```shell
    fpga_hw_env # a function defined in env.sh
    ```

5. run it on FPGA   

    ```shell  
    cd ./network_fpga/pkg/pcie/  
    ./network_fpga.exe  -f ./net.cl.hw.xclbin  -k convLayer,paddingLayer,poolingLayer,dataLayer,outputLayer,reluLayer  -n $(CAFFE_FPGA_CL_HOME)/convertor/output/mnist.json  -v 20 -l layer -i 30
    ```


## Extra

How to add more debug printing when you execute the code on FPGA?
- uncomment `#define DEBUG` in [net.cl](../kernels/net/net.cl). Then the code will transfer feature map to host memory no matter on-chip cache is used or not.
- In the same time, use `-l debug` for `network_fpga.exe`. Then it will print out intermediate feature map on the console.
