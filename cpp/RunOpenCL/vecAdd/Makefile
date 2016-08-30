CPP := g++

ROOT := ../../
SOURCES := $(ROOT)/cpp/main.cpp $(ROOT)/cpp/helper.cpp $(ROOT)/cpp/RunOpenCL/vecAdd/vecAdd.cpp  $(ROOT)/cpp/helper.h $(ROOT)/cpp/RunOpenCL/vecAdd/vecAdd.h

CUDA := /usr/local/cuda
OPENCL_INC := $(CUDA)/include
OPENCL_LIB := $(CUDA)/lib64
OPENCV_LIB := /opt/lib
OPENCV_INC := /opt/include
AV_LIB := /usr/local/lib
LINK_FLAGS := -lopencv_core -lopencv_highgui -lopencv_imgproc -lavcodec -lavformat -lavutil -lswscale -lOpenCL

CXX_FLAGS := -Wall -Werror -fpermissive
NOW := $(shell date +"%c" | tr ' :' '__')
OUTPUT := vecAdd_$(NOW)



.PHONY: all
all: exe

	# export LD_LIBRARY_PATH="" -> this is for runtime!!!!!!!
exe:
	$(CPP) $(CXX_FLAGS) -I$(OPENCL_INC) -I$(OPENCV_INC) -L$(OPENCL_LIB) -L$(OPENCV_LIB) -L$(AV_LIB)  $(LINK_FLAGS) $(SOURCES) -o $(OUTPUT).exe


clean:
	rm -rf *.exe *.o
