//
// Created by 张鑫语 on 9/24/16.
//

#include "net.h"
#include "mnist_data.h"
#include "../../helper.h"

double run(cmdArg arg, oclHardware hardware, oclSoftware software) {
    clock_t start = clock();
    int mnist_test_num = 50;
    int start_idx = 0;
    char *netJson = 0;
    INFO_LOG<<"Start to read "<<arg.network<<endl;
    int jsonSize = loadFile2Memory(arg.network, &netJson);
    if(jsonSize == -1){
        ERROR_LOG<<"Fail to load File "<<arg.network<<endl;
        exit(0);
    }
    INFO_LOG<<"Read network json "<<arg.network<<" finished"<<endl;
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
    bool forward_result;
    for(int i = start_idx;i<start_idx+mnist_test_num; i++){
        forward_result = net->forward(hardware, software, mnist_images[i], NETWORK_LOG_LEVEL);
        if(!forward_result){
            delete net;
            ERROR_LOG<<"FREE MEMORY AND EXITING...";
            exit(0);
        }
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
        INFO_LOG<<"NO "<<i+1<<","<<result<<" prediction on image "<<i<<": "<<predicted<<" = "<<mnist_labels[i]<<endl;
    }

    INFO_LOG<<"Accuracy = " << correctCounter/mnist_test_num*100<<"%."<<endl;
    delete net;
    return (clock() - start)/(double)CLOCKS_PER_SEC;
}
