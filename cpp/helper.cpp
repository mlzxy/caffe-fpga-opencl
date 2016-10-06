//
// Created by 张鑫语 on 8/20/16.
//
#include "helper.h"

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

bool endsWith(const string &s, const string &suffix) {
    return s.size() >= suffix.size() && s.rfind(suffix) == (s.size() - suffix.size());
}

string trim(string &str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

static const pair<cl_int, std::string> map_pairs[] = {
        make_pair(CL_SUCCESS, TO_STRING(CL_SUCCESS)),
        make_pair(CL_DEVICE_NOT_FOUND, TO_STRING(CL_DEVICE_NOT_FOUND)),
        make_pair(CL_DEVICE_NOT_AVAILABLE, TO_STRING(CL_DEVICE_NOT_AVAILABLE)),
        make_pair(CL_COMPILER_NOT_AVAILABLE, TO_STRING(CL_COMPILER_NOT_AVAILABLE)),
        make_pair(CL_MEM_OBJECT_ALLOCATION_FAILURE, TO_STRING(CL_MEM_OBJECT_ALLOCATION_FAILURE)),
        make_pair(CL_OUT_OF_RESOURCES, TO_STRING(CL_OUT_OF_RESOURCES)),
        make_pair(CL_OUT_OF_HOST_MEMORY, TO_STRING(CL_OUT_OF_HOST_MEMORY)),
        make_pair(CL_PROFILING_INFO_NOT_AVAILABLE, TO_STRING(CL_PROFILING_INFO_NOT_AVAILABLE)),
        make_pair(CL_MEM_COPY_OVERLAP, TO_STRING(CL_MEM_COPY_OVERLAP)),
        make_pair(CL_IMAGE_FORMAT_MISMATCH, TO_STRING(CL_IMAGE_FORMAT_MISMATCH)),
        make_pair(CL_IMAGE_FORMAT_NOT_SUPPORTED, TO_STRING(CL_IMAGE_FORMAT_NOT_SUPPORTED)),
        make_pair(CL_BUILD_PROGRAM_FAILURE, TO_STRING(CL_BUILD_PROGRAM_FAILURE)),
        make_pair(CL_MAP_FAILURE, TO_STRING(CL_MAP_FAILURE)),
        make_pair(CL_MISALIGNED_SUB_BUFFER_OFFSET, TO_STRING(CL_MISALIGNED_SUB_BUFFER_OFFSET)),
        make_pair(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST, TO_STRING(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_W)),
        make_pair(CL_INVALID_VALUE, TO_STRING(CL_INVALID_VALUE)),
        make_pair(CL_INVALID_DEVICE_TYPE, TO_STRING(CL_INVALID_DEVICE_TYPE)),
        make_pair(CL_INVALID_PLATFORM, TO_STRING(CL_INVALID_PLATFORM)),
        make_pair(CL_INVALID_DEVICE, TO_STRING(CL_INVALID_DEVICE)),
        make_pair(CL_INVALID_CONTEXT, TO_STRING(CL_INVALID_CONTEXT)),
        make_pair(CL_INVALID_QUEUE_PROPERTIES, TO_STRING(CL_INVALID_QUEUE_PROPERTIES)),
        make_pair(CL_INVALID_COMMAND_QUEUE, TO_STRING(CL_INVALID_COMMAND_QUEUE)),
        make_pair(CL_INVALID_HOST_PTR, TO_STRING(CL_INVALID_HOST_PTR)),
        make_pair(CL_INVALID_MEM_OBJECT, TO_STRING(CL_INVALID_MEM_OBJECT)),
        make_pair(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, TO_STRING(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)),
        make_pair(CL_INVALID_IMAGE_SIZE, TO_STRING(CL_INVALID_IMAGE_SIZE)),
        make_pair(CL_INVALID_SAMPLER, TO_STRING(CL_INVALID_SAMPLER)),
        make_pair(CL_INVALID_BINARY, TO_STRING(CL_INVALID_BINARY)),
        make_pair(CL_INVALID_BUILD_OPTIONS, TO_STRING(CL_INVALID_BUILD_OPTIONS)),
        make_pair(CL_INVALID_PROGRAM, TO_STRING(CL_INVALID_PROGRAM)),
        make_pair(CL_INVALID_PROGRAM_EXECUTABLE, TO_STRING(CL_INVALID_PROGRAM_EXECUTABLE)),
        make_pair(CL_INVALID_KERNEL_NAME, TO_STRING(CL_INVALID_KERNEL_NAME)),
        make_pair(CL_INVALID_KERNEL_DEFINITION, TO_STRING(CL_INVALID_KERNEL_DEFINITION)),
        make_pair(CL_INVALID_KERNEL, TO_STRING(CL_INVALID_KERNEL)),
        make_pair(CL_INVALID_ARG_INDEX, TO_STRING(CL_INVALID_ARG_INDEX)),
        make_pair(CL_INVALID_ARG_VALUE, TO_STRING(CL_INVALID_ARG_VALUE)),
        make_pair(CL_INVALID_ARG_SIZE, TO_STRING(CL_INVALID_ARG_SIZE)),
        make_pair(CL_INVALID_KERNEL_ARGS, TO_STRING(CL_INVALID_KERNEL_ARGS)),
        make_pair(CL_INVALID_WORK_DIMENSION, TO_STRING(CL_INVALID_WORK_DIMENSION)),
        make_pair(CL_INVALID_WORK_GROUP_SIZE, TO_STRING(CL_INVALID_WORK_GROUP_SIZE)),
        make_pair(CL_INVALID_WORK_ITEM_SIZE, TO_STRING(CL_INVALID_WORK_ITEM_SIZE)),
        make_pair(CL_INVALID_GLOBAL_OFFSET, TO_STRING(CL_INVALID_GLOBAL_OFFSET)),
        make_pair(CL_INVALID_EVENT_WAIT_LIST, TO_STRING(CL_INVALID_EVENT_WAIT_LIST)),
        make_pair(CL_INVALID_EVENT, TO_STRING(CL_INVALID_EVENT)),
        make_pair(CL_INVALID_OPERATION, TO_STRING(CL_INVALID_OPERATION)),
        make_pair(CL_INVALID_GL_OBJECT, TO_STRING(CL_INVALID_GL_OBJECT)),
        make_pair(CL_INVALID_BUFFER_SIZE, TO_STRING(CL_INVALID_BUFFER_SIZE)),
        make_pair(CL_INVALID_MIP_LEVEL, TO_STRING(CL_INVALID_MIP_LEVEL)),
        make_pair(CL_INVALID_GLOBAL_WORK_SIZE, TO_STRING(CL_INVALID_GLOBAL_WORK_SIZE)),
        make_pair(CL_INVALID_PROPERTY, TO_STRING(CL_INVALID_PROPERTY))};

static const std::map<cl_int, std::string> clErrorCodes(map_pairs,
                                                        map_pairs + sizeof(map_pairs) / sizeof(map_pairs[0]));

const char *clErrorCode(cl_int code) {
    map<cl_int, string>::const_iterator iter = clErrorCodes.find(code);
    if (iter == clErrorCodes.end())
        return "UNKNOWN ERROR";
    else
        return iter->second.c_str();
}

int loadFile2Memory(const char *filename, char **result) {
    int size = 0;

    std::ifstream stream(filename, std::ifstream::binary);
    if (!stream) {
        cout << "loadFile2Memory Error: " << filename << endl;
        return -1;
    }
    stream.seekg(0, stream.end);
    size = stream.tellg();
    stream.seekg(0, stream.beg);
    *result = new char[size + 1];
    stream.read(*result, size);
    stream.close();
    (*result)[size] = 0;
    cout << "INFO: loadFile2Memory " << filename << " successfully" << endl;
    return size;
}


void release(oclSoftware &software) {
    // for loop on software.kernalMap
    for (itKernelMap iterator = software.kernelMap->begin(); iterator != software.kernelMap->end();
         iterator++) {
        clReleaseKernel(iterator->second);
    }
    delete software.kernelMap;
    clReleaseProgram(software.mProgram);
}


void release(oclHardware &hardware) {
    clReleaseCommandQueue(hardware.mQueue);
    clReleaseContext(hardware.mContext);
    if ((hardware.mMajorVersion >= 1) && (hardware.mMinorVersion > 1)) {
        // Only available in OpenCL >= 1.2
        clReleaseDevice(hardware.mDevice);
    }
}


cl_int compileProgram(const oclHardware &hardware, oclSoftware &software) {
    cl_int err = clBuildProgram(software.mProgram, 1, &hardware.mDevice, software.mCompileOptions, 0, 0);
    if (err != CL_SUCCESS) {
        REPORT_ERR_NR("clBuildProgram");
        size_t size = 0;
        err = clGetProgramBuildInfo(software.mProgram, hardware.mDevice, CL_PROGRAM_BUILD_LOG, 0, 0, &size);
        REPORT_ERRM(err, "clGetProgramBuildInfo");

        std::vector<char> log(size + 1);
        err = clGetProgramBuildInfo(software.mProgram, hardware.mDevice, CL_PROGRAM_BUILD_LOG, size, &log[0], 0);
        REPORT_ERRM(err, "clGetProgramBuildInfo Second");
        std::cout << &log[0] << "\n";
        return err;
    }
    //kernel name must match
    cout << "Iterating through " << "kernelMap to create kernels, numOfKernels = " << software.kernelMap->size()
         << endl;
    for (itKernelMap iterator = software.kernelMap->begin(); iterator != software.kernelMap->end();
         iterator++) {
        cout << "Creating Kernel: " << iterator->first.c_str() << endl;
        (*software.kernelMap)[iterator->first] = clCreateKernel(software.mProgram, iterator->first.c_str(), &err);
        if ((*software.kernelMap)[iterator->first] == 0) {
            ERROR_LOG << "clCreateKernel on " + iterator->first << " failed" << endl;
        } else {
            DEBUG_LOG << "Creating Kernel: " << iterator->first.c_str() << " Successfully" << endl;
        }
    }
    return CL_SUCCESS;
}

void getDeviceVersion(oclHardware &hardware) {
    char versionString[512];
    size_t size = 0;
    cl_int err = clGetDeviceInfo(hardware.mDevice, CL_DEVICE_VERSION, 511, versionString, &size);
    REPORT_ERRM(, "clGetDeviceInfo");

    unsigned major = 0;
    unsigned minor = 0;
    unsigned state = 0;
    for (size_t i = 0; i < size; i++) {
        if (versionString[i] == ' ') {
            state++;
            continue;
        }
        if (versionString[i] == '.') {
            state++;
            continue;
        }
        if (state == 0) {
            continue;
        }
        if (state == 1) {
            major *= 10;
            major += (versionString[i] - '0');
            continue;
        }
        if (state == 2) {
            minor *= 10;
            minor += (versionString[i] - '0');
            continue;
        }
        break;
    }
    hardware.mMajorVersion = major;
    hardware.mMinorVersion = minor;
}


oclHardware getOclHardware(cl_device_type type, const char *target_device) {
    oclHardware hardware = {0, 0, 0, 0, 0, 0, 0};
    cl_platform_id platforms[16] = {0};
    cl_device_id devices[16];
    cl_device_id device_id;
    char platformName[256];
    char deviceName[256];
    cl_uint platformCount = 0;
    cl_int err;
    printTitle("Hardware Detecting and Choosing");
    err = clGetPlatformIDs(16, platforms, &platformCount);
    REPORT_ERRM(hardware, "clGetPlatformIDs");
    bool platformSelected = false;
    for (cl_uint i = 0; i < platformCount; i++) {
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 256, platformName, 0);
        REPORT_ERRM(hardware, "clGetPlatformInfo");
        cout << "DEBUG: " << "Available Platform Found: " << platformName << endl;
        cl_uint deviceCount = 0;
        err = clGetDeviceIDs(platforms[i], type, 16, devices, &deviceCount);
        REPORT_ERRM(hardware, string("clGetDeviceIDs at platform: ") + string(platformName));
        cl_uint idev;
        for (idev = 0; idev < deviceCount; idev++) {
            err = clGetDeviceInfo(devices[idev], CL_DEVICE_NAME, 256, deviceName, 0);
            REPORT_ERRM(hardware, "clGetDeviceInfo:" + string(deviceName));
            std::cout << "DEBUG: " << platformName << " Hardware Founded: " << deviceName << std::endl;
        }
    }

    for (cl_uint i = 0; i < platformCount; i++) {
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 256, platformName, 0);
        REPORT_ERRM(hardware, "clGetPlatformInfo");
        if (strcmp(platformName, PLATFORM_FILTER) != 0) {
            std::cout << "DEBUG: " << platformName << "!=" << PLATFORM_FILTER << endl;
            continue;
        }
        platformSelected = true;
        std::cout << "INFO: Selected platform " << platformName << std::endl;

        cl_uint deviceCount = 0;
        err = clGetDeviceIDs(platforms[i], type, 16, devices, &deviceCount);
        REPORT_ERRM(hardware, string("clGetDeviceIDs:") + string(platformName));
        if (deviceCount == 0) {
            continue;
        }

        //iterate all devices to find the target device
        cl_uint idev;
        for (idev = 0; idev < deviceCount; idev++) {
            err = clGetDeviceInfo(devices[idev], CL_DEVICE_NAME, 256, deviceName, 0);
            REPORT_ERRM(hardware, "clGetDeviceInfo:" + string(deviceName));
            if (strcmp(deviceName, target_device) == 0) {
                cout << "INFO: Selected Device " << deviceName << endl;
                device_id = devices[i];
                break;
            }
            if (DISABLE_DEVICE_FILTER) {
                device_id = devices[i];
                break;
            }
        }
        if (idev == deviceCount) {
            std::cout << "ERROR: target device " << target_device << " not found " << std::endl;
            return hardware;
        }

        cl_context_properties contextData[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platforms[i], 0};
        cl_context context = clCreateContextFromType(contextData, type, 0, 0, &err);
        REPORT_ERRM(hardware, "clCreateContextFromType");
        cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &err);
        REPORT_ERRM(hardware, "clCreateCommandQueue");

        hardware.mPlatform = platforms[i];
        hardware.mContext = context;
        hardware.mDevice = device_id;
        hardware.mQueue = queue;
        hardware.deviceType = type;
        getDeviceVersion(hardware);
        printTitle("Platform Hardware Info");
        std::cout << "INFO: Platform = " << platformName << "\n";
        std::cout << "INFO: Device = " << deviceName << "\n";
        std::cout << "INFO: OpenCL Version = " << hardware.mMajorVersion << '.' << hardware.mMinorVersion << "\n";
        return hardware;
    }
    if (!platformSelected)
        cout << "ERROR: No qualified platform founded!!" << endl;
    return hardware;
}


