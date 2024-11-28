cmake_minimum_required(VERSION 3.16.3)
 
# 项目信息
project(MVS_WORK)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/zipper/Documents/MVS_WORK")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "/home/zipper/Documents/ARMOR_DETECTED/libs")
 
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(OpenCV REQUIRED)
# 包含目录
include_directories( ${OpenCV_INCLUDE_DIRS} include /opt/MVS/include)
link_directories(/opt/MVS/lib/64)

# 添加可执行文件

add_library(MVS_WORK src/MVS_WORK.cpp)
add_executable(main main.cpp
                    src/MVS_WORK.cpp  ) 

target_link_libraries(MVS_WORK 
                        ${OpenCV_LIBS} 
                        MvCameraControl
                        MvCamLVision
                        MvCameraControlWrapper
                        MVGigEVisionSDK
                        MvUsb3vTL
                        X11
                        pthread
                        m
)
target_link_libraries(main MVS_WORK
                           pthread
                           ${OpenCV_LIBS} 
                           m)