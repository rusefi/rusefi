#!/bin/sh

cd "$(dirname "$0")/console"
# -Djava.library.path=. lets the JVM find libpcanbasic_jni.dylib (macOS PCAN
# bridge) and any other native helpers shipped next to rusefi_console.jar
java -Djava.library.path=. -jar ./rusefi_console.jar
