#!/bin/bash

# user=$1
# pass=$2
# host=$3
# destination_folder=$4
# file_to_upload=$5

if [ ! "$1" ] || [ ! "$2" ] || [ ! "$3" ]; then
 echo "upload_file.sh says No Secrets, exiting"
 exit 0
fi

echo -e "\nUploading file: $5"

# ncftpput -r 999 -m -R -v -u "$1" -p "$2" "$3" $4 $5

pwd
ls -l $5
echo "put $5 $4" | sshpass -p $2 sftp -o StrictHostKeyChecking=no $1@$3
