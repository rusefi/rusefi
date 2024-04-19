#!/usr/bin/env bash

USER=$1
PASS=$2
HOST=$3
BUNDLE_FILE_NAME=$4

SCRIPT_NAME=$(basename "$0")

if [ -n "${USER}" -a -n "$PASS" -a -n "${HOST}" ]; then
 echo "$SCRIPT_NAME: Uploading both bundles"

 if [ -n "${BUNDLE_FILE_NAME}" ]; then
   echo "$SCRIPT_NAME: BUNDLE_FILE_NAME is ${BUNDLE_FILE_NAME}"
 else
   echo "$SCRIPT_NAME: BUNDLE_FILE_NAME argument not specified"
   exit 1
 fi
 if [ -n "${bundle_upload_folder}" ]; then
   echo "$SCRIPT_NAME: bundle_upload_folder is ${bundle_upload_folder}"
 else
   bundle_upload_folder="rusefi_bundle"
   echo "$SCRIPT_NAME: bundle_upload_folder env variable was not specified using default ${bundle_upload_folder}"
 fi

 # technical debt: more than one file uses magic 'rusefi_bundle_' constant, can we extract constant?
 FULL_BUNDLE_FILE="rusefi_bundle_${BUNDLE_FILE_NAME}.zip"
 UPDATE_BUNDLE_FILE="rusefi_bundle_${BUNDLE_FILE_NAME}_autoupdate.zip"

     # sftp does not support -p flag on mkdir :(
     sshpass -p $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
mkdir ${bundle_upload_folder}
SSHCMD

 RET=0
 if [ "$LTS" == "true" -a -n "$REF" ]; then
   DESTINATION_FOLDER="${bundle_upload_folder}/lts/${REF}"
     # sftp does not support -p flag on mkdir :(
     sshpass -p $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
mkdir ${bundle_upload_folder}/lts
mkdir ${DESTINATION_FOLDER}
SSHCMD
 else
   DESTINATION_FOLDER="${bundle_upload_folder}"
 fi

     # sftp does not support -p flag on mkdir :(
     sshpass -p $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
cd ${DESTINATION_FOLDER}
put $FULL_BUNDLE_FILE
mkdir autoupdate
cd autoupdate
put $UPDATE_BUNDLE_FILE
SSHCMD
 echo "$SCRIPT_NAME: DONE $FULL_BUNDLE_FILE"
else
 echo "$SCRIPT_NAME: Upload not configured"
fi
