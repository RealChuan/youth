#设定版本号
cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "/usr/local/share/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

#设定工程名
project(youth VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CURRENT_SOURCE_DIR ON)

#set(CMAKE_CXX_FLAGS_DEBUG "-O0")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -Wconversion -Wvla -Wundef
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror -W -Wextra -Wpointer-arith -Wwrite-strings -Wunused -Wshadow -Winline -Wfloat-equal -Wsign-compare -Wno-undef -Wendif-labels -Wcast-align -Wtype-limits -Wempty-body -Wignored-qualifiers -Wno-unused-parameter -Wold-style-cast -Wno-vla -Woverloaded-virtual -Wdouble-promotion")

set(CMAKE_DEBUG_POSTFIX _d)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CURRENT_PLATFORM "-64")
else()
    set(CURRENT_PLATFORM "-32")
endif()

message(STATUS "Current Platform is ${CURRENT_PLATFORM}")

#设定可执行二进制文件的目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin${CURRENT_PLATFORM}/${CMAKE_BUILD_TYPE})  #源文件目录
#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)     #输出文件目录

#设定存放编译出来的库文件的目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib${CURRENT_PLATFORM}/${CMAKE_BUILD_TYPE})
#SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

#并且把该目录设为连接目录
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib${CURRENT_PLATFORM}/${CMAKE_BUILD_TYPE})
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

find_package(ZLIB REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
if(PROTOBUF_FOUND)
  message(STATUS "found protobuf")
endif()
if(ZLIB_FOUND)
  message(STATUS "found zlib")
endif()
if(benchmark_FOUND)
  message(STATUS "found benchmark")
endif()
if(GTest_FOUND)
  message(STATUS "found GTest")
endif()


include_directories( ${PROJECT_SOURCE_DIR} )

#增加子文件夹(注意我们的src路径是通过子文件夹的形式添加的，在这里面也是可以写CMakeLists.txt的)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/core)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/utils)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/youth/net)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/examples)

message("CMAKE_MAJOR_VERSION: ${CMAKE_MAJOR_VERSION}")
message("CMAKE_MINOR_VERSION: ${CMAKE_MINOR_VERSION}")
message("CMAKE_PATCH_VERSION: ${CMAKE_PATCH_VERSION}")
message("CMAKE_TWEAK_VERSION: ${CMAKE_TWEAK_VERSION}")
message("CMAKE_VERSION: ${CMAKE_VERSION}")
message("CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
message("CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
