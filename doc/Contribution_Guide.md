
# Contribution Guide

✊

The size of those cpp/python source code is not that big. After you get the above mnist example to run, I recommend you to read these source code:

- [cpp/network.h](../cpp/network.h): Network related classes/utilties declaration files, for example: `NetParam, Layer, Net, WeightData`
- [cpp/network.cpp](../cpp/network.cpp): Implementations of those network related utilties.
- [cpp/main.cpp](../cpp/main.cpp): the main function wrapper
- [cpp/net/net.cpp](../cpp/net/net.cpp): the implementation of mnist net
- [CMakeLists.txt](../CMakeLists.txt): the cmake file for the project (for cpu/gpu compilation on a non-fpga machine)
- [cpp/net/net.tcl](../cpp/net/net.tcl): the sdaccel building script for mnist net

They will give you a very good understanding of what's going on. When you have read them carefully, you are ready to add new features! 👍 Here I give **_two examples_** of how to add

- A new type of layer
- Support multiple branches network

## Adding new type of Layer

1. add cl code for `newLayer` in [kernels/net/net.cl](../kernels/net/net.cl)

2. add another `LayerType` for `newLayer` in [cpp/network.h](../cpp/network.h)

3. add corresponding code for `forward` and `Layer` method in [cpp/network.cpp](../cpp/network.cpp)

4. test on cpu/fpga

## Support multiple branches network

> This means to support any caffe model.

0. Modify the python script in [convertor/src/net.py](../convertor/src/net.py) and [convertor/src/layer.py](../convertor/src/layer.py), to generate a JSON file that contains multibranch information.

1. Create some kind of `mergelayer` or `concatLyaer` in [kernels/net/net.cl](../kernels/net/net.cl), that accepts multiple `inputFeatureMap` that current implementation doesn't support. You will need to have

  - different access pattern for `readFmCache` than other layers

2. add the newly created `LayerType` in [cpp/network.h](../cpp/network.h)

3. add corresponding code for `forward` and `Layer` method in [cpp/network.cpp](../cpp/network.cpp).
    1. `Layer` method should process the multibranch information in JSON, the `next*, prev*` pointer may be changed to `next**, prev**`.

    2. The `forward` method is more complex than usual, since it needs multiple `inputFeatureMap`. You may need to
        - Transfer rest of those `inputFeatureMap` even in FPGA memory traffic reduction mode.

4. carefully test on cpu/fpga

PS: When you plan to add new features, please fork this project and send pull request after you complete your tested features. Thank you very much.
