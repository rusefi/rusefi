# This scripts accepts .properties file name as an only argument and outputs value of `white_label` property.
# It outputs the default white-label `rusefi` when:
# - specified .properties file doesn't exist
# - or `white_label` property is not initialized in specified .properties file
set -u

if [ "$#" -ne 1 ]; then
    echo "$0 expects .properties file as the single argument" 1>&2
    exit 1
fi

properties_file=$1

if [ -f ${properties_file} ]; then
  white_label=$(grep -m1 "^white_label=" "${properties_file}" | cut -d'=' -f2)
fi

if [ -z "${white_label:-}" ]; then
  white_label=rusefi
fi
echo "${white_label}"
