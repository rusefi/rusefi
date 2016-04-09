rem This batch files reads rusefi_config.txt and produses firmware persistent configuration headers
rem the storage section of rusefi.ini is updated as well

java -jar ../java_tools/ConfigDefinition.jar integration tunerstudio controllers\algo ../java_console
