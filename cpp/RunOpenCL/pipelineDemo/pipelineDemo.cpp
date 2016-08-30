//
// Created by 张鑫语 on 8/22/16.
//

#include "pipelineDemo.h"
#include <math.h>


#define MAX_REC_N 128
#define PIPE_LEN 16



double run(cmdArg arg, oclHardware hardware, oclSoftware software) {
    clock_t start = clock();
    cl_kernel kernel0 = (*software.kernelMap)["kernel0"];
    cl_kernel kernel1 = (*software.kernelMap)["kernel1"];
    cl_int *h_input  = (cl_int *) malloc(sizeof(cl_int)*MAX_REC_N);
    cl_int *h_output = (cl_int *) malloc(sizeof(cl_int)*MAX_REC_N);


    cl_mem d_input    = clCreateBuffer(context, CL_MEM_READ_ONLY,  sizeof(cl_int)*MAX_REC_N, NULL, NULL);
    cl_mem d_output   = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int)*MAX_REC_N, NULL, NULL);
    clSetKernelArg(kernel0, 0, sizeof(cl_mem), &d_input);
    err  = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &d_output);

    for (int i = 0; i < MAX_REC_N; ++i) {
        h_input[i] = i;
    }
    clEnqueueWriteBuffer(hardware.mQueue, d_input, CL_TRUE, 0, MAX_REC_N*sizeof(cl_int), h_input, 0, NULL, NULL);

    size_t global[1];                   // global domain size for our calculation
    size_t local[1];                    // local domain size for our calculation
    global[0] = 1;
    local[0] = 1;
    clEnqueueNDRangeKernel(hardware.mQueue, kernel0, 1, NULL,
                           (size_t*)global, (size_t*)local, 0, NULL, NULL);
    clEnqueueNDRangeKernel(hardware.mQueue, kernel1, 1, NULL,
                                 (size_t*)global, (size_t*)local, 0, NULL, NULL);
    clFinish(hardware.mQueue);

    cl_event readevent;
    clEnqueueReadBuffer(hardware.mQueue, d_output, CL_TRUE, 0,
                        MAX_REC_N * sizeof(cl_int),
                        h_output, 0, NULL, &readevent);
    clWaitForEvents(1, &readevent);

    bool verify = true ;
    for (int i; i < MAX_REC_N; ++i) {
        if (h_input[i] != h_output[i]) {
            verify = false;
            break;
        }
    }
    if (verify)
        cout<<"Input and output matched."<<endl;
    else
        cout<<"Input and output did not match."<<endl;
    free(h_input);
    free(h_output);
    return (clock() - start)/(double)CLOCKS_PER_SEC;
}