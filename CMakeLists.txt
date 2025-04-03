cmake_minimum_required(VERSION 3.10)
project(CAM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json" FORCE)

set(OCCT_DIR "$ENV{HOME}/occt-install")
include_directories("${OCCT_DIR}/include/opencascade")
link_directories("${OCCT_DIR}/lib")

add_executable(CAM geometry_utils.cpp main.cpp step_test.cpp)

target_link_libraries(CAM
    TKernel
    TKMath
    TKG2d
    TKG3d
    TKBRep
    TKGeomBase
    TKGeomAlgo
    TKTopAlgo
    TKPrim
    TKShHealing
    TKBool
    TKFillet
    TKFeat
    TKOffset
    TKMesh
    TKSTEP
    TKSTEPAttr
    TKSTEPBase
    TKXSBase
    TKDE
    TKExpress
)
