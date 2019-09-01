rem TODO better place for this stuff, more automation so that each file does not need to be handled separately. maybe regexp?

rm gen_config2.log

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/engine_state.txt ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/EngineState.java ^
 -initialize_to_zero yes ^
 -c_destination controllers/generated/engine_state_generated.h

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/thermistor.txt ^
 -initialize_to_zero yes ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/ThermistorState.java ^
 -c_destination controllers/generated/thermistor.h

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/trigger.txt ^
 -initialize_to_zero yes ^
 -with_c_defines true ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/TriggerState.java ^
 -c_destination controllers/generated/trigger_structs.h

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/wall_fuel.txt ^
 -initialize_to_zero yes ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/WallFuelState.java ^
 -c_destination controllers/generated/wall_fuel.h

java -DSystemOut.name=gen_config2 ^
 -cp ../java_tools/ConfigDefinition.jar;../java_tools/configuration_definition/lib/snakeyaml.jar ^
 com.rusefi.ldmp.UsagesReader integration/LiveData.yaml
