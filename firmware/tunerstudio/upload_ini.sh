#!/bin/bash

# file=$1
# user=$2
# pass=$3
# host=$4

if [ ! "$1" ]; then
 echo "No file"
 exit 1
fi

if [ ! "$2" ] || [ ! "$3" ] || [ ! "$4" ]; then
 echo "No Secrets"
 exit 0
fi

pwd
echo -e "\nUploading .ini files"
ls -l .

if [ "$1" = "all" ]; then
 echo "Processing default 'all' file"
 sig=$(grep "^ *signature *=" rusefi.ini | cut -f2 -d "=")
else
 echo "Processing file $1:"
 sig=$(grep "^ *signature *=" $1         | cut -f2 -d "=")
fi
if [ ! -z "$sig" -a "$sig" != " " ]; then
  echo "* found signature: $sig"
  if [[ "$sig" =~ rusEFI.*([0-9]{4})\.([0-9]{2})\.([0-9]{2})\.([a-zA-Z0-9_-]+)\.([0-9]+) ]]; then
    year=${BASH_REMATCH[1]}
    month=${BASH_REMATCH[2]}
    day=${BASH_REMATCH[3]}
    board=${BASH_REMATCH[4]}
    hash=${BASH_REMATCH[5]}
    path="$year/$month/$day/$board/$hash.ini"
    echo "* found path: $path"
    # we do not have ssh for this user
    # sftp does not support -p flag on mkdir :(
    sshpass -p $3 sftp -o StrictHostKeyChecking=no $2@$4 <<SSHCMD
cd rusefi
mkdir $year
mkdir $year/$month
mkdir $year/$month/$day
mkdir $year/$month/$day/$board
put $1 $path
SSHCMD
    retVal=$?
    if [ $retVal -ne 0 ]; then
      echo "Upload failed"
      exit 1
    fi
    echo "* upload done!"
  else
    echo "Unexpected $sig"
  fi
fi
