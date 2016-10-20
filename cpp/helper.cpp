//
// Created by 张鑫语 on 8/20/16.
//
#include "helper.h"
#include "custom.h"

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

bool endsWith(const string &s, const string &suffix) {
    return s.size() >= suffix.size() &&
           s.rfind(suffix) == (s.size() - suffix.size());
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
        make_pair(CL_MEM_OBJECT_ALLOCATION_FAILURE,
                  TO_STRING(CL_MEM_OBJECT_ALLOCATION_FAILURE)),
        make_pair(CL_OUT_OF_RESOURCES, TO_STRING(CL_OUT_OF_RESOURCES)),
        make_pair(CL_OUT_OF_HOST_MEMORY, TO_STRING(CL_OUT_OF_HOST_MEMORY)),
        make_pair(CL_PROFILING_INFO_NOT_AVAILABLE,
                  TO_STRING(CL_PROFILING_INFO_NOT_AVAILABLE)),
        make_pair(CL_MEM_COPY_OVERLAP, TO_STRING(CL_MEM_COPY_OVERLAP)),
        make_pair(CL_IMAGE_FORMAT_MISMATCH, TO_STRING(CL_IMAGE_FORMAT_MISMATCH)),
        make_pair(CL_IMAGE_FORMAT_NOT_SUPPORTED,
                  TO_STRING(CL_IMAGE_FORMAT_NOT_SUPPORTED)),
        make_pair(CL_BUILD_PROGRAM_FAILURE, TO_STRING(CL_BUILD_PROGRAM_FAILURE)),
        make_pair(CL_MAP_FAILURE, TO_STRING(CL_MAP_FAILURE)),
        make_pair(CL_MISALIGNED_SUB_BUFFER_OFFSET,
                  TO_STRING(CL_MISALIGNED_SUB_BUFFER_OFFSET)),
        make_pair(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST,
                  TO_STRING(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_W)),
        make_pair(CL_INVALID_VALUE, TO_STRING(CL_INVALID_VALUE)),
        make_pair(CL_INVALID_DEVICE_TYPE, TO_STRING(CL_INVALID_DEVICE_TYPE)),
        make_pair(CL_INVALID_PLATFORM, TO_STRING(CL_INVALID_PLATFORM)),
        make_pair(CL_INVALID_DEVICE, TO_STRING(CL_INVALID_DEVICE)),
        make_pair(CL_INVALID_CONTEXT, TO_STRING(CL_INVALID_CONTEXT)),
        make_pair(CL_INVALID_QUEUE_PROPERTIES,
                  TO_STRING(CL_INVALID_QUEUE_PROPERTIES)),
        make_pair(CL_INVALID_COMMAND_QUEUE, TO_STRING(CL_INVALID_COMMAND_QUEUE)),
        make_pair(CL_INVALID_HOST_PTR, TO_STRING(CL_INVALID_HOST_PTR)),
        make_pair(CL_INVALID_MEM_OBJECT, TO_STRING(CL_INVALID_MEM_OBJECT)),
        make_pair(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR,
                  TO_STRING(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)),
        make_pair(CL_INVALID_IMAGE_SIZE, TO_STRING(CL_INVALID_IMAGE_SIZE)),
        make_pair(CL_INVALID_SAMPLER, TO_STRING(CL_INVALID_SAMPLER)),
        make_pair(CL_INVALID_BINARY, TO_STRING(CL_INVALID_BINARY)),
        make_pair(CL_INVALID_BUILD_OPTIONS, TO_STRING(CL_INVALID_BUILD_OPTIONS)),
        make_pair(CL_INVALID_PROGRAM, TO_STRING(CL_INVALID_PROGRAM)),
        make_pair(CL_INVALID_PROGRAM_EXECUTABLE,
                  TO_STRING(CL_INVALID_PROGRAM_EXECUTABLE)),
        make_pair(CL_INVALID_KERNEL_NAME, TO_STRING(CL_INVALID_KERNEL_NAME)),
        make_pair(CL_INVALID_KERNEL_DEFINITION,
                  TO_STRING(CL_INVALID_KERNEL_DEFINITION)),
        make_pair(CL_INVALID_KERNEL, TO_STRING(CL_INVALID_KERNEL)),
        make_pair(CL_INVALID_ARG_INDEX, TO_STRING(CL_INVALID_ARG_INDEX)),
        make_pair(CL_INVALID_ARG_VALUE, TO_STRING(CL_INVALID_ARG_VALUE)),
        make_pair(CL_INVALID_ARG_SIZE, TO_STRING(CL_INVALID_ARG_SIZE)),
        make_pair(CL_INVALID_KERNEL_ARGS, TO_STRING(CL_INVALID_KERNEL_ARGS)),
        make_pair(CL_INVALID_WORK_DIMENSION, TO_STRING(CL_INVALID_WORK_DIMENSION)),
        make_pair(CL_INVALID_WORK_GROUP_SIZE,
                  TO_STRING(CL_INVALID_WORK_GROUP_SIZE)),
        make_pair(CL_INVALID_WORK_ITEM_SIZE, TO_STRING(CL_INVALID_WORK_ITEM_SIZE)),
        make_pair(CL_INVALID_GLOBAL_OFFSET, TO_STRING(CL_INVALID_GLOBAL_OFFSET)),
        make_pair(CL_INVALID_EVENT_WAIT_LIST,
                  TO_STRING(CL_INVALID_EVENT_WAIT_LIST)),
        make_pair(CL_INVALID_EVENT, TO_STRING(CL_INVALID_EVENT)),
        make_pair(CL_INVALID_OPERATION, TO_STRING(CL_INVALID_OPERATION)),
        make_pair(CL_INVALID_GL_OBJECT, TO_STRING(CL_INVALID_GL_OBJECT)),
        make_pair(CL_INVALID_BUFFER_SIZE, TO_STRING(CL_INVALID_BUFFER_SIZE)),
        make_pair(CL_INVALID_MIP_LEVEL, TO_STRING(CL_INVALID_MIP_LEVEL)),
        make_pair(CL_INVALID_GLOBAL_WORK_SIZE,
                  TO_STRING(CL_INVALID_GLOBAL_WORK_SIZE)),
        make_pair(CL_INVALID_PROPERTY, TO_STRING(CL_INVALID_PROPERTY))};

