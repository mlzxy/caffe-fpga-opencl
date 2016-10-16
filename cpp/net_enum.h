
#ifndef OPENCL_LEARNING_NET_ENUM_H
#define OPENCL_LEARNING_NET_ENUM_H

enum LayerType {Convolution, Relu, Data, Split, Pooling, Accuracy, SoftmaxWithLoss, Output, Padding};
enum OpenCLVersion {OCL20, OCL12};
enum NetLogging {NO, LAYER, NET,DEBUG};

#endif
