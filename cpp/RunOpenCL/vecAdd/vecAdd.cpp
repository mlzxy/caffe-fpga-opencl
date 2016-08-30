//
// Created by 张鑫语 on 8/22/16.
//

#include "vecAdd.h"
#include <math.h>

double run(cmdArg arg, oclHardware hardware, oclSoftware software) {

    cout<<"Entering Custom Host Code"<<endl;
    unsigned int n = 1000;
    size_t bytes = n*sizeof(float);

    float *h_a;
    float *h_b;
    // Host output vector
    float *h_c;
    h_a = (float*)malloc(bytes);
    h_b = (float*)malloc(bytes);
    h_c = (float*)malloc(bytes);
    for(uint i = 0; i < n; i++ )
    {
        h_a[i] = sinf(i)*sinf(i);
        h_b[i] = cosf(i)*cosf(i);
    }
    cl_int err;
    cl_mem d_a = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY,
                                bytes, NULL, &err);
    REPORT_ERR(0.0);
    cl_mem d_b = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY,
                                bytes, NULL, &err);
    REPORT_ERR(0.0);
    cl_mem d_c = clCreateBuffer(hardware.mContext, CL_MEM_WRITE_ONLY,  //| CL_MEM_USE_HOST_PTR
                                bytes, NULL, &err);
    REPORT_ERR(0.0);
    size_t globalSize, localSize;


    localSize = 1;
    globalSize = 1000;
    cout<<"GlobalSize = "<<globalSize<<"; " << "LocalSize = " <<localSize <<endl;
    clock_t start = clock();
    // memory transfer
    err = clEnqueueWriteBuffer(hardware.mQueue, d_a, CL_TRUE, 0,
                               bytes, h_a, 0, NULL, NULL);

    REPORT_ERR(0.0);

    err = clEnqueueWriteBuffer(hardware.mQueue, d_b, CL_TRUE, 0,
                                bytes, h_b, 0, NULL, NULL);
    REPORT_ERR(0.0);
    cout<<"clEnqueueWriteBuffer Finished"<<endl;

    // set arguments
    err = clSetKernelArg(software.mKernel, 0, sizeof(cl_mem), &d_a);
    REPORT_ERR(0.0);

    err = clSetKernelArg(software.mKernel, 1, sizeof(cl_mem), &d_b);
    REPORT_ERR(0.0);

    err = clSetKernelArg(software.mKernel, 2, sizeof(cl_mem), &d_c);
    REPORT_ERR(0.0);

    err = clSetKernelArg(software.mKernel, 3, sizeof(unsigned int), &n);
    REPORT_ERR(0.0);
    cout<<"clSetKernelArg Finished"<<endl;

    // start to execuate kernel
    err = clEnqueueNDRangeKernel(hardware.mQueue, software.mKernel, 1, NULL, &globalSize, &localSize, //重要
                                 0, NULL, NULL); // https://jorudolph.wordpress.com/2012/02/03/opencl-work-item-ids-globalgrouplocal/
    REPORT_ERR(0.0);
    cout<<"clEnqueueNDRangeKernel Finished"<<endl;

    err = clFinish(hardware.mQueue);
    REPORT_ERR(0.0);
    cout<<"Finished" <<endl;

    // finish, fetch result.
    err = clEnqueueReadBuffer(hardware.mQueue, d_c, CL_TRUE, 0, bytes,
                              h_c, 0, NULL, NULL );
    REPORT_ERR(0.0);
    cout <<"Comparing Resulting" <<endl;
    bool ok = true;


    uint i;
    for(i = 0; i < n; i++ )
    {
        if (h_c[i] != ( h_a[i]  + h_b[i])) {
            ok = false;
            break;
        }
    }
    if (ok)
        cout<<"SUCCESS: Result Matched"<<endl;
    else
        cout<<"ERROR: Result not matched at " << i<<endl;

    free(h_a);
    free(h_b);
    free(h_c);
    return (clock() - start)/(double)CLOCKS_PER_SEC;
}
