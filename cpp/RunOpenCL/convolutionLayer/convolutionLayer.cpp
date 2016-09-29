//
// Created by 张鑫语 on 9/24/16.
//

#include "convolutionLayer.h"
#include "mnist_data.h"


#define ERR(x) REPORT_ERRM(0.0, x)
#define LAYERS (*(net->layers))
#define DATA_SIZE sizeof(float)
#define INPUT_SIZE(layer)  (DATA_SIZE*layer->param["input_channel"]*layer->param["input_width"]*layer->param["input_height"])
#define OUTPUT_SIZE(layer) (DATA_SIZE*layer->param["output_channel"]*layer->param["output_width"]*layer->param["output_height"])

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

    NetParam dataParam = LAYERS[0]->param;
    NetParam labelParam = LAYERS[net->num_layers-1]->param;

    cl_mem input_image = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY, dataParam["output_fm_data_num"], NULL, &err);
    ERR(0.0);
    cl_mem output_label = clCreateBuffer(hardware.mContext, CL_MEM_WRITE_ONLY, labelParam["output_fm_data_num"], NULL, &err);
    ERR(0.0);
    int (*images)[28][28]  = mnist_images;

    size_t globalSize[3], localSize[3], dim = 3;


    Layer *conv, *relu, *pool;
    dataType *input_feature_map_data, *output_feature_map_data;
    for(int i = 0; i<MNIST_TEST_NUM; i++, images++){
        for(int j = 0; j<net->num_layers;){
            int start = j;
            conv = NULL;
            relu = NULL;
            pool = NULL;
            Layer *conv = LAYERS[start];
            while(LAYERS[j+1]->type == Relu || LAYERS[j+1]->type == Pooling){
                if(LAYERS[j+1]->type == Relu){
                    relu = LAYERS[j+1];
                }else{
                    pool = LAYERS[j+1];
                }
                j = j + 1;
            }
            cl_mem  input_feature_map = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY,
                                                       INPUT_SIZE(conv), NULL, &err);
            ERR(0.0);
            cl_mem  input_weight = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY,
                                                      conv->weight_data_num*DATA_SIZE, NULL, &err);
            ERR(0.0);
            cl_mem  input_bias = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY,
                                                      conv->bias_data_num*DATA_SIZE, NULL, &err);
            ERR(0.0);
            cl_mem  output_feature_map = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY,
                                                       OUTPUT_SIZE((relu == NULL ? pool : relu)), NULL, &err);
            ERR(0.0);
            err = clEnqueueWriteBuffer(hardware.mQueue, input_image, CL_TRUE, 0, INPUT_SIZE(conv),  , 0, NULL, NULL);



            clReleaseMemObject(input_feature_map);
            clReleaseMemObject(input_weight);
            clReleaseMemObject(input_bias);
            clReleaseMemObject(output_feature_map);
        }




    }


    delete input_feature_map;
    delete output_feature_map;
    delete net;
}