oclSoftware getOclSoftware(const oclHardware &hardware, const char *kernelNames, const char *kernelFileName) {
    unsigned char *kernelCode = 0;

    oclSoftware soft;
    std::memset(&soft, 0, sizeof(oclSoftware));
    std::strcpy(soft.mFileName, kernelFileName);
    std::string kernelNameListString(kernelNames);
    vector<std::string> kernelNameList;
    split(kernelNameListString, ',', kernelNameList);
    soft.kernelMap = new map<std::string, cl_kernel>;
    //****************************//
    for (size_t i = 0; i < kernelNameList.size(); i++) {
        (*soft.kernelMap)[kernelNameList[i]] = 0;
    }
    // std::strcpy(soft.mKernelName, kernelName);
    cl_int err;

    std::sprintf(soft.mCompileOptions, OPENCL_COMPILE_OPTION);
    if (hardware.deviceType == CL_DEVICE_TYPE_CPU) {
        std::sprintf(soft.mCompileOptions, "-g");
    }

    printTitle("Kernel Compilation");
    std::cout << "INFO: Loading " << kernelFileName << "\n";
    int size = loadFile2Memory(kernelFileName, (char **) &kernelCode);
    if (size < 0) {
        std::cout << "ERROR: Failed to load kernel\n";
        return soft;
    }

    if (endsWith(string(soft.mFileName), ".xclbin")) {
        size_t n = size;
        soft.mProgram = clCreateProgramWithBinary(hardware.mContext, 1, &hardware.mDevice, &n,
                                                  (const unsigned char **) &kernelCode, 0, &err);
    } else {
        soft.mProgram = clCreateProgramWithSource(hardware.mContext, 1, (const char **) &kernelCode, 0, &err);
    }
    if (!soft.mProgram || (err != CL_SUCCESS)) {
        std::cout << "ERROR: " << clErrorCode(err) << " on Program Creation \n";
        return soft;
    }


    err = compileProgram(hardware, soft);
    REPORT_ERRM(soft, "compileProgram");
    delete[] kernelCode;
    return soft;
}


