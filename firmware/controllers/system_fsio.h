// this https://en.wikipedia.org/wiki/Reverse_Polish_notation is generated automatically
// from controllers/system_fsio.txt
// on 2017-06-12_17_45_41
//
//
// in this file we define system FSIO expressions
//
// see http://rusefi.com/wiki/index.php?title=Manual:Flexible_Logic
//
// Jan 19, 2017
// Andrey Belomutskiy, (c) 2012-2017
//

// Human-readable: (fan and (coolant > fan_off_setting)) | (coolant > fan_on_setting)
#define FAN_CONTROL_LOGIC "fan coolant fan_off_setting > and coolant fan_on_setting > |"
// todo: start-up fuel pump priming time should probably be configurable?

// Human-readable: (time_since_boot < 4) | (rpm > 0)
#define FUEL_PUMP_LOGIC "time_since_boot 4 < rpm 0 > |"

// Human-readable: vbatt < 14.5
#define ALTERNATOR_LOGIC "vbatt 14.5 <"

// Human-readable: ac_on_switch
#define AC_RELAY_LOGIC "ac_on_switch"
// Combined RPM, CLT and VBATT warning light

// Human-readable: (rpm > fsio_setting(2)) | ((coolant > fsio_setting(3)) | (vbatt < fsio_setting(4)))
#define COMBINED_WARNING_LIGHT "rpm 2 fsio_setting > coolant 3 fsio_setting > vbatt 4 fsio_setting < | |"
//needed by EFI_MAIN_RELAY_CONTROL

// Human-readable: (time_since_boot < 2) | (vbatt > 5)
#define MAIN_RELAY_LOGIC "time_since_boot 2 < vbatt 5 > |"
