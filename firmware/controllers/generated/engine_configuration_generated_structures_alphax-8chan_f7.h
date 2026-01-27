// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of stft_cell_cfg_s
struct stft_cell_cfg_s {
	/**
	 * Maximum % that the short term fuel trim can add
	 * units: %
	 * offset 0
	 */
	scaled_channel<uint8_t, 10, 1> maxAdd;
	/**
	 * Maximum % that the short term fuel trim can remove
	 * units: %
	 * offset 1
	 */
	scaled_channel<uint8_t, 10, 1> maxRemove;
	/**
	 * Commonly referred as Integral gain.
	 * Time constant for correction while in this cell: this sets responsiveness of the closed loop correction. A value of 5.0 means it will try to make most of the correction within 5 seconds, and a value of 1.0 will try to correct within 1 second.
	 * Lower values makes the correction more sensitive, higher values slow the correction down.
	 * units: sec
	 * offset 2
	 */
	scaled_channel<uint16_t, 10, 1> timeConstant;
};
static_assert(sizeof(stft_cell_cfg_s) == 4);

// start of stft_s
struct stft_s {
	/**
	 * Below this RPM, the idle region is active, idle+300 would be a good value
	 * units: RPM
	 * offset 0
	 */
	scaled_channel<uint8_t, 1, 50> maxIdleRegionRpm;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[1] = {};
	/**
	 * Below this engine load, the overrun region is active
	 * When tuning by MAP the units are kPa/psi, e.g. 30 would mean 30kPa. When tuning TPS, 30 would be 30%
	 * units: load
	 * offset 2
	 */
	uint16_t maxOverrunLoad;
	/**
	 * Above this engine load, the power region is active
	 * When tuning by MAP the units are kPa/psi
	 * units: load
	 * offset 4
	 */
	uint16_t minPowerLoad;
	/**
	 * When close to correct AFR, pause correction. This can improve stability by not changing the adjustment if the error is extremely small, but is not required.
	 * units: %
	 * offset 6
	 */
	scaled_channel<uint8_t, 10, 1> deadband;
	/**
	 * Minimum coolant temperature before closed loop operation is allowed.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 7
	 */
	int8_t minClt;
	/**
	 * Below this AFR, correction is paused
	 * units: afr
	 * offset 8
	 */
	scaled_channel<uint8_t, 10, 1> minAfr;
	/**
	 * Above this AFR, correction is paused
	 * units: afr
	 * offset 9
	 */
	scaled_channel<uint8_t, 10, 1> maxAfr;
	/**
	 * Time after startup before closed loop operation is allowed.
	 * units: seconds
	 * offset 10
	 */
	uint8_t startupDelay;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 11
	 */
	uint8_t alignmentFill_at_11[1] = {};
	/**
	 * offset 12
	 */
	stft_cell_cfg_s cellCfgs[STFT_CELL_COUNT] = {};
};
static_assert(sizeof(stft_s) == 28);

// start of ltft_s
struct ltft_s {
	/**
	 * Enables lambda sensor long term fuel corrections data gathering into LTFT trim tables
	offset 0 bit 0 */
	bool enabled : 1 {};
	/**
	 * Apply LTFT trims into fuel calculation on top of VE table.
	 * We do not adjust VE table automatically, please click 'Apply to VE' if you want to adjust your VE tables and reset trims.
	offset 0 bit 1 */
	bool correctionEnabled : 1 {};
	/**
	offset 0 bit 2 */
	bool unusedBit_2_2 : 1 {};
	/**
	offset 0 bit 3 */
	bool unusedBit_2_3 : 1 {};
	/**
	offset 0 bit 4 */
	bool unusedBit_2_4 : 1 {};
	/**
	offset 0 bit 5 */
	bool unusedBit_2_5 : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_2_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_2_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_2_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_2_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_2_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_2_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_2_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_2_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_2_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_2_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_2_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_2_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_2_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_2_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_2_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_2_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_2_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_2_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_2_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_2_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_2_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_2_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_2_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_2_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_2_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_2_31 : 1 {};
	/**
	 * When close to correct AFR, pause correction. This can improve stability by not changing the adjustment if the error is extremely small, but is not required.
	 * units: %
	 * offset 4
	 */
	scaled_channel<uint8_t, 10, 1> deadband;
	/**
	 * Maximum % that the long term fuel trim can add
	 * units: %
	 * offset 5
	 */
	scaled_channel<uint8_t, 10, 1> maxAdd;
	/**
	 * Maximum % that the long term fuel trim can remove
	 * units: %
	 * offset 6
	 */
	scaled_channel<uint8_t, 10, 1> maxRemove;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 7
	 */
	uint8_t alignmentFill_at_7[1] = {};
	/**
	 * Commonly referred as Integral gain.
	 * Time constant for correction while in this cell: this sets responsiveness of the closed loop correction. A value of 30.0 means it will try to make most of the correction within 30 seconds, and a value of 300.0 will try to correct within 5 minutes.
	 * Lower values makes the correction more sensitive, higher values slow the correction down.
	 * units: sec
	 * offset 8
	 */
	scaled_channel<uint16_t, 1, 1> timeConstant[STFT_CELL_COUNT] = {};
};
static_assert(sizeof(ltft_s) == 16);

// start of pid_s
struct pid_s {
	/**
	 * offset 0
	 */
	float pFactor;
	/**
	 * offset 4
	 */
	float iFactor;
	/**
	 * offset 8
	 */
	float dFactor;
	/**
	 * Linear addition to PID logic
	 * Also known as feedforward.
	 * offset 12
	 */
	int16_t offset;
	/**
	 * PID dTime
	 * units: ms
	 * offset 14
	 */
	int16_t periodMs;
	/**
	 * Output Min Duty Cycle
	 * offset 16
	 */
	int16_t minValue;
	/**
	 * Output Max Duty Cycle
	 * offset 18
	 */
	int16_t maxValue;
};
static_assert(sizeof(pid_s) == 20);

// start of MsIoBox_config_s
struct MsIoBox_config_s {
	/**
	 * offset 0
	 */
	MsIoBoxId id;
	/**
	 * offset 1
	 */
	MsIoBoxVss vss;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2] = {};
};
static_assert(sizeof(MsIoBox_config_s) == 4);

// start of cranking_parameters_s
struct cranking_parameters_s {
	/**
	 * This sets the RPM limit below which the ECU will use cranking fuel and ignition logic, typically this is around 350-450rpm. 
	 * set cranking_rpm X
	 * units: RPM
	 * offset 0
	 */
	int16_t rpm;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2] = {};
};
static_assert(sizeof(cranking_parameters_s) == 4);

// start of gppwm_channel
struct gppwm_channel {
	/**
	 * Select a pin to use for PWM or on-off output.
	 * offset 0
	 */
	output_pin_e pin;
	/**
	 * If an error (with a sensor, etc) is detected, this value is used instead of reading from the table.
	 * This should be a safe value for whatever hardware is connected to prevent damage.
	 * units: %
	 * offset 2
	 */
	uint8_t dutyIfError;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3
	 */
	uint8_t alignmentFill_at_3[1] = {};
	/**
	 * Select a frequency to run PWM at.
	 * Set this to 0hz to enable on-off mode.
	 * units: hz
	 * offset 4
	 */
	uint16_t pwmFrequency;
	/**
	 * Hysteresis: in on-off mode, turn the output on when the table value is above this duty.
	 * units: %
	 * offset 6
	 */
	uint8_t onAboveDuty;
	/**
	 * Hysteresis: in on-off mode, turn the output off when the table value is below this duty.
	 * units: %
	 * offset 7
	 */
	uint8_t offBelowDuty;
	/**
	 * Selects the Y axis to use for the table.
	 * offset 8
	 */
	gppwm_channel_e loadAxis;
	/**
	 * Selects the X axis to use for the table.
	 * offset 9
	 */
	gppwm_channel_e rpmAxis;
	/**
	 * offset 10
	 */
	scaled_channel<int16_t, 2, 1> loadBins[GPPWM_LOAD_COUNT] = {};
	/**
	 * offset 26
	 */
	int16_t rpmBins[GPPWM_RPM_COUNT] = {};
	/**
	 * units: duty
	 * offset 42
	 */
	scaled_channel<uint8_t, 2, 1> table[GPPWM_LOAD_COUNT][GPPWM_RPM_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 106
	 */
	uint8_t alignmentFill_at_106[2] = {};
};
static_assert(sizeof(gppwm_channel) == 108);

// start of air_pressure_sensor_config_s
struct air_pressure_sensor_config_s {
	/**
	 * kPa/psi value at low volts
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 0
	 */
	float lowValue;
	/**
	 * kPa/psi value at high volts
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 4
	 */
	float highValue;
	/**
	 * offset 8
	 */
	air_pressure_sensor_type_e type;
	/**
	 * offset 9
	 */
	adc_channel_e hwChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2] = {};
};
static_assert(sizeof(air_pressure_sensor_config_s) == 12);

// start of MAP_sensor_config_s
struct MAP_sensor_config_s {
	/**
	 * offset 0
	 */
	float samplingAngleBins[MAP_ANGLE_SIZE] = {};
	/**
	 * MAP averaging sampling start crank degree angle
	 * units: deg
	 * offset 32
	 */
	float samplingAngle[MAP_ANGLE_SIZE] = {};
	/**
	 * offset 64
	 */
	float samplingWindowBins[MAP_WINDOW_SIZE] = {};
	/**
	 * MAP averaging angle crank degree duration
	 * units: deg
	 * offset 96
	 */
	float samplingWindow[MAP_WINDOW_SIZE] = {};
	/**
	 * offset 128
	 */
	air_pressure_sensor_config_s sensor;
};
static_assert(sizeof(MAP_sensor_config_s) == 140);

/**
 * @brief Thermistor known values

*/
// start of thermistor_conf_s
struct thermistor_conf_s {
	/**
	 * these values are in Celcius
	 * units: *C
	 * offset 0
	 */
	float tempC_1;
	/**
	 * units: *C
	 * offset 4
	 */
	float tempC_2;
	/**
	 * units: *C
	 * offset 8
	 */
	float tempC_3;
	/**
	 * units: Ohm
	 * offset 12
	 */
	float resistance_1;
	/**
	 * units: Ohm
	 * offset 16
	 */
	float resistance_2;
	/**
	 * units: Ohm
	 * offset 20
	 */
	float resistance_3;
	/**
	 * Pull-up resistor value on your board
	 * units: Ohm
	 * offset 24
	 */
	float bias_resistor;
};
static_assert(sizeof(thermistor_conf_s) == 28);

// start of linear_sensor_s
struct linear_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3] = {};
	/**
	 * units: volts
	 * offset 4
	 */
	float v1;
	/**
	 * offset 8
	 */
	float value1;
	/**
	 * units: volts
	 * offset 12
	 */
	float v2;
	/**
	 * offset 16
	 */
	float value2;
};
static_assert(sizeof(linear_sensor_s) == 20);

// start of ThermistorConf
struct ThermistorConf {
	/**
	 * offset 0
	 */
	thermistor_conf_s config;
	/**
	 * offset 28
	 */
	adc_channel_e adcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 29
	 */
	uint8_t alignmentFill_at_29[3] = {};
};
static_assert(sizeof(ThermistorConf) == 32);

// start of injector_s
struct injector_s {
	/**
	 * This is your injector flow at the fuel pressure used in the vehicle
	 * See units setting below
	 * offset 0
	 */
	float flow;
	/**
	 * units: volts
	 * offset 4
	 */
	scaled_channel<int16_t, 100, 1> battLagCorrBattBins[VBAT_INJECTOR_CURVE_SIZE] = {};
	/**
	 * Injector correction pressure
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 20
	 */
	scaled_channel<uint32_t, 10, 1> battLagCorrPressBins[VBAT_INJECTOR_CURVE_PRESSURE_SIZE] = {};
	/**
	 * ms delay between injector open and close dead times
	 * units: ms
	 * offset 36
	 */
	scaled_channel<int16_t, 100, 1> battLagCorrTable[VBAT_INJECTOR_CURVE_PRESSURE_SIZE][VBAT_INJECTOR_CURVE_SIZE] = {};
};
static_assert(sizeof(injector_s) == 100);

// start of trigger_config_s
struct trigger_config_s {
	/**
	 * https://wiki.rusefi.com/All-Supported-Triggers
	 * offset 0
	 */
	trigger_type_e type;
	/**
	 * units: number
	 * offset 4
	 */
	int customTotalToothCount;
	/**
	 * units: number
	 * offset 8
	 */
	int customSkippedToothCount;
};
static_assert(sizeof(trigger_config_s) == 12);

// start of afr_sensor_s
struct afr_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	 * offset 1
	 */
	adc_channel_e hwChannel2;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2] = {};
	/**
	 * units: volts
	 * offset 4
	 */
	float v1;
	/**
	 * units: AFR
	 * offset 8
	 */
	float value1;
	/**
	 * units: volts
	 * offset 12
	 */
	float v2;
	/**
	 * units: AFR
	 * offset 16
	 */
	float value2;
};
static_assert(sizeof(afr_sensor_s) == 20);

// start of idle_hardware_s
struct idle_hardware_s {
	/**
	 * units: Hz
	 * offset 0
	 */
	int solenoidFrequency;
	/**
	 * offset 4
	 */
	output_pin_e solenoidPin;
	/**
	 * offset 6
	 */
	Gpio stepperDirectionPin;
	/**
	 * offset 8
	 */
	Gpio stepperStepPin;
	/**
	 * offset 10
	 */
	pin_output_mode_e solenoidPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 11
	 */
	uint8_t alignmentFill_at_11[1] = {};
};
static_assert(sizeof(idle_hardware_s) == 12);

// start of dc_io
struct dc_io {
	/**
	 * offset 0
	 */
	Gpio directionPin1;
	/**
	 * offset 2
	 */
	Gpio directionPin2;
	/**
	 * Acts as EN pin in two-wire mode
	 * offset 4
	 */
	Gpio controlPin;
	/**
	 * offset 6
	 */
	Gpio disablePin;
};
static_assert(sizeof(dc_io) == 8);

// start of vr_threshold_s
struct vr_threshold_s {
	/**
	 * units: rpm
	 * offset 0
	 */
	scaled_channel<uint8_t, 1, 50> rpmBins[6] = {};
	/**
	 * units: volts
	 * offset 6
	 */
	scaled_channel<uint8_t, 100, 1> values[6] = {};
	/**
	 * offset 12
	 */
	Gpio pin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 14
	 */
	uint8_t alignmentFill_at_14[2] = {};
};
static_assert(sizeof(vr_threshold_s) == 16);

// start of wbo_s
struct wbo_s {
	/**
	 * offset 0
	 */
	can_wbo_type_e type;
	/**
	 * offset 1
	 */
	can_wbo_re_id_e reId;
	/**
	 * offset 2
	 */
	can_wbo_aem_id_e aemId;
	/**
	 * offset 3
	 */
	can_wbo_re_hwidx_e reHwidx;
	/**
	offset 4 bit 0 */
	bool enableRemap : 1 {};
	/**
	offset 4 bit 1 */
	bool unusedBit_5_1 : 1 {};
	/**
	offset 4 bit 2 */
	bool unusedBit_5_2 : 1 {};
	/**
	offset 4 bit 3 */
	bool unusedBit_5_3 : 1 {};
	/**
	offset 4 bit 4 */
	bool unusedBit_5_4 : 1 {};
	/**
	offset 4 bit 5 */
	bool unusedBit_5_5 : 1 {};
	/**
	offset 4 bit 6 */
	bool unusedBit_5_6 : 1 {};
	/**
	offset 4 bit 7 */
	bool unusedBit_5_7 : 1 {};
	/**
	offset 4 bit 8 */
	bool unusedBit_5_8 : 1 {};
	/**
	offset 4 bit 9 */
	bool unusedBit_5_9 : 1 {};
	/**
	offset 4 bit 10 */
	bool unusedBit_5_10 : 1 {};
	/**
	offset 4 bit 11 */
	bool unusedBit_5_11 : 1 {};
	/**
	offset 4 bit 12 */
	bool unusedBit_5_12 : 1 {};
	/**
	offset 4 bit 13 */
	bool unusedBit_5_13 : 1 {};
	/**
	offset 4 bit 14 */
	bool unusedBit_5_14 : 1 {};
	/**
	offset 4 bit 15 */
	bool unusedBit_5_15 : 1 {};
	/**
	offset 4 bit 16 */
	bool unusedBit_5_16 : 1 {};
	/**
	offset 4 bit 17 */
	bool unusedBit_5_17 : 1 {};
	/**
	offset 4 bit 18 */
	bool unusedBit_5_18 : 1 {};
	/**
	offset 4 bit 19 */
	bool unusedBit_5_19 : 1 {};
	/**
	offset 4 bit 20 */
	bool unusedBit_5_20 : 1 {};
	/**
	offset 4 bit 21 */
	bool unusedBit_5_21 : 1 {};
	/**
	offset 4 bit 22 */
	bool unusedBit_5_22 : 1 {};
	/**
	offset 4 bit 23 */
	bool unusedBit_5_23 : 1 {};
	/**
	offset 4 bit 24 */
	bool unusedBit_5_24 : 1 {};
	/**
	offset 4 bit 25 */
	bool unusedBit_5_25 : 1 {};
	/**
	offset 4 bit 26 */
	bool unusedBit_5_26 : 1 {};
	/**
	offset 4 bit 27 */
	bool unusedBit_5_27 : 1 {};
	/**
	offset 4 bit 28 */
	bool unusedBit_5_28 : 1 {};
	/**
	offset 4 bit 29 */
	bool unusedBit_5_29 : 1 {};
	/**
	offset 4 bit 30 */
	bool unusedBit_5_30 : 1 {};
	/**
	offset 4 bit 31 */
	bool unusedBit_5_31 : 1 {};
};
static_assert(sizeof(wbo_s) == 8);

