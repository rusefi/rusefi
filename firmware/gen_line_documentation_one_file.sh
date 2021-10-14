#!/bin/bash

if [ ! "$1" ] || [ ! "$2" ]; then
    printf "Two parameters expected"
    exit 1
fi

meta_name=$1
# TODO: programmatically produce java output name based on meta file name
java_output=$2

meta_file_name=${meta_name}.txt
header_file="${meta_name}_generated.h"


java -DSystemOut.name=gen_live_documentation \
 -jar ../java_tools/ConfigDefinition.jar \
 -definition integration/$meta_file_name \
 -cache_zip_file tunerstudio/generated/cache.zip \
 -cache . \
 -java_destination ../java_console/models/src/main/java/com/rusefi/config/generated/$java_output \
 -c_destination controllers/generated/${header_file}