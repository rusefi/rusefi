#!/usr/bin/env bash

USER=$1
PASS=$2
HOST=$3
BUNDLE_FILE_NAME=$4
RELEASE_TAG=$5

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
   bundle_upload_folder="build_server"
   echo "$SCRIPT_NAME: bundle_upload_folder env variable was not specified using default ${bundle_upload_folder}"
 fi

 if [ -n "${RELEASE_TAG}" ]; then
   echo "$SCRIPT_NAME: RELEASE_TAG is ${RELEASE_TAG}"
 fi

 if [[ -z "${WHITE_LABEL}" ]]; then
     echo "WHITE_LABEL environment variable is not specified"
     exit 1
 fi

 FULL_BUNDLE_FILE="${WHITE_LABEL}_bundle_${BUNDLE_FILE_NAME}.zip"
 UPDATE_BUNDLE_FILE="${WHITE_LABEL}_bundle_${BUNDLE_FILE_NAME}_autoupdate.zip"

     # sftp does not support -p flag on mkdir :(
     sshpass -p $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
mkdir ${bundle_upload_folder}
SSHCMD

 RET=0
 if [ "$AUTOMATION_LTS" == "true" -a -n "$AUTOMATION_REF" ]; then # lts build
   DESTINATION_FOLDER="${bundle_upload_folder}/lts/${AUTOMATION_REF}"
     # sftp does not support -p flag on mkdir :(
     sshpass -p $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
mkdir ${bundle_upload_folder}/lts
mkdir ${DESTINATION_FOLDER}
SSHCMD
 elif [ -n "${RELEASE_TAG}" ]; then # daily release with tag
    DESTINATION_FOLDER="${bundle_upload_folder}/${SHORT_BOARD_NAME}/${RELEASE_TAG}"
    # sftp does not support -p flag on mkdir :(
    sshpass -p $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
mkdir ${bundle_upload_folder}/${SHORT_BOARD_NAME}
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
