[Notice: I am working on my master thesis, not able to respond for this project actively.](doc/Notice.md)

# Caffe OpenCL FPGA

This project provides

- Python Tool to convert your trained caffe model into JSON
- C++ code that read in your JSON file and run it with OpenCL in:
  - `CMakeLists.txt` for cpu in any OS
  - `Makefile` for mac CPU
  - `Makefile` for Nvidia GPU
  - `Tcl building script` for Xilinx FPGA
  - (Sorry for no altera support right now, but I assume it could be extended easily)


## How to Use

Please read the [Documentation](doc/Usage.md).


## How to Optimize

Please read the [Optimization Documentation](doc/Optimization.md)

## Contribution

If you want to contribute, please read the [Contribution Guide](doc/Contribution_Guide.md), example provided.


## Limitation

This project is experimental at this moment, it has following limitations:

- Current only support limited layer types
  - Conv
  - Pooling
  - Relu
- Don't support `mergeLayer`, `concatLayer` that accepts multiple inputs, although they can be added with some work.



## TODO

Not actively developing right now, but here are my thoughts in the top todo list.



- [ ] Contact Xilinx Support/Work with others to solve the on-chip cache buffer.
- [ ] Apply some optimizing attributes for benchmarking, which could be good examples for optimization.

- [ ] Implement `mergeLayer/concatLayer`, then it will basically be able to handle arbitrary structure network.


Besides above, I sincerely think this project could be used as an research basis for OpenCL FPGA design parameter tuning for deep learning application, which I am very interested in.
