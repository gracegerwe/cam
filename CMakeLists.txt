cmake_minimum_required(VERSION 3.10)
project(CAM)

# OpenCascade setup
find_package(OpenCascade REQUIRED)

# Include directories
include_directories(${OpenCascade_INCLUDE_DIR})

# Add source files
add_executable(CAM main.cpp geometry_utils.hpp/.cpp hello.cpp)

# Link OpenCascade libraries
target_link_libraries(CAM ${OpenCascade_LIBRARIES})
