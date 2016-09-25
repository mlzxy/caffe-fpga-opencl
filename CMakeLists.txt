cmake_minimum_required(VERSION 3.3)
project(opencl_learning)


## Set
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -std=c++11")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


## Package Find
find_package(OpenCL REQUIRED)
find_package(OpenCV REQUIRED )


## Include
include_directories( ${OPENCL_INCLUDE_DIR} )




set(SOURCE_FILES cpp/main.cpp cpp/helper.cpp cpp/helper.h cpp/json.hpp)
add_executable(opencl_learning ${SOURCE_FILES})

#MESSAGE(STATUS ${OPENCL_LIBRARIES})
## Linking
target_link_libraries( opencl_learning ${OPENCL_LIBRARIES}  ${OpenCV_LIBS})