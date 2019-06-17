
java -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/engine_state.txt ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/EngineState.java ^
 -initialize_to_zero yes ^
 -c_destination controllers/generated/engine_state_generated.h

java -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/thermistor.txt ^
 -initialize_to_zero yes ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/ThermistorState.java ^
 -c_destination controllers/generated/thermistor.h

java -cp ../java_tools/ConfigDefinition.jar ^
 com.rusefi.ldmp.LiveDocsMetaParser ^
 controllers/sensors/thermistors.cpp ^
 "../"
