#!/bin/sh
# Linux/macOS counterpart of rusefi_updater.exe: chdir into the bundle's console folder
# (rusefi_updater.xml does the same via launch4j <chdir>) and start the console.
# Arguments are forwarded so that Autoupdate#startConsoleAsANewProcess can pass them on.

cd "$(dirname "$0")/console" || exit 1
# -Djava.library.path=. lets System.loadLibrary find native helpers shipped next to
# rusefi_console.jar, notably libpcanbasic_jni.dylib (macOS PCAN bridge over MacCAN):
# unlike Windows, macOS does not search the working directory for JNI libraries.
exec java -Djava.library.path=. -jar ./rusefi_console.jar "$@"
