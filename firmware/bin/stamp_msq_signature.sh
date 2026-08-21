#!/bin/bash

# file stamp_msq_signature.sh
# Updates the signature/firmwareInfo/bibliography stamps inside a board default .msq
# so a checked-in default tune always matches the firmware that was just generated:
# a rebuild that rolls the build date (or changes the config hash) refreshes the tune
# too, instead of leaving it with a stale signature the console cannot resolve.
#
# usage: stamp_msq_signature.sh <msq file> <generated signature header>

# fail fast
set -e

MSQ_FILE=$1
SIG_HEADER=$2

if [ -z "${MSQ_FILE}" ] || [ -z "${SIG_HEADER}" ] || [ ! -f "${MSQ_FILE}" ] || [ ! -f "${SIG_HEADER}" ]; then
    echo "usage: stamp_msq_signature.sh <msq file> <generated signature header>" >&2
    exit 1
fi

# generated header contains: #define TS_SIGNATURE "rusEFI branch.YYYY.MM.DD.board.hash"
SIGNATURE=$(grep -o '#define TS_SIGNATURE "[^"]*"' "${SIG_HEADER}" | sed 's/.*"\([^"]*\)"/\1/')
if [ -z "${SIGNATURE}" ]; then
    echo "stamp_msq_signature.sh: TS_SIGNATURE not found in ${SIG_HEADER}" >&2
    exit 1
fi

# YYYYMMDD date part of the signature -> firmwareInfo/author stamp
# signature layout: rusEFI branch.YYYY.MM.DD.target.hash
FIRMWARE_INFO=$(echo "${SIGNATURE}" | sed -n 's/^[^.]*\.\([0-9][0-9][0-9][0-9]\)\.\([0-9][0-9]\)\.\([0-9][0-9]\)\..*/\1\2\3/p')
if [ -z "${FIRMWARE_INFO}" ]; then
    echo "stamp_msq_signature.sh: cannot parse date out of signature [${SIGNATURE}]" >&2
    exit 1
fi

TEMP_FILE="${MSQ_FILE}.stamped"

SIGNATURE="${SIGNATURE}" FIRMWARE_INFO="${FIRMWARE_INFO}" perl -pe '
s/signature="[^"]*"/signature="$ENV{SIGNATURE}"/g;
s/firmwareInfo="[^"]*"/firmwareInfo="$ENV{FIRMWARE_INFO}"/g;
s/author="rusEFI [^"]*"/author="rusEFI $ENV{FIRMWARE_INFO}"/g;
' "${MSQ_FILE}" > "${TEMP_FILE}"

# Only replace the real file when something actually changed, so config re-runs on the
# same day do not churn the working tree (same trick gen_signature.sh uses).
if cmp -s "${TEMP_FILE}" "${MSQ_FILE}"; then
    echo "stamp_msq_signature.sh: ${MSQ_FILE} already up to date (${SIGNATURE})"
    rm -f "${TEMP_FILE}"
else
    mv -f "${TEMP_FILE}" "${MSQ_FILE}"
    echo "stamp_msq_signature.sh: ${MSQ_FILE} stamped with ${SIGNATURE}"
fi
