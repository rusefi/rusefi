#!/bin/bash

JAR_TO_CHECK=$1
CHECK_MANIFEST_DIR=manifest_check
MANIFEST_FILE_IN_JAR=META-INF/MANIFEST.MF

rm -fr "$CHECK_MANIFEST_DIR"
mkdir "$CHECK_MANIFEST_DIR"

unzip "$JAR_TO_CHECK" "$MANIFEST_FILE_IN_JAR" -d "$CHECK_MANIFEST_DIR"

EXIT_CODE=0

if [[ "$JAR_TO_CHECK" == *_autoupdate.jar ]] || [[ "$JAR_TO_CHECK" == *_console.jar ]]; then
  echo "Checking manifest in $JAR_TO_CHECK..."
	if ! grep -q "Main-Class: " "$CHECK_MANIFEST_DIR/$MANIFEST_FILE_IN_JAR"; then
		echo "ERROR: '$MANIFEST_FILE_IN_JAR' file in '$JAR_TO_CHECK' doesn't contain 'Main-Class' definition"
		EXIT_CODE=1
	fi
fi

rm -fr "$CHECK_MANIFEST_DIR"
exit $EXIT_CODE
