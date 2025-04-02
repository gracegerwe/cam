cmake_minimum_required(VERSION 3.10)
project(CAM)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Generate compile_commands.json for VS Code
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json" FORCE)

# Set OpenCascade paths directly
set(OC_INCLUDE_DIRS "/opt/homebrew/include/opencascade")
file(GLOB OC_LIBRARIES "/opt/homebrew/lib/libTK*.dylib")

# Include directories
include_directories(${OC_INCLUDE_DIRS})

# Add source files
add_executable(CAM main.cpp)

# Link OpenCascade libraries
target_link_libraries(CAM PRIVATE ${OC_LIBRARIES})