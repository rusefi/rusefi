#!/bin/sh
# Linux/macOS counterpart of rusefi_updater.exe: chdir into the bundle's console folder
# (rusefi_updater.xml does the same via launch4j <chdir>) and start the console.
# Arguments are forwarded so that Autoupdate#startConsoleAsANewProcess can pass them on.

cd "$(dirname "$0")/console"
# -Djava.library.path=. lets the JVM find libpcanbasic_jni.dylib (macOS PCAN
# bridge) and any other native helpers shipped next to rusefi_console.jar
java -Djava.library.path=. -jar ./rusefi_console.jar
