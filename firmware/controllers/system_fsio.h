// this https://en.wikipedia.org/wiki/Reverse_Polish_notation is generated automatically
// from controllers/system_fsio.txt
// on 2019-09-08_16_41_20_788
//
//
// in this file we define system FSIO expressions
//
// system_fsio.txt is input for compile_fsio_file tool, see gen_system_fsio.bat
//
// see http://rusefi.com/wiki/index.php?title=Manual:Flexible_Logic
//
// Jan 19, 2017
// Andrey Belomutskiy, (c) 2012-2017
//

// Human-readable: (fan and (coolant > cfg_fanOffTemperature)) | (coolant > cfg_fanOnTemperature) | is_clt_broken
#define FAN_CONTROL_LOGIC "fan coolant cfg_fanOffTemperature > and coolant cfg_fanOnTemperature > | is_clt_broken |"

// Human-readable: (time_since_boot < startup_fuel_pump_duration) | (rpm > 0)
#define FUEL_PUMP_LOGIC "time_since_boot startup_fuel_pump_duration < rpm 0 > |"

// Human-readable: vbatt < 14.5
#define ALTERNATOR_LOGIC "vbatt 14.5 <"

// Human-readable: coolant > 120
#define TOO_HOT_LOGIC "coolant 120 >"

// Human-readable: ac_on_switch & rpm > 850 & time_since_ac_on_switch > 0.3
// ac_on_switch rpm & 850 time_since_ac_on_switch & > 0.3 >
#define AC_RELAY_LOGIC "ac_on_switch rpm & 850 >"
// Combined RPM, CLT and VBATT warning light

// Human-readable: (rpm > fsio_setting(2)) | ((coolant > fsio_setting(3)) | (vbatt < fsio_setting(4)))
#define COMBINED_WARNING_LIGHT "rpm 2 fsio_setting > coolant 3 fsio_setting > vbatt 4 fsio_setting < | |"
//needed by EFI_MAIN_RELAY_CONTROL

// Human-readable: (time_since_boot < 2) | (vbatt > 5) | in_shutdown
#define MAIN_RELAY_LOGIC "time_since_boot 2 < vbatt 5 > | in_shutdown |"
// could be used for simple variable intake geometry setups or warning light or starter block

// Human-readable: rpm > fsio_setting(1)
#define RPM_ABOVE_USER_SETTING_1 "rpm 1 fsio_setting >"
// could be used for simple variable intake geometry setups or warning light or starter block

// Human-readable: rpm > fsio_setting(1)
#define RPM_ABOVE_USER_SETTING_1 "rpm 1 fsio_setting >"

// Human-readable: rpm > fsio_setting(2)
#define RPM_ABOVE_USER_SETTING_2 "rpm 2 fsio_setting >"

// Human-readable: rpm > 5500
#define RPM_ABOVE_5500_ON_OFF "rpm 5500 >"

// Human-readable: rpm > 6000
#define RPM_ABOVE_6000_ON_OFF "rpm 6000 >"

// Human-readable: (rpm > 1000) * 0.7
#define RPM_ABOVE_1000_SOLENOID_70_DUTY "rpm 1000 > 0.7 *"

// Human-readable: (rpm > 2000) * 0.5
#define RPM_ABOVE_2000_SOLENOID_50_DUTY "rpm 2000 > 0.5 *"

// Human-readable: (rpm > 6000) * 0.8
#define RPM_ABOVE_6000_SOLENOID_80_DUTY "rpm 6000 > 0.8 *"

// Human-readable: rpm < fsio_setting(1)
#define RPM_BELOW_USER_SETTING_1 "rpm 1 fsio_setting <"
// starter block using configurable parameter

// Human-readable: rpm < cranking_rpm
#define STARTER_BLOCK "rpm cranking_rpm <"

// Human-readable: fsio_table (3, rpm, map) / 100
#define BOOST_CONTROLLER "3 rpm map fsio_table 100 /"

// Human-readable: if(fsio_input (0) > 20, 0, 10)
#define ANALOG_CONDITION "0 fsio_input 20 > 0 10 if"
