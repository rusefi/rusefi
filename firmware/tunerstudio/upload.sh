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
      ncftpput -m -R -v -u "$1" -p "$2" "$3" . rusefi.ini
      ncftpput -m -R -v -u "$1" -p "$2" "$3" autoupdate $path
      echo "* upload done!"
    fi
  fi
fi; done
