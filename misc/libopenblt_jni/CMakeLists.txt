cmake_minimum_required(VERSION 3.15)

project(OpenBLTJni)

set(CMAKE_BUILD_TYPE "Debug")

find_package(Java COMPONENTS Development)
set(JAVA_AWT_INCLUDE_PATH NotNeeded)
find_package(JNI REQUIRED)

if(APPLE)
	# Build both arm64 and Intel
	set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif(APPLE)

set(CXX_STANDARD 20)

# Set OpenBLT library related directory locations
set(LIBOPENBLT_LIB ${PROJECT_SOURCE_DIR}/../../firmware/ext/openblt/Host/)
set(LIBOPENBLT_INC ${LIBOPENBLT_LIB}/Source/LibOpenBLT)

# Set library name of the OpenBLT Host Library
if(CMAKE_C_COMPILER_ID MATCHES MSVC)
  # When building LibOpenBLT with Microsoft Visual Studio, "lib" was added to the name
  # of the DLL. This needs to be done as well when linking the library.
  set (LIBOPENBLT_LIBNAME libopenblt)
else()
  set (LIBOPENBLT_LIBNAME openblt)
endif()

add_library(openblt_jni SHARED)

target_include_directories(openblt_jni PRIVATE ${LIBOPENBLT_INC})
target_include_directories(openblt_jni PRIVATE ${JNI_INCLUDE_DIRS})
target_link_directories(openblt_jni PRIVATE ${LIBOPENBLT_LIB})
target_link_libraries(openblt_jni PRIVATE ${LIBOPENBLT_LIBNAME})
target_sources(openblt_jni PRIVATE openblt_jni.cpp)
