//
// Created by 张鑫语 on 9/24/16.
//

#include "net.h"
#include "mnist_data.h"
#include "../../helper.h"

double run(cmdArg arg, oclHardware hardware, oclSoftware software) {
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
    Net *net = new Net(netRoot, arg, OPENCL_VERSION);
    INFO_LOG<<"Net build finished."<<endl;
    float correctCounter = 0;
    dType softmax_output[10];
//    dType *image = new dType[784];
    for(int i = 0;i<MNIST_TEST_NUM;i++){
        net->forward(hardware, software, mnist_images[i]);
        Layer* output = net->outputLayer();
        softmax(output->outputBuffer, softmax_output, output->param.outputTotalDataNum);
        int predicted = maxLabel<dType, int>(softmax_output, output->param.outputTotalDataNum);
        string result;
        if(predicted == mnist_labels[i]){
            correctCounter++;
           result = "Correct";
        }else{
            result="Wrong";
        }
        INFO_LOG<<result<<" prediction on image "<<i<<": "<<predicted<<" = "<<mnist_labels[i]<<endl;
    }

    INFO_LOG<<"Accuracy = " << correctCounter/MNIST_TEST_NUM*100<<"%."<<endl;
    delete net;
}