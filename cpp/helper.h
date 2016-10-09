//
// Created by 张鑫语 on 8/20/16.
//

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

#ifdef __APPLE__
#include <OpenCL/cl.h>
//#include <OpenCL/cl.hpp>
#else
#include <CL/cl.h>
//#include <CL/cl.hpp> // Xilinx Opencl doesn't support Opencl C++ Bindings.
#endif



/**
 * # Debugging Macros #
 */
#define TO_STRING(x) #x
#define ERR_CHECK(y)  if(err != CL_SUCCESS) {std::cout <<"ERROR: From " <<y <<" -> " << clErrorCode(err) << "\n"; exit(1);} else{cout<<"DEBUG: "<<y<<" -> Success"<<endl; }
#define DEBUG_LOG  cout<<"DEBUG: "
#define INFO_LOG  cout<<"INFO: "
#define ERROR_LOG  cout<<"ERROR: "



using namespace cv;
using namespace std;


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
    char dataDir[1024];
    char network[1024];
} cmdArg;


/**
 * OpenCL Related Utilties #
 */
oclSoftware getOclSoftware(const oclHardware &hardware, const char* kernelNames, const char* kernelFileName);
oclHardware getOclHardware(cl_device_type type);
cl_int compileProgram(const oclHardware &hardware, oclSoftware &software);
void getDeviceVersion(oclHardware& hardware);
void printHelp();
cmdArg parseCmdArg(int argc, char** argv);
void release(oclSoftware& software);
void release(oclHardware& hardware);
int loadFile2Memory(const char *filename, char **result);
const char *clErrorCode(cl_int code);
typedef double (*RunOpenCL)(cmdArg arg, oclHardware hardware, oclSoftware software);
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
 * Network Related Structure Declaration
 */
//#include "network.h"
#endif //C_VERSION_HELPER_H
