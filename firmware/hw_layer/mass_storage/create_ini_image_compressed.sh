#!/bin/bash

# from firmware folder:
# hw_layer/mass_storage/create_ini_image_compressed.sh tunerstudio/generated/rusefi.ini hw_layer/mass_storage/ramdisk_image.h  1024 test https://rusefi.com/s/test

# fail on error
set -e

if [ "$(expr substr $(uname -s) 1 6)" == "CYGWIN" ] || [ "$(expr substr $(uname -s) 1 5)" == "MINGW" ]; then
  echo No image on $(uname -s)
  exit 0
fi

FULL_INI=$1
H_OUTPUT=$2
FS_SIZE=$3
SHORT_BOARD_NAME=$4
BOARD_SPECIFIC_URL=$5

# mkfs.fat and fatlabel are privileged on some systems
PATH="$PATH:/usr/sbin"

echo "create_ini_image_compressed: ini $FULL_INI to $H_OUTPUT size $FS_SIZE for $SHORT_BOARD_NAME [$BOARD_SPECIFIC_URL]"




if [[ $OSTYPE == 'darwin'* ]]; then
  # Mac OS comes with Bash version 3 which is quite limiting and lack key features
  current_date='huh-MAC'
else
  bash --version
  # bash >=4.3 magic
  printf -v current_date '%(%Y-%m-%d)T' -1
  echo "create_ini_image_compressed.sh says [${current_date}]"
fi

README_FILE_PATH=hw_layer/mass_storage/README-${current_date}.txt
WIKI_FILE_PATH=hw_layer/mass_storage/rusEFI\ ${SHORT_BOARD_NAME}\ Wiki.url

cp hw_layer/mass_storage/filesystem_contents/rusEFI_Wiki_template.url "${WIKI_FILE_PATH}"
echo "URL=${BOARD_SPECIFIC_URL}" >> "${WIKI_FILE_PATH}"
cp hw_layer/mass_storage/filesystem_contents/README.nozip.template.txt "${README_FILE_PATH}"
echo ${BOARD_SPECIFIC_URL}       >> "${README_FILE_PATH}"

if [[ -z "${EXTRA_FILES_TO_COPY_ON_IMAGE_FOLDER}" ]]; then
  EXTRA_FILES_TO_COPY_ON_IMAGE=("hw_layer/mass_storage/filesystem_contents/rusEFI Forum.url" "hw_layer/mass_storage/filesystem_contents/rusEFI Quick Start.url" "${WIKI_FILE_PATH}")
  echo "We are using default files to copy on image: ${EXTRA_FILES_TO_COPY_ON_IMAGE[@]}"
elif [[ -d "${EXTRA_FILES_TO_COPY_ON_IMAGE_FOLDER}" ]]; then
  EXTRA_FILES_TO_COPY_ON_IMAGE=(${EXTRA_FILES_TO_COPY_ON_IMAGE_FOLDER}/*)
  echo "We are using files from ${EXTRA_FILES_TO_COPY_ON_IMAGE_FOLDER} folder to copy on image: ${EXTRA_FILES_TO_COPY_ON_IMAGE[@]}"
else
  echo "Error: ${EXTRA_FILES_TO_COPY_ON_IMAGE_FOLDER} is not a folder"
  exit 1
fi

hw_layer/mass_storage/create_image.sh $H_OUTPUT $FS_SIZE true $FULL_INI "${README_FILE_PATH}" "${EXTRA_FILES_TO_COPY_ON_IMAGE[@]}"

rm "${README_FILE_PATH}" "${WIKI_FILE_PATH}"
exit 0
