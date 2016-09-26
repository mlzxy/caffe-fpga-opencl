//
// Created by 张鑫语 on 9/24/16.
//

#include "convolutionLayer.h"
#include "mnist_data.h"


#define ERR(x) REPORT_ERRM(0.0, x)


double run(cmdArg arg, oclHardware hardware, oclSoftware software) {
    cl_int err;
    char *netJson = 0;
    INFO_LOG<<"Start to read "<<arg.network<<endl;
    int jsonSize = loadFile2Memory(arg.network, &netJson);
    INFO_LOG<<"Read network json "<<arg.network<<" finished"<<endl;
    if(jsonSize < 0){
        ERROR_LOG<<"Fail to load "<<arg.network<<endl;
    }
    Json::Reader reader;
    Json::Value netRoot;
    bool parsingSuccessful = reader.parse( std::string(netJson), netRoot );
    if ( !parsingSuccessful){
        ERROR_LOG<<"Fail to parse "<<arg.network<<endl;
    }
    INFO_LOG<<"Start to build net."<<endl;
    Net *net = new Net(netRoot);
    INFO_LOG<<"Net build finished."<<endl;

    NetParam dataParam = (*(net->layers))[0]->param;
    NetParam labelParam = (*(net->layers))[net->num_layers-1]->param;

    cl_mem input_image = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY, dataParam["output_fm_data_num"], NULL, &err);
    ERR(0.0);
    cl_mem output_label = clCreateBuffer(hardware.mContext, CL_MEM_WRITE_ONLY, labelParam["output_fm_data_num"], NULL, &err);
    ERR(0.0);
    for(int i = 0; i<MNIST_TEST_NUM; i++){
        //TODO
        //setKernelArg, enqueue, clevent, then compare results.
        // 0. calculate maximum number of read buffer needed, feature map cache needed, write buffer needed.
        // 1. 保证json肯定是可以读进来， 而且正确
        // 2. 写convolution/deconvolution的matlab
        // 3. 写convolution的opencl代码，并且用pyopencl验证
        // 4. 写opencl中的pingpong buffer.
    }
    clReleaseMemObject(input_image);
    clReleaseMemObject(output_label);
    delete net;
}