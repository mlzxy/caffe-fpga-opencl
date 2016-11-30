## Adding new type of Layer

1. add cl code for `newLayer` in [kernels/net/net.cl](../kernels/net/net.cl)

2. add another `LayerType` for `newLayer` in [cpp/network.h](../cpp/network.h)

3. add corresponding OpenCL/Host communication code for `forward` and `Layer` method in [cpp/network.cpp](../cpp/network.cpp) if needed.
    - usually special layer needs this, layer like sigmoid or fully connected won't need.
