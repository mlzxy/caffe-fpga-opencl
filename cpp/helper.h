#ifndef C_VERSION_HELPER_H
#define C_VERSION_HELPER_H

#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#include <map>
#include <getopt.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include <fstream>
#include <glob.h>
#include <opencv2/highgui/highgui.hpp>
#include "./json/json.h"
#include "net_enum.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
//#include <OpenCL/cl.hpp>
#else
#include <CL/cl.h>
//#include <CL/cl.hpp> // Xilinx Opencl doesn't support Opencl C++ Bindings.
#endif


using namespace cv;
using namespace std;



/**
 * # Debugging Macros #
 */
#define TO_STRING(x) #x
#define ERR_CHECK(y)  if(err != CL_SUCCESS) {std::cout <<"ERROR: From " <<y <<" -> " << clErrorCode(err) << "\n"; exit(1);} else{cout<<"DEBUG: "<<y<<" -> Success"<<endl; }
#define DEBUG_LOG  cout<<"DEBUG: "
#define INFO_LOG  cout<<"INFO: "
#define WARN_LOG  cout<<"WARN: "
#define ERROR_LOG  cout<<"ERROR: "
#define SOLID_LINE cout<<"==========================================\n"
#define DOTTED_LINE cout<<"------------------------------------------\n"

/**
 * # Constants #
 */
#define DEVICE_GPU    CL_DEVICE_TYPE_GPU
#define DEVICE_CPU    CL_DEVICE_TYPE_CPU
#define DEVICE_FPGA   CL_DEVICE_TYPE_ACCELERATOR
#define XILINX_FPGA "Xilinx"
#define APPLE_MAC  "Apple"
#define NVIDIA_CUDA  "NVIDIA CUDA"
#ifdef __APPLE__
#define DEFAULT_PLATFORM APPLE_MAC
#define DEFAULT_DEVICE DEVICE_CPU
#else
#define DEFAULT_PLATFORM XILINX_FPGA
#define DEFAULT_DEVICE CL_DEVICE_TYPE_ACCELERATOR
#endif



/**
 * # OPENCL Related Data Structure #
 */
typedef  struct oclHardware {
    cl_platform_id mPlatform;
    cl_context mContext;
    cl_device_id mDevice;
    cl_command_queue mQueue;
    cl_device_type deviceType;
    short mMajorVersion;
    short mMinorVersion;
} oclHardware;

typedef  struct oclSoftware {
    cl_program mProgram;
    char mFileName[1024];
    char mCompileOptions[1024];
    map<std::string, cl_kernel> *kernelMap;
} oclSoftware;

//cl_kernel mKernel;
//char mKernelName[128];
typedef std::map<std::string, cl_kernel>::iterator itKernelMap;

typedef struct cmdArg{
    cl_device_type deviceType;
    char mFileName[1024];
    char mKernelName[128];
    char platformName[1024];
    int  deviceID;
    char network[1024];
    NetLogging networkLoggingLevel;
    OpenCLVersion  openclVersion;
    char info[1024]; //custom information
} cmdArg;


/**
 * OpenCL Related Utilties #
 */
oclSoftware getOclSoftware(const oclHardware &hardware, const char* kernelNames, const char* kernelFileName);
oclHardware getOclHardware(cmdArg arg);
cl_int compileProgram(const oclHardware &hardware, oclSoftware &software);
void getDeviceVersion(oclHardware& hardware);
void printHelp();
void printArgument(cmdArg arg);
cmdArg parseCmdArg(int argc, char** argv);
void release(oclSoftware& software);
void release(oclHardware& hardware);
int loadFile2Memory(const char *filename, char **result);
const char *clErrorCode(cl_int code);
typedef bool (*RunOpenCL)(cmdArg arg, oclHardware hardware, oclSoftware software);
double runProgram(int argc, char** argv, RunOpenCL F);


/**
 * # MISC Utilities #
 */
Mat readImage(string fpath);
size_t imageSize(Mat m);

typedef vector<std::string>  FileList;
FileList ls(const std::string& pattern);
std::string getFileName(const std::string& s);

void split(const string &s, char delim, vector<string> &elems);
bool endsWith(const string& s, const string& suffix);
string trim(string& str);

inline void printTitle(string x){
cout << "\n\n++++++++++++++++++++++++++++++++++++ "<<x<<" ++++++++++++++++++++++++++++++++++++"
    << endl;
}
inline void printCLConstant(){
    INFO_LOG<<"CL_DEVICE_MAX_WORK_GROUP_SIZE = "<<CL_DEVICE_MAX_WORK_GROUP_SIZE<<endl;
    INFO_LOG<<"CL_DEVICE_MAX_WORK_ITEM_SIZES   " <<CL_DEVICE_MAX_WORK_ITEM_SIZES<<endl;
}


/**
 * # The "main" function #
 */
bool run(cmdArg arg, oclHardware hardware, oclSoftware software);
#endif
