//
// Created by 张鑫语 on 8/20/16.
//

#include "fpganet.h"


int main(int argc, char** argv) {
    double time = runProgram(argc, argv, run);
    INFO_LOG << "Total Custom Code Run Time: " << time << endl;
    return 0;
}
