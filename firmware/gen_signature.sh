#!/bin/bash

# file gen_signature.sh

SHORT_BOARDNAME=$1


SIGNATURE_FILE_NAME=tunerstudio/generated/signature_${SHORT_BOARDNAME}.txt
echo "Generating signature for ${SHORT_BOARDNAME}"

# generate a unique signature
date=`TZ=Europe/London date +"%Y.%m.%d"`
echo "#define TS_SIGNATURE \"rusEFI $date.${SHORT_BOARDNAME}.@@SIGNATURE_HASH@@\"" >> ${SIGNATURE_FILE_NAME}

exit 0
