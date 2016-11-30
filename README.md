# Caffe OpenCL FPGA

This project provides

- Python Tool to convert your trained caffe model into JSON
- C++ code that read in your JSON file and run it with OpenCL in:
  - `CMakeLists.txt` for cpu in any OS
  - `Makefile` for mac CPU
  - `Makefile` for Nvidia GPU
  - `Tcl building script` for Xilinx FPGA
  - (Sorry for no altera support right now, but I assume it could be extended easily)


Send an issue if you have any problems, or email me if you want to collaborate.

#### How to Use

Here is a brief [intro & example](doc/usage.md).  

#### How to Optimize

[Some notes](doc/optimization.md)

#### How to Contribute

[Some notes](doc/code_structure_explain.md) explaining this code base structure, which is helpful to you :smile: And two "how to add new feature" provided

- [How to add new layer](doc/how_to_add_new_layer.md)
- [How to support multiple branch network](doc/how_to_support_multiple_branches_network.md)


## Limitation

This project is experimental at this moment, it has following (big) :broken_heart: limitations:

- Current only support limited layer types
  - Conv
  - Pooling
  - Relu
- Don't support `mergeLayer`, `concatLayer` that accepts multiple inputs, although they can be added with some work.



## TODO

Here are some of my thoughts in the top todo list.


- [ ] Contact Xilinx Support/Work with others to solve the on-chip cache buffer.
- [ ] Apply some optimizing attributes for benchmarking, which could be good examples for optimization.

- [ ] Implement `mergeLayer/concatLayer`, then it will basically be able to handle arbitrary structure network.


- [ ] Besides above, this project is good for an research basis for OpenCL FPGA design parameter tuning for neural network application, which I am very interested in and willing to dig in :smiley:
