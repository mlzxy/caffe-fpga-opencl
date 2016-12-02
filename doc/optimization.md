# Some notes on Optimization



### GlobalSize/localSize

Modify `globalsize/localsize/offset` in [network-config.json](../convertor/output/mnist-config.json) that generated, after you change the config JSON file, run this to apply config to network JSON.

```shell
python ./convertor/src/applyConfig.py  --net ./convertor/output/mnist.json  -config ./convertor/output/mnist-config.json   --output [by default same with --net]
```

It will determine how much resources is allocated for kernel.


### Memory Bandwidth

:warning:  On Chip buffer **has bugs right now**, it works for emulation but not on real hardware. :warning:  

It supports two mode
- OpenCL 1.2 mode, outputFeatureMap to external memory traffic for each layer, low performance.
     ![](images/mode12.png)
- OpenCL 2.0 mode, intermediate Feature maps are stored in on-chip pingpong buffer, less memory traffic needed.
     ![](images/mode20.png)

To use the first one:

1. use the `-v 12` program parameter
2. comment out the `#define BUFFER_SIZE N`  in [net.cl](../kernels/net/net.cl)


To use the second one:

1. use `-v 20` program parameter
2. uncomment that line above.



### Attributes

For example:


- `Unroll`
- `Pipeline Loop`
- `Pipeline work item`


More details in [xilinx sdaccel optimization document](https://www.xilinx.com/support/documentation/sw_manuals/ug1207-sdaccel-performance-optimization.pdf). Right now I haven't add them in the [net.cl](../kernels/net/net.cl) although I do want to try.  Feel free to explore them.


### Mathematical model

Use some mathematical model to find the best pipeline/unroll parameters, this paper is a very good start: [Optimizing FPGA-based Accelerator Design for Deep Convolutional Neural Networks](http://dl.acm.org/citation.cfm?id=2689060).


### Limitation

> The on-chip buffer size has to be the maximum feature map size among all layers, which is not efficient at all. But it's difficult to do further trick on memory transfer under the FPGA OpenCL framework.
-- *From Readme*
