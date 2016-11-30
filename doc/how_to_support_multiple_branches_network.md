## Support multiple branches network

> This means to support most caffe models. Need a lot of work.

0. Modify the python script in [convertor/src/net.py](../convertor/src/net.py) and [convertor/src/layer.py](../convertor/src/layer.py), to generate a JSON file that contains multibranch information.

1. Create some kind of `mergelayer` or `concatLyaer` in [kernels/net/net.cl](../kernels/net/net.cl), that accepts multiple `inputFeatureMap` that current implementation doesn't support. You will need to have

  - different access pattern for `readFmCache` than other layers

2. add the newly created `LayerType` in [cpp/network.h](../cpp/network.h)

3. add corresponding code for `forward` and `Layer` method in [cpp/network.cpp](../cpp/network.cpp).
    1. `Layer` method should process the multibranch information in JSON, the `next*, prev*` pointer may be changed to `next**, prev**`.

    2. The `forward` method is more complex than usual, since it needs multiple `inputFeatureMap`. You may need to
        - Transfer rest of those `inputFeatureMap` even in FPGA memory traffic reduction mode.

4. Test/Debug on cpu/fpga
