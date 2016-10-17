#@IgnoreInspection BashAddShebang

## change this
export FPGA=xilinx:adm-pcie-ku3:2ddr-xpr:3.1
export SDACCEL_HOME=${HOME}/Applications/SDAccel/SDAccel/2016.2


export CAFFE_FPGA_CL_HOME="$(exec 2>/dev/null;cd -- $(dirname "$0"); unset PWD; /usr/bin/pwd || /bin/pwd || pwd)"
export SDACCEL_GCC_BIN_PATH=${SDACCEL_HOME}/lnx64/tools/gcc/bin

function test_pcie_fpga(){
	sudo lspci -s :04:00.0 -vv
}


function  xocc_sw(){
	xocc -g --xdevice ${FPGA}  -t sw_emu  -o $1.sw_emu.xclbin   $1
}

function  xocc_hwem(){
	xocc  --xdevice ${FPGA} -o $1.hw_emu.xclbin -t hw_emu $1
}

function  xocc_hw(){
	xocc  --xdevice ${FPGA} -o $1.hw.xclbin -t hw $1
}

function xocc_clean(){
	rm -rf *.xclbin  *.dir
}

function cpu_env(){
   # I only run cpu mode on my mac os, which only has opencl cpu support.
   # So if you have opencl cpu installed in a linux machine
   # then customize this variable, make sure it points to correct *.so
   export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}
}

function gpu_env() {
    # this may differ for each machine, so take care and modify to your own.
	export LD_LIBRARY_PATH=/usr/local/cuda/lib64:/opt/lib:/usr/local/lib:/usr/lib64/nvidia/
}

function fpga_env(){
	export LD_LIBRARY_PATH=${SDACCEL_HOME}/lib/lnx64.o:${XILINX_OPENCL}/runtime/lib/x86_64/:${SDACCEL_HOME}/runtime/lib/x86_64:${SDACCEL_HOME}/lnx64/tools/opencv
}

function convert_net(){
    local ROOT = ${CAFFE_FPGA_CL_HOME} ## ROOT is shorter.
    # make sure you install the correct dependencies
    python2.7 ${ROOT}/convertor/src/convert.py  -m ${ROOT}/convertor/data/${1}/model.caffemodel  -p ${ROOT}/convertor/data/${1}/train.prototxt  -o ${ROOT}/convertor/output  -n ${1}
}