void printHelp() {
    cout << "usage: %s <options>\n";
    cout << "  -d <cpu|gpu|fpga>\n";
    cout << "  -f <kernel_file> \n";
    cout << "  -k <kernel_name>\n";
    cout << "  -i <dir to read images>\n";
    cout << "  -h <print help info>\n";
}


cmdArg parseCmdArg(int argc, char **argv) {
    cmdArg arg;
    const static struct option long_options[] = {
            {"device",      required_argument, 0, 'd'},
            {"kernel_name", required_argument, 0, 'k'},
            {"kernel_file", required_argument, 0, 'f'},
            {"network",     optional_argument, 0, 'n'},
            {"data_dir",    optional_argument, 0, 'i'},
            {"help",        no_argument,       0, 'h'},
            {0, 0,                             0, 0}
    };
    int ch;
    // must pass -n like short argument, but not --network
    while ((ch = getopt_long(argc, argv, "d:k:f:h:i:n:", long_options, NULL)) != -1) {
        // check to see if a single character or long option came through
        switch (ch) {
            case 'k':
                strcpy(arg.mKernelName, optarg);
                break;
            case 'f':
                strcpy(arg.mFileName, optarg);
                break;
            case 'i':
                strcpy(arg.dataDir, optarg);
                break;
            case 'd':
                if (strcmp(optarg, "gpu") == 0)
                    arg.deviceType = DEVICE_GPU;
                else if (strcmp(optarg, "cpu") == 0)
                    arg.deviceType = DEVICE_CPU;
                else if (strcmp(optarg, "fpga") == 0)
                    arg.deviceType = DEVICE_FPGA;
                else {
                    cout << "Incorrect platform specified\n";
                    printHelp();
                    exit(1);
                }
                break;
            case 'n':
                strcpy(arg.network, optarg);
                break;
            case 'h':
            default:
                printHelp();
                exit(1);
        }
    }
    return arg;
}



