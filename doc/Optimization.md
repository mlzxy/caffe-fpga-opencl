# Optimization

🚀

It's very important for hardware


### Quick view of network parameter

```shell
python ./convertor/src/lsNet.py  --net ./convertor/output/mnist.json --layer [layerid, by default -1 means all layers]
```

### GlobalSize/localSize

Modify `globalsize/localsize/offset` in [network-config.json](convertor/output/mnist-config.json) that generated, after you change the config JSON file, run this to apply config to network JSON.

```shell
python ./convertor/src/applyConfig.py  --net ./convertor/output/mnist.json  -config ./convertor/output/mnist-config.json   --output [by default same with --net]
```


### Memory Bandwidth

This project supports two mode
- OpenCL 1.2 mode, outputFeatureMap to external memory traffic for each layer, low performance.
     ![](images/mode12.png)
- OpenCL 2.0 mode, intermediate Feature maps are stored in on-chip buffer, less memory traffic needed with PingPong Buffer.
     ![](images/mode20.png)


The settings is in [cpp/custom.h](../cpp/custom.h), right now the `OpenCL 2.0` mode has bugs, please use `OCL12`.

```C++
#ifdef __APPLE__
#define PLATFORM_FILTER APPLE_MAC
#define OPENCL_VERSION OCL12
#else
#define PLATFORM_FILTER XILINX_FPGA
#define OPENCL_VERSION OCL12
#endif
```

However, because Opencl is very restristed in a hardware design perspective, **you can't really use pingpong buffer to optimize the weight/bias external memory loading.**  Only then intermediate feature map are buffered, weight/bias are loaded as function parameters for each layer kernel.


### Attributes

Since you need different hardware optimization scheme for each network, so I didn't add optimization `__attribute__` in [kernels/net/net.cl](../kernels/net/net.cl), but you will need to add some definitely, for example:

- `Unroll`
- `Pipeline Loop`
- `Pipeline work item`

Details please read carefully at [xilinx sdaccel optimization guide](https://www.xilinx.com/support/documentation/sw_manuals/ug1207-sdaccel-performance-optimization.pdf).


### Mathematical model

I recommend you to read some papers to find the best pipeline/unroll parameters if you are serious about this, for example:

- [Optimizing FPGA-based Accelerator Design for Deep Convolutional Neural Networks](http://dl.acm.org/citation.cfm?id=2689060)

But again, because of the restriction from opencl, you can't really split/pipeline the memory traffic inside a single layer. They must be loaded at bundle. 
