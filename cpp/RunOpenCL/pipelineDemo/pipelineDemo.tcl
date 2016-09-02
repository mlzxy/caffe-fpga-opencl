# SDAccel command script
set projectName "pipelineDemo"

## ======== ##

set root "../../"
set device "xilinx:adm-pcie-ku3:1ddr:2.1"
set solutionName "${projectName}_board_compilation_solution"
set cppList [list "cpp/main.cpp" "cpp/helper.cpp" "cpp/RunOpenCL/${projectName}/${projectName}.cpp"]
set hList [list "cpp/helper.h" "cpp/RunOpenCL/${projectName}/${projectName}.h"]
set kernelNameList {"kernel0" "kernel1"}
set kernelFile "kernels/${projectName}/${projectName}.cl"
set containerName "bcontainer"
set region "OCL_REGION_0"
set cxxFlags "-g -O0 -std=c++0x -I$::env(PWD) -I$::env(XILINX_SDACCEL)/include -L$::env(XILINX_SDACCEL)/lnx64/tools/opencv -lopencv_core -lopencv_highgui -lopencv_imgproc -lavcodec -lavformat -lavutil -lswscale"

set xoccFlags "-g"


set kernelNameString  [join $kernelNameList ","]
puts "Kernels: $kernelNameString"

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
compile_emulation -flow cpu -opencl_binary [get_opencl_binary ${containerName}]

# Generate the system estimate report
# report_estimate

# Run the design in CPU emulation mode
run_emulation -flow cpu -args "-f ${containerName}.xclbin -k ${kernelNameString} -d fpga"


# compile_emulation -flow hardware -opencl_binary [get_opencl_binary ${containerName}]

# Generate the system estimate report
# report_estimate

# Run the design in CPU emulation mode
# run_emulation -flow hardware -args "-f ${containerName}.xclbin -k ${kernelNameString} -d fpga"


# Build the application for hardware
# build_system

# Package the results for the card
# package_system
