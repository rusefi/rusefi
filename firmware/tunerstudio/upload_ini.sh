#!/bin/bash

fileName=$1
# user=$2
# pass=$3
# host=$4

if [ ! "$fileName" ]; then
 echo "No $fileName"
 exit 1
fi

if [ ! "$2" ] || [ ! "$3" ] || [ ! "$4" ]; then
 echo "upload_ini.sh says No Secrets, exiting"
 exit 0
fi

pwd
echo -e "\nUploading .ini files"
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
    # we do not have ssh for this user
    # sftp does not support -p flag on mkdir :(
    sshpass -p $3 sftp -o StrictHostKeyChecking=no $2@$4 <<SSHCMD
cd rusefi
mkdir $branch
mkdir $branch/$year
mkdir $branch/$year/$month
mkdir $branch/$year/$month/$day
mkdir $branch/$year/$month/$day/$board
put $fileName $path
SSHCMD
    retVal=$?
    if [ $retVal -ne 0 ]; then
      echo "Upload failed"
      exit 1
    fi
    echo "* upload done!"
  else
    echo "[upload_ini] Unexpected Signature: [$sig]"
  fi
else
  echo "Signature not found in $fileName"
  exit 1
fi
