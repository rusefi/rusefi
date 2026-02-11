#!/usr/bin/env bash

SCAN_DIR=$1

# The full path of the firmware directory
FDIR=$(cd "$(dirname "$0")/.."; pwd -P)

cd $FDIR

getTarget ()
{
	source config/boards/common_script_read_meta_env.inc $1 >/dev/null
	SKIP_RATE=${SKIP_RATE:-0}
	ONLY=$(echo "$COMMIT_MESSAGE" | grep -Po '(?<=only:)[^\s]*')
	if [[ ( "$EVENT_NAME" == 'workflow_dispatch' \
						|| "$EVENT_NAME" == 'schedule' \
						|| "$EVENT_NAME" == 'pull_request' \
						|| "$RUN_ATTEMPT" -gt 1 ) \
					&& "SKIP_RATE" -lt 100 \
					|| ( $((RANDOM % 100)) -ge "$SKIP_RATE" && -z "$ONLY" ) \
					|| "$ONLY" == "$BUNDLE_NAME" ]]; then
		echo "{\"build-target\": \"$BUNDLE_NAME\", \"meta-info\": \"$1\"},"
	fi
}

echo -n '{"include": ['
find ${SCAN_DIR} -name "meta-info*.env" -print0 | while IFS= read -r -d '' f; do
	echo -n "$(getTarget $f)"
done
echo "]}"