static const std::map<cl_int, std::string>
        clErrorCodes(map_pairs,
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
    for (itKernelMap iterator = software.kernelMap->begin();
         iterator != software.kernelMap->end(); iterator++) {
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
    cl_int err = clBuildProgram(software.mProgram, 1, &hardware.mDevice,
                                software.mCompileOptions, 0, 0);
    if (err != CL_SUCCESS) {
        ERR_CHECK("clBuildProgram");
        size_t size = 0;
        err = clGetProgramBuildInfo(software.mProgram, hardware.mDevice,
                                    CL_PROGRAM_BUILD_LOG, 0, 0, &size);
        ERR_CHECK("clGetProgramBuildInfo");

        std::vector<char> log(size + 1);
        err = clGetProgramBuildInfo(software.mProgram, hardware.mDevice,
                                    CL_PROGRAM_BUILD_LOG, size, &log[0], 0);
        ERR_CHECK("clGetProgramBuildInfo Second");
        std::cout << &log[0] << "\n";
        return err;
    }
    // kernel name must match
    cout << "Iterating through "
         << "kernelMap to create kernels, numOfKernels = "
         << software.kernelMap->size() << endl;
    for (itKernelMap iterator = software.kernelMap->begin();
         iterator != software.kernelMap->end(); iterator++) {
        cout << "Creating Kernel: " << iterator->first.c_str() << endl;
        (*software.kernelMap)[iterator->first] =
                clCreateKernel(software.mProgram, iterator->first.c_str(), &err);
        if ((*software.kernelMap)[iterator->first] == 0) {
            ERROR_LOG << "clCreateKernel on " + iterator->first << " failed" << endl;
        } else {
            DEBUG_LOG << "Creating Kernel: " << iterator->first.c_str()
                      << " Successfully" << endl;
        }
    }
    return CL_SUCCESS;
}

void getDeviceVersion(oclHardware &hardware) {
    char versionString[512];
    size_t size = 0;
    cl_int err = clGetDeviceInfo(hardware.mDevice, CL_DEVICE_VERSION, 511,
                                 versionString, &size);
    ERR_CHECK("clGetDeviceInfo");

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

oclHardware getOclHardware(cmdArg arg) {
    cl_device_type type = arg.deviceType;
    oclHardware hardware = {0, 0, 0, 0, 0, 0, 0};
    cl_platform_id platforms[16] = {0};
    cl_device_id devices[16];
    cl_device_id device_id;
    char platformName[256];
    char deviceName[256];
    cl_uint platformCount = 0;
    cl_int err;
    printTitle("Hardware Detecting");
    err = clGetPlatformIDs(16, platforms, &platformCount);
    ERR_CHECK("clGetPlatformIDs");
    bool platformSelected = false;
    for (cl_uint i = 0; i < platformCount; i++) {
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 256, platformName, 0);
        ERR_CHECK("clGetPlatformInfo");
        DEBUG_LOG << "Available Platform Found: " << platformName << endl;
        cl_uint deviceCount = 0;
        err = clGetDeviceIDs(platforms[i], type, 16, devices, &deviceCount);
        ERR_CHECK(string("clGetDeviceIDs at platform: ") + string(platformName));
        cl_uint idev;
        for (idev = 0; idev < deviceCount; idev++) {
            err = clGetDeviceInfo(devices[idev], CL_DEVICE_NAME, 256, deviceName, 0);
            ERR_CHECK("clGetDeviceInfo:" + string(deviceName));
            std::cout << "DEBUG: " << platformName
                      << " Hardware Founded: " << deviceName << std::endl;
        }
    }

    printTitle("Hardware Selecting - Platform");
    for (cl_uint i = 0; i < platformCount; i++) {
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 256, platformName, 0);
        ERR_CHECK("clGetPlatformInfo");
        if (strcmp(platformName, arg.platformName) != 0) {
            std::cout << "DEBUG: " << platformName << "!=" << arg.platformName << endl;
            continue;
        }
        platformSelected = true;
        std::cout << "INFO: Selected platform " << platformName << std::endl;

        cl_uint deviceCount = 0;
        err = clGetDeviceIDs(platforms[i], type, 16, devices, &deviceCount);
        ERR_CHECK(string("clGetDeviceIDs:") + string(platformName));
        if (deviceCount == 0) {
            continue;
        }

        // iterate all devices to find the target device
        cl_uint idev;
        bool found = false;
        printTitle("Hardware Selecting - Device");
        for (idev = 0; idev < deviceCount; idev++) {
            err = clGetDeviceInfo(devices[idev], CL_DEVICE_NAME, 256, deviceName, 0);
            ERR_CHECK("clGetDeviceInfo:" + string(deviceName));
            INFO_LOG << "Available Device: " << deviceName << endl;
            device_id = devices[i];
            found = true;
            if (idev >= arg.deviceID) {
                break;
            }
        }
        if (found) {
            INFO_LOG << "Choose Device: " << deviceName << endl;
        } else {
            ERROR_LOG << "Available Device not found." << endl;
            exit(1);
        }

        cl_context_properties contextData[3] = {
                CL_CONTEXT_PLATFORM, (cl_context_properties) platforms[i], 0};
        cl_context context = clCreateContextFromType(contextData, type, 0, 0, &err);
        ERR_CHECK("clCreateContextFromType");
        cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &err);
        ERR_CHECK("clCreateCommandQueue");

        hardware.mPlatform = platforms[i];
        hardware.mContext = context;
        hardware.mDevice = device_id;
        hardware.mQueue = queue;
        hardware.deviceType = type;
        getDeviceVersion(hardware);
        printTitle("Platform Hardware Info");
        std::cout << "INFO: Platform = " << platformName << "\n";
        std::cout << "INFO: Device = " << deviceName << "\n";
        std::cout << "INFO: OpenCL Version = " << hardware.mMajorVersion << '.'
                  << hardware.mMinorVersion << "\n";
        return hardware;
    }
    if (!platformSelected)
        cout << "ERROR: No qualified platform founded!!" << endl;
    return hardware;
}

