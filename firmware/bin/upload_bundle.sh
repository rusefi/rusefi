#!/usr/bin/env bash

USER=$1
PASS=$2
HOST=$3

SCRIPT_NAME=$(basename "$0")
# technical debt: more than one file uses magic 'rusefi_bundle_' constant, can we extract constant?
FULL_BUNDLE_FILE="rusefi_bundle_${BUNDLE_NAME}.zip"
UPDATE_BUNDLE_FILE="rusefi_bundle_${BUNDLE_NAME}_autoupdate.zip"

if [ -n "${USER}" -a -n "$PASS" -a -n "${HOST}" ]; then
 echo "$SCRIPT_NAME: Uploading both bundles"
 RET=0
 if [ "$LTS" == "true" -a -n "$REF" ]; then
   DESRINATION_FOLDER="build_server/lts/${REF}"
 else
   DESRINATION_FOLDER="build_server"
 fi
 tar -czf - $FULL_BUNDLE_FILE    | sshpass -p $PASS ssh -o StrictHostKeyChecking=no ${USER}@${SERVER} "mkdir -p ${DESTINATION_FOLDER};            tar -xzf - -C ${DESTINATION_FOLDER}"
 RET=$((RET+$?+PIPESTATUS))
 tar -czf - $UPDATE_BUNDLE_FILE  | sshpass -p $PASS ssh -o StrictHostKeyChecking=no ${USER}@${SERVER} "mkdir -p ${DESTINATION_FOLDER}/autoupdate; tar -xzf - -C ${DESTINATION_FOLDER}/autoupdate"
 RET=$((RET+$?+PIPESTATUS))
 if [ $RET -ne 0 ]; then
  echo "$SCRIPT_NAME: Bundle upload failed"
  exit 1
 fi
else
 echo "$SCRIPT_NAME: Upload not configured"
fi
