#!/usr/bin/env bash

# Erase the whole main flash and write a fresh image via st-flash, with retries.
#
# Usage:
#   .github/workflows/hw-ci/st_flash_wipe_and_flash.sh [path/to/rusefi.bin]
#
# Optional environment:
#   HARDWARE_CI_STLINK_SERIAL  - pick a specific ST-LINK when several are attached
#   ST_FLASH_ATTEMPTS          - attempts per st-flash operation (default 5)
#   ST_FLASH_RETRY_DELAY       - seconds between attempts (default 3)
#
# Why the retries and --connect-under-reset everywhere: an ECU that is already running
# firmware can refuse a plain SWD attach (sleep modes, SWD pins re-used, or the debug
# port simply busy), and st-flash then reports
#   "Can not connect to target. Please use 'connect under reset' and try again"
# Holding NRST while attaching works around that; a short retry loop covers the rest
# (ST-LINK USB re-enumeration after the previous run, transient probe hiccups).

set -u

IMAGE=${1:-firmware/deliver/rusefi.bin}
ATTEMPTS=${ST_FLASH_ATTEMPTS:-5}
RETRY_DELAY=${ST_FLASH_RETRY_DELAY:-3}

if [ ! -f "$IMAGE" ]; then
	echo "[st_flash_wipe_and_flash.sh] image not found: $IMAGE"
	exit 1
fi

SERIAL_ARGS=()
if [ -n "${HARDWARE_CI_STLINK_SERIAL:-}" ]; then
	SERIAL_ARGS=(--serial "$HARDWARE_CI_STLINK_SERIAL")
fi

# Run an st-flash command until it succeeds or we run out of attempts.
# Between attempts we re-probe the ST-LINK so the log shows whether the probe
# itself disappeared or only the target refused to attach.
run_with_retry() {
	local label=$1
	shift
	local attempt
	for attempt in $(seq 1 "$ATTEMPTS"); do
		echo "[st_flash_wipe_and_flash.sh] $label: attempt $attempt of $ATTEMPTS"
		if st-flash "${SERIAL_ARGS[@]}" "$@"; then
			echo "[st_flash_wipe_and_flash.sh] $label: OK"
			return 0
		fi
		echo "[st_flash_wipe_and_flash.sh] $label: failed (exit $?)"
		if [ "$attempt" -lt "$ATTEMPTS" ]; then
			sleep "$RETRY_DELAY"
			st-info --probe || true
		fi
	done
	echo "[st_flash_wipe_and_flash.sh] $label: giving up after $ATTEMPTS attempts"
	return 1
}

echo "[st_flash_wipe_and_flash.sh] enter [$IMAGE] serial=[${HARDWARE_CI_STLINK_SERIAL:-any}]"
st-flash --version

# Full erase so every HW CI run starts from default settings, not whatever the
# previous run left in the settings sector.
run_with_retry "erase" --connect-under-reset --area=main erase || exit 1

run_with_retry "write" --connect-under-reset --reset --format=binary write "$IMAGE" 0x08000000 || exit 1

echo "[st_flash_wipe_and_flash.sh] Done!"
