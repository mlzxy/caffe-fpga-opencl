#include "network.h"
#include "fpganet.h"

WeightData createWeightData(Json::Value data) {
  WeightData learnedParam;
  learnedParam.weight_dim_num = data["weight"]["num_dim"].asInt();
  learnedParam.bias_dim_num = data["bias"]["num_dim"].asInt();
  learnedParam.weightShape = new int[learnedParam.weight_dim_num];
  learnedParam.biasShape = new int[learnedParam.bias_dim_num];
  for (int i = 0; i < learnedParam.weight_dim_num; i++) {
    learnedParam.weightShape[i] = data["weight"]["shape"][i].asInt();
  }
  for (int i = 0; i < learnedParam.bias_dim_num; i++) {
    learnedParam.biasShape[i] = data["bias"]["shape"][i].asInt();
  }
  learnedParam.weight_data_num = data["weight"]["num_data"].asLargestInt();
  learnedParam.bias_data_num = data["bias"]["num_data"].asLargestInt();
  learnedParam.weight = new dType[learnedParam.weight_data_num];
  for (int i = 0; i < learnedParam.weight_data_num; i++) {
    learnedParam.weight[i] = (dType)data["weight"]["data"][i].asDouble();
  }
  learnedParam.bias = new dType[learnedParam.bias_data_num];
  for (int i = 0; i < learnedParam.bias_data_num; i++) {
    learnedParam.bias[i] = (dType)data["bias"]["data"][i].asDouble();
  }
  return learnedParam;
}

NetParam createNetParam(Json::Value data) {
  NetParam param;
  param.scale = (dType)data["scale"].asDouble();
  param.stride = data["stride"].asInt();
  param.kernelSize = data["kernel_size"].asInt();
  param.dilation = data["dilation"].asInt();
  param.pad = data["pad"].asInt();
  param.inputChannel = data["input_channel"].asInt();
  param.inputHeight = data["input_height"].asInt();
  param.inputWidth = data["input_width"].asInt();
  param.outputChannel = data["output_channel"].asInt();
  param.outputHeight = data["output_height"].asInt();
  param.outputWidth = data["output_width"].asInt();
  param.inputTotalDataNum = data["input_fm_data_num"].asInt();
  param.outputTotalDataNum = data["output_fm_data_num"].asInt();
  return param;
}



Layer::Layer(Json::Value data) {
  string ts = data.get("type", "").asString();
  if (ts == "Convolution") {
    type = Convolution;
    kernelKey = "convLayer";
  } else if (ts == "ReLU") {
    type = Relu;
    kernelKey = "reluLayer";
  } else if (ts == "Data") {
    type = Data;
    kernelKey = "dataLayer";
  } else if (ts == "Pooling") {
    type = Pooling;
    kernelKey = "poolingLayer";
  } else if (ts == "Padding") {
    type = Padding;
    kernelKey = "paddingLayer";
  } else if (ts == "Output") {
    type = Output;
    kernelKey = "outputLayer";
  } else {
    ERROR_LOG << "Unknown Layer Type: " << ts << endl;
    return;
  }
  info = data["info"].asString();
  learnedParam = createWeightData(data);
  param = createNetParam(data["param"]);
  outputBuffer = new dType[param.outputTotalDataNum];
  // set memory to zero
  memset(outputBuffer, 0, param.outputTotalDataNum * sizeof(dType));
  inputBuffer = NULL;
  next = NULL;
  prev = NULL;
  weightCL = NULL;
  biasCL = NULL;
  outputBufferCL = NULL;
  inputBufferCL = NULL;
  paramCL = NULL;
  //     globalSize[3], localSize[3], offset[3];
  //    Json::Value globalSizeJson = data["config"]["global_size"];
  //    globalSize[0] =  globalSizeJson[0].asUInt();
  SET_CL_3D_SIZE(data["config"], globalSize, "global_size");
  SET_CL_3D_SIZE(data["config"], localSize, "local_size");
  SET_CL_3D_SIZE(data["config"], offset, "offset");
}

Layer::~Layer() {
  delete outputBuffer;
  delete learnedParam.weight;
  delete learnedParam.bias;
  delete learnedParam.weightShape;
  delete learnedParam.biasShape;
}



bool Layer::freeCLMemory() {
  cl_int err = CL_SUCCESS;
  CL_RELEASE(outputBufferCL);
  CL_RELEASE(inputBufferCL);
  CL_RELEASE(weightCL);
  CL_RELEASE(biasCL);
  CL_RELEASE(paramCL);
  CL_RELEASE(phaseCL);
  return err == CL_SUCCESS;
}

