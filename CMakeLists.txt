# 版本限定
cmake_minimum_required(VERSION 3.10.2)
# 工程名称
project(CatImg)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
#设置编译链
SET(CMAKE_C_COMPILER "aarch64-buildroot-linux-gnu-gcc")
SET(CMAKE_CXX_COMPILER "aarch64-buildroot-linux-gnu-g++")
SET(CMAKE_AR "aarch64-buildroot-linux-gnu-ar")
SET(CMAKE_LINKER "aarch64-buildroot-linux-gnu-ld")

#opencv库路径
set(OpenCV_DIR ${CMAKE_CURRENT_LIST_DIR}/user/rk3588_opencv455/lib/cmake/opencv4)
find_package(OpenCV REQUIRED COMPONENTS imgproc core highgui videoio)  # 指定需要的 OpenCV 组件
include_directories(${OpenCV_INCLUDE_DIRS})

# 指定源文件列表
set(SRC_LIST
    src/CatImg.cpp
)

# 指定源文件列表
set(SRC_LIST_MOVIE
    src/CatMovie.cpp
)

# 编译命令行显示图片的可执行文件
add_executable(CatImg ${SRC_LIST})
target_link_libraries (CatImg ${OpenCV_LIBS})

# 编译命令行显示视频的可执行文件
add_executable(CatMovie ${SRC_LIST_MOVIE})
target_link_libraries (CatMovie ${OpenCV_LIBS})
