cmake_minimum_required(VERSION 3.15)

project(OpenBLTJni)

set(CMAKE_BUILD_TYPE "Debug")


# Set OpenBLT library related directory locations
set(LIBOPENBLT_LIB ${PROJECT_SOURCE_DIR}/../../firmware/ext/openblt/Host/)
set(LIBOPENBLT_INC ${LIBOPENBLT_LIB}/Source/LibOpenBLT)

include_directories("${LIBOPENBLT_INC}")
link_directories("${LIBOPENBLT_LIB}")


add_library(openblt_jni SHARED)

target_sources(openblt_jni PRIVATE openblt_jni.cpp)

