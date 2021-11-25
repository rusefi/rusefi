#!/bin/bash

#
# we have code duplication with three "upload_xxx*" files
# todo: clean it up? use this version with five parameters for all use-cases?
# todo: rename this script since it's a universal one?
#

# user=$1
# pass=$2
# host=$3
# destination_folder=$4
# file_to_upload=$5

if [ ! "$1" ] || [ ! "$2" ] || [ ! "$3" ]; then
 echo "No Secrets"
 exit 0
fi

echo -e "\nUploading file: $5"

# ncftpput -r 999 -m -R -v -u "$1" -p "$2" "$3" $4 $5

sshpass -p $2 sftp -o StrictHostKeyChecking=no $1@$3 "put $5 $4"
