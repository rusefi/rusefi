#!/usr/bin/env bash
# OpenBLT over CAN, using Java 17 and a PCAN adapter/native library.
# Usage: bash java_console/bin/openblt_can.sh [options] [firmware.srec]
set -e
console_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
# Incremental build prevents accidentally running a stale flasher.
(cd "$console_dir/.." && ./gradlew :openblt_can:fatJar)
# Keep the caller's working directory and arguments, including relative SREC paths.
exec java "-Djava.library.path=$console_dir" -cp "$console_dir/openblt_can/build/libs/openblt_can-all.jar" \
  com.rusefi.openblt.OpenBltCanFlasher "$@"
