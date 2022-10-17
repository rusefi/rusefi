#include "global.h"
#include "live_data_ids.h"
// was generated automatically by rusEFI tool  from live_data_ids.h // by enum2string.jar tool on Fri Oct 07 17:58:29 UTC 2022
// see also gen_config_and_enums.bat



const char *getLive_data_e(live_data_e value){
switch(value) {
case LDS_ac_control:
  return "LDS_ac_control";
case LDS_boost_control:
  return "LDS_boost_control";
case LDS_electronic_throttle:
  return "LDS_electronic_throttle";
case LDS_engine_state:
  return "LDS_engine_state";
case LDS_fan_control:
  return "LDS_fan_control";
case LDS_fuel_computer:
  return "LDS_fuel_computer";
case LDS_fuel_pump_control:
  return "LDS_fuel_pump_control";
case LDS_high_pressure_fuel_pump:
  return "LDS_high_pressure_fuel_pump";
case LDS_idle_state:
  return "LDS_idle_state";
case LDS_ignition_state:
  return "LDS_ignition_state";
case LDS_injector_model:
  return "LDS_injector_model";
case LDS_knock_controller:
  return "LDS_knock_controller";
case LDS_launch_control_state:
  return "LDS_launch_control_state";
case LDS_main_relay:
  return "LDS_main_relay";
case LDS_output_channels:
  return "LDS_output_channels";
case LDS_tps_accel_state:
  return "LDS_tps_accel_state";
case LDS_trigger_central:
  return "LDS_trigger_central";
case LDS_trigger_state:
  return "LDS_trigger_state";
case LDS_trigger_state_primary:
  return "LDS_trigger_state_primary";
case LDS_wall_fuel_state:
  return "LDS_wall_fuel_state";
case LDS_wideband_state:
  return "LDS_wideband_state";
  }
 return NULL;
}
