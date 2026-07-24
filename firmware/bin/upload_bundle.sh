#!/usr/bin/env bash

set -e

USER=$1
PASS=$2
HOST=$3
BUNDLE_NAME=$4
SUBFOLDER_TO_UPLOAD=$5
# optional folder override in bundle_upload_folder env variable
# optional history-folder override in firmware_rollback_upload_folder; defaults to build_server/lts or bundle_upload_folder

SCRIPT_NAME=$(basename "$0")
# Set true to validate file names and destination order without bundle files, credentials, or network access.
DRY_RUN=${UPLOAD_BUNDLE_DRY_RUN:-false}

if [[ "$DRY_RUN" == "true" || (-n "$USER" && -n "$PASS" && -n "$HOST") ]]; then
 echo "$SCRIPT_NAME: Uploading both bundle.zip and autoupdate.zip"

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
 if [ -z "${firmware_rollback_upload_folder}" ]; then
     if [ "$bundle_upload_folder" == "build_server" ]; then
         firmware_rollback_upload_folder="${bundle_upload_folder}/lts"
     else
         firmware_rollback_upload_folder="${bundle_upload_folder}"
     fi
 fi
 echo "$SCRIPT_NAME: firmware_rollback_upload_folder is ${firmware_rollback_upload_folder}"

 if [ -n "${SUBFOLDER_TO_UPLOAD}" ]; then
   echo "$SCRIPT_NAME: SUBFOLDER_TO_UPLOAD is ${SUBFOLDER_TO_UPLOAD}"
 fi

 if [[ -z "${WHITE_LABEL}" ]]; then
     echo "WHITE_LABEL environment variable is not specified"
     exit 1
 fi

 FULL_BUNDLE_FILE="${WHITE_LABEL}_bundle_${BUNDLE_NAME}.zip"
 UPDATE_BUNDLE_FILE="${WHITE_LABEL}_bundle_${BUNDLE_NAME}_autoupdate.zip"

 DESTINATION_FOLDERS=()
 if [ -n "${SUBFOLDER_TO_UPLOAD}" ]; then # subfolder to upload bundle is specified explicitly
     DESTINATION_FOLDERS+=("${bundle_upload_folder}/${SHORT_BOARD_NAME}/${SUBFOLDER_TO_UPLOAD}")
 elif [ "$AUTOMATION_LTS" == "true" -a -n "$AUTOMATION_REF" ]; then # lts build
     if [[ -n "$SHORT_BOARD_NAME" && "$GITHUB_SHA" =~ ^[0-9a-fA-F]{40}$ ]]; then
         DESTINATION_FOLDERS+=("${firmware_rollback_upload_folder}/${SHORT_BOARD_NAME}/${AUTOMATION_REF}/${GITHUB_SHA}")
     else
         echo "$SCRIPT_NAME: Cannot publish LTS without SHORT_BOARD_NAME and a full GITHUB_SHA"
         exit 1
     fi
     # Publish immutable history before advancing the mutable LTS alias.
     DESTINATION_FOLDERS+=("${bundle_upload_folder}/lts/${AUTOMATION_REF}")
 else
     DESTINATION_FOLDERS+=("${bundle_upload_folder}")
 fi

 if [[ "$DRY_RUN" == "true" ]]; then
     for DESTINATION_FOLDER in "${DESTINATION_FOLDERS[@]}"; do
         echo "$SCRIPT_NAME: DRY RUN $FULL_BUNDLE_FILE and $UPDATE_BUNDLE_FILE -> ${DESTINATION_FOLDER}"
     done
     exit 0
 fi

 # Sometimes generated bundles have scanty manifests in .jar files.
 # We don't know why it happens, but we definitely do not want to upload bundles with broken manifests. See #7925
 CHECK_MANIFESTS_IN_BUNDLE_SCRIPT=$(realpath "$(dirname "$0")")/check_manifests_in_bundle.sh
 "$CHECK_MANIFESTS_IN_BUNDLE_SCRIPT" "$FULL_BUNDLE_FILE"
 "$CHECK_MANIFESTS_IN_BUNDLE_SCRIPT" "$UPDATE_BUNDLE_FILE"

 # todo: why does upload_ini.sh just invokes a bunch of mkdir from one sshpass invocation, but here we seem to do something else?
 SSHPASS_RETRY=$(realpath "$(dirname "$0")")/sshpass-retry.sh
 for DESTINATION_FOLDER in "${DESTINATION_FOLDERS[@]}"; do
     CURRENT_FOLDER=""
     IFS='/' read -ra SUBFOLDER_ARRAY <<< "${DESTINATION_FOLDER}"
     for SUBFOLDER_COMPONENT in "${SUBFOLDER_ARRAY[@]}"; do
         if [ -n "${SUBFOLDER_COMPONENT}" ]; then
             CURRENT_FOLDER="${CURRENT_FOLDER:+${CURRENT_FOLDER}/}${SUBFOLDER_COMPONENT}"
             "$SSHPASS_RETRY" "$PASS" sftp -o StrictHostKeyChecking=no "${USER}@${HOST}" <<SSHCMD
mkdir ${CURRENT_FOLDER}
SSHCMD
         fi
     done

     "$SSHPASS_RETRY" "$PASS" sftp -o StrictHostKeyChecking=no "${USER}@${HOST}" <<SSHCMD
dir
cd ${CURRENT_FOLDER}
put $FULL_BUNDLE_FILE
mkdir autoupdate
cd autoupdate
put $UPDATE_BUNDLE_FILE
dir
SSHCMD
     echo "$SCRIPT_NAME: DONE $FULL_BUNDLE_FILE -> ${CURRENT_FOLDER}"
 done
else
 echo "$SCRIPT_NAME: Upload not configured"
fi
