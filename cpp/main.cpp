//
// Created by 张鑫语 on 8/20/16.
//

#include "helper.h"
#include "./RunOpenCL/vecAdd/vecAdd.h"

int main(int argc, char** argv) {
    double time = runProgram(argc, argv, run);
    cout<< "Total Custom Code Run Time: " << time << endl;
    return 0;
}
