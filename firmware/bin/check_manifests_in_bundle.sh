#!/bin/bash

SCRIPT_DIR=$(realpath $(dirname "$0"))
BUNDLE_TO_CHECK=$1
CHECK_BUNDLE_DIR=bundle_check

rm -fr "$CHECK_BUNDLE_DIR"
mkdir "$CHECK_BUNDLE_DIR"

unzip "$BUNDLE_TO_CHECK" -d "$CHECK_BUNDLE_DIR"

EXIT_CODE=0
if ! find "$CHECK_BUNDLE_DIR" -type f -name "*.jar" -print0 | xargs -n1 -0 "$SCRIPT_DIR/check_manifest_in_jar.sh"; then
	echo "ERROR: some bundle .jar files have invalid minifest (for details see errors above)"
	EXIT_CODE=1
fi

rm -fr "$CHECK_BUNDLE_DIR"
exit $EXIT_CODE
