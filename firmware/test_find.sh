#!/bin/bash
shopt -s globstar

# first find all *.activeConfiguration & *.persistentConfiguration

# base file:
cp integration/rusefi_config.txt integration/rusefi_config.generated.txt

for activeConfigurationFile in **/*.activeConfiguration.txt; do
    echo processing $activeConfigurationFile
    printf "%s\n" "/include_file@@BOARD_ENGINE_CONFIGURATION_FROM_FILE@@/r $activeConfigurationFile" w | ed -s integration/rusefi_config.generated.txt
done

for persistenConfigurationFile in **/*.persistentConfiguration.txt; do
    echo processing persistenConfigurationFile
    printf "%s\n" "/include_file@@BOARD_CONFIG_FROM_FILE@@/r $persistenConfigurationFile" w | ed -s integration/rusefi_config.generated.txt
done