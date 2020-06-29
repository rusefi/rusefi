#!/bin/bash


if [ ! "$1" ] || [ ! "$2" ] || [ ! "$3" ]; then
 echo "No Secrets"
 exit 0
fi

echo -e "\nUploading .ini files"

for f in *.ini; do if [[ -f "$f" ]]; then
  echo "Processing file $f:"
  sig=$(grep "^ *signature *=" $f | cut -f2 -d "=")
  if [ ! -z "$sig" -a "$sig" != " " ]; then
    echo "* found signature: $sig"
    if [[ "$sig" =~ rusEFI.*([0-9]{4}\.[^\"]+) ]]; then
      sig=${BASH_REMATCH[1]}
      sig="${sig//\./\/}.ini"
      path="/rusefi/$sig"
      echo "* found path: $path"
      # unbeliveable, ncftpput does not work with special characters in password?!
      # ncftpput -m -R -v -u "$1" -p "$2" "$3" $path $f
      # we do not have ssh for this user
      # sftp does not support -p flag on mkdir :(
      echo put $f > cmd
      sshpass -p $2 sftp -o StrictHostKeyChecking=no $1@$3 <<< `cat cmd`
      retVal=$?
      if [ $retVal -ne 0 ]; then
        echo "Upload failed"
        exit -1
      fi
      echo "* upload done!"
    fi
  fi
fi; done
