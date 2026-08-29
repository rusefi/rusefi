#!/bin/bash
# Record a PCAN-USB CAN bus dump to a PCAN-View compatible .trc file.
# Requires a PCAN-USB adapter (MacCAN/libPCBUSB on macOS, PEAK driver on
# Windows/Linux). Runs the adapter in listen-only mode by default - the
# sniffer never ACKs on the bus, so capturing does not disturb the ECU.
#
# Usage: can_dump.sh [options]
#   see com.rusefi.candump.CanDump --help
#
# NOTE: close the rusEFI console first - it holds the PCAN adapter exclusively.

cd "$(dirname "$0")/.." || exit 1

JAR=mcp_can/build/libs/mcp_can-all.jar
(cd .. && ./gradlew :mcp_can:fatJar) || exit 1

# libpcanbasic_jni.dylib / PCANBasic_JNI.dll live in java_console/
exec java -Djava.library.path=. -cp "$JAR" com.rusefi.candump.CanDump "$@"