double runProgram(int argc, char **argv, RunOpenCL F) {
    cmdArg arg = parseCmdArg(argc, argv);
    oclHardware hardware = getOclHardware(arg.deviceType, ALPHA_DATA_KU3_DDR1);
    oclSoftware software = getOclSoftware(hardware, arg.mKernelName, arg.mFileName);

    for (itKernelMap iterator = software.kernelMap->begin(); iterator != software.kernelMap->end();
         iterator++) {
        cout << "Kernel: " << iterator->first << " " << iterator->second << endl;
    }


    printTitle("OpenCL RunTime Environment");
    printCLConstant();
    printTitle("Entering Custom Host Code");
    double delay = (*F)(arg, hardware, software);
    release(software);
    release(hardware);
    return delay;
}


FileList ls(const std::string &pattern) {
    glob_t glob_result;
    glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    vector<string> files;
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        files.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
}


Mat readImage(string fpath) {
    return imread(fpath, CV_LOAD_IMAGE_GRAYSCALE);
}

size_t imageSize(Mat m) {
    return m.cols * m.rows * m.elemSize();
}


std::string getFileName(const std::string &s) {
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t i = s.rfind(sep, s.length());
    if (i != string::npos) {
        return (s.substr(i + 1, s.length() - i));
    }
    return ("");
}


