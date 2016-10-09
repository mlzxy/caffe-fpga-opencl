# Caffe OpenCL FPGA

This project provides

- Python Tool to convert your trained caffe model into JSON

- C++ code that read in your JSON file and run it with OpenCL in:

  - Support CPU in any machine with CMakeLists.txt
  - Nvidia GPU with custom Makefile
  - Xilinx FPGA with custom tcl building script (So it means to **run a caffe model on FPGA** 😎).
  - (no altera support, but I assume it could be extended easily)

> If you have questions/feature requests, **don't email me**, please send an issue/pull request. I hold **ZERO** responsibilities to help anyone to

> - Fix your FPGA configuration problem
> - Help you port/optimize any caffe model
> - Implement any new features

> But I may do it when time/interest allows me.

## Limitation

This project is experimental, it has following limitations:

- Current only support limited layer types

  - Conv, Pooling, Relu

- s

## How to Use

## How to Optimize

🚀

## Contribution

The size of those cpp/python source code is not that big. After you get the above mnist example to run, I recommend you to read these source code:

- network.cpp
- dsfa
- sadf

They will give you a very good understanding of what's going on. When you have read them carefully, you are ready to add new features! 👍

Please fork this project and send pull request after you complete your tested features. Thank you very much.
