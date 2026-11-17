#!/bin/sh
# Builds the macOS PCANBasic JNI bridge (see PCANBasic_JNI_macos.c) into
# java_console/libpcanbasic_jni.dylib as a universal arm64 + x86_64 binary.
# Must run on macOS with Xcode command line tools and a JDK (for jni.h).
# MacCAN's libPCBUSB is NOT needed at build time: the bridge dlopen()s it.
set -e
cd "$(dirname "$0")"

if [ -z "$JAVA_HOME" ]; then
  JAVA_HOME=$(/usr/libexec/java_home)
fi

OUT=../../java_console/libpcanbasic_jni.dylib
ARCHS="${ARCHS:--arch arm64 -arch x86_64}"

# shellcheck disable=SC2086
cc -dynamiclib -O2 -Wall -Wextra $ARCHS \
   -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/darwin" \
   -o "$OUT" PCANBasic_JNI_macos.c

file "$OUT"
