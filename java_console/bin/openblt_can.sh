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

# Resolve relative file arguments against the CALLER's cwd BEFORE the cd
# below - the flasher resolves its paths at startup and a relative srec
# path passed from the repo root would otherwise resolve inside
# java_console/ (the 2026-08-23 run failed with NoSuchFileException).
resolved_args=()
for arg in "$@"; do
  case "$arg" in
    -*) resolved_args+=("$arg") ;;
    *)
      if [ -e "$arg" ]; then
        resolved_args+=("$(cd "$(dirname "$arg")" && pwd)/$(basename "$arg")")
      else
        resolved_args+=("$arg")
      fi
      ;;
  esac
done

# libpcanbasic_jni.dylib / PCANBasic_JNI.dll live in java_console/
exec java -Djava.library.path=. -cp "$JAR" com.rusefi.openblt.OpenBltCanFlasher "${resolved_args[@]}"
