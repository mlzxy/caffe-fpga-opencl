//
// Created by 张鑫语 on 8/22/16.
//

#include "pipelineDemo.h"
#include <math.h>





double run(cmdArg arg, oclHardware hardware, oclSoftware software) {

    clock_t start = clock();
    FileList filelist = ls(std::string(arg.dataDir) + "/*");
    if (filelist.size() < 1){
        cout<<"no images."<<endl;
        return 0.0;
    }
    cl_int err;
    size_t globalSize[2], localSize[2] = {1,1}, offset[2] = {1,1}, dim = 2;
    size_t imgsize, rows, cols;
    Mat img;
    for (vector<string>::size_type i = 0; i != filelist.size(); i++) {
        cout <<"Reading Image: "<< filelist[i] << endl;
        img = readImage(filelist[i]);
        if(! img.data )  {
            cout <<  "Could not open or find the image: "<< filelist[i] << " Skipping..."<< endl;
            continue;
        }
        cout<<"Image Readed."<<endl;
        globalSize[0]=img.rows - 2;
        globalSize[1]=img.cols - 2;
        rows = img.rows;
        cols = img.cols;
        cout<<"Rows = "<<rows <<"; Cols = " << cols <<endl;
        //  globalSize, localSize
        imgsize = imageSize(img);
        cl_mem input_image = clCreateBuffer(hardware.mContext, CL_MEM_READ_ONLY, imgsize, NULL, &err);
        REPORT_ERR(0.0);
        cl_mem output_image = clCreateBuffer(hardware.mContext, CL_MEM_WRITE_ONLY, imgsize, NULL, &err);
        REPORT_ERR(0.0);
        err = clEnqueueWriteBuffer(hardware.mQueue, input_image, CL_TRUE, 0, imgsize, img.data, 0, NULL, NULL);
        REPORT_ERR(0.0);
        err = clSetKernelArg(software.mKernel, 0, sizeof(cl_mem), &input_image);
        REPORT_ERR(0.0);
        err = clSetKernelArg(software.mKernel, 1, sizeof(cl_mem), &output_image);
        REPORT_ERR(0.0);
        err = clSetKernelArg(software.mKernel, 2, sizeof(size_t), &rows);
        REPORT_ERR(0.0);
        err = clSetKernelArg(software.mKernel, 3, sizeof(size_t), &cols);
        REPORT_ERR(0.0);
        err = clEnqueueNDRangeKernel(hardware.mQueue, software.mKernel, dim, offset,
                                     globalSize, localSize, 0, NULL, NULL);
        REPORT_ERR(0.0);

        cout<<"Executing"<<endl;
        err = clFinish(hardware.mQueue);
        cout<<"CLFinished"<<endl;
        REPORT_ERR(0.0);

        err = clEnqueueReadBuffer(hardware.mQueue, output_image, CL_TRUE, 0, imgsize,
                                    img.data , 0, NULL, NULL );
        REPORT_ERR(0.0);


        // imwrite here.
        string out(string(arg.dataDir)+ "/output/" + getFileName(filelist[i]));
        cout <<"Writing Image: "<<out<<endl;
        imwrite(out,img);
        clReleaseMemObject(input_image);
        clReleaseMemObject(output_image);
    }



    return (clock() - start)/(double)CLOCKS_PER_SEC;
}