// start of vvl_s
struct vvl_s {
	/**
	 * units: %
	 * offset 0
	 */
	int8_t fuelAdderPercent;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3] = {};
	/**
	 * Retard timing to remove from actual final timing (after all corrections) due to additional air.
	 * units: deg
	 * offset 4
	 */
	float ignitionRetard;
	/**
	 * offset 8
	 */
	int minimumTps;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 12
	 */
	int16_t minimumClt;
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 14
	 */
	int16_t maximumMap;
	/**
	 * units: afr
	 * offset 16
	 */
	scaled_channel<uint8_t, 10, 1> maximumAfr;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 17
	 */
	uint8_t alignmentFill_at_17[1] = {};
	/**
	 * units: rpm
	 * offset 18
	 */
	uint16_t activationRpm;
	/**
	 * units: rpm
	 * offset 20
	 */
	uint16_t deactivationRpm;
	/**
	 * units: rpm
	 * offset 22
	 */
	uint16_t deactivationRpmWindow;
};
static_assert(sizeof(vvl_s) == 24);

// start of rotational_idle_accumulator_s
struct rotational_idle_accumulator_s {
	/**
	 * Accumulator adder
	 * offset 0
	 */
	uint8_t acc_adder;
	/**
	 * Max value for the rotational idle accumulator to skip
	 * offset 1
	 */
	uint8_t acc_max;
	/**
	 * Rotational pattern shift
	 * offset 2
	 */
	uint8_t acc_offset;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3
	 */
	uint8_t alignmentFill_at_3[1] = {};
};
static_assert(sizeof(rotational_idle_accumulator_s) == 4);

// start of rotational_idle_s
struct rotational_idle_s {
	/**
	 * rotational idle enable feature
	offset 0 bit 0 */
	bool enabled : 1 {};
	/**
	offset 0 bit 1 */
	bool unusedBit_1_1 : 1 {};
	/**
	offset 0 bit 2 */
	bool unusedBit_1_2 : 1 {};
	/**
	offset 0 bit 3 */
	bool unusedBit_1_3 : 1 {};
	/**
	offset 0 bit 4 */
	bool unusedBit_1_4 : 1 {};
	/**
	offset 0 bit 5 */
	bool unusedBit_1_5 : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_1_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_1_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_1_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_1_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_1_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_1_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_1_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_1_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_1_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_1_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_1_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_1_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_1_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_1_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_1_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_1_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_1_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_1_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_1_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_1_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_1_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_1_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_1_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_1_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_1_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_1_31 : 1 {};
	/**
	 * rotational idle cut mode
	 * offset 4
	 */
	RotationalCutMode cut_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3] = {};
	/**
	 * Automatic engagement of rotational idle
	offset 8 bit 0 */
	bool auto_engage : 1 {};
	/**
	offset 8 bit 1 */
	bool unusedBit_35_1 : 1 {};
	/**
	offset 8 bit 2 */
	bool unusedBit_35_2 : 1 {};
	/**
	offset 8 bit 3 */
	bool unusedBit_35_3 : 1 {};
	/**
	offset 8 bit 4 */
	bool unusedBit_35_4 : 1 {};
	/**
	offset 8 bit 5 */
	bool unusedBit_35_5 : 1 {};
	/**
	offset 8 bit 6 */
	bool unusedBit_35_6 : 1 {};
	/**
	offset 8 bit 7 */
	bool unusedBit_35_7 : 1 {};
	/**
	offset 8 bit 8 */
	bool unusedBit_35_8 : 1 {};
	/**
	offset 8 bit 9 */
	bool unusedBit_35_9 : 1 {};
	/**
	offset 8 bit 10 */
	bool unusedBit_35_10 : 1 {};
	/**
	offset 8 bit 11 */
	bool unusedBit_35_11 : 1 {};
	/**
	offset 8 bit 12 */
	bool unusedBit_35_12 : 1 {};
	/**
	offset 8 bit 13 */
	bool unusedBit_35_13 : 1 {};
	/**
	offset 8 bit 14 */
	bool unusedBit_35_14 : 1 {};
	/**
	offset 8 bit 15 */
	bool unusedBit_35_15 : 1 {};
	/**
	offset 8 bit 16 */
	bool unusedBit_35_16 : 1 {};
	/**
	offset 8 bit 17 */
	bool unusedBit_35_17 : 1 {};
	/**
	offset 8 bit 18 */
	bool unusedBit_35_18 : 1 {};
	/**
	offset 8 bit 19 */
	bool unusedBit_35_19 : 1 {};
	/**
	offset 8 bit 20 */
	bool unusedBit_35_20 : 1 {};
	/**
	offset 8 bit 21 */
	bool unusedBit_35_21 : 1 {};
	/**
	offset 8 bit 22 */
	bool unusedBit_35_22 : 1 {};
	/**
	offset 8 bit 23 */
	bool unusedBit_35_23 : 1 {};
	/**
	offset 8 bit 24 */
	bool unusedBit_35_24 : 1 {};
	/**
	offset 8 bit 25 */
	bool unusedBit_35_25 : 1 {};
	/**
	offset 8 bit 26 */
	bool unusedBit_35_26 : 1 {};
	/**
	offset 8 bit 27 */
	bool unusedBit_35_27 : 1 {};
	/**
	offset 8 bit 28 */
	bool unusedBit_35_28 : 1 {};
	/**
	offset 8 bit 29 */
	bool unusedBit_35_29 : 1 {};
	/**
	offset 8 bit 30 */
	bool unusedBit_35_30 : 1 {};
	/**
	offset 8 bit 31 */
	bool unusedBit_35_31 : 1 {};
	/**
	 * Engage rotational idle under this Driver Intent.
	 * units: %TPS
	 * offset 12
	 */
	uint8_t max_tps;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3] = {};
	/**
	 * Rotational Idle Auto engage CLT
	offset 16 bit 0 */
	bool auto_engage_clt_enable : 1 {};
	/**
	offset 16 bit 1 */
	bool unusedBit_69_1 : 1 {};
	/**
	offset 16 bit 2 */
	bool unusedBit_69_2 : 1 {};
	/**
	offset 16 bit 3 */
	bool unusedBit_69_3 : 1 {};
	/**
	offset 16 bit 4 */
	bool unusedBit_69_4 : 1 {};
	/**
	offset 16 bit 5 */
	bool unusedBit_69_5 : 1 {};
	/**
	offset 16 bit 6 */
	bool unusedBit_69_6 : 1 {};
	/**
	offset 16 bit 7 */
	bool unusedBit_69_7 : 1 {};
	/**
	offset 16 bit 8 */
	bool unusedBit_69_8 : 1 {};
	/**
	offset 16 bit 9 */
	bool unusedBit_69_9 : 1 {};
	/**
	offset 16 bit 10 */
	bool unusedBit_69_10 : 1 {};
	/**
	offset 16 bit 11 */
	bool unusedBit_69_11 : 1 {};
	/**
	offset 16 bit 12 */
	bool unusedBit_69_12 : 1 {};
	/**
	offset 16 bit 13 */
	bool unusedBit_69_13 : 1 {};
	/**
	offset 16 bit 14 */
	bool unusedBit_69_14 : 1 {};
	/**
	offset 16 bit 15 */
	bool unusedBit_69_15 : 1 {};
	/**
	offset 16 bit 16 */
	bool unusedBit_69_16 : 1 {};
	/**
	offset 16 bit 17 */
	bool unusedBit_69_17 : 1 {};
	/**
	offset 16 bit 18 */
	bool unusedBit_69_18 : 1 {};
	/**
	offset 16 bit 19 */
	bool unusedBit_69_19 : 1 {};
	/**
	offset 16 bit 20 */
	bool unusedBit_69_20 : 1 {};
	/**
	offset 16 bit 21 */
	bool unusedBit_69_21 : 1 {};
	/**
	offset 16 bit 22 */
	bool unusedBit_69_22 : 1 {};
	/**
	offset 16 bit 23 */
	bool unusedBit_69_23 : 1 {};
	/**
	offset 16 bit 24 */
	bool unusedBit_69_24 : 1 {};
	/**
	offset 16 bit 25 */
	bool unusedBit_69_25 : 1 {};
	/**
	offset 16 bit 26 */
	bool unusedBit_69_26 : 1 {};
	/**
	offset 16 bit 27 */
	bool unusedBit_69_27 : 1 {};
	/**
	offset 16 bit 28 */
	bool unusedBit_69_28 : 1 {};
	/**
	offset 16 bit 29 */
	bool unusedBit_69_29 : 1 {};
	/**
	offset 16 bit 30 */
	bool unusedBit_69_30 : 1 {};
	/**
	offset 16 bit 31 */
	bool unusedBit_69_31 : 1 {};
	/**
	 * Rotational Idle Auto engage CLT.
	 * units: C
	 * offset 20
	 */
	int8_t auto_engage_clt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 21
	 */
	uint8_t alignmentFill_at_21[3] = {};
	/**
	 * Rotational Idle accumulators
	 * offset 24
	 */
	rotational_idle_accumulator_s accumulators[3] = {};
};
static_assert(sizeof(rotational_idle_s) == 36);

