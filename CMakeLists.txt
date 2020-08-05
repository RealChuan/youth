#设定版本号
cmake_minimum_required(VERSION 3.5)

#SET(CMAKE_TOOLCHAIN_FILE "/home/ubuntu64/code/vcpkg/scripts/buildsystems/vcpkg.cmake")

#设定工程名
project(youth C CXX)

#find_package(Boost REQUIRED)

#include_directories( ${Boost_INCLUDE_DIRS} )

include_directories( ${PROJECT_SOURCE_DIR} )

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

#设置编译选项
set(CXX_FLAGS
    -g
    # -DVALGRIND
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall
    -Wextra
    -Werror
    -Wconversion
    -Wno-unused-parameter
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    -Wshadow
    -Wwrite-strings
    -march=native
    # -MMD
    -std=c++17
    -rdynamic
    )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

#设定可执行二进制文件的目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  #源文件目录
#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)     #输出文件目录

#设定存放编译出来的库文件的目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

#并且把该目录设为连接目录
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

#增加子文件夹(注意我们的src路径是通过子文件夹的形式添加的，在这里面也是可以写CMakeLists.txt的)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/core)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/utils)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/net)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/examples)

#AUX_SOURCE_DIRECTORY(. DIR_SRCS)