/**
 * Network related implementation
 */
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
        learnedParam.weight[i] = (dType) data["weight"]["data"][i].asDouble();
    }
    learnedParam.bias = new dType[learnedParam.bias_data_num];
    for (int i = 0; i < learnedParam.bias_data_num; i++) {
        learnedParam.bias[i] = (dType) data["bias"]["data"][i].asDouble();
    }
    return learnedParam;
}

NetParam createNetParam(Json::Value data) {
    NetParam param;
    param.scale = (dType) data["scale"].asDouble();
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


#define SET_3D(array, x, y, z) array[0]=(x); array[1]=(y); array[2]=(z);
#define SET_CL_3D_SIZE(data, offset, key) \
    SET_3D(offset, \
        data[key][0].asUInt(), \
        data[key][1].asUInt(), \
        data[key][2].asUInt());


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
    //set memory to zero
    memset(outputBuffer, 0, param.outputTotalDataNum*sizeof(dType));
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

#define FORWARD_ERROR_CHECK REPORT_ERR(false)
#define CL_RELEASE(x)       \
if(x != NULL)               \
    err = clReleaseMemObject(x);  \
x = NULL;

#define CL_KERNEL_ARG(kernel,size, mem) err = clSetKernelArg(kernel, argCounter, size, mem); \
argCounter++; \
FORWARD_ERROR_CHECK;

#define CL_CREATE_BUFFER(...)  clCreateBuffer(__VA_ARGS__);

#define CL_ENQUEUE_WRITE_BUFFER(...) err = clEnqueueWriteBuffer(__VA_ARGS__); \
FORWARD_ERROR_CHECK;

#define CL_ENQUEUE_READ_BUFFER(...) err = clEnqueueReadBuffer(__VA_ARGS__); \
FORWARD_ERROR_CHECK;

#define CL_FINISH(...) err = clFinish(__VA_ARGS__); \
FORWARD_ERROR_CHECK;

#define KERNEL_ENQUEUE(...)  err = clEnqueueNDRangeKernel(__VA_ARGS__); \
FORWARD_ERROR_CHECK;

bool Layer::freeCLMemory() {
    cl_int err = CL_SUCCESS;
    CL_RELEASE(outputBufferCL);
    CL_RELEASE(inputBufferCL);
    CL_RELEASE(weightCL);
    CL_RELEASE(biasCL);
    CL_RELEASE(paramCL);
    return err == CL_SUCCESS;
}


bool Layer::forward(oclHardware hardware, oclSoftware software, OpenCLVersion mode, NetLogging log) {
    if(log == LAYER) {
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

    size_t inputSize = 0, outputSize = 0;
    if (mode == OCL12) {
        outputSize = (size_t) param.outputTotalDataNum;
        inputSize = (size_t) param.inputTotalDataNum;
    } else {
        if (type == Output) {
            outputSize = (size_t) param.outputTotalDataNum;
        } else if (type == Data) {
            inputSize = (size_t) param.inputTotalDataNum;
        }
    }
    inputBufferCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY, inputSize * sizeof(dType), NULL, &err);
    FORWARD_ERROR_CHECK;
    outputBufferCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_WRITE_ONLY, outputSize * sizeof(dType), NULL, &err);
    FORWARD_ERROR_CHECK;
    paramCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY, sizeof(NetParam), NULL, &err); // works good
    FORWARD_ERROR_CHECK;


    //Input Feature Map
