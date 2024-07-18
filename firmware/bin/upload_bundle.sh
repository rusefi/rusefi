#!/usr/bin/env bash

USER=$1
PASS=$2
HOST=$3
BUNDLE_NAME=$4
SUBFOLDER_TO_UPLOAD=$5

SCRIPT_NAME=$(basename "$0")

if [ -n "${USER}" -a -n "$PASS" -a -n "${HOST}" ]; then
 echo "$SCRIPT_NAME: Uploading both bundles"

 if [ -n "${BUNDLE_NAME}" ]; then
   echo "$SCRIPT_NAME: BUNDLE_NAME is ${BUNDLE_NAME}"
 else
   echo "$SCRIPT_NAME: BUNDLE_NAME argument not specified"
   exit 1
 fi
 if [ -n "${bundle_upload_folder}" ]; then
   echo "$SCRIPT_NAME: bundle_upload_folder is ${bundle_upload_folder}"
 else
   bundle_upload_folder="build_server"
   echo "$SCRIPT_NAME: bundle_upload_folder env variable was not specified using default ${bundle_upload_folder}"
 fi

 if [ -n "${SUBFOLDER_TO_UPLOAD}" ]; then
   echo "$SCRIPT_NAME: SUBFOLDER_TO_UPLOAD is ${SUBFOLDER_TO_UPLOAD}"
 fi

 if [[ -z "${WHITE_LABEL}" ]]; then
     echo "WHITE_LABEL environment variable is not specified"
     exit 1
 fi

 FULL_BUNDLE_FILE="${WHITE_LABEL}_bundle_${BUNDLE_NAME}.zip"
 UPDATE_BUNDLE_FILE="${WHITE_LABEL}_bundle_${BUNDLE_NAME}_autoupdate.zip"

 RET=0
 if [ -n "${SUBFOLDER_TO_UPLOAD}" ]; then # subfolder to upload bundle is specified explicitly
     DESTINATION_SUBFOLDER="${SHORT_BOARD_NAME}/${SUBFOLDER_TO_UPLOAD}"
 elif [ "$AUTOMATION_LTS" == "true" -a -n "$AUTOMATION_REF" ]; then # lts build
    DESTINATION_SUBFOLDER="lts/${AUTOMATION_REF}"
 else
    DESTINATION_SUBFOLDER=""
 fi

 DESTINATION_FOLDER="${bundle_upload_folder}"
 # sftp does not support -p flag on mkdir :(
 sshpass -p $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
mkdir ${DESTINATION_FOLDER}
SSHCMD

 readarray -d "/" -t SUBFOLDER_ARRAY <<< "${DESTINATION_SUBFOLDER}"
 for ((n=0; n < ${#SUBFOLDER_ARRAY[*]}; n++))
 do
    SUBFOLDER_COMPONENT="${SUBFOLDER_ARRAY[n]}"
    if [ -n "${SUBFOLDER_COMPONENT}" ]; then
      DESTINATION_FOLDER="${DESTINATION_FOLDER}/${SUBFOLDER_COMPONENT}"
      sshpass -p $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
mkdir ${DESTINATION_FOLDER}
SSHCMD
    fi
 done

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
