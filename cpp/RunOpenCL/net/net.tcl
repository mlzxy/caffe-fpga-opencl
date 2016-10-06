# SDAccel command script
set projectName "net"
set flow "cpu"
set networkJSON "convertor/output/mnist.json"
## ======== ##

set root "../../"
set absoluteRoot "/home/xy0/Dropbox/Centos_WorkSpace/OpenCL/C++_Version/"
set networkFullPath "${absoluteRoot}${networkJSON}"
set device "xilinx:adm-pcie-ku3:1ddr:2.1"
set solutionName "${projectName}_board_compilation_solution"
set cppList [list "cpp/main.cpp" "cpp/helper.cpp" "cpp/jsoncpp.cpp" "cpp/RunOpenCL/${projectName}/${projectName}.cpp"]
set hList [list "cpp/helper.h" "cpp/json/json.h" "cpp/json/json-forwards.h" "cpp/RunOpenCL/${projectName}/${projectName}.h"   "cpp/RunOpenCL/${projectName}/mnist_data.h"]

set kernelNameList {"convLayer" "reluLayer" "dataLayer" "poolingLayer" "paddingLayer" "outputLayer"}
set kernelFile "kernels/${projectName}/${projectName}.cl"
set containerName "bcontainer"
set region "OCL_REGION_0"
set cxxFlags "-g -O0 -std=c++0x -I$::env(PWD) -I$::env(XILINX_SDACCEL)/include -L$::env(XILINX_SDACCEL)/lnx64/tools/opencv -lopencv_core -lopencv_highgui -lopencv_imgproc -lavcodec -lavformat -lavutil -lswscale"

set xoccFlags ""

set kernelNameString  [join $kernelNameList ","]
puts "Kernels: $kernelNameString"
set commandLineArgument "-f ${containerName}.xclbin -k ${kernelNameString} -d fpga  -n ${networkFullPath}"

# puts ${cxxFlags}

# Define a solution name
create_solution -name ${solutionName} -dir . -force

# Define the target platform of the application
add_device -vbnv ${device}

# Host source files
foreach f $cppList {
  add_files ${root}${f}
}

# Header files
foreach f  $hList {
  add_files ${root}${f}
  set_property file_type "c header files" [get_files [file tail ${f}]]
}

set_property -name host_cflags -value ${cxxFlags}  -objects [current_solution]

# Define binary containers
create_opencl_binary ${containerName}
set_property region ${region} [get_opencl_binary ${containerName}]

foreach kn $kernelNameList {
  create_kernel ${kn} -type clc
  add_files -kernel [get_kernels ${kn}] ${root}${kernelFile}
  set_property -name kernel_flags -value ${xoccFlags} -objects [get_kernels ${kn}]
  create_compute_unit -opencl_binary [get_opencl_binary ${containerName}] -kernel [get_kernels ${kn}] -name "instance_${kn}"
}





# Compile the design for CPU based emulation
compile_emulation -flow $flow -opencl_binary [get_opencl_binary ${containerName}]

# Generate the system estimate report
# report_estimate

# Run the design in CPU emulation mode
run_emulation -flow $flow -args "${commandLineArgument}"


# compile_emulation -flow hardware -opencl_binary [get_opencl_binary ${containerName}]

# Generate the system estimate report
# report_estimate

# Run the design in CPU emulation mode
# run_emulation -flow hardware -args "-f ${containerName}.xclbin -k ${kernelNameString} -d fpga"


# Build the application for hardware
# build_system


# Package the results for the card
# package_system


# run_system -args "${commandLineArgument}"
