#!/bin/bash
# Flash the connected ECU through the OpenBLT bootloader over CAN.
# Requires a PCAN-USB adapter (MacCAN/libPCBUSB on macOS, PEAK driver on
# Windows/Linux) and the rusefi_update.srec application image.
#
# Usage: openblt_can.sh [options] [firmware.srec]
#   see com.rusefi.openblt.OpenBltCanFlasher --help

cd "$(dirname "$0")/.." || exit 1

JAR=openblt_can/build/libs/openblt_can-all.jar
# Always run the gradle task: it is incremental and no-ops when the sources
# are up to date, while the old "build only if missing" check silently
# reused a stale jar (the 2026-08-22 PcanLink poll fix first ran from an
# old jar on one occasion).
(cd .. && ./gradlew :openblt_can:fatJar) || exit 1

# libpcanbasic_jni.dylib / PCANBasic_JNI.dll live in java_console/
exec java -Djava.library.path=. -cp "$JAR" com.rusefi.openblt.OpenBltCanFlasher "$@"
