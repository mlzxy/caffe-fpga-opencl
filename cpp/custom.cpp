#include "custom.h"
#include "fpganet.h"
#include "./other/mnist_data.h"


bool run(cmdArg arg, oclHardware hardware, oclSoftware software) {
    int mnist_test_num;
    try {
         mnist_test_num = std::stoi(arg.info);
    } catch (std::exception const &e) {
        INFO_LOG<<"Number of images not specified, use 1"<<endl;
        mnist_test_num = 1;
    }

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
    Net *net = new Net(netRoot, arg);
    INFO_LOG<<"Net build finished."<<endl;
    float correctCounter = 0;
    dType softmax_output[10];
    bool forward_result;
    for(int i = start_idx;i<start_idx+mnist_test_num; i++){
        forward_result = net->forward(hardware, software, mnist_images[i], arg.networkLoggingLevel);
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


    return true;
}
