cmake_minimum_required(VERSION 3.15)

project(OpenBLTJni)

set(CMAKE_BUILD_TYPE "Debug")

find_package(Java COMPONENTS Development)
set(JAVA_AWT_INCLUDE_PATH NotNeeded)
find_package(JNI REQUIRED)

# Set OpenBLT library related directory locations
set(LIBOPENBLT_LIB ${PROJECT_SOURCE_DIR}/../../firmware/ext/openblt/Host/)
set(LIBOPENBLT_INC ${LIBOPENBLT_LIB}/Source/LibOpenBLT)

add_library(openblt_jni SHARED)

target_include_directories(openblt_jni PRIVATE ${LIBOPENBLT_INC})
target_include_directories(openblt_jni PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(openblt_jni ${LIBOPENBLT_LIBNAME})
target_sources(openblt_jni PRIVATE openblt_jni.cpp)
