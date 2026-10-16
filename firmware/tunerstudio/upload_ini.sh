#!/usr/bin/env bash

fileName="rusefi_$SHORT_BOARD_NAME.ini"
USER=$1
PASS=$2
HOST=$3

SCRIPT_NAME=$(basename "$0")

if [ ! "$SHORT_BOARD_NAME" ]; then
 echo "No SHORT_BOARD_NAME"
 exit 1
fi

if [ ! "$USER" ] || [ ! "$PASS" ] || [ ! "$HOST" ]; then
 MISSING=""
 [ -z "$USER" ] && MISSING="${MISSING} USER(arg1)"
 [ -z "$PASS" ] && MISSING="${MISSING} PASS(arg2)"
 [ -z "$HOST" ] && MISSING="${MISSING} HOST(arg3)"
 echo "$SCRIPT_NAME: Upload not configured: missing${MISSING} - typically RUSEFI_ONLINE_FTP_USER/RUSEFI_ONLINE_FTP_PASS/RUSEFI_FTP_SERVER secrets are not available on this build"
 exit 0
fi

pwd
echo -e "\nUploading all .ini files into [$INI_DESTINATION_FOLDER] subfolder"
ls -l .

if [ "$fileName" == "no" ]; then
  echo "[upload_ini] signature file not needed"
  exit 0
fi

echo "[upload_ini] Looking for signature in [$fileName]..."
sig=$(grep "^\s*signature\s*=" $fileName         | cut -f2 -d "=")
if [ ! -z "$sig" -a "$sig" != " " ]; then
  echo "* found signature: $sig"
  if [[ "$sig" =~ rusEFI\ ([a-zA-Z0-9_-]+)\.([0-9]{4})\.([0-9]{2})\.([0-9]{2})\.([a-zA-Z0-9_-]+)\.([a-zA-Z0-9_-]+) ]]; then
    branch=${BASH_REMATCH[1]}
    year=${BASH_REMATCH[2]}
    month=${BASH_REMATCH[3]}
    day=${BASH_REMATCH[4]}
    board=${BASH_REMATCH[5]}
    hash=${BASH_REMATCH[6]}
    path="$branch/$year/$month/$day/$board/$hash.ini"
    echo "* found path: $path"
    # [tag:ini_upload_status] SFTP needs batch mode to fail on cd/put errors.
    # A batch file is reopened on every retry; stdin would already be consumed.
    batchFile=$(mktemp) || exit 1
    trap 'rm -f "$batchFile"' EXIT
    # SFTP has no mkdir -p. Allow existing directories, but keep cd/put fatal.
    if ! cat > "$batchFile" <<SSHCMD
-mkdir "${INI_DESTINATION_FOLDER}rusefi"
cd "${INI_DESTINATION_FOLDER}rusefi"
-mkdir "$branch"
-mkdir "$branch/$year"
-mkdir "$branch/$year/$month"
-mkdir "$branch/$year/$month/$day"
-mkdir "$branch/$year/$month/$day/$board"
put "$fileName" "$path"
SSHCMD
    then
      exit 1
    fi

    # Keep password authentication via sshpass available in SFTP batch mode.
    "$(realpath "$(dirname "$0")")/../bin/sshpass-retry.sh" "$PASS" \
      sftp -oBatchMode=no -b "$batchFile" -o StrictHostKeyChecking=no "${USER}@${HOST}"
    retVal=$?

    if [ $retVal -ne 0 ]; then
      exit 1
    fi
    echo "${SCRIPT_NAME} * upload done!"
  else
    echo "${SCRIPT_NAME} Unexpected Signature: [$sig]"
  fi
else
  echo "Signature not found in $fileName"
  exit 1
fi
