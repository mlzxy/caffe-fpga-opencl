//
// Created by 张鑫语 on 8/22/16.
//

#include "pipelineDemo.h"
#include <math.h>


#define MAX_REC_N 16


#define ERR(x) REPORT_ERRM(0.0, x)

double run(cmdArg arg, oclHardware hardware, oclSoftware software) {
    clock_t start = clock();
    cl_kernel kernel0 = (*software.kernelMap)["kernel0"];
    DEBUG_LOG<<"Get Kernel0: "<<kernel0<<endl;

    cl_kernel kernel1 = (*software.kernelMap)["kernel1"];
    DEBUG_LOG<<"Get Kernel1: "<<kernel1<<endl;

    cl_int *h_input  = (cl_int *) malloc(sizeof(cl_int)*MAX_REC_N);
    cl_int *h_output = (cl_int *) malloc(sizeof(cl_int)*MAX_REC_N);
    cl_int err;

    cl_mem d_input    = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY,  sizeof(cl_int)*MAX_REC_N, NULL, NULL);
    cl_mem d_output   = clCreateBuffer(hardware.mContext, CL_MEM_WRITE_ONLY, sizeof(cl_int)*MAX_REC_N, NULL, NULL);

    err = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &d_output);
    ERR("clSetKernelArg on kernel1");

    err = clSetKernelArg(kernel0, 0, sizeof(cl_mem), &d_input);
    ERR("clSetKernelArg on kernel0");

    for (int i = 0; i < MAX_REC_N; ++i) {
        h_input[i] = i;
    }
    DEBUG_LOG<<"h_input set"<<endl;

    err = clEnqueueWriteBuffer(hardware.mQueue, d_input, CL_TRUE, 0, MAX_REC_N*sizeof(cl_int), h_input, 0, NULL, NULL);
    ERR("clEnqueueWriteBuffer");

    size_t global[1];                   // global domain size for our calculation
    size_t local[1];                    // local domain size for our calculation
    global[0] = 1;
    local[0] = 1;
    clEnqueueNDRangeKernel(hardware.mQueue, kernel0, 1, NULL,
                           (size_t*)global, (size_t*)local, 0, NULL, NULL);
    ERR("clEnqueueNDRangeKernel kernel0") // => get print

    clEnqueueNDRangeKernel(hardware.mQueue, kernel1, 1, NULL,
                                 (size_t*)global, (size_t*)local, 0, NULL, NULL);
    ERR("clEnqueueNDRangeKernel kernel1")

    err = clFinish(hardware.mQueue);
    ERR("clFinish")

    cl_event readevent;
    err = clEnqueueReadBuffer(hardware.mQueue, d_output, CL_TRUE, 0,
                        MAX_REC_N * sizeof(cl_int),
                        h_output, 0, NULL, &readevent);
    ERR("clEnqueueReadBuffer");

    err = clWaitForEvents(1, &readevent);
    ERR("clWaitForEvents");

    bool verify = true;
    for (int i; i < MAX_REC_N; ++i) {
        if (h_input[i] != h_output[i]) {
            verify = false;
            ERROR_LOG<<"NO' "<<i<<" not matched!"<<endl;
            break;
        }
    }
    if (verify)
        DEBUG_LOG<<"Input and output matched."<<endl;
    else
        ERROR_LOG<<"Input and output did not match."<<endl;
    free(h_input);
    free(h_output);
    return (clock() - start)/(double)CLOCKS_PER_SEC;
}
