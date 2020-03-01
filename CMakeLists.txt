#设定版本号
cmake_minimum_required(VERSION 3.5)

#设定工程名
project(youth LANGUAGES CXX)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#设置编译选项
#允许c++11标准、O3优化、多线程。match选项可避免一些cpu上的问题
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

#设定可执行二进制文件的目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  #源文件目录
#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)     #输出文件目录

#设定存放编译出来的库文件的目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

#并且把该目录设为连接目录
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

#增加子文件夹(注意我们的src路径是通过子文件夹的形式添加的，在这里面也是可以写CMakeLists.txt的)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/core)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/utils)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/net)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/examples)

#AUX_SOURCE_DIRECTORY(. DIR_SRCS)
