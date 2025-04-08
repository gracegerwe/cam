cmake_minimum_required(VERSION 3.10)
project(CAM)

# Enable Qt's meta-object compiler for signals/slots
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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
    TKXSDRAW
)

# GUI (adds Qt6 too)
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

target_link_libraries(cam_gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    TKV3d
    TKOpenGl
    TKService
    TKOpenGl
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
    TKXSDRAW
)