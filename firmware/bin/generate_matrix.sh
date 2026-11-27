#!/usr/bin/env bash

SCAN_DIR=$1

# The full path of the firmware directory
FDIR=$(cd "$(dirname "$0")/.."; pwd -P)

PR_UAEFI_ONLY=false
if [[ "$EVENT_NAME" == pull_request ]]; then
	parents=$(git -C "$FDIR/.." rev-list --parents -n 1 HEAD 2>/dev/null) || parents=
	read -r -a parent_list <<< "$parents"
	if [[ ${#parent_list[@]} -eq 3 \
		&& -n "$PR_BASE_SHA" && -n "$PR_HEAD_SHA" \
		&& "${parent_list[1]}" == "$PR_BASE_SHA" \
		&& "${parent_list[2]}" == "$PR_HEAD_SHA" ]]; then
		git -C "$FDIR/.." diff --quiet --no-renames "${parent_list[1]}" HEAD -- .
		diff_status=$?
		if [[ $diff_status -eq 1 ]] && git -C "$FDIR/.." diff --quiet --no-renames \
			"${parent_list[1]}" HEAD -- . \
			':(top,exclude,glob)java_console/ui/src/**/*.java' \
			':(top,exclude,glob)java_console/inifile/src/**/*.java' \
			':(top,exclude)java_console/ui/src/main/resources' \
			':(top,exclude,glob)java_console/*/src/test/**' \
			':(top,exclude,glob)java_console/*/src/testFixtures/**' \
			':(top,exclude,glob)*.md' \
			':(top,exclude,glob)docs/**/*.md' \
			':(top,exclude).github/workflows/build-firmware.yaml' \
			':(top,exclude)firmware/bin/generate_matrix.sh' \
			':(top,exclude)misc/actions/add-ubuntu-latest-apt-mirrors.sh' \
			':(top,exclude)misc/actions/ubuntu-install-tools.sh'; then
			PR_UAEFI_ONLY=true
		fi
	fi
fi

getTarget ()
{
	source "${2}/config/boards/common_script_read_meta_env.inc" "$1" >/dev/null
	# H7 builds require an externally supplied hardware port.
	if [[ "$PROJECT_CPU" == ARCH_STM32H7 ]]; then
		return
	fi
	SKIP_RATE=${SKIP_RATE:-0}
	ONLY=$(echo "$COMMIT_MESSAGE" | grep -Po '(?<=only:)[^\s]*')
	if [[ ( "$EVENT_NAME" == 'workflow_dispatch' \
						|| "$EVENT_NAME" == 'schedule' \
						|| "$EVENT_NAME" == 'pull_request' \
						|| "$RUN_ATTEMPT" -gt 1 ) \
					&& "SKIP_RATE" -lt 100 \
					|| ( $((RANDOM % 100)) -ge "$SKIP_RATE" && -z "$ONLY" ) \
					|| "$ONLY" == "$BUNDLE_NAME" ]]; then
		printf '%s\n' "$BUNDLE_NAME"
	fi
}

TARGETS=()
UA_EFI_ENTRY=
while IFS= read -r -d '' F; do
	TARGET_NAME=$(getTarget "$F" "$FDIR")
	if [[ -n "$TARGET_NAME" ]]; then
		TARGET_ENTRY="{\"build-target\": \"$TARGET_NAME\", \"meta-info\": \"$F\"}"
		TARGETS+=("$TARGET_ENTRY")
		if [[ "$TARGET_NAME" == uaefi ]]; then
			UA_EFI_ENTRY=$TARGET_ENTRY
		fi
	fi
done < <(find "${SCAN_DIR}" -name "meta-info*.env" -print0)

# If this checkout has no uaefi target, retain the existing PR coverage.
if [[ "$PR_UAEFI_ONLY" == true && -n "$UA_EFI_ENTRY" ]]; then
	TARGETS=("$UA_EFI_ENTRY")
fi
IFS=,
printf '{"include": [%s]}\n' "${TARGETS[*]}"
