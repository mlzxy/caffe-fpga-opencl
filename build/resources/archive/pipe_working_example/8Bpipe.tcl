set device "xilinx:adm-pcie-ku3:1ddr:2.1"

create_solution -name 8BPipe_Solution -dir . -force
add_device -vbnv ${device}

add_files host.cpp
create_opencl_binary "pipe"
set_property region "OCL_REGION_0" [get_opencl_binary "pipe"]

create_kernel "kernel0"  -type clc
add_files -kernel [get_kernels "kernel0"] "device.cl"
create_compute_unit -opencl_binary [get_opencl_binary "pipe"] -kernel [get_kernels "kernel0"] -name "instance_0"


create_kernel "kernel1"  -type clc
add_files -kernel [get_kernels "kernel1"] "device.cl"
create_compute_unit -opencl_binary [get_opencl_binary "pipe"] -kernel [get_kernels "kernel1"] -name "instance_1"


compile_emulation -flow cpu -opencl_binary [get_opencl_binary "pipe"]


run_emulation -flow cpu -args ""