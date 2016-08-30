# SDAccel command script
set root "../../"
set device "xilinx:adm-pcie-ku3:1ddr:2.1"
set solutionName "convolution_board_compilation_solution"
set cppList [list "cpp/main.cpp" "cpp/helper.cpp" "cpp/RunOpenCL/convolution/convolution.cpp"]
set hList [list "cpp/helper.h" "cpp/RunOpenCL/convolution/convolution.h"]
set kernelName "convolution"
set kernelFile "kernels/convolution/convolution.cl"
set containerName "bcontainer"
set region "OCL_REGION_0"
set cxxFlags "-g -O0 -std=c++0x -I$::env(PWD) -I$::env(XILINX_SDACCEL)/include -L$::env(XILINX_SDACCEL)/lnx64/tools/opencv -lopencv_core -lopencv_highgui -lopencv_imgproc -lavcodec -lavformat -lavutil -lswscale"

set xoccFlags "-g"

exec rm -rf /home/xy0/Dropbox/Centos_WorkSpace/OpenCL/C++_Version/build/data/convolution/output/*
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



# Kernel definition
create_kernel ${kernelName} -type clc
add_files -kernel [get_kernels ${kernelName}] ${root}${kernelFile}
set_property -name kernel_flags -value ${xoccFlags} -objects [get_kernels ${kernelName}]

# Define binary containers
create_opencl_binary ${containerName}
set_property region ${region} [get_opencl_binary ${containerName}]
create_compute_unit -opencl_binary [get_opencl_binary ${containerName}] -kernel [get_kernels ${kernelName}] -name K1

# Compile the design for CPU based emulation
compile_emulation -flow cpu -opencl_binary [get_opencl_binary ${containerName}]

# Generate the system estimate report
# report_estimate

# Run the design in CPU emulation mode
run_emulation -flow cpu -args "-f ${containerName}.xclbin -k ${kernelName} -d fpga -i /home/xy0/Dropbox/Centos_WorkSpace/OpenCL/C++_Version/build/data/convolution"


compile_emulation -flow hardware -opencl_binary [get_opencl_binary ${containerName}]

# Generate the system estimate report
# report_estimate

# Run the design in CPU emulation mode
run_emulation -flow hardware -args "-f ${containerName}.xclbin -k ${kernelName} -d fpga -i /home/xy0/Dropbox/Centos_WorkSpace/OpenCL/C++_Version/build/data/convolution"


# Build the application for hardware
# build_system

# Package the results for the card
# package_system
