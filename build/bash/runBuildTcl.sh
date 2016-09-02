#@IgnoreInspection BashAddShebang
# ORG_DIR=$(pwd)
# DIR=$( dirname "${BASH_SOURCE[0]}" )
# cd ${DIR} || echo "cd ${DIR} failed!" && exit
# now figure out why! oh yeah
RUN_BUILD_TCL_ROOT=/home/xy0/Dropbox/Centos_WorkSpace/OpenCL/C++_Version

function gen_ku3_driver(){
	xbinst -x $FPGA_KU3_DDR1 -d $1
}

function test_pcie_fpga(){
	sudo lspci -s :04:00.0 -vv
}


function  xocc_sw(){
	xocc -g --xdevice ${FPGA_KU3_DDR1}  -t sw_emu  -o $1.sw_emu.xclbin   $1
}

function  xocc_hwem(){
	xocc  --xdevice ${FPGA_KU3_DDR1} -o $1.hw_emu.xclbin -t hw_emu $1
}

function  xocc_hw(){
	xocc  --xdevice ${FPGA_KU3_DDR1} -o $1.hw.xclbin -t hw $1
}

function xocc_clean(){
	rm -rf *.xclbin  *.dir
}



function sdaccel_time {
	local start
	start=$(date +%s.%N)
	sdaccel $1
	local dur
	dur=$(echo "$(date +%s.%N) - $start" | bc)
	printf "Execution time: %.6f seconds" $dur
}

function make_time {
	local start
	start=$(date +%s.%N)
	make $1
	local dur
	dur=$(echo "$(date +%s.%N) - $start" | bc)
	printf "Execution time: %.6f seconds" $dur
}


function runBuildTCL {
    local PWD
		PWD=`pwd`
    cd ${RUN_BUILD_TCL_ROOT}/build/output/ ||  echo "cd ${RUN_BUILD_TCL_ROOT}/build/output/ failed!"
    sdaccel_time  ${RUN_BUILD_TCL_ROOT}/cpp/RunOpenCL/$1/$1.tcl

    cd ${PWD} ||  echo "cd ${PWD} failed!"
}

function runBuildGPUMake() {
	local PWD
	PWD=`pwd`
	cd ${RUN_BUILD_TCL_ROOT}/build/output/ ||  echo "cd ${RUN_BUILD_TCL_ROOT}/build/output/ failed!"
	rm -rf ./*.exe
	cp ${RUN_BUILD_TCL_ROOT}/cpp/RunOpenCL/$1/Makefile.gpu ./Makefile
	make_time
	cd ${PWD} ||  echo "cd ${PWD} failed!"
}

function runBuildFPGAMake() {
	local PWD
	PWD=`pwd`
	cd ${RUN_BUILD_TCL_ROOT}/build/output/ ||  echo "cd ${RUN_BUILD_TCL_ROOT}/build/output/ failed!"
	# rm -rf ./*.exe || :
	cp ${RUN_BUILD_TCL_ROOT}/cpp/RunOpenCL/$1/Makefile.fpga ./Makefile
	make_time build
	cd ${PWD} ||  echo "cd ${PWD} failed!"
}

function gpu_env() {
	source ${RUN_BUILD_TCL_ROOT}/build/resources/gpu.env
}

function fpga_env(){
	source ${RUN_BUILD_TCL_ROOT}/build/resources/sdaccel.env
}



# cd ${ORG_DIR} || echo "cd ${ORG_DIR} failed!" && exit
