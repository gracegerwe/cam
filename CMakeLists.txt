cmake_minimum_required(VERSION 3.10)
project(CAM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json" FORCE)

set(OCCT_DIR "$ENV{HOME}/occt-install")
include_directories("${OCCT_DIR}/include/opencascade")
link_directories("${OCCT_DIR}/lib")

# CLI App
add_executable(cam_cli
    src/main_cli.cpp
    src/geometry_utils.cpp
)

# GUI App
add_executable(cam_gui
    src/main_gui.cpp
    src/main_window.cpp
    src/viewer/OCCTWidget.cpp
    src/geometry_utils.cpp
)

# CLI (minimal link, OpenCascade only)
target_link_libraries(cam_cli
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
    TKDESTEP
    TKXSBase
    TKDE
    TKExpress
    TKXSDRAWSTEP
)

# GUI (adds Qt6 too)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL)

target_link_libraries(cam_gui
    Qt6::Widgets
    Qt6::OpenGL
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
    TKDESTEP
    TKXSBase
    TKDE
    TKExpress
    TKXSDRAWSTEP
)