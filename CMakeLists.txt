cmake_minimum_required(VERSION 3.3)
project(opencl_learning)


## Set
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -std=c++11 -L/usr/local/lib/ ")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


## Package Find
find_package(OpenCL REQUIRED)
find_package(OpenCV REQUIRED )


## Include
include_directories( ${OPENCL_INCLUDE_DIR} )




set(SOURCE_FILES cpp/main.cpp cpp/helper.cpp  cpp/fpganet.h cpp/network.cpp cpp/network.h cpp/custom.h cpp/helper.h  cpp/jsoncpp.cpp cpp/json/json.h  cpp/json/json-forwards.h
        cpp/custom.cpp   cpp/other/mnist_data.h cpp/net_enum.h)
add_executable(opencl_learning ${SOURCE_FILES})

#MESSAGE(STATUS ${OPENCL_LIBRARIES})
## Linking
target_link_libraries( opencl_learning ${OPENCL_LIBRARIES}  ${OpenCV_LIBS})
