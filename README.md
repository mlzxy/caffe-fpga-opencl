[Notice: I am working on my master thesis, not able to respond for this project actively.](doc/Notice.md)

# Caffe OpenCL FPGA

This project provides

- Python Tool to convert your trained caffe model into JSON
- C++ code that read in your JSON file and run it with OpenCL in:
  - Support CPU in any machine with CMakeLists.txt
  - Nvidia GPU with custom Makefile
  - Xilinx FPGA with custom tcl building script (So it means to **run a caffe model on FPGA** 😎).
  - (no altera support, but I assume it could be extended easily)


## How to Use

Please read the [Documentation](doc/Usage.md).


## How to Optimize

Please read the [Optimization Documentation](doc/Optimization.md)

## Contribution

If you want to contribute, please read the [Contribution Guide](doc/Contribution_Guide.md), example provided!


## Limitation

This project is experimental at this moment, it has following limitations:

- Current only support limited layer types
  - Conv, Pooling, Relu
- Don't support `mergeLayer`, `concatLayer` that accepts multiple inputs.



## Plan

My recent plan is to convert the caffe model from this paper [Multi-Scale Context Aggregation by Dilated Convolutions](http://arxiv.org/abs/1511.07122) to FPGA, which implements a scene segmentation network leveraging dilated convolution.

If everything works out, the conversion process should be smooth. *Dilated convolution is very interesting idea* to increase the receptive field of convolution (because you want larger field on top layers, since there is more abstract information than bottom), which also succeed in audio processing from [wavenet](https://deepmind.com/blog/wavenet-generative-model-raw-audio/) paper that google recently published.
