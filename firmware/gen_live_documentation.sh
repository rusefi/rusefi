#!/bin/bash

rm gen_live_documentation.log

bash gen_live_documentation_one_file.sh ac_control AcControl.java controllers/actuators
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_live_documentation_one_file.sh fuel_pump FuelPump.java controllers/actuators
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_live_documentation_one_file.sh pid_state PidState.java util/math
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_live_documentation_one_file.sh engine_state EngineState.java controllers/algo
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_live_documentation_one_file.sh tps_accel_state TpsAccelState.java controllers/algo
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_live_documentation_one_file.sh trigger_central TriggerCentral.java controllers/trigger
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_live_documentation_one_file.sh trigger_state TriggerState.java controllers/trigger
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_live_documentation_one_file.sh wall_fuel_state WallFuelState.java controllers/algo
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

bash gen_live_documentation_one_file.sh idle_state IdleState.java controllers/actuators
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }

java -DSystemOut.name=logs/gen_live_documentation \
 -cp ../java_tools/ConfigDefinition.jar \
 com.rusefi.ldmp.UsagesReader integration/LiveData.yaml
[ $? -eq 0 ] || { echo "ERROR generating"; exit 1; }
