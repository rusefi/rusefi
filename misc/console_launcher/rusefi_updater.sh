#!/bin/sh

cd "$(dirname "$0")/console"
# -Djava.library.path=. lets the JVM find libpcanbasic_jni.dylib (macOS PCAN
# bridge) and any other native helpers shipped next to rusefi_console.jar.
# -Xss512k halves the per-thread stack: the console's serial port scanner
# spawns one probe thread per port and on macOS these pile up until the JVM
# hits the native thread limit ("unable to create native thread"); the smaller
# stack doubles the thread budget. PCAN-only setups can also skip the serial
# port scan entirely by adding -Dserial.port.scan=false
java -Xss512k -Djava.library.path=. -jar ./rusefi_console.jar
