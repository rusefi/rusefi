#!/bin/bash

rm gen_live_documentation.log

bash gen_line_documentation_one_file.sh pid_state.txt PidState.java

bash gen_line_documentation_one_file.sh engine_state EngineState.java

bash gen_line_documentation_one_file.sh trigger_centralTriggerCentral.java

bash gen_line_documentation_one_file.sh trigger_state.txt TriggerState.java

bash gen_line_documentation_one_file.sh wall_fuel_state.txtWallFuelState.java

java -DSystemOut.name=gen_live_documentation \
 -cp ../java_tools/ConfigDefinition.jar \
 com.rusefi.ldmp.UsagesReader integration/LiveData.yaml
