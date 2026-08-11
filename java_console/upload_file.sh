#!/bin/bash

# user=$1
# pass=$2
# host=$3
# destination_folder=$4
# file_to_upload=$5

SCRIPT_NAME=$(basename "$0")

if [ ! "$1" ] || [ ! "$2" ] || [ ! "$3" ]; then
 MISSING=""
 [ -z "$1" ] && MISSING="${MISSING} USER(arg1)"
 [ -z "$2" ] && MISSING="${MISSING} PASS(arg2)"
 [ -z "$3" ] && MISSING="${MISSING} HOST(arg3)"
 echo "$SCRIPT_NAME: Upload not configured: missing${MISSING} - typically RUSEFI_SSH_USER/RUSEFI_SSH_PASS/RUSEFI_SSH_SERVER secrets are not available on this build"
 exit 0
fi

echo -e "\nUploading file: $5"

# ncftpput -r 999 -m -R -v -u "$1" -p "$2" "$3" $4 $5

pwd
ls -l $5
echo "put $5 $4" | sshpass -p $2 sftp -o StrictHostKeyChecking=no $1@$3
