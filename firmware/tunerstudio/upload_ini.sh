#!/bin/bash

# user=$1
# pass=$2
# host=$3

if [ ! "$1" ] || [ ! "$2" ] || [ ! "$3" ]; then
 echo "No Secrets"
 exit 0
fi

pwd
echo -e "\nUploading .ini files"
ls -l .

for f in *.ini; do if [[ -f "$f" ]]; then
  echo "Processing file $f:"
  sig=$(grep "^ *signature *=" $f | cut -f2 -d "=")
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
      # unbeliveable, ncftpput does not work with special characters in password?!
      # ncftpput -m -R -v -u "$1" -p "$2" "$3" $path $f
      # we do not have ssh for this user
      # sftp does not support -p flag on mkdir :(
      echo cd rusefi > cmd
      echo mkdir $year >> cmd
      echo mkdir $year/$month >> cmd
      echo mkdir $year/$month/$day >> cmd
      echo mkdir $year/$month/$day/$board >> cmd
      echo put $f $path >> cmd
      cat cmd
      sshpass -p $2 sftp -o StrictHostKeyChecking=no $1@$3 <<< `cat cmd`
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
fi; done