//    dType *_inputBuffer = new dType[784];
//    _inputBuffer[0] = 10.0;
//    for(int i = 0; i< 784;i++){
//        _inputBuffer[i] = (float)inputBuffer[i];
//    }
//    print2D(_inputBuffer, 28,28);
    CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, inputBufferCL, CL_TRUE, 0, inputSize*sizeof(dType), (void *)inputBuffer, 0, NULL, NULL);
    CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, paramCL, CL_TRUE, 0, sizeof(NetParam), (void *)&param, 0, NULL, NULL); //works good



    //Set Kernel Arg
    cl_kernel kernel = (*software.kernelMap)[kernelKey];
    size_t argCounter = 0;
    CL_KERNEL_ARG(kernel,  sizeof(cl_mem), &inputBufferCL);
    CL_KERNEL_ARG(kernel,  sizeof(cl_mem), &outputBufferCL);
    if(type == Convolution){
        weightCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY, learnedParam.weight_data_num*sizeof(dType), NULL, &err);
        biasCL = CL_CREATE_BUFFER(hardware.mContext, CL_MEM_READ_ONLY, learnedParam.bias_data_num*sizeof(dType), NULL, &err);
        FORWARD_ERROR_CHECK;
        CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, weightCL, CL_TRUE, 0, learnedParam.weight_data_num*sizeof(dType), learnedParam.weight, 0, NULL, NULL);
        CL_ENQUEUE_WRITE_BUFFER(hardware.mQueue, biasCL, CL_TRUE, 0, learnedParam.bias_data_num*sizeof(dType), learnedParam.bias, 0, NULL, NULL);
        CL_KERNEL_ARG(kernel, sizeof(cl_mem), &weightCL);
        CL_KERNEL_ARG(kernel,  sizeof(cl_mem), &biasCL);
    }
    CL_KERNEL_ARG(kernel, sizeof(cl_mem), &paramCL);
    CL_KERNEL_ARG(kernel,  sizeof(int), &phase);


    FORWARD_ERROR_CHECK
    //Enqueue Kernel
    KERNEL_ENQUEUE(hardware.mQueue, kernel, 3, offset, globalSize, localSize, 0, NULL, NULL);

    //Output Feature Map
    if (outputSize > 0) {
        CL_FINISH(hardware.mQueue);
        CL_ENQUEUE_READ_BUFFER(hardware.mQueue, outputBufferCL, CL_TRUE, 0, outputSize*sizeof(dType), outputBuffer, 0, NULL, NULL);
        CL_FINISH(hardware.mQueue);
        freeCLMemory();
    }
    //Ping Pong
    if (next) {
        next->phase = !phase;
    }
    return err == CL_SUCCESS;
}

