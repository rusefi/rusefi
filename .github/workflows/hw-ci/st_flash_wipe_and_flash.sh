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

ATTEMPT_LOG=$(mktemp) || exit 1
trap 'rm -f "$ATTEMPT_LOG"' EXIT

# Run an st-flash command until it succeeds or we run out of attempts.
# After failures, list all probes so the log shows whether the selected probe
# itself disappeared or only the target refused to attach.
run_with_retry() {
	local label=$1
	shift
	local attempt status output reported_failure
	local -a pipeline_status
	for attempt in $(seq 1 "$ATTEMPTS"); do
		echo "[st_flash_wipe_and_flash.sh] $label: attempt $attempt of $ATTEMPTS"
		LC_ALL=C st-flash "${SERIAL_ARGS[@]}" "$@" 2>&1 | tee "$ATTEMPT_LOG"
		pipeline_status=("${PIPESTATUS[@]}")
		status=${pipeline_status[0]}
		if [ "${pipeline_status[1]}" -ne 0 ]; then
			echo "[st_flash_wipe_and_flash.sh] $label: cannot capture diagnostics (st-flash exit $status, tee exit ${pipeline_status[1]})"
			return 1
		fi
		output=$(cat "$ATTEMPT_LOG")
		reported_failure=false
		if [[ "$output" =~ (^|[[:space:]])0[[:space:]]+KiB[[:space:]]+flash ]]; then
			echo "[st_flash_wipe_and_flash.sh] $label: target reports zero flash size; chip/probe identification alone does not confirm usable flash access. Check target power, NRST and SWD wiring/jumpers."
			reported_failure=true
		fi
		if [[ "$output" == *"Soft reset failed"* || "$output" == *"Failed to reset device"* ]]; then
			echo "[st_flash_wipe_and_flash.sh] $label: target reset failed despite connect-under-reset. Check NRST and target power; st-flash 1.8.0 can return exit 0 after a reset failure."
			reported_failure=true
		fi
		if [[ "$output" == *"Unknown memory region"* ]]; then
			echo "[st_flash_wipe_and_flash.sh] $label: st-flash rejected the address; check the detected flash size above (zero size also causes this error)."
			reported_failure=true
		fi
		if [ "$status" -eq 0 ] && [ "$reported_failure" = false ]; then
			echo "[st_flash_wipe_and_flash.sh] $label: OK"
			return 0
		fi
		echo "[st_flash_wipe_and_flash.sh] $label: failed (st-flash exit $status, reported failure: $reported_failure)"
		echo "[st_flash_wipe_and_flash.sh] Probe diagnostics: all attached probes; selected serial=[${HARDWARE_CI_STLINK_SERIAL:-any}]"
		if st-info --probe; then
			:
		else
			status=$?
			echo "[st_flash_wipe_and_flash.sh] st-info --probe failed (exit $status)"
		fi
		if [ "$attempt" -lt "$ATTEMPTS" ]; then
			echo "[st_flash_wipe_and_flash.sh] Retrying $label in $RETRY_DELAY seconds"
			sleep "$RETRY_DELAY"
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
