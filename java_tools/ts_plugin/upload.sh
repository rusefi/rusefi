#!/bin/bash


if [ ! "$1" ] || [ ! "$2" ] || [ ! "$3" ]; then
 echo "No Secrets"
 exit 0
fi

echo -e "\nUploading plugin body"

ncftpput -m -R -v -u "$1" -p "$2" "$3" build/jar/rusefi_plugin_body.jar autoupdate