Net::Net(Json::Value data, cmdArg arg, OpenCLVersion version) {
    name = string(arg.network);
    mode = version;
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
        if(!result){
            ERROR_LOG<<"FREE CL MEMORY ERROR in "<<layers[i]->info<<endl;
        }
    }
    return result;
}

bool Net::forward(oclHardware hardware, oclSoftware software, dType *data, NetLogging log) {
    if(log != NO) {
        INFO_LOG << "Forward Pass: " << name << endl;
    }
    layers[0]->inputBuffer = data;
    layers[0]->phase = 0;
    bool result = true;
    for (int i = 0; i < num_layers; i++) {
//        if(i == 1){
//            cout<<"OUTPUT BUFFER BEFORE"<<endl;
//            print2D(layers[i]->outputBuffer, layers[i]->param.outputHeight, layers[i]->param.outputWidth);
//        }
        result = layers[i]->forward(hardware, software, mode, log);
        if (!result) {
            ERROR_LOG << layers[i]->type << ": " << layers[i]->info << endl;
            return false;
        }

        // for debugging //
//        if(i == 1){
//            cout<<"OUTPUT BUFFER AFTER"<<endl;
//            print2D(layers[i]->outputBuffer, layers[i]->param.outputHeight, layers[i]->param.outputWidth);
//        }
        // for debugging //
    }
    result = freeCLMemory();
    return result;
}

Layer *Net::outputLayer() {
    return layers[num_layers - 1];
}

void print2D(dType *fm, int height, int width){
    cout<<"\n\n";
    for(int i = 0; i<height;i++){
        for(int j = 0;j<width;j++){
            cout<<fm[i*width+j]<<",";
        }
        cout<<endl;
    }
    cout<<"\n\n";
}

void softmax(dType *input, dType *output, int size){
    dType sum = 0;
    for (int i = 0; i<size;i++){
        sum += exp(input[i]);
    }
    for (int i = 0; i<size;i++){
        output[i] = exp(input[i])/sum;
    }
}