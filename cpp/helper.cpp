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

string trim(string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
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

static const std::map<cl_int, std::string> clErrorCodes(map_pairs, map_pairs + sizeof(map_pairs) / sizeof(map_pairs[0]));

const char* clErrorCode(cl_int code)
{
    map<cl_int, string>::const_iterator iter = clErrorCodes.find(code);
    if (iter == clErrorCodes.end())
        return "UNKNOWN ERROR";
    else
        return iter->second.c_str();
}

int loadFile2Memory(const char *filename, char **result)
{
    int size = 0;

    std::ifstream stream(filename, std::ifstream::binary);
    if (!stream) {
        cout<<"loadFile2Memory Error: "<<filename<<endl;
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


void release(oclSoftware& software)
{
    // for loop on software.kernalMap
    for (itKernelMap iterator = software.kernelMap->begin(); iterator!=software.kernelMap->end();
            iterator++){
        clReleaseKernel(iterator->second);
    }
    delete software.kernelMap;
    clReleaseProgram(software.mProgram);
}


void release(oclHardware& hardware)
{
    clReleaseCommandQueue(hardware.mQueue);
    clReleaseContext(hardware.mContext);
    if ((hardware.mMajorVersion >= 1) && (hardware.mMinorVersion > 1)) {
        // Only available in OpenCL >= 1.2
        clReleaseDevice(hardware.mDevice);
    }
}


cl_int compileProgram(const oclHardware &hardware, oclSoftware &software)
{
    cl_int err = clBuildProgram(software.mProgram, 1, &hardware.mDevice, software.mCompileOptions, 0, 0);
    if (err != CL_SUCCESS)
    {
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
    for (itKernelMap iterator = software.kernelMap->begin(); iterator!=software.kernelMap->end();
         iterator++){
        (*software.kernelMap)[iterator->first] =  clCreateKernel(software.mProgram, iterator->first.c_str(), &err);
        if ((*software.kernelMap)[iterator->first] == 0)
        {
            REPORT_ERRM(err, "clCreateKernel on "+iterator->first);
        }
    }
    return CL_SUCCESS;
}

void getDeviceVersion(oclHardware& hardware)
{
    char versionString[512];
    size_t size = 0;
    cl_int err = clGetDeviceInfo (hardware.mDevice, CL_DEVICE_VERSION, 511, versionString, &size);
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



oclHardware getOclHardware(cl_device_type type, const char *target_device)
{
    oclHardware hardware = {0, 0, 0, 0, 0, 0, 0};
    cl_platform_id platforms[16] = { 0 };
    cl_device_id devices[16];
    cl_device_id device_id;
    char platformName[256];
    char deviceName[256];
    cl_uint platformCount = 0;
    cl_int err;

    err = clGetPlatformIDs(16, platforms, &platformCount);
    REPORT_ERRM(hardware, "clGetPlatformIDs");
    bool platformSelected = false;
    for (cl_uint i = 0; i < platformCount; i++) {
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 256, platformName, 0);
        REPORT_ERRM(hardware, "clGetPlatformInfo");
        cout<<"DEBUG: " <<"Available Platform Found: " << platformName << endl;
        cl_uint deviceCount = 0;
        err = clGetDeviceIDs(platforms[i], type, 16, devices, &deviceCount);
        REPORT_ERRM(hardware, string("clGetDeviceIDs at platform: ") + string(platformName));
        cl_uint idev;
        for (idev=0; idev < deviceCount; idev++) {
            err = clGetDeviceInfo(devices[idev], CL_DEVICE_NAME, 256, deviceName, 0);
            REPORT_ERRM(hardware, "clGetDeviceInfo:"+string(deviceName));
            std::cout<<"DEBUG: "<<platformName<<" Hardware Founded: " << deviceName << std::endl;
        }
    }

    for (cl_uint i = 0; i < platformCount; i++) {
        err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 256, platformName, 0);
        REPORT_ERRM(hardware, "clGetPlatformInfo");
        if (strcmp(platformName, PLATFORM_FILTER) != 0) {
            std::cout<<"DEBUG: "<<platformName<<"!="<<PLATFORM_FILTER<<endl;
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
        for (idev=0; idev < deviceCount; idev++) {
            err = clGetDeviceInfo(devices[idev], CL_DEVICE_NAME, 256, deviceName, 0);
            REPORT_ERRM(hardware, "clGetDeviceInfo:"+string(deviceName));
            if (strcmp(deviceName, target_device) == 0) {
                cout << "INFO: Selected Device " << deviceName << endl;
                device_id = devices[i];
                break;
            }
            if (DISABLE_DEVICE_FILTER){
                device_id = devices[i];
                break;
            }
        }
        if (idev == deviceCount) {
            std::cout << "ERROR: target device " << target_device << " not found " << std::endl;
            return hardware;
        }

        cl_context_properties contextData[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i], 0};
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
        cout<<"+++++++++++++++++++++++++Runtime Info+++++++++++++++++++++++++++"<<endl;
        std::cout << "INFO: Platform = " << platformName << "\n";
        std::cout << "INFO: Device = " << deviceName << "\n";
        std::cout << "INFO: OpenCL Version = " << hardware.mMajorVersion << '.' << hardware.mMinorVersion << "\n";
        return hardware;
    }
    if (!platformSelected)
        cout <<"ERROR: No qualified platform founded!!" <<endl;
    return hardware;
}


oclSoftware getOclSoftware(const oclHardware &hardware, const char* kernelNames, const char* kernelFileName)
{
    unsigned char *kernelCode = 0;

    oclSoftware soft;
    std::memset(&soft, 0, sizeof(oclSoftware));
    std::strcpy(soft.mFileName, kernelFileName);
    std::string kernelNameListString(kernelNames);
    vector<std::string> kernelNameList;
    split(kernelNameListString, ',', kernelNameList);
    soft.kernelMap = new map<std::string, cl_kernel>;
    //****************************//
    for (size_t i = 0; i < kernelNameList.size(); i++){
        (*soft.kernelMap)[kernelNameList[i]] = 0;
    }
    // std::strcpy(soft.mKernelName, kernelName);
    cl_int err;
    if (hardware.deviceType == CL_DEVICE_TYPE_CPU) {
        std::sprintf(soft.mCompileOptions, "-g");
    }

    std::cout << "INFO: Loading " << kernelFileName << "\n";
    int size = loadFile2Memory(kernelFileName, (char **) &kernelCode);
    if (size < 0) {
        std::cout << "ERROR: Failed to load kernel\n";
        return soft;
    }

    if (hardware.deviceType == CL_DEVICE_TYPE_ACCELERATOR) {
        size_t n = size;
        soft.mProgram = clCreateProgramWithBinary(hardware.mContext, 1, &hardware.mDevice, &n,
                                                  (const unsigned char **) &kernelCode, 0, &err);
    }
    else {
        soft.mProgram = clCreateProgramWithSource(hardware.mContext, 1, (const char **)&kernelCode, 0, &err);
    }
    if (!soft.mProgram || (err != CL_SUCCESS)) {
        std::cout <<"ERROR: "<< clErrorCode(err) << " on Program Creation \n";
        return soft;
    }

    err = compileProgram(hardware, soft);
    REPORT_ERRM(soft, "compileProgram Failure (ignore the success)");
    delete [] kernelCode;
    return soft;
}


void printHelp()
{
    cout << "usage: %s <options>\n";
    cout << "  -d <cpu|gpu|fpga>\n";
    cout << "  -f <kernel_file> \n";
    cout << "  -k <kernel_name>\n";
    cout << "  -i <dir to read images>\n";
    cout << "  -h <print help info>\n";
}



cmdArg parseCmdArg(int argc, char** argv){
    cmdArg arg;
    const static struct option long_options[] = {
            {"device",      required_argument, 0, 'd'},
            {"kernel_name", required_argument, 0, 'k'},
            {"kernel_file", required_argument, 0, 'f'},
            {"data_dir",    optional_argument,  0, 'i'},
            {"help",        no_argument,       0, 'h'},
            {0, 0, 0, 0}
    };
    int ch;
    while ((ch = getopt_long(argc, argv, "d:k:f:h:i:", long_options, NULL)) != -1)
    {
        // check to see if a single character or long option came through
        switch (ch)
        {
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
            case 'h':
            default:
                printHelp();
                exit(1);
        }
    }
    return arg;
}


double runProgram(int argc, char** argv, RunOpenCL F){
    cmdArg arg = parseCmdArg(argc, argv);
    oclHardware hardware = getOclHardware(arg.deviceType, ALPHA_DATA_KU3_DDR1);
    oclSoftware software = getOclSoftware(hardware, arg.mKernelName, arg.mFileName);
//    cout<<"INFO: CL_DEVICE_MAX_WORK_GROUP_SIZE = "<<CL_DEVICE_MAX_WORK_GROUP_SIZE<<endl;
//    cout<<"INFO: CL_DEVICE_MAX_WORK_ITEM_SIZES = " <<CL_DEVICE_MAX_WORK_ITEM_SIZES<<endl;

    // run the program here!
    // - read images from cmdArg.imagesDir
    // - enqeue those images in batch? need experiments
    // - anyway, start with simple one.
    cout<<"++++++++++++++++++++++++++++++++++++ Entering Custom Host Code ++++++++++++++++++++++++++++++++++++ \n"<<endl;
    double delay = (*F)(arg, hardware, software);
    release(software);
    release(hardware);
    return delay;
}


FileList ls(const std::string& pattern){
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        files.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
}



Mat readImage(string fpath){
    return imread(fpath, CV_LOAD_IMAGE_GRAYSCALE);
}

size_t imageSize(Mat m){
    return m.cols * m.rows * m.elemSize();
}



std::string getFileName(const std::string& s) {
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t i = s.rfind(sep, s.length());
    if (i != string::npos) {
        return(s.substr(i+1, s.length() - i));
    }
    return("");
}
