rem TODO better place for this stuff, more automation so that each file does not need to be handled separately. maybe regexp?

rm gen_config2.log

set LIVE_DOCS_COMMAND=java -DSystemOut.name=gen_config2 ^
 -cp ../java_tools/ConfigDefinition.jar ^
 com.rusefi.ldmp.LiveDocsMetaParser ^
 "../"

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

%LIVE_DOCS_COMMAND% controllers/actuators/idle_thread.cpp
%LIVE_DOCS_COMMAND% controllers/trigger/trigger_decoder.cpp
%LIVE_DOCS_COMMAND% controllers/sensors/thermistors.cpp
%LIVE_DOCS_COMMAND% controllers/sensors/tps.cpp
%LIVE_DOCS_COMMAND% controllers/math/speed_density.cpp
%LIVE_DOCS_COMMAND% controllers/actuators/electronic_throttle.cpp

java -DSystemOut.name=gen_config2 ^
 -jar ../java_tools/ConfigDefinition.jar ^
 -definition integration/wall_fuel.txt ^
 -initialize_to_zero yes ^
 -java_destination ../java_console/models/src/com/rusefi/config/generated/WallFuelState.java ^
 -c_destination controllers/generated/wall_fuel.h

%LIVE_DOCS_COMMAND% controllers/algo/accel_enrichment.cpp