oclSoftware getOclSoftware(const oclHardware &hardware, const char *kernelNames,
                           const char *kernelFileName) {
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

//  std::sprintf(soft.mCompileOptions, OPENCL_COMPILE_OPTION);
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
        soft.mProgram =
                clCreateProgramWithBinary(hardware.mContext, 1, &hardware.mDevice, &n,
                                          (const unsigned char **) &kernelCode, 0, &err);
    } else {
        soft.mProgram = clCreateProgramWithSource(
                hardware.mContext, 1, (const char **) &kernelCode, 0, &err);
    }
    if (!soft.mProgram || (err != CL_SUCCESS)) {
        std::cout << "ERROR: " << clErrorCode(err) << " on Program Creation \n";
        return soft;
    }

    err = compileProgram(hardware, soft);
    ERR_CHECK("compileProgram");
    delete[] kernelCode;
    return soft;
}

void printHelp() {
    cout << "usage: %s <options>\n";
    cout << "  -f <kernel_file>   kernel source/binary file path, required\n\n";

    cout << "  -k <kernel_name1,kernel_name2>  kernels to compile, required\n\n";

    cout << "  -n <network file>  network json file that converted by python script, required\n\n";

    cout << "  -d <cpu|gpu|fpga> device type, default cpu in apple, otherwise fpga\n\n";

//    cout << "  -r <true|false> release is true or false, default false\n\n";

    cout << "  -l <no|layer|net|debug> network logging level, debug will print feature map\n\n";

    cout << "  -c <0|1|2> device index to choose, choose first available device by default\n\n";

    cout << "  -v <12|20> opencl version\n\n";

    cout << "  -p <Xilinx|Apple|\'NVIDIA CUDA\'> platform to choose, by default Apple in mac, Xilinx otherwise.\n\n";

    cout << "  -i <extra information> parse it in your own custom run function\n\n";

    cout << "  -h <print help info>\n";
}

