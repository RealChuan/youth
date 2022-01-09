#设定版本号
cmake_minimum_required(VERSION 3.5)

#set(CMAKE_TOOLCHAIN_FILE "/home/youth/code/vcpkg/scripts/buildsystems/vcpkg.cmake")

#设定工程名
project(youth C CXX)

#设置编译选项
set(CXX_FLAGS
    -g
    # -DVALGRIND
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall
    -Wextra
    -Werror
    # -Wconversion
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

if(CMAKE_BUILD_BITS EQUAL 32)
    list(APPEND CXX_FLAGS "-m32")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND CXX_FLAGS "-Wno-null-dereference")
    list(APPEND CXX_FLAGS "-Wno-sign-conversion")
    list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
    list(APPEND CXX_FLAGS "-Wthread-safety")
    list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

include_directories( ${PROJECT_SOURCE_DIR} )

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

#find_package(Protobuf)
#if(PROTOBUF_FOUND)
#    include_directories(${Protobuf_INCLUDE_DIRS})
#endif()

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