// start of engine_configuration_s
struct engine_configuration_s {
	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
	 * set engine_type X
	 * offset 0
	 */
	engine_type_e engineType;
	/**
	 * offset 2
	 */
	uint16_t startButtonSuppressOnStartUpMs;
	/**
	 * A secondary Rev limit engaged by the driver to help launch the vehicle faster
	 * units: rpm
	 * offset 4
	 */
	uint16_t launchRpm;
	/**
	 * set rpm_hard_limit X
	 * units: rpm
	 * offset 6
	 */
	uint16_t rpmHardLimit;
	/**
	 * Engine sniffer would be disabled above this rpm
	 * set engineSnifferRpmThreshold X
	 * units: RPM
	 * offset 8
	 */
	uint16_t engineSnifferRpmThreshold;
	/**
	 * Disable multispark above this engine speed.
	 * units: rpm
	 * offset 10
	 */
	scaled_channel<uint8_t, 1, 50> multisparkMaxRpm;
	/**
	 * Above this RPM, disable AC. Set to 0 to disable check.
	 * units: rpm
	 * offset 11
	 */
	scaled_channel<uint8_t, 1, 50> maxAcRpm;
	/**
	 * Above this TPS, disable AC. Set to 0 to disable check.
	 * units: %
	 * offset 12
	 */
	uint8_t maxAcTps;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[1] = {};
	/**
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 14
	 */
	int16_t maxAcClt;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	 * units: CR
	 * offset 16
	 */
	float compressionRatio;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	 * units: mv
	 * offset 20
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	 * units: mv
	 * offset 22
	 */
	uint16_t idlePositionMax;
	/**
	 * Enable LTIT (Long Term Idle Trim) learning
	offset 24 bit 0 */
	bool ltitEnabled : 1 {};
	/**
	offset 24 bit 1 */
	bool unusedBit_14_1 : 1 {};
	/**
	offset 24 bit 2 */
	bool unusedBit_14_2 : 1 {};
	/**
	offset 24 bit 3 */
	bool unusedBit_14_3 : 1 {};
	/**
	offset 24 bit 4 */
	bool unusedBit_14_4 : 1 {};
	/**
	offset 24 bit 5 */
	bool unusedBit_14_5 : 1 {};
	/**
	offset 24 bit 6 */
	bool unusedBit_14_6 : 1 {};
	/**
	offset 24 bit 7 */
	bool unusedBit_14_7 : 1 {};
	/**
	offset 24 bit 8 */
	bool unusedBit_14_8 : 1 {};
	/**
	offset 24 bit 9 */
	bool unusedBit_14_9 : 1 {};
	/**
	offset 24 bit 10 */
	bool unusedBit_14_10 : 1 {};
	/**
	offset 24 bit 11 */
	bool unusedBit_14_11 : 1 {};
	/**
	offset 24 bit 12 */
	bool unusedBit_14_12 : 1 {};
	/**
	offset 24 bit 13 */
	bool unusedBit_14_13 : 1 {};
	/**
	offset 24 bit 14 */
	bool unusedBit_14_14 : 1 {};
	/**
	offset 24 bit 15 */
	bool unusedBit_14_15 : 1 {};
	/**
	offset 24 bit 16 */
	bool unusedBit_14_16 : 1 {};
	/**
	offset 24 bit 17 */
	bool unusedBit_14_17 : 1 {};
	/**
	offset 24 bit 18 */
	bool unusedBit_14_18 : 1 {};
	/**
	offset 24 bit 19 */
	bool unusedBit_14_19 : 1 {};
	/**
	offset 24 bit 20 */
	bool unusedBit_14_20 : 1 {};
	/**
	offset 24 bit 21 */
	bool unusedBit_14_21 : 1 {};
	/**
	offset 24 bit 22 */
	bool unusedBit_14_22 : 1 {};
	/**
	offset 24 bit 23 */
	bool unusedBit_14_23 : 1 {};
	/**
	offset 24 bit 24 */
	bool unusedBit_14_24 : 1 {};
	/**
	offset 24 bit 25 */
	bool unusedBit_14_25 : 1 {};
	/**
	offset 24 bit 26 */
	bool unusedBit_14_26 : 1 {};
	/**
	offset 24 bit 27 */
	bool unusedBit_14_27 : 1 {};
	/**
	offset 24 bit 28 */
	bool unusedBit_14_28 : 1 {};
	/**
	offset 24 bit 29 */
	bool unusedBit_14_29 : 1 {};
	/**
	offset 24 bit 30 */
	bool unusedBit_14_30 : 1 {};
	/**
	offset 24 bit 31 */
	bool unusedBit_14_31 : 1 {};
	/**
	 * EMA filter constant for LTIT (0-255)
	 * units: 0-255
	 * offset 28
	 */
	uint8_t ltitEmaAlpha;
	/**
	 * RPM range to consider stable idle
	 * units: rpm
	 * offset 29
	 */
	uint8_t ltitStableRpmThreshold;
	/**
	 * Minimum time of stable idle before learning
	 * units: s
	 * offset 30
	 */
	uint8_t ltitStableTime;
	/**
	 * LTIT learning rate
	 * units: %/s
	 * offset 31
	 */
	uint8_t ltitCorrectionRate;
	/**
	 * Delay after ignition ON before LTIT learning/application
	 * units: s
	 * offset 32
	 */
	uint8_t ltitIgnitionOnDelay;
	/**
	 * Delay after ignition OFF before LTIT save
	 * units: s
	 * offset 33
	 */
	uint8_t ltitIgnitionOffSaveDelay;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 34
	 */
	uint8_t alignmentFill_at_34[2] = {};
	/**
	 * Minimum LTIT multiplicative correction value
	 * units: %
	 * offset 36
	 */
	float ltitClampMin;
	/**
	 * Maximum LTIT multiplicative correction value
	 * units: %
	 * offset 40
	 */
	float ltitClampMax;
	/**
	 * LTIT table regional smoothing intensity (0=no smoothing)
	 * units: ratio
	 * offset 44
	 */
	scaled_channel<uint8_t, 100, 1> ltitSmoothingIntensity;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 45
	 */
	uint8_t alignmentFill_at_45[3] = {};
	/**
	 * Minimum threshold of PID integrator for LTIT correction
	 * units: %
	 * offset 48
	 */
	float ltitIntegratorThreshold;
	/**
	 * offset 52
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 54
	 */
	Gpio sdCardCsPin;
	/**
	 * offset 56
	 */
	Gpio canTxPin;
	/**
	 * offset 58
	 */
	Gpio canRxPin;
	/**
	 * Pin that activates the reduction/cut for shifting. Sometimes shared with the Launch Control pin
	 * offset 60
	 */
	switch_input_pin_e torqueReductionTriggerPin;
	/**
	 * units: %
	 * offset 62
	 */
	int8_t launchFuelAdderPercent;
	/**
	 * Time after which the throttle is considered jammed.
	 * units: sec
	 * offset 63
	 */
	scaled_channel<uint8_t, 50, 1> etbJamTimeout;
	/**
	 * offset 64
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 66
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * Additional idle % while A/C is active
	 * units: %
	 * offset 67
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	 * units: deg
	 * offset 68
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	 * units: count
	 * offset 69
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * units: RPM
	 * offset 70
	 */
	int16_t vvtControlMinRpm;
	/**
	 * offset 72
	 */
	injector_s injector;
	/**
	 * offset 172
	 */
	injector_s injectorSecondary;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 272 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 272 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 272 bit 2 */
	bool enableKline : 1 {};
	/**
	offset 272 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 272 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 272 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 272 bit 6 */
	bool enableTrailingSparks : 1 {};
	/**
	 * TLE7209 and L6205 use two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 272 bit 7 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 272 bit 8 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 272 bit 9 */
	bool useEeprom : 1 {};
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 272 bit 10 */
	bool useCicPidForIdle : 1 {};
	/**
	offset 272 bit 11 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 272 bit 12 */
	bool kickStartCranking : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 272 bit 13 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 272 bit 14 */
	bool launchControlEnabled : 1 {};
	/**
	offset 272 bit 15 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 272 bit 16 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	offset 272 bit 17 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 272 bit 18 */
	bool useTLE8888_stepper : 1 {};
	/**
	offset 272 bit 19 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 272 bit 20 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 272 bit 21 */
	bool artificialTestMisfire : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 272 bit 22 */
	bool useFordRedundantPps : 1 {};
	/**
	offset 272 bit 23 */
	bool cltSensorPulldown : 1 {};
	/**
	offset 272 bit 24 */
	bool iatSensorPulldown : 1 {};
	/**
	offset 272 bit 25 */
	bool allowIdenticalPps : 1 {};
	/**
	offset 272 bit 26 */
	bool overrideVvtTriggerGaps : 1 {};
	/**
	 * If enabled - use onboard SPI Accelerometer, otherwise listen for CAN messages
	offset 272 bit 27 */
	bool useSpiImu : 1 {};
	/**
	offset 272 bit 28 */
	bool enableStagedInjection : 1 {};
	/**
	offset 272 bit 29 */
	bool useIdleAdvanceWhileCoasting : 1 {};
	/**
	offset 272 bit 30 */
	bool unusedBit_102_30 : 1 {};
	/**
	offset 272 bit 31 */
	bool unusedBit_102_31 : 1 {};
	/**
	 * Closed voltage for primary throttle position sensor
	 * offset 276
	 */
	tps_limit_t tpsMin;
	/**
	 * Fully opened voltage for primary throttle position sensor
	 * offset 278
	 */
	tps_limit_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 280
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 282
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 284
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	 * units: ms
	 * offset 288
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	 * units: rpm
	 * offset 292
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	 * units: rpm
	 * offset 294
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 296
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 436
	 */
	ThermistorConf clt;
	/**
	 * offset 468
	 */
	ThermistorConf iat;
	/**
	 * units: deg
	 * offset 500
	 */
	float launchTimingRetard;
	/**
	 * Maximum commanded airmass for the idle controller.
	 * units: mg
	 * offset 504
	 */
	scaled_channel<uint8_t, 1, 2> idleMaximumAirmass;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 505
	 */
	uint8_t alignmentFill_at_505[1] = {};
	/**
	 * iTerm min value
	 * offset 506
	 */
	int16_t alternator_iTermMin;
	/**
	 * iTerm max value
	 * offset 508
	 */
	int16_t alternator_iTermMax;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 510
	 */
	uint8_t alignmentFill_at_510[2] = {};
	/**
	 * @@DISPLACEMENT_TOOLTIP@@
	 * units: L
	 * offset 512
	 */
	float displacement;
	/**
	 * units: RPM
	 * offset 516
	 */
	uint16_t triggerSimulatorRpm;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 518
	 */
	uint8_t alignmentFill_at_518[2] = {};
	/**
	 * Number of cylinder the engine has.
	 * offset 520
	 */
	uint32_t cylindersCount;
	/**
	 * offset 524
	 */
	firing_order_e firingOrder;
	/**
	 * offset 525
	 */
	uint8_t justATempTest;
	/**
	 * Delta kPa/psi for MAP sync
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 526
	 */
	uint8_t mapSyncThreshold;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 527
	 */
	uint8_t alignmentFill_at_527[1] = {};
	/**
	 * @@CYLINDER_BORE_TOOLTIP@@
	 * units: mm
	 * offset 528
	 */
	float cylinderBore;
	/**
	 * Determines the method used for calculating fuel delivery. The following options are available:
	 * Uses intake manifold pressure (MAP) and intake air temperature (IAT) to calculate air density and fuel requirements. This is a common strategy, especially for naturally aspirated or turbocharged engines.
	 * Alpha-N: Uses throttle position as the primary load input for fuel calculation. This strategy is generally used in engines with individual throttle bodies or those that lack a reliable MAP signal.
	 * MAF Air Charge: Relies on a Mass Air Flow (MAF) sensor to measure the amount of air entering the engine directly, making it effective for engines equipped with a MAF sensor.
	 * Lua: Allows for custom fuel calculations using Lua scripting, enabling highly specific tuning applications where the other strategies don't apply.
	 * offset 532
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * units: %
	 * offset 533
	 */
	uint8_t ALSMaxTPS;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 534
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups.
	 * offset 535
	 */
	injection_mode_e injectionMode;
	/**
	 * Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 536
	 */
	uint16_t boostControlMinRpm;
	/**
	 * Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 538
	 */
	uint8_t boostControlMinTps;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 539
	 */
	uint8_t alignmentFill_at_539[1] = {};
	/**
	 * Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 540
	 */
	uint16_t boostControlMinMap;
	/**
	 * Wastegate control Solenoid, set to 'NONE' if you are using DC wastegate
	 * offset 542
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 544
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 545
	 */
	uint8_t alignmentFill_at_545[3] = {};
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	 * units: deg
	 * offset 548
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 552
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 553
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	 * units: SPECIAL_CASE_SPEED
	 * offset 554
	 */
	uint8_t maxIdleVss;
	/**
	 * Allowed range around detection position
	 * offset 555
	 */
	uint8_t camDecoder2jzPrecision;
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 556
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing
	 * Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 558
	 */
	timing_mode_e timingMode;
	/**
	 * offset 559
	 */
	can_nbc_e canNbcType;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	 * units: RPM
	 * offset 560
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchronization point before TDC and negative in case of synchronization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * HOWTO:
	 * 1: Switch to fixed timing mode on 'ignition setting' dialog
	 * 2: use an actual timing light to calibrate
	 * 3: add/subtract until timing light confirms desired fixed timing value!'
	 * units: deg btdc
	 * offset 564
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	 * units: coef
	 * offset 568
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	 * units: coef
	 * offset 572
	 */
	float vbattDividerCoeff;
	/**
	 * offset 576
	 */
	output_pin_e fanPin;
	/**
	 * offset 578
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 579
	 */
	uint8_t alignmentFill_at_579[1] = {};
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 580
	 */
	int16_t fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 582
	 */
	int16_t fanOffTemperature;
	/**
	 * offset 584
	 */
	output_pin_e fan2Pin;
	/**
	 * offset 586
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 587
	 */
	uint8_t alignmentFill_at_587[1] = {};
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 588
	 */
	int16_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 590
	 */
	int16_t fan2OffTemperature;
	/**
	 * offset 592
	 */
	int8_t disableFan1AtSpeed;
	/**
	 * offset 593
	 */
	int8_t disableFan2AtSpeed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 594
	 */
	uint8_t alignmentFill_at_594[2] = {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 596 bit 0 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 596 bit 1 */
	bool disableFan2WhenStopped : 1 {};
	/**
	offset 596 bit 2 */
	bool unusedBit_168_2 : 1 {};
	/**
	offset 596 bit 3 */
	bool unusedBit_168_3 : 1 {};
	/**
	offset 596 bit 4 */
	bool unusedBit_168_4 : 1 {};
	/**
	offset 596 bit 5 */
	bool unusedBit_168_5 : 1 {};
	/**
	offset 596 bit 6 */
	bool unusedBit_168_6 : 1 {};
	/**
	offset 596 bit 7 */
	bool unusedBit_168_7 : 1 {};
	/**
	offset 596 bit 8 */
	bool unusedBit_168_8 : 1 {};
	/**
	offset 596 bit 9 */
	bool unusedBit_168_9 : 1 {};
	/**
	offset 596 bit 10 */
	bool unusedBit_168_10 : 1 {};
	/**
	offset 596 bit 11 */
	bool unusedBit_168_11 : 1 {};
	/**
	offset 596 bit 12 */
	bool unusedBit_168_12 : 1 {};
	/**
	offset 596 bit 13 */
	bool unusedBit_168_13 : 1 {};
	/**
	offset 596 bit 14 */
	bool unusedBit_168_14 : 1 {};
	/**
	offset 596 bit 15 */
	bool unusedBit_168_15 : 1 {};
	/**
	offset 596 bit 16 */
	bool unusedBit_168_16 : 1 {};
	/**
	offset 596 bit 17 */
	bool unusedBit_168_17 : 1 {};
	/**
	offset 596 bit 18 */
	bool unusedBit_168_18 : 1 {};
	/**
	offset 596 bit 19 */
	bool unusedBit_168_19 : 1 {};
	/**
	offset 596 bit 20 */
	bool unusedBit_168_20 : 1 {};
	/**
	offset 596 bit 21 */
	bool unusedBit_168_21 : 1 {};
	/**
	offset 596 bit 22 */
	bool unusedBit_168_22 : 1 {};
	/**
	offset 596 bit 23 */
	bool unusedBit_168_23 : 1 {};
	/**
	offset 596 bit 24 */
	bool unusedBit_168_24 : 1 {};
	/**
	offset 596 bit 25 */
	bool unusedBit_168_25 : 1 {};
	/**
	offset 596 bit 26 */
	bool unusedBit_168_26 : 1 {};
	/**
	offset 596 bit 27 */
	bool unusedBit_168_27 : 1 {};
	/**
	offset 596 bit 28 */
	bool unusedBit_168_28 : 1 {};
	/**
	offset 596 bit 29 */
	bool unusedBit_168_29 : 1 {};
	/**
	offset 596 bit 30 */
	bool unusedBit_168_30 : 1 {};
	/**
	offset 596 bit 31 */
	bool unusedBit_168_31 : 1 {};
	/**
	 * offset 600
	 */
	int wizardPanelToShow;
	/**
	 * offset 604
	 */
	output_pin_e acrPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 606
	 */
	uint8_t alignmentFill_at_606[2] = {};
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	 * units: revs/km
	 * offset 608
	 */
	float driveWheelRevPerKm;
	/**
	 * CANbus thread period in ms
	 * units: ms
	 * offset 612
	 */
	int canSleepPeriodMs;
	/**
	 * units: index
	 * offset 616
	 */
	int byFirmwareVersion;
	/**
	 * First analog throttle body, first sensor. See also pedalPositionAdcChannel
	 * Analog TPS inputs have 200Hz low-pass cutoff.
	 * offset 620
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 621
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 622
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * offset 623
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 624
	 */
	float idle_derivativeFilterLoss;
	/**
	 * offset 628
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	 * units: %
	 * offset 640
	 */
	float airByRpmTaper;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	 * units: %
	 * offset 644
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 645
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 646
	 */
	uint8_t acrRevolutions;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 647
	 */
	uint8_t alignmentFill_at_647[1] = {};
	/**
	 * offset 648
	 */
	int calibrationBirthday;
	/**
	 * units: volts
	 * offset 652
	 */
	float adcVcc;
	/**
	 * Magic engine phase: we compare instant MAP at X to instant MAP at x+360 angle in one complete cycle
	 * units: Deg
	 * offset 656
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 660
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 668
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 688
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * TPS/PPS error threshold
	 * units: %
	 * offset 689
	 */
	scaled_channel<uint8_t, 10, 1> etbSplit;
	/**
	 * offset 690
	 */
	Gpio tle6240_cs;
	/**
	 * offset 692
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * offset 693
	 */
	pin_output_mode_e mc33810_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 694
	 */
	uint8_t alignmentFill_at_694[2] = {};
	/**
	 * @see hasBaroSensor
	 * offset 696
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 708
	 */
	idle_hardware_s idle;
	/**
	 * Ignition timing to remove when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 720
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	 * units: deg/s
	 * offset 721
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 722
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * offset 723
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	 * units: ratio
	 * offset 724
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 726
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	 * units: count
	 * offset 727
	 */
	uint8_t vssToothCount;
	/**
	 * Allows you to change the default load axis used for the VE table, which is typically MAP (manifold absolute pressure).
	 * offset 728
	 */
	ve_override_e idleVeOverrideMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 729
	 */
	uint8_t alignmentFill_at_729[1] = {};
	/**
	 * offset 730
	 */
	Gpio l9779_cs;
	/**
	 * offset 732
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 756
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 780
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 781
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 782
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 784
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * How many consecutive VVT gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 785
	 */
	int8_t gapVvtTrackingLengthOverride;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 786
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 788
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 789
	 */
	uint8_t alignmentFill_at_789[1] = {};
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 790
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 792
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 794
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 795
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 796
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT] = {};
	/**
	 * offset 804
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 805
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 806
	 */
	Gpio debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 808
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 809
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 810
	 */
	Gpio mc33972_cs;
	/**
	 * offset 812
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 813
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 814
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 815
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 816
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 817
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 818
	 */
	Gpio mc33810_cs[C_MC33810_COUNT] = {};
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 820
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 824
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT] = {};
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	 * units: us
	 * offset 828
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * Ratio between the wheels and your transmission output.
	 * units: ratio
	 * offset 830
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 832
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 834
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 835
	 */
	uint8_t alignmentFill_at_835[1] = {};
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 836
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * units: g/s
	 * offset 840
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 842
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * offset 844
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 845
	 */
	uint8_t alignmentFill_at_845[1] = {};
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 846
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 848
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * units: RPM
	 * offset 849
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionMinRpm;
	/**
	 * units: %
	 * offset 850
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionMinLoad;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 851
	 */
	uint8_t alignmentFill_at_851[1] = {};
	/**
	offset 852 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 852 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 852 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	offset 852 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 852 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 852 bit 5 */
	bool rethrowHardFault : 1 {};
	/**
	offset 852 bit 6 */
	bool verboseQuad : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 852 bit 7 */
	bool useStepperIdle : 1 {};
	/**
	offset 852 bit 8 */
	bool lambdaProtectionEnable : 1 {};
	/**
	offset 852 bit 9 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	offset 852 bit 10 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 852 bit 11 */
	bool externalRusEfiGdiModule : 1 {};
	/**
	 * Useful for individual intakes
	offset 852 bit 12 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 852 bit 13 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 852 bit 14 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 852 bit 15 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 852 bit 16 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 852 bit 17 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 852 bit 18 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 852 bit 19 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 852 bit 20 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 852 bit 21 */
	bool pauseEtbControl : 1 {};
	/**
	offset 852 bit 22 */
	bool verboseKLine : 1 {};
	/**
	offset 852 bit 23 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 852 bit 24 */
	bool enableAemXSeries : 1 {};
	/**
	offset 852 bit 25 */
	bool modeledFlowIdle : 1 {};
	/**
	offset 852 bit 26 */
	bool isTuningDetectorEnabled : 1 {};
	/**
	offset 852 bit 27 */
	bool useAbsolutePressureForLagTime : 1 {};
	/**
	offset 852 bit 28 */
	bool unusedBit_311_28 : 1 {};
	/**
	offset 852 bit 29 */
	bool unusedBit_311_29 : 1 {};
	/**
	offset 852 bit 30 */
	bool unusedBit_311_30 : 1 {};
	/**
	offset 852 bit 31 */
	bool unusedBit_311_31 : 1 {};
	/**
	 * offset 856
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT] = {};
	/**
	 * offset 864
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 865
	 */
	uint8_t alignmentFill_at_865[3] = {};
	/**
	 * offset 868
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	 * units: v
	 * offset 872
	 */
	uint8_t mc33_hvolt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 873
	 */
	uint8_t alignmentFill_at_873[1] = {};
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 874
	 */
	uint16_t minimumBoostClosedLoopMap;
	/**
	 * units: %
	 * offset 876
	 */
	int8_t initialIgnitionCutPercent;
	/**
	 * units: %
	 * offset 877
	 */
	int8_t finalIgnitionCutPercentBeforeLaunch;
	/**
	 * offset 878
	 */
	gppwm_channel_e boostOpenLoopYAxis;
	/**
	 * offset 879
	 */
	spi_device_e l9779spiDevice;
	/**
	 * offset 880
	 */
	imu_type_e imuType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 881
	 */
	uint8_t alignmentFill_at_881[1] = {};
	/**
	 * How far above idle speed do we consider idling, i.e. coasting detection threshold.
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	 * units: RPM
	 * offset 882
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	 * units: ms
	 * offset 884
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 886
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 888
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 890
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 892
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 894
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 895
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 896
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT] = {};
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 912
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * Since torque reduction pin is usually shared with launch control, most people have an RPM where behavior under that is Launch Control, over that is Flat Shift/Torque Reduction
	 * units: rpm
	 * offset 914
	 */
	uint16_t torqueReductionArmingRpm;
	/**
	 * offset 916
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 917
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	 * units: :1
	 * offset 918
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	 * units: %
	 * offset 919
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	 * units: hz
	 * offset 920
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 922
	 */
	adc_channel_e idlePositionChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 923
	 */
	uint8_t alignmentFill_at_923[1] = {};
	/**
	 * offset 924
	 */
	uint16_t launchCorrectionsEndRpm;
	/**
	 * offset 926
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 928
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 929
	 */
	uint8_t alignmentFill_at_929[1] = {};
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 930
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 932
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * units: RPM
	 * offset 934
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionRestoreRpm;
	/**
	 * offset 935
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	 * units: count
	 * offset 936
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	 * units: %
	 * offset 940
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * units: %
	 * offset 942
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * Closed voltage for secondary throttle position sensor
	 * offset 944
	 */
	tps_limit_t tps1SecondaryMin;
	/**
	 * Fully opened voltage for secondary throttle position sensor
	 * offset 946
	 */
	tps_limit_t tps1SecondaryMax;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	 * units: Seconds
	 * offset 948
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 950
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 952
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 953
	 */
	uint8_t alignmentFill_at_953[1] = {};
	/**
	 * offset 954
	 */
	output_pin_e acRelayPin;
	/**
	 * units: %
	 * offset 956
	 */
	uint8_t lambdaProtectionMinTps;
	/**
	 * Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed
	 * units: s
	 * offset 957
	 */
	scaled_channel<uint8_t, 10, 1> lambdaProtectionTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 958
	 */
	uint8_t alignmentFill_at_958[2] = {};
	/**
	 * offset 960
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT] = {};
	/**
	 * offset 992
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 994
	 */
	Gpio spi1misoPin;
	/**
	 * offset 996
	 */
	Gpio spi1sckPin;
	/**
	 * offset 998
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 1000
	 */
	Gpio spi2misoPin;
	/**
	 * offset 1002
	 */
	Gpio spi2sckPin;
	/**
	 * offset 1004
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 1006
	 */
	Gpio spi3misoPin;
	/**
	 * offset 1008
	 */
	Gpio spi3sckPin;
	/**
	 * UNUSED
	 * Will remove in 2026 for sure
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 1010
	 */
	Gpio cdmInputPin;
	/**
	 * offset 1012
	 */
	uart_device_e consoleUartDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1013
	 */
	uint8_t alignmentFill_at_1013[3] = {};
	/**
	 * Ramp the idle target down from the entry threshold over N seconds when returning to idle. Helps prevent overshooting (below) the idle target while returning to idle from coasting.
	offset 1016 bit 0 */
	bool idleReturnTargetRamp : 1 {};
	/**
	offset 1016 bit 1 */
	bool useInjectorFlowLinearizationTable : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 1016 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 1016 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 1016 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 1016 bit 5 */
	bool canInputBCM : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	offset 1016 bit 6 */
	bool consumeObdSensors : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 1016 bit 7 */
	bool enableCanVss : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 1016 bit 8 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 1016 bit 9 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 1016 bit 10 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 1016 bit 11 */
	bool verboseVVTDecoding : 1 {};
	/**
	offset 1016 bit 12 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * When set to yes, it enables intake air temperature-based corrections for Alpha-N tuning strategies.
	offset 1016 bit 13 */
	bool alphaNUseIat : 1 {};
	/**
	offset 1016 bit 14 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 1016 bit 15 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 1016 bit 16 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 1016 bit 17 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 1016 bit 18 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 1016 bit 19 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 1016 bit 20 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 1016 bit 21 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 1016 bit 22 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 1016 bit 23 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 1016 bit 24 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 1016 bit 25 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 1016 bit 26 */
	bool tcuEnabled : 1 {};
	/**
	offset 1016 bit 27 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coils
	offset 1016 bit 28 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 1016 bit 29 */
	bool verboseCan2 : 1 {};
	/**
	offset 1016 bit 30 */
	bool unusedBit_410_30 : 1 {};
	/**
	offset 1016 bit 31 */
	bool unusedBit_410_31 : 1 {};
	/**
	 * offset 1020
	 */
	dc_io etbIo[ETB_COUNT] = {};
	/**
	 * offset 1036
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 1038
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 1040
	 */
	pid_s boostPid;
	/**
	 * offset 1060
	 */
	boostType_e boostType;
	/**
	 * offset 1061
	 */
	pin_input_mode_e ignitionKeyDigitalPinMode;
	/**
	 * offset 1062
	 */
	Gpio ignitionKeyDigitalPin;
	/**
	 * units: Hz
	 * offset 1064
	 */
	int boostPwmFrequency;
	/**
	 * offset 1068
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1069
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * offset 1070
	 */
	cranking_condition_e crankingCondition;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	 * units: sec
	 * offset 1071
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	 * units: roc
	 * offset 1072
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	 * units: coeff
	 * offset 1076
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * Selects the acceleration enrichment strategy.
	 * offset 1080
	 */
	accel_enrichment_mode_e accelEnrichmentMode;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	 * units: sec
	 * offset 1081
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Pause closed loop fueling after acceleration fuel occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel accel.
	 * units: sec
	 * offset 1082
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterAccelTime;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1083
	 */
	uint8_t alignmentFill_at_1083[1] = {};
	/**
	 * Launch disabled above this speed if setting is above zero
	 * units: SPECIAL_CASE_SPEED
	 * offset 1084
	 */
	int launchSpeedThreshold;
	/**
	 * Starting Launch RPM window to activate (subtracts from Launch RPM)
	 * units: RPM
	 * offset 1088
	 */
	int launchRpmWindow;
	/**
	 * units: ms
	 * offset 1092
	 */
	float triggerEventsTimeoutMs;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '100%' means no filtering, 98% would be some filtering.
	 * units: percent
	 * offset 1096
	 */
	float ppsExpAverageAlpha;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 1100
	 */
	float mapExpAverageAlpha;
	/**
	 * offset 1104
	 */
	float magicNumberAvailableForDevTricks;
	/**
	 * offset 1108
	 */
	float turbochargerFilter;
	/**
	 * offset 1112
	 */
	int launchTpsThreshold;
	/**
	 * offset 1116
	 */
	float launchActivateDelay;
	/**
	 * offset 1120
	 */
	stft_s stft;
	/**
	 * offset 1148
	 */
	ltft_s ltft;
	/**
	 * offset 1164
	 */
	dc_io stepperDcIo[DC_PER_STEPPER] = {};
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1180
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1212
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1244
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1276
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT] = {};
	/**
	 * offset 1288
	 */
	dc_function_e etbFunctions[ETB_COUNT] = {};
	/**
	 * offset 1290
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1291
	 */
	uint8_t alignmentFill_at_1291[1] = {};
	/**
	 * offset 1292
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1294
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1295
	 */
	uint8_t alignmentFill_at_1295[1] = {};
	/**
	 * offset 1296
	 */
	Gpio drv8860_miso;
	/**
	 * offset 1298
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1314
	 */
	uint8_t alignmentFill_at_1314[2] = {};
	/**
	 * Angle between cam sensor and VVT zero position
	 * units: value
	 * offset 1316
	 */
	float vvtOffsets[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1332
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT] = {};
	/**
	 * offset 1380
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS] = {};
	/**
	 * Closed voltage for secondary throttle position sensor
	 * offset 1444
	 */
	tps_limit_t tps2SecondaryMin;
	/**
	 * Fully opened voltage for secondary throttle position sensor
	 * offset 1446
	 */
	tps_limit_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1448 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1448 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Write SD card log even when powered by USB
	offset 1448 bit 2 */
	bool alwaysWriteSdCard : 1 {};
	/**
	 * Second harmonic (aka double) is usually quieter background noise
	offset 1448 bit 3 */
	bool knockDetectionUseDoubleFrequency : 1 {};
	/**
	 * Unlocking only via rusEFI console using 'unlock PICODEBUG' command. Use 'reset to default firmware' if pincode is lost.
	offset 1448 bit 4 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1448 bit 5 */
	bool silentTriggerError : 1 {};
	/**
	offset 1448 bit 6 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1448 bit 7 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write. See also can1ListenMode
	offset 1448 bit 8 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1448 bit 9 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1448 bit 10 */
	bool enableOilPressureProtect : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1448 bit 11 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1448 bit 12 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1448 bit 13 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1448 bit 14 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	 * When enabled, this option cuts the fuel supply when the RPM limit is reached. Cutting fuel provides a smoother limiting action; however, it may lead to slightly higher combustion chamber temperatures since unburned fuel is not present to cool the combustion process.
	offset 1448 bit 15 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * When selected, this option cuts the spark to limit RPM. Cutting spark can produce flames from the exhaust due to unburned fuel igniting in the exhaust system. Additionally, this unburned fuel can help cool the combustion chamber, which may be beneficial in high-performance applications.
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1448 bit 16 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1448 bit 17 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1448 bit 18 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1448 bit 19 */
	bool torqueReductionEnabled : 1 {};
	/**
	 * When we sync cam sensor is that first or second full engine revolution of the four stroke cycle?
	offset 1448 bit 20 */
	bool camSyncOnSecondCrankRevolution : 1 {};
	/**
	offset 1448 bit 21 */
	bool limitTorqueReductionTime : 1 {};
	/**
	 * Are you a developer troubleshooting TS over CAN ISO/TP?
	offset 1448 bit 22 */
	bool verboseIsoTp : 1 {};
	/**
	 * In this mode only trigger events go into engine sniffer and not coils/injectors etc
	offset 1448 bit 23 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1448 bit 24 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1448 bit 25 */
	bool skippedWheelOnCam : 1 {};
	/**
	offset 1448 bit 26 */
	bool unusedBit_486_26 : 1 {};
	/**
	offset 1448 bit 27 */
	bool unusedBit_486_27 : 1 {};
	/**
	offset 1448 bit 28 */
	bool unusedBit_486_28 : 1 {};
	/**
	offset 1448 bit 29 */
	bool unusedBit_486_29 : 1 {};
	/**
	offset 1448 bit 30 */
	bool unusedBit_486_30 : 1 {};
	/**
	offset 1448 bit 31 */
	bool unusedBit_486_31 : 1 {};
	/**
	 * A/C button input
	 * offset 1452
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1454
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	 * units: %
	 * offset 1455
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1456
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1457
	 */
	uint8_t alignmentFill_at_1457[3] = {};
	/**
	offset 1460 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1460 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1460 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1460 bit 3 */
	bool complexWallModel : 1 {};
	/**
	 * RPM is measured based on last 720 degrees while instant RPM is measured based on the last 90 degrees of crank revolution
	offset 1460 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1460 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra advance at low idle speeds will prevent stalling and extra retard at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1460 bit 6 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1460 bit 7 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1460 bit 8 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 1460 bit 9 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	offset 1460 bit 10 */
	bool hondaK : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1460 bit 11 */
	bool twoWireBatchIgnition : 1 {};
	/**
	 * Read MAP sensor on ECU start-up to use as baro value.
	offset 1460 bit 12 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1460 bit 13 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT and PID idle).
	 * You probably don't need this.
	offset 1460 bit 14 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1460 bit 15 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1460 bit 16 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1460 bit 17 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1460 bit 18 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1460 bit 19 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1460 bit 20 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors.
	 * WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1460 bit 21 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1460 bit 22 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1460 bit 23 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1460 bit 24 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	offset 1460 bit 25 */
	bool sdTriggerLog : 1 {};
	/**
	offset 1460 bit 26 */
	bool stepper_dc_use_two_wires : 1 {};
	/**
	offset 1460 bit 27 */
	bool watchOutForLinearTime : 1 {};
	/**
	offset 1460 bit 28 */
	bool unusedBit_525_28 : 1 {};
	/**
	offset 1460 bit 29 */
	bool unusedBit_525_29 : 1 {};
	/**
	offset 1460 bit 30 */
	bool unusedBit_525_30 : 1 {};
	/**
	offset 1460 bit 31 */
	bool unusedBit_525_31 : 1 {};
	/**
	 * units: count
	 * offset 1464
	 */
	uint32_t engineChartSize;
	/**
	 * units: mult
	 * offset 1468
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * Idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	 * units: RPM
	 * offset 1472
	 */
	int16_t acIdleRpmTarget;
	/**
	 * set warningPeriod X
	 * units: seconds
	 * offset 1474
	 */
	int16_t warningPeriod;
	/**
	 * units: angle
	 * offset 1476
	 */
	float knockDetectionWindowStart;
	/**
	 * units: ms
	 * offset 1480
	 */
	float idleStepperReactionTime;
	/**
	 * units: count
	 * offset 1484
	 */
	int idleStepperTotalSteps;
	/**
	 * Pedal position to realize that we need to reduce torque when the trigger pin is triggered
	 * offset 1488
	 */
	int torqueReductionArmingApp;
	/**
	 * offset 1492
	 */
	float referenceTorqueForGenerator;
	/**
	 * offset 1496
	 */
	float referenceMapForGenerator;
	/**
	 * offset 1500
	 */
	float referenceVeForGenerator;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1504
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	 * units: Seconds
	 * offset 1508
	 */
	float wwaeTau;
	/**
	 * offset 1512
	 */
	pid_s alternatorControl;
	/**
	 * offset 1532
	 */
	pid_s etb;
	/**
	 * RPM range above upper limit for extra air taper
	 * units: RPM
	 * offset 1552
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1554
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed voltage for primary throttle position sensor
	 * offset 1556
	 */
	tps_limit_t tps2Min;
	/**
	 * Fully opened voltage for primary throttle position sensor
	 * offset 1558
	 */
	tps_limit_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1560
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1562
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1563
	 */
	uint8_t alignmentFill_at_1563[1] = {};
	/**
	 * offset 1564
	 */
	Gpio mc33816_flag0;
	/**
	 * offset 1566
	 */
	scaled_channel<uint16_t, 1000, 1> tachPulsePerRev;
	/**
	 * kPa/psi value which is too low to be true
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 1568
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa/psi value which is too high to be true
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 1572
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	 * units: ms
	 * offset 1576
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	 * units: ms
	 * offset 1578
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1580
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	 * units: Fraction
	 * offset 1600
	 */
	float wwaeBeta;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1604
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1606
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1608
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT] = {};
	/**
	 * offset 1612
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1614
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * units: volts
	 * offset 1616
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * units: volts
	 * offset 1620
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	 * units: seconds
	 * offset 1624
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * larger value = larger intake manifold volume
	 * offset 1626
	 */
	uint16_t mafFilterParameter;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	 * units: RPM
	 * offset 1628
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1630
	 */
	uint8_t alignmentFill_at_1630[2] = {};
	/**
	 * See Over/Undervoltage Shutdown/Retry bit in documentation
	offset 1632 bit 0 */
	bool mc33810DisableRecoveryMode : 1 {};
	/**
	offset 1632 bit 1 */
	bool mc33810Gpgd0Mode : 1 {};
	/**
	offset 1632 bit 2 */
	bool mc33810Gpgd1Mode : 1 {};
	/**
	offset 1632 bit 3 */
	bool mc33810Gpgd2Mode : 1 {};
	/**
	offset 1632 bit 4 */
	bool mc33810Gpgd3Mode : 1 {};
	/**
	 * Send out board statistics
	offset 1632 bit 5 */
	bool enableExtendedCanBroadcast : 1 {};
	/**
	 * global_can_data performance hack
	offset 1632 bit 6 */
	bool luaCanRxWorkaround : 1 {};
	/**
	offset 1632 bit 7 */
	bool flexSensorInverted : 1 {};
	/**
	offset 1632 bit 8 */
	bool useHardSkipInTraction : 1 {};
	/**
	 * Use Aux Speed 1 as one of speeds for wheel slip ratio?
	offset 1632 bit 9 */
	bool useAuxSpeedForSlipRatio : 1 {};
	/**
	 * VSS and Aux Speed 1 or Aux Speed 1 with Aux Speed 2?
	offset 1632 bit 10 */
	bool useVssAsSecondWheelSpeed : 1 {};
	/**
	offset 1632 bit 11 */
	bool is_enabled_spi_5 : 1 {};
	/**
	offset 1632 bit 12 */
	bool is_enabled_spi_6 : 1 {};
	/**
	 * AEM X-Series EGT gauge kit or rusEFI EGT sensor from Wideband controller
	offset 1632 bit 13 */
	bool enableAemXSeriesEgt : 1 {};
	/**
	offset 1632 bit 14 */
	bool startRequestPinInverted : 1 {};
	/**
	offset 1632 bit 15 */
	bool tcu_rangeSensorPulldown : 1 {};
	/**
	offset 1632 bit 16 */
	bool devBit01 : 1 {};
	/**
	offset 1632 bit 17 */
	bool devBit0 : 1 {};
	/**
	offset 1632 bit 18 */
	bool devBit1 : 1 {};
	/**
	offset 1632 bit 19 */
	bool devBit2 : 1 {};
	/**
	offset 1632 bit 20 */
	bool devBit3 : 1 {};
	/**
	offset 1632 bit 21 */
	bool devBit4 : 1 {};
	/**
	offset 1632 bit 22 */
	bool devBit5 : 1 {};
	/**
	offset 1632 bit 23 */
	bool devBit6 : 1 {};
	/**
	offset 1632 bit 24 */
	bool devBit7 : 1 {};
	/**
	offset 1632 bit 25 */
	bool invertExhaustCamVVTSignal : 1 {};
	/**
	 * "Available via TS Plugin see https://rusefi.com/s/knock"
	offset 1632 bit 26 */
	bool enableKnockSpectrogram : 1 {};
	/**
	offset 1632 bit 27 */
	bool enableKnockSpectrogramFilter : 1 {};
	/**
	offset 1632 bit 28 */
	bool unusedBit_598_28 : 1 {};
	/**
	offset 1632 bit 29 */
	bool unusedBit_598_29 : 1 {};
	/**
	offset 1632 bit 30 */
	bool unusedBit_598_30 : 1 {};
	/**
	offset 1632 bit 31 */
	bool unusedBit_598_31 : 1 {};
	/**
	 * This value is an added for base idle value. Idle Value added when coasting and transitioning into idle.
	 * units: percent
	 * offset 1636
	 */
	int16_t iacByTpsTaper;
	/**
	 * offset 1638
	 */
	Gpio accelerometerCsPin;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: SPECIAL_CASE_SPEED
	 * offset 1640
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: SPECIAL_CASE_SPEED
	 * offset 1641
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1642
	 */
	uint8_t alignmentFill_at_1642[2] = {};
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	 * units: roc
	 * offset 1644
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1648
	 */
	brain_input_pin_e auxSpeedSensorInputPin[AUX_SPEED_SENSOR_COUNT] = {};
	/**
	 * offset 1652
	 */
	uint8_t totalGearsCount;
	/**
	 * Defines when fuel is injected relative to the intake valve opening. Options include End of Injection or other timing references.
	 * offset 1653
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1654
	 */
	debug_mode_e debugMode;
	/**
	 * Additional idle % when fan #1 is active
	 * units: %
	 * offset 1655
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * Band rate for primary TTL
	 * units: BPs
	 * offset 1656
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * units: volts
	 * offset 1660
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	 * units: volts
	 * offset 1664
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * offset 1668
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1669
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1670
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1671
	 */
	load_override_e afrOverrideMode;
	/**
	 * units: A
	 * offset 1672
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	 * units: A
	 * offset 1673
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	 * units: us
	 * offset 1674
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	 * units: ms
	 * offset 1675
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1676 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Allow OpenBLT on Primary CAN
	offset 1676 bit 1 */
	bool canOpenBLT : 1 {};
	/**
	 * Allow OpenBLT on Secondary CAN
	offset 1676 bit 2 */
	bool can2OpenBLT : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (default, cc/min) or mass flow (g/s).
	offset 1676 bit 3 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1676 bit 4 */
	bool boardUseCanTerminator : 1 {};
	/**
	offset 1676 bit 5 */
	bool kLineDoHondaSend : 1 {};
	/**
	 * ListenMode is about acknowledging CAN traffic on the protocol level. Different from canWriteEnabled
	offset 1676 bit 6 */
	bool can1ListenMode : 1 {};
	/**
	offset 1676 bit 7 */
	bool can2ListenMode : 1 {};
	/**
	offset 1676 bit 8 */
	bool unusedBit_632_8 : 1 {};
	/**
	offset 1676 bit 9 */
	bool unusedBit_632_9 : 1 {};
	/**
	offset 1676 bit 10 */
	bool unusedBit_632_10 : 1 {};
	/**
	offset 1676 bit 11 */
	bool unusedBit_632_11 : 1 {};
	/**
	offset 1676 bit 12 */
	bool unusedBit_632_12 : 1 {};
	/**
	offset 1676 bit 13 */
	bool unusedBit_632_13 : 1 {};
	/**
	offset 1676 bit 14 */
	bool unusedBit_632_14 : 1 {};
	/**
	offset 1676 bit 15 */
	bool unusedBit_632_15 : 1 {};
	/**
	offset 1676 bit 16 */
	bool unusedBit_632_16 : 1 {};
	/**
	offset 1676 bit 17 */
	bool unusedBit_632_17 : 1 {};
	/**
	offset 1676 bit 18 */
	bool unusedBit_632_18 : 1 {};
	/**
	offset 1676 bit 19 */
	bool unusedBit_632_19 : 1 {};
	/**
	offset 1676 bit 20 */
	bool unusedBit_632_20 : 1 {};
	/**
	offset 1676 bit 21 */
	bool unusedBit_632_21 : 1 {};
	/**
	offset 1676 bit 22 */
	bool unusedBit_632_22 : 1 {};
	/**
	offset 1676 bit 23 */
	bool unusedBit_632_23 : 1 {};
	/**
	offset 1676 bit 24 */
	bool unusedBit_632_24 : 1 {};
	/**
	offset 1676 bit 25 */
	bool unusedBit_632_25 : 1 {};
	/**
	offset 1676 bit 26 */
	bool unusedBit_632_26 : 1 {};
	/**
	offset 1676 bit 27 */
	bool unusedBit_632_27 : 1 {};
	/**
	offset 1676 bit 28 */
	bool unusedBit_632_28 : 1 {};
	/**
	offset 1676 bit 29 */
	bool unusedBit_632_29 : 1 {};
	/**
	offset 1676 bit 30 */
	bool unusedBit_632_30 : 1 {};
	/**
	offset 1676 bit 31 */
	bool unusedBit_632_31 : 1 {};
	/**
	 * Angle of tooth detection within engine phase cycle
	 * units: angle
	 * offset 1680
	 */
	uint16_t camDecoder2jzPosition;
	/**
	 * offset 1682
	 */
	mc33810maxDwellTimer_e mc33810maxDwellTimer;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1683
	 */
	uint8_t alignmentFill_at_1683[1] = {};
	/**
	 * Duration of each test pulse
	 * units: ms
	 * offset 1684
	 */
	scaled_channel<uint16_t, 100, 1> benchTestOnTime;
	/**
	 * units: %
	 * offset 1686
	 */
	uint8_t lambdaProtectionRestoreTps;
	/**
	 * units: %
	 * offset 1687
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionRestoreLoad;
	/**
	 * offset 1688
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1689
	 */
	uint8_t alignmentFill_at_1689[1] = {};
	/**
	 * offset 1690
	 */
	Gpio can2TxPin;
	/**
	 * offset 1692
	 */
	Gpio can2RxPin;
	/**
	 * offset 1694
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1695
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1696
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1697
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1698
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1700
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1701
	 */
	uint8_t alignmentFill_at_1701[3] = {};
	/**
	 * Specifies the boost pressure allowed before triggering a cut. Setting this to 0 will DISABLE overboost cut.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 1704
	 */
	float boostCutPressure;
	/**
	 * units: kg/h
	 * offset 1708
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16] = {};
	/**
	 * units: ratio
	 * offset 1724
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16] = {};
	/**
	 * Fixed timing, useful for TDC testing
	 * units: deg
	 * offset 1740
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1744
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1748
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * units: value
	 * offset 1752
	 */
	float egoValueShift;
	/**
	 * VVT output solenoid pin for this cam
	 * offset 1756
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1764
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMinTps;
	/**
	 * offset 1765
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMaxTps;
	/**
	 * offset 1766
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMinTps;
	/**
	 * offset 1767
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMaxTps;
	/**
	 * offset 1768
	 */
	pwm_freq_t vvtOutputFrequency;
	/**
	 * Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.
	 * units: deg BTDC
	 * offset 1770
	 */
	int8_t minimumIgnitionTiming;
	/**
	 * Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC
	 * units: deg BTDC
	 * offset 1771
	 */
	int8_t maximumIgnitionTiming;
	/**
	 * units: Hz
	 * offset 1772
	 */
	int alternatorPwmFrequency;
	/**
	 * offset 1776
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK] = {};
	/**
	 * Additional idle % when fan #2 is active
	 * units: %
	 * offset 1778
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	 * units: sec
	 * offset 1779
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1780
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT] = {};
	/**
	 * offset 1788
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 1812
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1813
	 */
	pin_output_mode_e accelerometerCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 *  HPFP fuel mass compensation = manual mode for GDI engines
	 * offset 1814
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1815
	 */
	uint8_t alignmentFill_at_1815[1] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa/50.7psi here.
	 * This is gauge pressure/in reference to atmospheric.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 1816
	 */
	float fuelReferencePressure;
	/**
	 * offset 1820
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1852
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * units: Deg
	 * offset 1884
	 */
	int16_t knockSamplingDuration;
	/**
	 * units: Hz
	 * offset 1886
	 */
	int16_t etbFreq;
	/**
	 * offset 1888
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 1908
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	 * units: %
	 * offset 1909
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	 * units: %
	 * offset 1910
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 1911
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder ignition and fueling timing correction for uneven engines
	 * units: deg
	 * offset 1912
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: seconds
	 * offset 1960
	 */
	float idlePidActivationTime;
	/**
	 * Minimum coolant temperature to activate VVT
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 1964
	 */
	int16_t vvtControlMinClt;
	/**
	 * offset 1966
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 1967
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 1968
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 1969
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 1970
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 1971
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 1972
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 1973
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 1974
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 1975
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 1976
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 1978
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 1980
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1982
	 */
	uint8_t alignmentFill_at_1982[2] = {};
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 1984
	 */
	pid_s auxPid[CAMS_PER_BANK] = {};
	/**
	 * offset 2024
	 */
	float injectorCorrectionPolynomial[8] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 2056
	 */
	scaled_channel<int16_t, 1, 1> primeBins[PRIME_CURVE_COUNT] = {};
	/**
	 * offset 2072
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2092
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2093
	 */
	uint8_t alignmentFill_at_2093[1] = {};
	/**
	 * offset 2094
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2096
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2098
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2099
	 */
	uint8_t alignmentFill_at_2099[1] = {};
	/**
	 * offset 2100
	 */
	Gpio mc33816_cs;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2102
	 */
	uint8_t alignmentFill_at_2102[2] = {};
	/**
	 * units: hz
	 * offset 2104
	 */
	float auxFrequencyFilter;
	/**
	 * offset 2108
	 */
	sent_input_pin_e sentInputPins[SENT_INPUT_COUNT] = {};
	/**
	 * This sets the RPM above which fuel cut is active.
	 * units: rpm
	 * offset 2110
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	 * units: rpm
	 * offset 2112
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	 * units: %
	 * offset 2114
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 2116
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * units: %
	 * offset 2118
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 2120
	 */
	int16_t coastingFuelCutMap;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2122
	 */
	uint8_t alignmentFill_at_2122[2] = {};
	/**
	 * offset 2124
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2144
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * offset 2164
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT] = {};
	/**
	 * offset 2260
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT] = {};
	/**
	 * offset 2324
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT] = {};
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2452
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2456
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	 * units: kg/h
	 * offset 2460
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2464
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2468
	 */
	float tChargeAirDecrLimit;
	/**
	 * iTerm min value
	 * offset 2472
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2474
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2476
	 */
	pid_s idleTimingPid;
	/**
	 * When entering idle, and the PID settings are aggressive, it's good to make a soft entry upon entering closed loop
	 * offset 2496
	 */
	float idleTimingSoftEntryTime;
	/**
	 * offset 2500
	 */
	pin_input_mode_e torqueReductionTriggerPinMode;
	/**
	 * offset 2501
	 */
	torqueReductionActivationMode_e torqueReductionActivationMode;
	/**
	 * A delay in cycles between fuel-enrich. portions
	 * units: cycles
	 * offset 2502
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	 * units: coef
	 * offset 2504
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2508
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2509
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2510
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2512
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	 * units: :1
	 * offset 2513
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2514
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position. Can also be interpreted as the maximum allowed TPS% Opening for Idle Control.
	 * units: %
	 * offset 2516
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2520
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: mg
	 * offset 2532
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[PRIME_CURVE_COUNT] = {};
	/**
	 * Trigger comparator center point voltage
	 * units: V
	 * offset 2540
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	 * units: V
	 * offset 2541
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	 * units: V
	 * offset 2542
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	 * units: RPM
	 * offset 2543
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * units: ratio
	 * offset 2544
	 */
	scaled_channel<uint16_t, 100, 1> tractionControlSlipBins[TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * units: RPM
	 * offset 2556
	 */
	uint8_t tractionControlSpeedBins[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE] = {};
	/**
	 * offset 2562
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2563
	 */
	uint8_t alignmentFill_at_2563[1] = {};
	/**
	 * offset 2564
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS] = {};
	/**
	 * Boost Current
	 * units: mA
	 * offset 2996
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	 * units: mA
	 * offset 2998
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	 * units: mA
	 * offset 3000
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	 * units: us
	 * offset 3002
	 */
	uint16_t mc33_t_max_boost;
	/**
	 * units: us
	 * offset 3004
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	 * units: us
	 * offset 3006
	 */
	uint16_t mc33_t_peak_tot;
	/**
	 * units: us
	 * offset 3008
	 */
	uint16_t mc33_t_bypass;
	/**
	 * units: us
	 * offset 3010
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	 * units: us
	 * offset 3012
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 3014
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 3015
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 3016
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 3017
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3023
	 */
	uint8_t alignmentFill_at_3023[1] = {};
	/**
	 * units: ratio
	 * offset 3024
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3096
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH] = {};
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	 * units: rpm
	 * offset 3168
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	 * units: sec
	 * offset 3169
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	 * units: sec
	 * offset 3170
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * offset 3171
	 */
	tChargeMode_e tChargeMode;
	/**
	 * units: mg
	 * offset 3172
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	 * Threshold in ETB error (target vs. actual) above which the jam timer is started. If the timer reaches the time specified in the jam detection timeout period, the throttle is considered jammed, and engine operation limited.
	 * units: %
	 * offset 3174
	 */
	uint8_t etbJamDetectThreshold;
	/**
	 * units: lobes/cam
	 * offset 3175
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 3176
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Low engine speed for A/C. Larger engines can survive lower values
	 * units: RPM
	 * offset 3177
	 */
	scaled_channel<int8_t, 1, 10> acLowRpmLimit;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	 * units: deg
	 * offset 3178
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3179
	 */
	uint8_t alignmentFill_at_3179[1] = {};
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	 * units: cc
	 * offset 3180
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	 * https://rusefi.com/forum/viewtopic.php?t=2192
	 * units: deg
	 * offset 3182
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 3183
	 */
	uint8_t issFilterReciprocal;
	/**
	 * units: %/kPa
	 * offset 3184
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	 * units: %/kPa/lobe
	 * offset 3186
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * iTerm min value
	 * offset 3188
	 */
	int16_t hpfpPid_iTermMin;
	/**
	 * iTerm max value
	 * offset 3190
	 */
	int16_t hpfpPid_iTermMax;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	 * units: kPa/s
	 * offset 3192
	 */
	uint16_t hpfpTargetDecay;
	/**
	 * offset 3194
	 */
	output_pin_e stepper_raw_output[4] = {};
	/**
	 * units: ratio
	 * offset 3202
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[TCU_GEAR_COUNT] = {};
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	 * units: ms
	 * offset 3222
	 */
	uint16_t vvtActivationDelayMs;
	/**
	 * offset 3224
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3225
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * During revolution where ACR should be disabled at what specific angle to disengage
	 * units: deg
	 * offset 3226
	 */
	uint16_t acrDisablePhase;
	/**
	 * offset 3228
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3248
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 3268
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 3270
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3271
	 */
	uint8_t alignmentFill_at_3271[1] = {};
	/**
	 * offset 3272
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 3274
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3275
	 */
	uint8_t alignmentFill_at_3275[1] = {};
	/**
	 * offset 3276
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 3278
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 3280
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3281
	 */
	uint8_t alignmentFill_at_3281[1] = {};
	/**
	 * offset 3282
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 3284
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 3286
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3287
	 */
	uint8_t alignmentFill_at_3287[1] = {};
	/**
	 * offset 3288
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * offset 3290
	 */
	output_pin_e acrPin2;
	/**
	 * Set a minimum allowed target position to avoid slamming/driving against the hard mechanical stop in the throttle.
	 * units: %
	 * offset 3292
	 */
	scaled_channel<uint8_t, 10, 1> etbMinimumPosition;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3293
	 */
	uint8_t alignmentFill_at_3293[1] = {};
	/**
	 * offset 3294
	 */
	uint16_t tuneHidingKey;
	/**
	 * Individual characters are accessible using vin(index) Lua function
	 * offset 3296
	 */
	vin_number_t vinNumber;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 3313
	 */
	int8_t torqueReductionActivationTemperature;
	/**
	 * offset 3314
	 */
	fuel_pressure_sensor_mode_e fuelPressureSensorMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3315
	 */
	uint8_t alignmentFill_at_3315[1] = {};
	/**
	 * offset 3316
	 */
	switch_input_pin_e luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * units: rpm
	 * offset 3332
	 */
	int16_t ALSMinRPM;
	/**
	 * units: rpm
	 * offset 3334
	 */
	int16_t ALSMaxRPM;
	/**
	 * units: sec
	 * offset 3336
	 */
	int16_t ALSMaxDuration;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 3338
	 */
	int8_t ALSMinCLT;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 3339
	 */
	int8_t ALSMaxCLT;
	/**
	 * offset 3340
	 */
	uint8_t alsMinTimeBetween;
	/**
	 * offset 3341
	 */
	uint8_t alsEtbPosition;
	/**
	 * units: %
	 * offset 3342
	 */
	uint8_t acRelayAlternatorDutyAdder;
	/**
	 * If you have digital SENT TPS sensor please select type. For analog TPS leave None
	 * offset 3343
	 */
	SentEtbType sentEtbType;
	/**
	 * offset 3344
	 */
	uint16_t customSentTpsMin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3346
	 */
	uint8_t alignmentFill_at_3346[2] = {};
	/**
	 * units: %
	 * offset 3348
	 */
	int ALSIdleAdd;
	/**
	 * units: %
	 * offset 3352
	 */
	int ALSEtbAdd;
	/**
	 * offset 3356
	 */
	float ALSSkipRatio;
	/**
	 * Hysterisis: if Pressure High Disable is 240kpa, and acPressureEnableHyst is 20, when the ECU sees 240kpa, A/C will be disabled, and stay disabled until 240-20=220kpa is reached
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3360
	 */
	scaled_channel<uint8_t, 2, 1> acPressureEnableHyst;
	/**
	 * offset 3361
	 */
	pin_input_mode_e ALSActivatePinMode;
	/**
	 * For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	 * units: %
	 * offset 3362
	 */
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	/**
	 * For Toyota ETCS-i, use ~69%
	 * units: %
	 * offset 3363
	 */
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	/**
	 * offset 3364
	 */
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * offset 3372
	 */
	uint16_t customSentTpsMax;
	/**
	 * offset 3374
	 */
	uint16_t kLineBaudRate;
	/**
	 * offset 3376
	 */
	CanGpioType canGpioType;
	/**
	 * offset 3377
	 */
	UiMode uiMode;
	/**
	 * Crank angle ATDC of first lobe peak
	 * units: deg
	 * offset 3378
	 */
	int16_t hpfpPeakPos;
	/**
	 * units: us
	 * offset 3380
	 */
	int16_t kLinePeriodUs;
	/**
	 * Degrees of timing REMOVED from actual timing during soft RPM limit window
	 * units: deg
	 * offset 3382
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitTimingRetard;
	/**
	 * % of fuel ADDED during window
	 * units: %
	 * offset 3383
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitFuelAdded;
	/**
	 * Sets a buffer below the RPM hard limit, helping avoid rapid cycling of cut actions by defining a range within which RPM must drop before cut actions are re-enabled.
	 * Hysterisis: if the hard limit is 7200rpm and rpmHardLimitHyst is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached
	 * units: RPM
	 * offset 3384
	 */
	scaled_channel<uint8_t, 1, 10> rpmHardLimitHyst;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3385
	 */
	uint8_t alignmentFill_at_3385[1] = {};
	/**
	 * Time between bench test pulses
	 * units: ms
	 * offset 3386
	 */
	scaled_channel<uint16_t, 10, 1> benchTestOffTime;
	/**
	 * Defines a pressure range below the cut limit at which boost can resume, providing smoother control over boost cut actions.
	 * For example: if hard cut is 240kpa, and boost cut hysteresis is 20, when the ECU sees 240kpa, fuel/ign will cut, and stay cut until 240-20=220kpa is reached
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3388
	 */
	scaled_channel<uint8_t, 2, 1> boostCutPressureHyst;
	/**
	 * Boost duty cycle modified by gear
	 * units: %
	 * offset 3389
	 */
	scaled_channel<int8_t, 2, 1> gearBasedOpenLoopBoostAdder[TCU_GEAR_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3399
	 */
	uint8_t alignmentFill_at_3399[1] = {};
	/**
	 * How many test bench pulses do you want
	 * offset 3400
	 */
	uint32_t benchTestCount;
	/**
	 * How long initial idle adder is held before starting to decay.
	 * units: seconds
	 * offset 3404
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsHoldTime;
	/**
	 * How long it takes to remove initial IAC adder to return to normal idle.
	 * units: seconds
	 * offset 3405
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsDecayTime;
	/**
	 * offset 3406
	 */
	switch_input_pin_e tcu_rangeInput[RANGE_INPUT_COUNT] = {};
	/**
	 * offset 3418
	 */
	pin_input_mode_e tcu_rangeInputMode[RANGE_INPUT_COUNT] = {};
	/**
	 * Scale the reported vehicle speed value from CAN. Example: Parameter set to 1.1, CAN VSS reports 50kph, ECU will report 55kph instead.
	 * units: ratio
	 * offset 3424
	 */
	scaled_channel<uint16_t, 10000, 1> canVssScaling;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3426
	 */
	uint8_t alignmentFill_at_3426[2] = {};
	/**
	 * offset 3428
	 */
	ThermistorConf oilTempSensor;
	/**
	 * offset 3460
	 */
	ThermistorConf fuelTempSensor;
	/**
	 * offset 3492
	 */
	ThermistorConf ambientTempSensor;
	/**
	 * offset 3524
	 */
	ThermistorConf compressorDischargeTemperature;
	/**
	 * Place the sensor before the throttle, but after any turbocharger/supercharger and intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3556
	 */
	adc_channel_e throttleInletPressureChannel;
	/**
	 * Place the sensor after the turbocharger/supercharger, but before any intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3557
	 */
	adc_channel_e compressorDischargePressureChannel;
	/**
	 * offset 3558
	 */
	Gpio dacOutputPins[DAC_OUTPUT_COUNT] = {};
	/**
	 * offset 3562
	 */
	output_pin_e speedometerOutputPin;
	/**
	 * Number of speedometer pulses per kilometer travelled.
	 * offset 3564
	 */
	uint16_t speedometerPulsePerKm;
	/**
	 * offset 3566
	 */
	uint8_t simulatorCamPosition[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 3570
	 */
	adc_channel_e ignKeyAdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3571
	 */
	uint8_t alignmentFill_at_3571[1] = {};
	/**
	 * offset 3572
	 */
	float ignKeyAdcDivider;
	/**
	 * offset 3576
	 */
	pin_mode_e spi6MisoMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3577
	 */
	uint8_t alignmentFill_at_3577[3] = {};
	/**
	 * units: ratio
	 * offset 3580
	 */
	float triggerVVTGapOverrideFrom[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3596
	 */
	float triggerVVTGapOverrideTo[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: %
	 * offset 3612
	 */
	int8_t tractionControlEtbDrop[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * This sets an immediate limit on injector duty cycle. If this threshold is reached, the system will immediately cut the injectors.
	 * units: %
	 * offset 3648
	 */
	uint8_t maxInjectorDutyInstant;
	/**
	 * This limit allows injectors to operate up to the specified duty cycle percentage for a short period (as defined by the delay). After this delay, if the duty cycle remains above the limit, it will trigger a cut.
	 * units: %
	 * offset 3649
	 */
	uint8_t maxInjectorDutySustained;
	/**
	 * Timeout period for duty cycle over the sustained limit to trigger duty cycle protection.
	 * units: sec
	 * offset 3650
	 */
	scaled_channel<uint8_t, 10, 1> maxInjectorDutySustainedTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3651
	 */
	uint8_t alignmentFill_at_3651[1] = {};
	/**
	 * offset 3652
	 */
	output_pin_e injectionPinsStage2[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: Deg
	 * offset 3676
	 */
	int8_t tractionControlTimingDrop[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * units: %
	 * offset 3712
	 */
	int8_t tractionControlIgnitionSkip[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * offset 3748
	 */
	float auxSpeed1Multiplier;
	/**
	 * offset 3752
	 */
	float brakeMeanEffectivePressureDifferential;
	/**
	 * offset 3756
	 */
	Gpio spi4mosiPin;
	/**
	 * offset 3758
	 */
	Gpio spi4misoPin;
	/**
	 * offset 3760
	 */
	Gpio spi4sckPin;
	/**
	 * offset 3762
	 */
	Gpio spi5mosiPin;
	/**
	 * offset 3764
	 */
	Gpio spi5misoPin;
	/**
	 * offset 3766
	 */
	Gpio spi5sckPin;
	/**
	 * offset 3768
	 */
	Gpio spi6mosiPin;
	/**
	 * offset 3770
	 */
	Gpio spi6misoPin;
	/**
	 * offset 3772
	 */
	Gpio spi6sckPin;
	/**
	 * offset 3774
	 */
	pin_mode_e spi4SckMode;
	/**
	 * offset 3775
	 */
	pin_mode_e spi4MosiMode;
	/**
	 * offset 3776
	 */
	pin_mode_e spi4MisoMode;
	/**
	 * offset 3777
	 */
	pin_mode_e spi5SckMode;
	/**
	 * offset 3778
	 */
	pin_mode_e spi5MosiMode;
	/**
	 * offset 3779
	 */
	pin_mode_e spi5MisoMode;
	/**
	 * offset 3780
	 */
	pin_mode_e spi6SckMode;
	/**
	 * offset 3781
	 */
	pin_mode_e spi6MosiMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3782
	 */
	uint8_t alignmentFill_at_3782[2] = {};
	/**
	 * Secondary TTL channel baud rate
	 * units: BPs
	 * offset 3784
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 3788
	 */
	Gpio camSimulatorPin;
	/**
	 * offset 3790
	 */
	pin_output_mode_e camSimulatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3791
	 */
	uint8_t alignmentFill_at_3791[1] = {};
	/**
	 * offset 3792
	 */
	int anotherCiTest;
	/**
	 * offset 3796
	 */
	uint32_t device_uid[3] = {};
	/**
	 * offset 3808
	 */
	adc_channel_e tcu_rangeAnalogInput[RANGE_INPUT_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3814
	 */
	uint8_t alignmentFill_at_3814[2] = {};
	/**
	 * units: Ohm
	 * offset 3816
	 */
	float tcu_rangeSensorBiasResistor;
	/**
	 * offset 3820
	 */
	MsIoBox_config_s msIoBox0;
	/**
	 * Nominal coil charge current, 0.25A step
	 * units: A
	 * offset 3824
	 */
	scaled_channel<uint8_t, 4, 1> mc33810Nomi;
	/**
	 * Maximum coil charge current, 1A step
	 * units: A
	 * offset 3825
	 */
	uint8_t mc33810Maxi;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3826
	 */
	uint8_t alignmentFill_at_3826[2] = {};
	/**
	 * offset 3828
	 */
	linear_sensor_s acPressure;
	/**
	 * value of A/C pressure in kPa/psi before that compressor is disengaged
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3848
	 */
	uint16_t minAcPressure;
	/**
	 * value of A/C pressure in kPa/psi after that compressor is disengaged
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3850
	 */
	uint16_t maxAcPressure;
	/**
	 * Delay before cutting fuel due to low oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 3852
	 */
	scaled_channel<uint8_t, 10, 1> minimumOilPressureTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3853
	 */
	uint8_t alignmentFill_at_3853[3] = {};
	/**
	 * offset 3856
	 */
	linear_sensor_s auxLinear3;
	/**
	 * offset 3876
	 */
	linear_sensor_s auxLinear4;
	/**
	 * Below TPS value all knock suppression will be disabled.
	 * units: %
	 * offset 3896
	 */
	scaled_channel<uint8_t, 1, 1> knockSuppressMinTps;
	/**
	 * Fuel to odd when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 3897
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimAggression;
	/**
	 * After a knock event, reapply fuel at this rate.
	 * units: 1%/s
	 * offset 3898
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimReapplyRate;
	/**
	 * Fuel trim when knock, max 30%
	 * units: %
	 * offset 3899
	 */
	scaled_channel<uint8_t, 1, 1> knockFuelTrim;
	/**
	 * units: sense
	 * offset 3900
	 */
	float knockSpectrumSensitivity;
	/**
	 * "Estimated knock frequency, ignore cylinderBore if this one > 0"
	 * units: Hz
	 * offset 3904
	 */
	float knockFrequency;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 3908
	 */
	injector_compensation_mode_e secondaryInjectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3909
	 */
	uint8_t alignmentFill_at_3909[3] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3912
	 */
	float secondaryInjectorFuelReferencePressure;
	/**
	 * SENT input connected to ETB
	 * offset 3916
	 */
	SentInput EtbSentInput;
	/**
	 * SENT input used for high pressure fuel sensor
	 * offset 3917
	 */
	SentInput FuelHighPressureSentInput;
	/**
	 * If you have SENT High Pressure Fuel Sensor please select type. For analog TPS leave None
	 * offset 3918
	 */
	SentFuelHighPressureType FuelHighPressureSentType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3919
	 */
	uint8_t alignmentFill_at_3919[1] = {};
	/**
	offset 3920 bit 0 */
	bool nitrousControlEnabled : 1 {};
	/**
	offset 3920 bit 1 */
	bool vvlControlEnabled : 1 {};
	/**
	offset 3920 bit 2 */
	bool unusedFancy3 : 1 {};
	/**
	offset 3920 bit 3 */
	bool unusedFancy4 : 1 {};
	/**
	offset 3920 bit 4 */
	bool unusedFancy5 : 1 {};
	/**
	offset 3920 bit 5 */
	bool unusedFancy6 : 1 {};
	/**
	offset 3920 bit 6 */
	bool unusedFancy7 : 1 {};
	/**
	offset 3920 bit 7 */
	bool unusedFancy8 : 1 {};
	/**
	offset 3920 bit 8 */
	bool unusedFancy9 : 1 {};
	/**
	offset 3920 bit 9 */
	bool unusedFancy10 : 1 {};
	/**
	offset 3920 bit 10 */
	bool unusedFancy11 : 1 {};
	/**
	offset 3920 bit 11 */
	bool unusedFancy12 : 1 {};
	/**
	offset 3920 bit 12 */
	bool unusedFancy13 : 1 {};
	/**
	offset 3920 bit 13 */
	bool unusedFancy14 : 1 {};
	/**
	offset 3920 bit 14 */
	bool unusedFancy15 : 1 {};
	/**
	offset 3920 bit 15 */
	bool unusedFancy16 : 1 {};
	/**
	offset 3920 bit 16 */
	bool unusedFancy17 : 1 {};
	/**
	offset 3920 bit 17 */
	bool unusedFancy18 : 1 {};
	/**
	offset 3920 bit 18 */
	bool unusedFancy19 : 1 {};
	/**
	offset 3920 bit 19 */
	bool unusedFancy20 : 1 {};
	/**
	offset 3920 bit 20 */
	bool unusedFancy21 : 1 {};
	/**
	offset 3920 bit 21 */
	bool unusedFancy22 : 1 {};
	/**
	offset 3920 bit 22 */
	bool unusedFancy23 : 1 {};
	/**
	offset 3920 bit 23 */
	bool unusedFancy24 : 1 {};
	/**
	offset 3920 bit 24 */
	bool unusedFancy25 : 1 {};
	/**
	offset 3920 bit 25 */
	bool unusedFancy26 : 1 {};
	/**
	offset 3920 bit 26 */
	bool unusedFancy27 : 1 {};
	/**
	offset 3920 bit 27 */
	bool unusedFancy28 : 1 {};
	/**
	offset 3920 bit 28 */
	bool unusedFancy29 : 1 {};
	/**
	offset 3920 bit 29 */
	bool unusedFancy30 : 1 {};
	/**
	offset 3920 bit 30 */
	bool unusedFancy31 : 1 {};
	/**
	offset 3920 bit 31 */
	bool unusedFancy32 : 1 {};
	/**
	 * offset 3924
	 */
	nitrous_arming_method_e nitrousControlArmingMethod;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3925
	 */
	uint8_t alignmentFill_at_3925[1] = {};
	/**
	 * Pin that activates nitrous control
	 * offset 3926
	 */
	switch_input_pin_e nitrousControlTriggerPin;
	/**
	 * offset 3928
	 */
	pin_input_mode_e nitrousControlTriggerPinMode;
	/**
	 * offset 3929
	 */
	lua_gauge_e nitrousLuaGauge;
	/**
	 * offset 3930
	 */
	lua_gauge_meaning_e nitrousLuaGaugeMeaning;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3931
	 */
	uint8_t alignmentFill_at_3931[1] = {};
	/**
	 * offset 3932
	 */
	float nitrousLuaGaugeArmingValue;
	/**
	 * offset 3936
	 */
	int nitrousMinimumTps;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 3940
	 */
	int16_t nitrousMinimumClt;
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3942
	 */
	int16_t nitrousMaximumMap;
	/**
	 * units: afr
	 * offset 3944
	 */
	scaled_channel<uint8_t, 10, 1> nitrousMaximumAfr;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3945
	 */
	uint8_t alignmentFill_at_3945[1] = {};
	/**
	 * units: rpm
	 * offset 3946
	 */
	uint16_t nitrousActivationRpm;
	/**
	 * units: rpm
	 * offset 3948
	 */
	uint16_t nitrousDeactivationRpm;
	/**
	 * units: rpm
	 * offset 3950
	 */
	uint16_t nitrousDeactivationRpmWindow;
	/**
	 * Retard timing by this amount during DFCO. Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: deg
	 * offset 3952
	 */
	uint8_t dfcoRetardDeg;
	/**
	 * Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: s
	 * offset 3953
	 */
	scaled_channel<uint8_t, 10, 1> dfcoRetardRampInTime;
	/**
	 * offset 3954
	 */
	output_pin_e nitrousRelayPin;
	/**
	 * offset 3956
	 */
	pin_output_mode_e nitrousRelayPinMode;
	/**
	 * units: %
	 * offset 3957
	 */
	int8_t nitrousFuelAdderPercent;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3958
	 */
	uint8_t alignmentFill_at_3958[2] = {};
	/**
	 * Retard timing to remove from actual final timing (after all corrections) due to additional air.
	 * units: deg
	 * offset 3960
	 */
	float nitrousIgnitionRetard;
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 3964
	 */
	uint16_t nitrousMinimumVehicleSpeed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3966
	 */
	uint8_t alignmentFill_at_3966[2] = {};
	/**
	 * Exponential Average Alpha filtering parameter
	 * offset 3968
	 */
	float fuelLevelAveragingAlpha;
	/**
	 * How often do we update fuel level gauge
	 * units: seconds
	 * offset 3972
	 */
	float fuelLevelUpdatePeriodSec;
	/**
	 * Error below specified value
	 * units: v
	 * offset 3976
	 */
	float fuelLevelLowThresholdVoltage;
	/**
	 * Error above specified value
	 * units: v
	 * offset 3980
	 */
	float fuelLevelHighThresholdVoltage;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 3984
	 */
	float afrExpAverageAlpha;
	/**
	 * Compensates for trigger delay due to belt stretch, or other electromechanical issues. beware that raising this value advances ignition timing!
	 * units: uS
	 * offset 3988
	 */
	scaled_channel<uint8_t, 1, 1> sparkHardwareLatencyCorrection;
	/**
	 * Delay before cutting fuel due to extra high oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 3989
	 */
	scaled_channel<uint8_t, 10, 1> maxOilPressureTimeout;
	/**
	 * units: kg/h
	 * offset 3990
	 */
	scaled_channel<uint16_t, 100, 1> idleFlowEstimateFlow[8] = {};
	/**
	 * units: %
	 * offset 4006
	 */
	scaled_channel<uint8_t, 2, 1> idleFlowEstimatePosition[8] = {};
	/**
	 * units: mg
	 * offset 4014
	 */
	int8_t airmassToTimingBins[8] = {};
	/**
	 * units: deg
	 * offset 4022
	 */
	int8_t airmassToTimingValues[8] = {};
	/**
	 * idle return target ramp duration
	 * units: seconds
	 * offset 4030
	 */
	scaled_channel<uint8_t, 10, 1> idleReturnTargetRampDuration;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4031
	 */
	uint8_t alignmentFill_at_4031[1] = {};
	/**
	 * Voltage when the wastegate is fully open
	 * units: v
	 * offset 4032
	 */
	float wastegatePositionOpenedVoltage;
	/**
	 * Voltage when the wastegate is closed
	 * units: v
	 * offset 4036
	 */
	float wastegatePositionClosedVoltage;
	/**
	 * offset 4040
	 */
	wbo_s canWbo[CAN_WBO_COUNT] = {};
	/**
	 * offset 4056
	 */
	output_pin_e vvlRelayPin;
	/**
	 * offset 4058
	 */
	pin_output_mode_e vvlRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4059
	 */
	uint8_t alignmentFill_at_4059[1] = {};
	/**
	 * offset 4060
	 */
	vvl_s vvlController;
	/**
	 * offset 4084
	 */
	rotational_idle_s rotationalIdleController;
};
static_assert(sizeof(engine_configuration_s) == 4120);

// start of ign_cyl_trim_s
struct ign_cyl_trim_s {
	/**
	 * offset 0
	 */
	scaled_channel<int8_t, 5, 1> table[IGN_TRIM_SIZE][IGN_TRIM_SIZE] = {};
};
static_assert(sizeof(ign_cyl_trim_s) == 16);

// start of fuel_cyl_trim_s
struct fuel_cyl_trim_s {
	/**
	 * offset 0
	 */
	scaled_channel<int8_t, 5, 1> table[FUEL_TRIM_SIZE][FUEL_TRIM_SIZE] = {};
};
static_assert(sizeof(fuel_cyl_trim_s) == 16);

// start of blend_table_s
struct blend_table_s {
	/**
	 * offset 0
	 */
	scaled_channel<int16_t, 10, 1> table[BLEND_TABLE_COUNT][BLEND_TABLE_COUNT] = {};
	/**
	 * units: Load
	 * offset 128
	 */
	uint16_t loadBins[BLEND_TABLE_COUNT] = {};
	/**
	 * units: RPM
	 * offset 144
	 */
	uint16_t rpmBins[BLEND_TABLE_COUNT] = {};
	/**
	 * offset 160
	 */
	gppwm_channel_e blendParameter;
	/**
	 * offset 161
	 */
	gppwm_channel_e yAxisOverride;
	/**
	 * offset 162
	 */
	scaled_channel<int16_t, 10, 1> blendBins[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: %
	 * offset 178
	 */
	scaled_channel<uint8_t, 2, 1> blendValues[BLEND_FACTOR_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 186
	 */
	uint8_t alignmentFill_at_186[2] = {};
};
static_assert(sizeof(blend_table_s) == 188);

// start of persistent_config_s
struct persistent_config_s {
	/**
	 * offset 0
	 */
	engine_configuration_s engineConfiguration;
	/**
	 * offset 4120
	 */
	float tmfTable[TMF_RATIO_SIZE][TMF_SIZE] = {};
	/**
	 * offset 4136
	 */
	float tmfRatioBins[TMF_RATIO_SIZE] = {};
	/**
	 * offset 4144
	 */
	float tmfOpeningBins[TMF_SIZE] = {};
	/**
	 * units: Nm
	 * offset 4152
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_RPM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 4188
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 4200
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE] = {};
	/**
	 * units: mult
	 * offset 4212
	 */
	float postCrankingFactor[CRANKING_ENRICH_CLT_COUNT][CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: count
	 * offset 4356
	 */
	uint16_t postCrankingDurationBins[CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 4368
	 */
	int16_t postCrankingCLTBins[CRANKING_ENRICH_CLT_COUNT] = {};
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	 * units: target TPS position
	 * offset 4380
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: ETB duty cycle bias
	 * offset 4412
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * target Wastegate value, 0 to 100%
	 * units: target DC position
	 * offset 4444
	 */
	int8_t dcWastegateBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: DC wastegate duty cycle bias
	 * offset 4452
	 */
	scaled_channel<int16_t, 100, 1> dcWastegateBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * units: %
	 * offset 4468
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE] = {};
	/**
	 * units: Load
	 * offset 4532
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE] = {};
	/**
	 * units: RPM
	 * offset 4540
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_RPM_SIZE] = {};
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	 * units: RPM
	 * offset 4548
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: ms
	 * offset 4564
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE] = {};
	/**
	 * CLT-based target RPM for automatic idle controller
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 4580
	 */
	scaled_channel<int16_t, 1, 1> cltIdleRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * See idleRpmPid
	 * units: RPM
	 * offset 4612
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE] = {};
	/**
	 * units: deg
	 * offset 4628
	 */
	scaled_channel<int16_t, 10, 1> ignitionCltCorrTable[CLT_TIMING_LOAD_AXIS_SIZE][CLT_TIMING_TEMP_AXIS_SIZE] = {};
	/**
	 * CLT-based timing correction
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 4678
	 */
	scaled_channel<int16_t, 1, 1> ignitionCltCorrTempBins[CLT_TIMING_TEMP_AXIS_SIZE] = {};
	/**
	 * units: Load
	 * offset 4688
	 */
	scaled_channel<uint8_t, 1, 5> ignitionCltCorrLoadBins[CLT_TIMING_LOAD_AXIS_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4693
	 */
	uint8_t alignmentFill_at_4693[3] = {};
	/**
	 * units: x
	 * offset 4696
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 4760
	 */
	float scriptCurve1[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 4824
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 4888
	 */
	float scriptCurve2[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 4952
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 4984
	 */
	float scriptCurve3[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 5016
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5048
	 */
	float scriptCurve4[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 5080
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5112
	 */
	float scriptCurve5[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 5144
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5176
	 */
	float scriptCurve6[SCRIPT_CURVE_8] = {};
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 5208
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5224
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE] = {};
	/**
	 * units: ratio
	 * offset 5240
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE] = {};
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * units: Ratio
	 * offset 5304
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 5336
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: RPM
	 * offset 5368
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: deg
	 * offset 5376
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: RPM
	 * offset 5384
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: %
	 * offset 5400
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE] = {};
	/**
	 * offset 5416
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * units: RPM
	 * offset 5480
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT] = {};
	/**
	 * offset 5488
	 */
	uint16_t boostOpenLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * offset 5504
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * offset 5568
	 */
	uint16_t boostClosedLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * units: %
	 * offset 5584
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 5648
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5656
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_RPM_SIZE] = {};
	/**
	 * CLT-based cranking position %. The values in this curve represent a percentage of the ETB Maximum angle. e.g. If "ETB Idle Maximum Angle" is 10, a value of 70 means 7% ETB Position.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 5664
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based cranking position %. The values in this curve represent a percentage of the ETB Maximum angle. e.g. If "ETB Idle Maximum Angle" is 10, a value of 70 means 7% ETB Position.
	 * units: percent
	 * offset 5696
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 5728
	 */
	float afterCrankingIACtaperDurationBins[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	 * Should be 100 once tune is better
	 * units: cycles
	 * offset 5752
	 */
	uint16_t afterCrankingIACtaperDuration[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: RPM
	 * offset 5764
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: deg
	 * offset 5772
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5804
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE_RPM] = {};
	/**
	 * units: load
	 * offset 5808
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE] = {};
	/**
	 * units: %
	 * offset 5812
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE_RPM] = {};
	/**
	 * offset 5844
	 */
	lua_script_t luaScript;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 45844
	 */
	float cltFuelCorrBins[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 45908
	 */
	float cltFuelCorr[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 45972
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 46036
	 */
	float iatFuelCorr[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 46100
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 46132
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: counter
	 * offset 46164
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 46196
	 */
	int16_t crankingCycleFuelCltBins[CRANKING_CYCLE_CLT_SIZE] = {};
	/**
	 * Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for IAT, TPS ect, to give the final cranking pulse width.
	 * A reasonable starting point is 60mg per liter per cylinder.
	 * ex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.
	 * units: mg
	 * offset 46204
	 */
	float crankingCycleBaseFuel[CRANKING_CYCLE_CLT_SIZE][CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 46332
	 */
	float cltIdleCorrBins[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: %
	 * offset 46356
	 */
	float cltIdleCorrTable[CLT_IDLE_TABLE_RPM_SIZE][CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * units: Target RPM
	 * offset 46452
	 */
	scaled_channel<uint8_t, 1, 100> rpmIdleCorrBins[CLT_IDLE_TABLE_RPM_SIZE] = {};
	/**
	 * Long Term Idle Trim (LTIT) multiplicativo para idle open loop
	 * units: %
	 * offset 46456
	 */
	scaled_channel<uint16_t, 10, 1> ltitTable[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	 * units: kg/hour
	 * offset 46468
	 */
	float mafDecoding[MAF_DECODING_COUNT] = {};
	/**
	 * units: V
	 * offset 46596
	 */
	float mafDecodingBins[MAF_DECODING_COUNT] = {};
	/**
	 * units: deg
	 * offset 46724
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IAT_IGN_CORR_LOAD_COUNT][IAT_IGN_CORR_TEMP_COUNT] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 46852
	 */
	int8_t ignitionIatCorrTempBins[IAT_IGN_CORR_TEMP_COUNT] = {};
	/**
	 * units: Load
	 * offset 46860
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[IAT_IGN_CORR_LOAD_COUNT] = {};
	/**
	 * units: deg
	 * offset 46868
	 */
	int16_t injectionPhase[INJ_PHASE_LOAD_COUNT][INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: Load
	 * offset 47380
	 */
	uint16_t injPhaseLoadBins[INJ_PHASE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 47412
	 */
	uint16_t injPhaseRpmBins[INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: onoff
	 * offset 47444
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT] = {};
	/**
	 * This table represents MAP at a given TPS vs RPM, which we use if our MAP sensor has failed, or if we are using MAP Prediciton. 
	 *  This table should be a direct representation of MAP, you can tune it manually by disconnecting MAP sensor, and filling out the table with values that match an external gauge that shows MAP.
	 * Additionally, you can also use MLV to get the map values and/or generate the table for you
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 47504
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[MAP_EST_LOAD_COUNT][MAP_EST_RPM_COUNT] = {};
	/**
	 * units: % TPS
	 * offset 48016
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[MAP_EST_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 48048
	 */
	uint16_t mapEstimateRpmBins[MAP_EST_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 48080
	 */
	int8_t vvtTable1[VVT_TABLE_SIZE][VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 48144
	 */
	uint16_t vvtTable1LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 48160
	 */
	uint16_t vvtTable1RpmBins[VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 48176
	 */
	int8_t vvtTable2[VVT_TABLE_SIZE][VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 48240
	 */
	uint16_t vvtTable2LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 48256
	 */
	uint16_t vvtTable2RpmBins[VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 48272
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT] = {};
	/**
	 * units: Load
	 * offset 48784
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 48816
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT] = {};
	/**
	 * units: %
	 * offset 48848
	 */
	scaled_channel<uint16_t, 10, 1> veTable[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(fuelUnits, fuelAlgorithm) }
	 * offset 49360
	 */
	uint16_t veLoadBins[VE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 49392
	 */
	uint16_t veRpmBins[VE_RPM_COUNT] = {};
	/**
	 * units: lambda
	 * offset 49424
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT] = {};
	/**
	 * offset 49680
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 49712
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 49744
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: %
	 * offset 50000
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: %
	 * offset 50032
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: value
	 * offset 50064
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8] = {};
	/**
	 * units: L
	 * offset 50320
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 50336
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: value
	 * offset 50352
	 */
	float scriptTable2[TABLE_2_LOAD_SIZE][TABLE_2_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 50608
	 */
	int16_t scriptTable2LoadBins[TABLE_2_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 50624
	 */
	int16_t scriptTable2RpmBins[TABLE_2_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 50640
	 */
	uint8_t scriptTable3[TABLE_3_LOAD_SIZE][TABLE_3_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 50704
	 */
	int16_t scriptTable3LoadBins[TABLE_3_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 50720
	 */
	int16_t scriptTable3RpmBins[TABLE_3_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 50736
	 */
	uint8_t scriptTable4[TABLE_4_LOAD_SIZE][TABLE_4_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 50816
	 */
	int16_t scriptTable4LoadBins[TABLE_4_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 50832
	 */
	int16_t scriptTable4RpmBins[TABLE_4_RPM_SIZE] = {};
	/**
	 * offset 50852
	 */
	uint16_t ignTrimLoadBins[IGN_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 50860
	 */
	uint16_t ignTrimRpmBins[IGN_TRIM_SIZE] = {};
	/**
	 * offset 50868
	 */
	ign_cyl_trim_s ignTrims[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 51060
	 */
	uint16_t fuelTrimLoadBins[FUEL_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 51068
	 */
	uint16_t fuelTrimRpmBins[FUEL_TRIM_SIZE] = {};
	/**
	 * offset 51076
	 */
	fuel_cyl_trim_s fuelTrims[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: ratio
	 * offset 51268
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: Airmass
	 * offset 51284
	 */
	scaled_channel<uint8_t, 1, 5> tcu_pcAirmassBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51292
	 */
	uint8_t tcu_pcValsR[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51300
	 */
	uint8_t tcu_pcValsN[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51308
	 */
	uint8_t tcu_pcVals1[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51316
	 */
	uint8_t tcu_pcVals2[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51324
	 */
	uint8_t tcu_pcVals3[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51332
	 */
	uint8_t tcu_pcVals4[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51340
	 */
	uint8_t tcu_pcVals12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51348
	 */
	uint8_t tcu_pcVals23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51356
	 */
	uint8_t tcu_pcVals34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51364
	 */
	uint8_t tcu_pcVals21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51372
	 */
	uint8_t tcu_pcVals32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51380
	 */
	uint8_t tcu_pcVals43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: TPS
	 * offset 51388
	 */
	uint8_t tcu_tccTpsBins[8] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 51396
	 */
	uint8_t tcu_tccLockSpeed[8] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 51404
	 */
	uint8_t tcu_tccUnlockSpeed[8] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 51412
	 */
	uint8_t tcu_32SpeedBins[8] = {};
	/**
	 * units: %
	 * offset 51420
	 */
	uint8_t tcu_32Vals[8] = {};
	/**
	 * units: %
	 * offset 51428
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[ETB2_TRIM_SIZE][ETB2_TRIM_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 51464
	 */
	uint8_t throttle2TrimTpsBins[ETB2_TRIM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51470
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[ETB2_TRIM_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 51476
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[KNOCK_TABLE_SIZE][KNOCK_TABLE_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 51512
	 */
	uint8_t maxKnockRetardLoadBins[KNOCK_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51518
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[KNOCK_TABLE_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 51524
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51556
	 */
	uint16_t alsIgnRetardLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51564
	 */
	uint16_t alsIgnRetardrpmBins[ALS_RPM_SIZE] = {};
	/**
	 * units: percent
	 * offset 51572
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51604
	 */
	uint16_t alsFuelAdjustmentLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51612
	 */
	uint16_t alsFuelAdjustmentrpmBins[ALS_RPM_SIZE] = {};
	/**
	 * units: ratio
	 * offset 51620
	 */
	scaled_channel<int16_t, 1, 10> ALSIgnSkipTable[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51652
	 */
	uint16_t alsIgnSkipLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51660
	 */
	uint16_t alsIgnSkiprpmBins[ALS_RPM_SIZE] = {};
	/**
	 * offset 51668
	 */
	blend_table_s ignBlends[IGN_BLEND_COUNT] = {};
	/**
	 * offset 52420
	 */
	blend_table_s veBlends[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 53172
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaBins[THR_EST_SIZE] = {};
	/**
	 * In units of g/s normalized to choked flow conditions
	 * units: g/s
	 * offset 53196
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaValues[THR_EST_SIZE] = {};
	/**
	 * offset 53220
	 */
	blend_table_s boostOpenLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 53596
	 */
	blend_table_s boostClosedLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * units: level
	 * offset 53972
	 */
	float tcu_rangeP[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 53996
	 */
	float tcu_rangeR[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54020
	 */
	float tcu_rangeN[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54044
	 */
	float tcu_rangeD[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54068
	 */
	float tcu_rangeM[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54092
	 */
	float tcu_rangeM3[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54116
	 */
	float tcu_rangeM2[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54140
	 */
	float tcu_rangeM1[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54164
	 */
	float tcu_rangePlus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54188
	 */
	float tcu_rangeMinus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54212
	 */
	float tcu_rangeLow[RANGE_INPUT_COUNT] = {};
	/**
	 * units: lambda
	 * offset 54236
	 */
	scaled_channel<uint8_t, 100, 1> lambdaMaxDeviationTable[LAM_SIZE][LAM_RPM_SIZE] = {};
	/**
	 * offset 54252
	 */
	uint16_t lambdaMaxDeviationLoadBins[LAM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 54260
	 */
	uint16_t lambdaMaxDeviationRpmBins[LAM_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 54268
	 */
	uint8_t injectorStagingTable[INJ_STAGING_COUNT][INJ_STAGING_RPM_SIZE] = {};
	/**
	 * offset 54304
	 */
	uint16_t injectorStagingLoadBins[INJ_STAGING_COUNT] = {};
	/**
	 * units: RPM
	 * offset 54316
	 */
	uint16_t injectorStagingRpmBins[INJ_STAGING_RPM_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 54328
	 */
	int16_t wwCltBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 54344
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 54352
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 54360
	 */
	uint8_t wwMapBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 54368
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 54376
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: %
	 * offset 54384
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: deg
	 * offset 54400
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: volts
	 * offset 54416
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: ms
	 * offset 54424
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: kPa
	 * offset 54440
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE] = {};
	/**
	 * units: load
	 * offset 54640
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 54660
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: %
	 * offset 54670
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_RPM_SIZE] = {};
	/**
	 * units: cc/lobe
	 * offset 54770
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: RPM
	 * offset 54790
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 54800
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensation[HPFP_FUEL_MASS_COMPENSATION_SIZE][HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 54928
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensationFuelMass[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: bar
	 * offset 54944
	 */
	scaled_channel<uint16_t, 10, 1> hpfpFuelMassCompensationFuelPressure[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: ms
	 * offset 54960
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearization[FLOW_LINEARIZATION_PRESSURE_SIZE][FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 55472
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearizationFuelMassBins[FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: bar
	 * offset 55504
	 */
	scaled_channel<uint16_t, 10, 1> injectorFlowLinearizationPressureBins[FLOW_LINEARIZATION_PRESSURE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55536
	 */
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	 * units: dB
	 * offset 55568
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55584
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: multiplier
	 * offset 55588
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: RPM
	 * offset 55592
	 */
	scaled_channel<uint8_t, 1, 50> predictiveMapBlendDurationBins[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: second
	 * offset 55596
	 */
	scaled_channel<uint8_t, 50, 1> predictiveMapBlendDurationValues[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 55600
	 */
	scaled_channel<int16_t, 1, 1> cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55608
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: volt
	 * offset 55616
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: %
	 * offset 55632
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: volts
	 * offset 55640
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: multiplier
	 * offset 55648
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 55656
	 */
	scaled_channel<uint8_t, 1, 1> tcu_shiftTpsBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 55664
	 */
	uint8_t tcu_shiftSpeed12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 55672
	 */
	uint8_t tcu_shiftSpeed23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 55680
	 */
	uint8_t tcu_shiftSpeed34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 55688
	 */
	uint8_t tcu_shiftSpeed21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 55696
	 */
	uint8_t tcu_shiftSpeed32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: SPECIAL_CASE_SPEED
	 * offset 55704
	 */
	uint8_t tcu_shiftSpeed43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: ms
	 * offset 55712
	 */
	float tcu_shiftTime;
	/**
	 * units: volts
	 * offset 55716
	 */
	scaled_channel<int16_t, 10, 1> alternatorVoltageTargetTable[ALTERNATOR_VOLTAGE_TARGET_SIZE][ALTERNATOR_VOLTAGE_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 55748
	 */
	uint16_t alternatorVoltageTargetLoadBins[ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55756
	 */
	uint16_t alternatorVoltageTargetRpmBins[ALTERNATOR_VOLTAGE_RPM_SIZE] = {};
	/**
	 * units: C
	 * offset 55764
	 */
	float cltBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 55784
	 */
	float cltBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 55804
	 */
	float iatBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 55824
	 */
	float iatBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 55844
	 */
	float cltBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 55864
	 */
	float cltBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 55884
	 */
	float iatBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 55904
	 */
	float iatBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55924
	 */
	scaled_channel<uint8_t, 1, 100> minimumOilPressureBins[8] = {};
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 55932
	 */
	scaled_channel<uint8_t, 1, 10> minimumOilPressureValues[8] = {};
	/**
	 * offset 55940
	 */
	blend_table_s targetAfrBlends[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * @@DYNO_RPM_STEP_TOOLTIP@@
	 * units: Rpm
	 * offset 56316
	 */
	scaled_channel<uint8_t, 1, 1> dynoRpmStep;
	/**
	 * @@DYNO_SAE_TEMPERATURE_C_TOOLTIP@@
	 * units: C
	 * offset 56317
	 */
	scaled_channel<int8_t, 1, 1> dynoSaeTemperatureC;
	/**
	 * @@DYNO_SAE_RELATIVE_HUMIDITY_TOOLTIP@@
	 * units: %
	 * offset 56318
	 */
	scaled_channel<uint8_t, 1, 1> dynoSaeRelativeHumidity;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56319
	 */
	uint8_t alignmentFill_at_56319[1] = {};
	/**
	 * @@DYNO_SAE_BARO_TOOLTIP@@
	 * units: KPa
	 * offset 56320
	 */
	scaled_channel<float, 1, 1> dynoSaeBaro;
	/**
	 * @@DYNO_CAR_WHEEL_DIA_INCH_TOOLTIP@@
	 * units: Inch
	 * offset 56324
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelDiaInch;
	/**
	 * @@DYNO_CAR_WHEEL_ASPECT_RATIO_TOOLTIP@@
	 * units: Aspect Ratio (height)
	 * offset 56325
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelAspectRatio;
	/**
	 * @@DYNO_CAR_WHEEL_TIRE_WIDTH_TOOLTIP@@
	 * units: Width mm
	 * offset 56326
	 */
	scaled_channel<int16_t, 1, 1> dynoCarWheelTireWidthMm;
	/**
	 * @@DYNO_CAR_GEAR_PRIMARY_REDUCTION_TOOLTIP@@
	 * units: Units
	 * offset 56328
	 */
	scaled_channel<float, 1, 1> dynoCarGearPrimaryReduction;
	/**
	 * @@DYNO_CAR_GEAR_RATIO_TOOLTIP@@
	 * units: Units
	 * offset 56332
	 */
	scaled_channel<float, 1, 1> dynoCarGearRatio;
	/**
	 * @@DYNO_CAR_GEAR_FINAL_DRIVE_TOOLTIP@@
	 * units: Units
	 * offset 56336
	 */
	scaled_channel<float, 1, 1> dynoCarGearFinalDrive;
	/**
	 * @@DYNO_CAR_CAR_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 56340
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCarMassKg;
	/**
	 * @@DYNO_CAR_CARGO_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 56342
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCargoMassKg;
	/**
	 * @@DYNO_CAR_COEFF_OF_DRAG_TOOLTIP@@
	 * units: Coeff
	 * offset 56344
	 */
	scaled_channel<float, 1, 1> dynoCarCoeffOfDrag;
	/**
	 * @@DYNO_CAR_FRONTAL_AREA_TOOLTIP@@
	 * units: m2
	 * offset 56348
	 */
	scaled_channel<float, 1, 1> dynoCarFrontalAreaM2;
	/**
	 * units: deg
	 * offset 56352
	 */
	scaled_channel<int8_t, 10, 1> trailingSparkTable[TRAILING_SPARK_SIZE][TRAILING_SPARK_RPM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 56368
	 */
	scaled_channel<uint8_t, 1, 50> trailingSparkRpmBins[TRAILING_SPARK_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 56372
	 */
	scaled_channel<uint8_t, 1, 5> trailingSparkLoadBins[TRAILING_SPARK_SIZE] = {};
	/**
	 * units: RPM
	 * offset 56376
	 */
	scaled_channel<uint8_t, 1, 100> maximumOilPressureBins[4] = {};
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 56380
	 */
	scaled_channel<uint8_t, 1, 10> maximumOilPressureValues[4] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 56384
	 */
	gppwm_channel_e torqueReductionCutXaxis;
	/**
	 * How many % of ignition events will be cut
	 * units: %
	 * offset 56385
	 */
	int8_t torqueReductionIgnitionCutTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56397
	 */
	uint8_t alignmentFill_at_56397[1] = {};
	/**
	 * offset 56398
	 */
	int16_t torqueReductionCutXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 56410
	 */
	int8_t torqueReductionCutGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 56412
	 */
	gppwm_channel_e torqueReductionTimeXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56413
	 */
	uint8_t alignmentFill_at_56413[3] = {};
	/**
	 * For how long after the pin has been triggered will the cut/reduction stay active. After that, even if the pin is still triggered, torque is re-introduced
	 * units: ms
	 * offset 56416
	 */
	float torqueReductionTimeTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 56464
	 */
	int16_t torqueReductionTimeXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 56476
	 */
	int8_t torqueReductionTimeGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 56478
	 */
	gppwm_channel_e torqueReductionIgnitionRetardXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56479
	 */
	uint8_t alignmentFill_at_56479[1] = {};
	/**
	 * How many degrees of timing advance will be reduced during the Torque Reduction Time
	 * units: deg
	 * offset 56480
	 */
	float torqueReductionIgnitionRetardTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 56528
	 */
	int16_t torqueReductionIgnitionRetardXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 56540
	 */
	int8_t torqueReductionIgnitionRetardGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56542
	 */
	uint8_t alignmentFill_at_56542[2] = {};
	/**
	offset 56544 bit 0 */
	bool boardUseTempPullUp : 1 {};
	/**
	offset 56544 bit 1 */
	bool boardUse2stepPullDown : 1 {};
	/**
	offset 56544 bit 2 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 56544 bit 3 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 56544 bit 4 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 56544 bit 5 */
	bool boardUseH1PullDown : 1 {};
	/**
	offset 56544 bit 6 */
	bool boardUseH2PullDown : 1 {};
	/**
	offset 56544 bit 7 */
	bool boardUseH3PullDown : 1 {};
	/**
	offset 56544 bit 8 */
	bool boardUseH4PullDown : 1 {};
	/**
	offset 56544 bit 9 */
	bool boardUseH5PullDown : 1 {};
	/**
	offset 56544 bit 10 */
	bool boardUseFlexPullDown : 1 {};
	/**
	offset 56544 bit 11 */
	bool unusedBit_285_11 : 1 {};
	/**
	offset 56544 bit 12 */
	bool unusedBit_285_12 : 1 {};
	/**
	offset 56544 bit 13 */
	bool unusedBit_285_13 : 1 {};
	/**
	offset 56544 bit 14 */
	bool unusedBit_285_14 : 1 {};
	/**
	offset 56544 bit 15 */
	bool unusedBit_285_15 : 1 {};
	/**
	offset 56544 bit 16 */
	bool unusedBit_285_16 : 1 {};
	/**
	offset 56544 bit 17 */
	bool unusedBit_285_17 : 1 {};
	/**
	offset 56544 bit 18 */
	bool unusedBit_285_18 : 1 {};
	/**
	offset 56544 bit 19 */
	bool unusedBit_285_19 : 1 {};
	/**
	offset 56544 bit 20 */
	bool unusedBit_285_20 : 1 {};
	/**
	offset 56544 bit 21 */
	bool unusedBit_285_21 : 1 {};
	/**
	offset 56544 bit 22 */
	bool unusedBit_285_22 : 1 {};
	/**
	offset 56544 bit 23 */
	bool unusedBit_285_23 : 1 {};
	/**
	offset 56544 bit 24 */
	bool unusedBit_285_24 : 1 {};
	/**
	offset 56544 bit 25 */
	bool unusedBit_285_25 : 1 {};
	/**
	offset 56544 bit 26 */
	bool unusedBit_285_26 : 1 {};
	/**
	offset 56544 bit 27 */
	bool unusedBit_285_27 : 1 {};
	/**
	offset 56544 bit 28 */
	bool unusedBit_285_28 : 1 {};
	/**
	offset 56544 bit 29 */
	bool unusedBit_285_29 : 1 {};
	/**
	offset 56544 bit 30 */
	bool unusedBit_285_30 : 1 {};
	/**
	offset 56544 bit 31 */
	bool unusedBit_285_31 : 1 {};
};
static_assert(sizeof(persistent_config_s) == 56548);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt
