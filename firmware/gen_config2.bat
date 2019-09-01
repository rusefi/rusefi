rem TODO better place for this stuff, more automation so that each file does not need to be handled separately. maybe regexp?

rm gen_config2.log

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/engine_state.txt ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/EngineState.java ^
 -c_destination controllers/generated/engine_state_generated.h

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/thermistor.txt ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/ThermistorState.java ^
 -c_destination controllers/generated/thermistor_generated.h

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/trigger.txt ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/TriggerState.java ^
 -c_destination controllers/generated/trigger_generated.h

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/wall_fuel.txt ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/WallFuelState.java ^
 -c_destination controllers/generated/wall_fuel_generated.h

java -DSystemOut.name=gen_config2 ^
 -cp ../java_tools/ConfigDefinition.jar;../java_tools/configuration_definition/lib/snakeyaml.jar ^
 com.rusefi.ldmp.UsagesReader integration/LiveData.yaml