bool Layer::forward(oclHardware hardware, oclSoftware software,
                    OpenCLVersion mode, NetLogging log) {
  if (log == LAYER || log == DEBUG) {
    INFO_LOG << "    Forward Pass: " << info << endl;
  }
  if (prev != NULL) {
    inputBuffer = prev->outputBuffer;
  }
  if (inputBuffer == NULL) {
    ERROR_LOG << "inputBuffer = NULL!" << endl;
    return false;
  }
  cl_int err = CL_SUCCESS;

  size_t inputSize = 1, outputSize = 1;
  if (mode == OCL12 || log == DEBUG) {
    outputSize = (size_t)param.outputTotalDataNum;
    inputSize = (size_t)param.inputTotalDataNum;
  } else {
    if (type == Output) {
      outputSize = (size_t)param.outputTotalDataNum;
    } else if (type == Data) {
      inputSize = (size_t)param.inputTotalDataNum;
    }
  }
  inputBufferCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY,
                                   inputSize * sizeof(dType), NULL, &err);
  FORWARD_ERROR_CHECK;
  outputBufferCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_WRITE_ONLY,
                                    outputSize * sizeof(dType), NULL, &err);
  FORWARD_ERROR_CHECK;
  paramCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY,
                             sizeof(NetParam), NULL, &err); // works good
  FORWARD_ERROR_CHECK;
  phaseCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY, sizeof(int),
                             NULL, &err); // works good
  FORWARD_ERROR_CHECK;
  CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, inputBufferCL, CL_TRUE, 0,
                          inputSize * sizeof(dType), (void *)inputBuffer, 0,
                          NULL, NULL);
  CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, paramCL, CL_TRUE, 0,
                          sizeof(NetParam), (void *)&param, 0, NULL,
                          NULL); // works good
  CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, phaseCL, CL_TRUE, 0, sizeof(int),
                          (void *)&phase, 0, NULL, NULL);

  // Set Kernel Arg
  cl_kernel kernel = (*software.kernelMap)[kernelKey];
  size_t argCounter = 0;
  CL_KERNEL_ARG(kernel, sizeof(cl_mem), &inputBufferCL);
  CL_KERNEL_ARG(kernel, sizeof(cl_mem), &outputBufferCL);
  if (type == Convolution) {
    weightCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY,
                                learnedParam.weight_data_num * sizeof(dType),
                                NULL, &err);
    biasCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY,
                              learnedParam.bias_data_num * sizeof(dType), NULL,
                              &err);
    FORWARD_ERROR_CHECK;
    CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, weightCL, CL_TRUE, 0,
                            learnedParam.weight_data_num * sizeof(dType),
                            learnedParam.weight, 0, NULL, NULL);
    CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, biasCL, CL_TRUE, 0,
                            learnedParam.bias_data_num * sizeof(dType),
                            learnedParam.bias, 0, NULL, NULL);
    CL_KERNEL_ARG(kernel, sizeof(cl_mem), &weightCL);
    CL_KERNEL_ARG(kernel, sizeof(cl_mem), &biasCL);
  }
  CL_KERNEL_ARG(kernel, sizeof(cl_mem), &paramCL);
  CL_KERNEL_ARG(kernel, sizeof(cl_mem), &phaseCL);

  FORWARD_ERROR_CHECK
  // Enqueue Kernel
  KERNEL_ENQUEUE(hardware.mQueue, kernel, 3, offset, globalSize, localSize, 0,
                 NULL, NULL);

  // Output Feature Map
  if (outputSize > 1) {
    CL_FINISH(hardware.mQueue);
    CL_ENQUEUE_READ_BUFFER(hardware.mQueue, outputBufferCL, CL_TRUE, 0,
                           outputSize * sizeof(dType), outputBuffer, 0, NULL,
                           NULL);
    CL_FINISH(hardware.mQueue);
    if(log == DEBUG){
        cout<<"Input: "<<endl;
        print2D(inputBuffer, param.inputHeight, param.inputWidth);
        DOTTED_LINE;
        cout<<"Output: "<<endl;
        print2D(outputBuffer, param.outputHeight, param.outputWidth);
        SOLID_LINE;
    }
    freeCLMemory();
  }

  // Ping Pong
  if (next) {
    next->phase = !phase;
  }
  return err == CL_SUCCESS;
}

Net::Net(Json::Value data, cmdArg arg) {
  name = string(arg.network);
  mode = arg.openclVersion;
  if (mode == OCL12) {
      INFO_LOG<<"Building Network with Opencl 1.2"<<endl;
  }
  else {
      INFO_LOG<<"Building Network with Opencl 2.0, with memory optimization with pingpong buffer."<<endl;
  }
  num_layers = data["num_layers"].asInt();
  layers = new Layer *[num_layers];
  Layer *current = NULL;
  for (int i = 0; i < num_layers; i++) {
    layers[i] = new Layer(data["layers"][i]);
    current = layers[i];
    if (i > 0) {
      layers[i - 1]->next = current;
      current->prev = layers[i - 1];
    }
  }
}

Net::~Net() {
  for (int i = 0; i < num_layers; i++)
    delete layers[i];
  delete layers;
}

bool Net::freeCLMemory() {
  bool result = true;
  for (int i = 0; i < num_layers; i++) {
    result = layers[i]->freeCLMemory();
    if (!result) {
      ERROR_LOG << "FREE CL MEMORY ERROR in " << layers[i]->info << endl;
    }
  }
  return result;
}

bool Net::forward(oclHardware hardware, oclSoftware software, dType *data,
                  NetLogging log) {
  if (log != NO) {
    INFO_LOG << "Forward Pass: " << name << endl;
  }
  layers[0]->inputBuffer = data;
  layers[0]->phase = 0;
  bool result = true;
  for (int i = 0; i < num_layers; i++) {
    result = layers[i]->forward(hardware, software, mode, log);
    if (!result) {
      ERROR_LOG << layers[i]->type << ": " << layers[i]->info << endl;
      return false;
    }
  }
  result = freeCLMemory();
  return result;
}

Layer* Net::outputLayer() { return layers[num_layers - 1]; }

void softmax(dType *input, dType *output, int size) {
  dType sum = 0;
  for (int i = 0; i < size; i++) {
    sum += exp(input[i]);
  }
  for (int i = 0; i < size; i++) {
    output[i] = exp(input[i]) / sum;
  }
}

void print2D(dType *fm, int height, int width) {
  cout << "\n\n";
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cout <<std::fixed << std::setprecision(2)<< fm[i * width + j] << ",";
    }
    cout << endl;
  }
  cout << "\n\n";
}
