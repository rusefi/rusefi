#!/bin/bash

rm gen_live_documentation.log

bash gen_live_documentation_one_file.sh ac_control AcControl.java

bash gen_live_documentation_one_file.sh pid_state PidState.java

bash gen_live_documentation_one_file.sh engine_state EngineState.java

bash gen_live_documentation_one_file.sh trigger_central TriggerCentral.java

bash gen_live_documentation_one_file.sh trigger_state TriggerState.java

bash gen_live_documentation_one_file.sh wall_fuel_state WallFuelState.java

java -DSystemOut.name=gen_live_documentation \
 -cp ../java_tools/ConfigDefinition.jar \
 com.rusefi.ldmp.UsagesReader integration/LiveData.yaml