cmdArg parseCmdArg(int argc, char **argv) {
    cmdArg arg;
    strcpy(arg.platformName, DEFAULT_PLATFORM);
    arg.deviceID = 0;
    arg.networkLoggingLevel = DEBUG;
    arg.openclVersion = OCL12;
    arg.deviceType = DEFAULT_DEVICE;

    const static struct option long_options[] = {
            {"device_type",     required_argument, 0, 'd'},
            {"kernel_name",     required_argument, 0, 'k'},
            {"kernel_file",     required_argument, 0, 'f'},
            {"network",         required_argument, 0, 'n'},
            {"info",            required_argument, 0, 'i'},
            {"platform",        required_argument, 0, 'p'},
            {"help",            no_argument,       0, 'h'},
            {"choose_device",   required_argument, 0, 'c'},
//            {"release",         required_argument, 0, 'r'},
            {"network_logging", required_argument, 0, 'l'},
            {"opencl_version",  required_argument, 0, 'v'},
            {0, 0,                                 0, 0}
    };

    int ch;
    // must pass -n like short argument, but not --network
    bool kernelProvided = false, kernelFileProvided = false, networkProvided = false;
    int ocl_version;
    while ((ch = getopt_long(argc, argv, "d:k:f:h:i:n:p:r:l:v:", long_options, NULL)) !=
           -1) {
        switch (ch) {
            case 'k':
                strcpy(arg.mKernelName, optarg);
                kernelProvided = true;
                break;
            case 'f':
                strcpy(arg.mFileName, optarg);
                kernelFileProvided = true;
                break;
            case 'i':
                strcpy(arg.info, optarg);
                break;
            case 'd':
                if (strcmp(optarg, "gpu") == 0) {
                    arg.deviceType = DEVICE_GPU;
                } else if (strcmp(optarg, "cpu") == 0) {
                    arg.deviceType = DEVICE_CPU;
                } else if (strcmp(optarg, "fpga") == 0) {
                    arg.deviceType = DEVICE_FPGA;
                } else {
                    ERROR_LOG << "Incorrect platform specified" << endl;
                    printHelp();
                    exit(1);
                }
                break;
            case 'n':
                strcpy(arg.network, optarg);
                networkProvided = true;
                break;
            case 'p':
                strcpy(arg.platformName, optarg);
                break;
            case 'c':
                try {
                    arg.deviceID = std::stoi(optarg);
                } catch (std::exception const &e) {
                    INFO_LOG << "Parsing Device Id failed, use 0" << endl;
                    arg.deviceID = 0;
                }
                break;
//            case 'r':
//                istringstream(optarg) >> std::boolalpha >> arg.debug;
//                arg.debug = !arg.debug; //because the flag is release
//                break;
            case 'l':
                if (strcmp(optarg, "no") == 0)
                    arg.networkLoggingLevel = NO;
                else if (strcmp(optarg, "layer") == 0)
                    arg.networkLoggingLevel = LAYER;
                else if (strcmp(optarg, "net") == 0)
                    arg.networkLoggingLevel = NET;
                else if (strcmp(optarg, "debug") == 0)
                    arg.networkLoggingLevel = DEBUG;
                break;
            case 'v':
                ocl_version = std::stoi(optarg);
                try {
                    ocl_version = std::stoi(optarg);
                } catch (std::exception const &e) {
                    INFO_LOG << "Parsing Opencl verison failed, use 1.2" << endl;
                    ocl_version = 12;
                }
                if (ocl_version == 12)
                    arg.openclVersion = OCL12;
                else if (ocl_version == 20)
                    arg.openclVersion = OCL20;
                break;
            case 'h':
            default:
                printHelp();
                exit(1);
        }
    }

    if (!kernelFileProvided) {
        ERROR_LOG << "Kernel Source/Binary File is not provided, please use -f" << endl;
    }
    if (!kernelProvided) {
        ERROR_LOG << "Kernels are not provided, please use -k" << endl;
    }
    if (!networkProvided) {
        ERROR_LOG << "Network File is not provided, please use -n" << endl;
    }
    printTitle("Parsing Argument");
    printArgument(arg);
    return arg;
}
void printArgument(cmdArg arg){
    INFO_LOG<<"Kernel File        = "<<arg.mFileName<<endl;
    INFO_LOG<<"Kernels to compile = "<<arg.mKernelName<<endl;
    INFO_LOG<<"Network JSON File  = "<<arg.network<<endl;

    string deviceType, openclVersion = "OpenCL 1.2", networkLoggingLevel = "debug";
    switch(arg.deviceType){
        case CL_DEVICE_TYPE_CPU:
            deviceType = "CPU";
            break;
        case CL_DEVICE_TYPE_GPU:
            deviceType = "GPU";
            break;
        case CL_DEVICE_TYPE_ACCELERATOR:
            deviceType = "FPGA";
            break;
    }
    if (arg.openclVersion == OCL20){
        openclVersion = "OpenCL 2.0";
    }
    switch(arg.networkLoggingLevel){
        case NO:
            networkLoggingLevel = "no";
            break;
        case LAYER:
            networkLoggingLevel = "layer";
            break;
        case NET:
            networkLoggingLevel = "net";
            break;
    }
    INFO_LOG<<"Device Type        = "<<deviceType<<endl;
    INFO_LOG<<"Device Id          = "<<arg.deviceID<<endl;
    INFO_LOG<<"Platform Name      = "<<arg.platformName<<endl;
    INFO_LOG<<"OpenCL Version     = "<<openclVersion<<endl;
    INFO_LOG<<"Network Logging    = "<<networkLoggingLevel<<endl;
    INFO_LOG<<"Extra Infomation   = "<<arg.info<<endl;
}

double runProgram(int argc, char **argv, RunOpenCL F) {

    cmdArg arg = parseCmdArg(argc, argv);
    oclHardware hardware = getOclHardware(arg);
    oclSoftware software =
            getOclSoftware(hardware, arg.mKernelName, arg.mFileName);

    for (itKernelMap iterator = software.kernelMap->begin();
         iterator != software.kernelMap->end(); iterator++) {
        cout << "Kernel: " << iterator->first << " " << iterator->second << endl;
    }

    printTitle("OpenCL RunTime Environment");
    printCLConstant();
    printTitle("Custom Host Code");
    clock_t start = clock();
    bool success = (*F)(arg, hardware, software);
    double delay = (clock() - start) / (double) CLOCKS_PER_SEC;;
    release(software);
    release(hardware);
    if (success) {
        INFO_LOG << "Custom Function finished successfully" << endl;
    } else {
        ERROR_LOG << "Custom Function failed" << endl;
    }
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

Mat readImage(string fpath) { return imread(fpath, CV_LOAD_IMAGE_GRAYSCALE); }

size_t imageSize(Mat m) { return m.cols * m.rows * m.elemSize(); }

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
