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
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
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
	scaled_channel<uint16_t, 10, 1> timeConstant[STFT_CELL_COUNT] = {};
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
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 0
	 */
	float lowValue;
	/**
	 * kPa/psi value at high volts
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
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
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 0
	 */
	float tempC_1;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 4
	 */
	float tempC_2;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
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
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
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
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 12
	 */
	int16_t minimumClt;
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
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

// start of i2c_config_s
struct i2c_config_s {
	/**
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
	 * offset 4
	 */
	i2c_speed_e speed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[1] = {};
	/**
	 * offset 6
	 */
	Gpio sdaPin;
	/**
	 * offset 8
	 */
	Gpio sclPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2] = {};
};
static_assert(sizeof(i2c_config_s) == 12);

// start of pca9685_config_s
struct pca9685_config_s {
	/**
	 * Inverted outputs, if set active level will be 0
	offset 0 bit 0 */
	bool inverted : 1 {};
	/**
	 * Open Drain outputs
	offset 0 bit 1 */
	bool od : 1 {};
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
	 * offset 4
	 */
	i2c_bus_e i2c_bus;
	/**
	 * I2C address of chip (dec)
	 * offset 5
	 */
	uint8_t i2c_addr;
	/**
	 * PWM frequency (one for all outputs)
	 * units: Hz
	 * offset 6
	 */
	uint16_t pwm_freq;
};
static_assert(sizeof(pca9685_config_s) == 8);

// start of can_sniffer_channel_s
struct can_sniffer_channel_s {
	/**
	 * Sniffer listens to this CAN channel
	offset 0 bit 0 */
	bool read : 1 {};
	/**
	 * Sniffer also listen to what RusEFI ECU says
	offset 0 bit 1 */
	bool listenOurs : 1 {};
	/**
	 * Messages transmitted from PC will be also handled by RusEFI ECU like it received from CAN
	offset 0 bit 2 */
	bool handleInjected : 1 {};
	/**
	offset 0 bit 3 */
	bool unusedBit_3_3 : 1 {};
	/**
	offset 0 bit 4 */
	bool unusedBit_3_4 : 1 {};
	/**
	offset 0 bit 5 */
	bool unusedBit_3_5 : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_3_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_3_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_3_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_3_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_3_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_3_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_3_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_3_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_3_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_3_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_3_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_3_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_3_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_3_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_3_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_3_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_3_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_3_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_3_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_3_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_3_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_3_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_3_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_3_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_3_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_3_31 : 1 {};
};
static_assert(sizeof(can_sniffer_channel_s) == 4);

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
	 * The target engine speed (RPM) to maintain during launch.
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
	 * need 4 byte alignment
	 * units: units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2] = {};
	/**
	 * Enable LTIT (Long Term Idle Trim) learning
	offset 12 bit 0 */
	bool ltitEnabled : 1 {};
	/**
	offset 12 bit 1 */
	bool useMetricOnInterface : 1 {};
	/**
	offset 12 bit 2 */
	bool useLambdaOnInterface : 1 {};
	/**
	offset 12 bit 3 */
	bool unusedBit_9_3 : 1 {};
	/**
	offset 12 bit 4 */
	bool unusedBit_9_4 : 1 {};
	/**
	offset 12 bit 5 */
	bool unusedBit_9_5 : 1 {};
	/**
	offset 12 bit 6 */
	bool unusedBit_9_6 : 1 {};
	/**
	offset 12 bit 7 */
	bool unusedBit_9_7 : 1 {};
	/**
	offset 12 bit 8 */
	bool unusedBit_9_8 : 1 {};
	/**
	offset 12 bit 9 */
	bool unusedBit_9_9 : 1 {};
	/**
	offset 12 bit 10 */
	bool unusedBit_9_10 : 1 {};
	/**
	offset 12 bit 11 */
	bool unusedBit_9_11 : 1 {};
	/**
	offset 12 bit 12 */
	bool unusedBit_9_12 : 1 {};
	/**
	offset 12 bit 13 */
	bool unusedBit_9_13 : 1 {};
	/**
	offset 12 bit 14 */
	bool unusedBit_9_14 : 1 {};
	/**
	offset 12 bit 15 */
	bool unusedBit_9_15 : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_9_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_9_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_9_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_9_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_9_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_9_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_9_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_9_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_9_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_9_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_9_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_9_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_9_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_9_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_9_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_9_31 : 1 {};
	/**
	 * Disable multispark above this engine speed.
	 * units: rpm
	 * offset 16
	 */
	scaled_channel<uint8_t, 1, 50> multisparkMaxRpm;
	/**
	 * Above this RPM, disable AC. Set to 0 to disable check.
	 * units: rpm
	 * offset 17
	 */
	scaled_channel<uint8_t, 1, 50> maxAcRpm;
	/**
	 * Above this TPS, disable AC. Set to 0 to disable check.
	 * units: %
	 * offset 18
	 */
	uint8_t maxAcTps;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 19
	 */
	uint8_t alignmentFill_at_19[1] = {};
	/**
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 20
	 */
	int16_t maxAcClt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 22
	 */
	uint8_t alignmentFill_at_22[2] = {};
	/**
	 * Just for reference really, not taken into account by any logic at this point
	 * units: CR
	 * offset 24
	 */
	float compressionRatio;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	 * units: mv
	 * offset 28
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	 * units: mv
	 * offset 30
	 */
	uint16_t idlePositionMax;
	/**
	 * EMA filter constant for LTIT (0-255)
	 * units: 0-255
	 * offset 32
	 */
	uint8_t ltitEmaAlpha;
	/**
	 * RPM range to consider stable idle
	 * units: rpm
	 * offset 33
	 */
	uint8_t ltitStableRpmThreshold;
	/**
	 * Minimum time of stable idle before learning
	 * units: s
	 * offset 34
	 */
	uint8_t ltitStableTime;
	/**
	 * LTIT learning rate
	 * units: %/s
	 * offset 35
	 */
	uint8_t ltitCorrectionRate;
	/**
	 * Delay after ignition ON before LTIT learning/application
	 * units: s
	 * offset 36
	 */
	uint8_t ltitIgnitionOnDelay;
	/**
	 * Delay after ignition OFF before LTIT save
	 * units: s
	 * offset 37
	 */
	uint8_t ltitIgnitionOffSaveDelay;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 38
	 */
	uint8_t alignmentFill_at_38[2] = {};
	/**
	 * Minimum LTIT multiplicative correction value
	 * units: %
	 * offset 40
	 */
	float ltitClampMin;
	/**
	 * Maximum LTIT multiplicative correction value
	 * units: %
	 * offset 44
	 */
	float ltitClampMax;
	/**
	 * LTIT table regional smoothing intensity (0=no smoothing)
	 * units: ratio
	 * offset 48
	 */
	scaled_channel<uint8_t, 100, 1> ltitSmoothingIntensity;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 49
	 */
	uint8_t alignmentFill_at_49[3] = {};
	/**
	 * Minimum threshold of PID integrator for LTIT correction
	 * units: %
	 * offset 52
	 */
	float ltitIntegratorThreshold;
	/**
	 * offset 56
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 58
	 */
	Gpio sdCardCsPin;
	/**
	 * offset 60
	 */
	Gpio canTxPin;
	/**
	 * offset 62
	 */
	Gpio canRxPin;
	/**
	 * Pin that activates the reduction/cut for shifting. Sometimes shared with the Launch Control pin
	 * offset 64
	 */
	switch_input_pin_e torqueReductionTriggerPin;
	/**
	 * Fuel enrichment adder percentage.
	 * units: %
	 * offset 66
	 */
	int8_t launchFuelAdderPercent;
	/**
	 * Time after which the throttle is considered jammed.
	 * units: sec
	 * offset 67
	 */
	scaled_channel<uint8_t, 50, 1> etbJamTimeout;
	/**
	 * offset 68
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 70
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * Additional idle % while A/C is active
	 * units: %
	 * offset 71
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	 * units: deg
	 * offset 72
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	 * units: count
	 * offset 73
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * units: RPM
	 * offset 74
	 */
	int16_t vvtControlMinRpm;
	/**
	 * offset 76
	 */
	injector_s injector;
	/**
	 * offset 176
	 */
	injector_s injectorSecondary;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 276 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 276 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 276 bit 2 */
	bool enableKline : 1 {};
	/**
	offset 276 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	offset 276 bit 4 */
	bool unusedBit_enableFan1WithAc : 1 {};
	/**
	offset 276 bit 5 */
	bool unusedBit_enableFan2WithAc : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 276 bit 6 */
	bool enableTrailingSparks : 1 {};
	/**
	 * TLE7209 and L6205 use two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 276 bit 7 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 276 bit 8 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 276 bit 9 */
	bool useEeprom : 1 {};
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 276 bit 10 */
	bool useCicPidForIdle : 1 {};
	/**
	offset 276 bit 11 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	 * Kick-start cranking mode: below 800 RPM both coils are charged right at the trigger mark and fired a dwell-time later, normal spark scheduling is suppressed.
	offset 276 bit 12 */
	bool kickStartCranking : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 276 bit 13 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 276 bit 14 */
	bool launchControlEnabled : 1 {};
	/**
	offset 276 bit 15 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table). Note: in 'Fuel Map' (running math) mode the base mass already reflects the flex-adjusted stoich ratio, so the cranking flex multipliers act as ADDITIONAL enrichment on top of that - do not re-apply the full ethanol correction there.
	offset 276 bit 16 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	offset 276 bit 17 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 276 bit 18 */
	bool useTLE8888_stepper : 1 {};
	/**
	offset 276 bit 19 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 276 bit 20 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 276 bit 21 */
	bool artificialTestMisfire : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 276 bit 22 */
	bool useFordRedundantPps : 1 {};
	/**
	offset 276 bit 23 */
	bool cltSensorPulldown : 1 {};
	/**
	offset 276 bit 24 */
	bool iatSensorPulldown : 1 {};
	/**
	offset 276 bit 25 */
	bool allowIdenticalPps : 1 {};
	/**
	offset 276 bit 26 */
	bool overrideVvtTriggerGaps : 1 {};
	/**
	 * If enabled - use onboard SPI Accelerometer, otherwise listen for CAN messages
	offset 276 bit 27 */
	bool useSpiImu : 1 {};
	/**
	offset 276 bit 28 */
	bool enableStagedInjection : 1 {};
	/**
	offset 276 bit 29 */
	bool useIdleAdvanceWhileCoasting : 1 {};
	/**
	offset 276 bit 30 */
	bool unusedBit_104_30 : 1 {};
	/**
	offset 276 bit 31 */
	bool unusedBit_104_31 : 1 {};
	/**
	 * Closed voltage for primary throttle position sensor
	 * offset 280
	 */
	tps_limit_t tpsMin;
	/**
	 * Fully opened voltage for primary throttle position sensor
	 * offset 282
	 */
	tps_limit_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 284
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 286
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 288
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	 * units: ms
	 * offset 292
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	 * units: rpm
	 * offset 296
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	 * units: rpm
	 * offset 298
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 300
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 440
	 */
	ThermistorConf clt;
	/**
	 * offset 472
	 */
	ThermistorConf iat;
	/**
	 * The target absolute ignition timing value (e.g., -10 means -10 degrees, not 10 degrees of retard relative to base timing).
	 * units: deg
	 * offset 504
	 */
	float launchTimingRetard;
	/**
	 * Maximum commanded airmass for the idle controller.
	 * units: mg
	 * offset 508
	 */
	scaled_channel<uint8_t, 1, 2> idleMaximumAirmass;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 509
	 */
	uint8_t alignmentFill_at_509[1] = {};
	/**
	 * iTerm min value
	 * offset 510
	 */
	int16_t alternator_iTermMin;
	/**
	 * iTerm max value
	 * offset 512
	 */
	int16_t alternator_iTermMax;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 514
	 */
	uint8_t alignmentFill_at_514[2] = {};
	/**
	 * @@DISPLACEMENT_TOOLTIP@@
	 * units: L
	 * offset 516
	 */
	float displacement;
	/**
	 * units: RPM
	 * offset 520
	 */
	uint16_t triggerSimulatorRpm;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 522
	 */
	uint8_t alignmentFill_at_522[2] = {};
	/**
	 * Number of cylinder the engine has.
	 * offset 524
	 */
	uint32_t cylindersCount;
	/**
	 * offset 528
	 */
	firing_order_e firingOrder;
	/**
	 * offset 529
	 */
	uint8_t justATempTest;
	/**
	 * Delta kPa/psi for MAP sync
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 530
	 */
	uint8_t mapSyncThreshold;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 531
	 */
	uint8_t alignmentFill_at_531[1] = {};
	/**
	 * @@CYLINDER_BORE_TOOLTIP@@
	 * units: mm
	 * offset 532
	 */
	float cylinderBore;
	/**
	 * Determines the method used for calculating fuel delivery. The following options are available:
	 * Uses intake manifold pressure (MAP) and intake air temperature (IAT) to calculate air density and fuel requirements. This is a common strategy, especially for naturally aspirated or turbocharged engines.
	 * Alpha-N: Uses throttle position as the primary load input for fuel calculation. This strategy is generally used in engines with individual throttle bodies or those that lack a reliable MAP signal.
	 * MAF Air Charge: Relies on a Mass Air Flow (MAF) sensor to measure the amount of air entering the engine directly, making it effective for engines equipped with a MAF sensor.
	 * Lua: Allows for custom fuel calculations using Lua scripting, enabling highly specific tuning applications where the other strategies don't apply.
	 * offset 536
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * units: %
	 * offset 537
	 */
	uint8_t ALSMaxTPS;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 538
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups.
	 * offset 539
	 */
	injection_mode_e injectionMode;
	/**
	 * Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 540
	 */
	uint16_t boostControlMinRpm;
	/**
	 * Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 542
	 */
	uint8_t boostControlMinTps;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 543
	 */
	uint8_t alignmentFill_at_543[1] = {};
	/**
	 * Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 544
	 */
	uint16_t boostControlMinMap;
	/**
	 * Wastegate control Solenoid, set to 'NONE' if you are using DC wastegate
	 * offset 546
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 548
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 549
	 */
	uint8_t alignmentFill_at_549[3] = {};
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	 * units: deg
	 * offset 552
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 556
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 557
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 558
	 */
	uint8_t maxIdleVss;
	/**
	 * Allowed range around detection position
	 * offset 559
	 */
	uint8_t camDecoder2jzPrecision;
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 560
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing
	 * Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 562
	 */
	timing_mode_e timingMode;
	/**
	 * offset 563
	 */
	can_nbc_e canNbcType;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	 * units: RPM
	 * offset 564
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
	 * offset 568
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	 * units: coef
	 * offset 572
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	 * units: coef
	 * offset 576
	 */
	float vbattDividerCoeff;
	/**
	 * offset 580
	 */
	output_pin_e fanPin;
	/**
	 * offset 582
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 583
	 */
	uint8_t alignmentFill_at_583[1] = {};
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius. Only used by the on/off relay path - ignored when PWM mode is enabled, which uses the PWM curve's own bins instead.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 584
	 */
	int16_t fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius. Only used by the on/off relay path - ignored when PWM mode is enabled, which uses the PWM curve's own bins instead.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 586
	 */
	int16_t fanOffTemperature;
	/**
	 * offset 588
	 */
	output_pin_e fan2Pin;
	/**
	 * offset 590
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 591
	 */
	uint8_t alignmentFill_at_591[1] = {};
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius. Only used by the on/off relay path - ignored when PWM mode is enabled, which uses the PWM curve's own bins instead.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 592
	 */
	int16_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius. Only used by the on/off relay path - ignored when PWM mode is enabled, which uses the PWM curve's own bins instead.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 594
	 */
	int16_t fan2OffTemperature;
	/**
	 * Inhibit the fan above this vehicle speed. Zero disables the check.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 596
	 */
	int8_t disableFan1AtSpeed;
	/**
	 * Inhibit the fan above this vehicle speed. Zero disables the check.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 597
	 */
	int8_t disableFan2AtSpeed;
	/**
	 * Hysteresis below the disable-at-speed threshold before the fan is allowed back on. Prevents rapid on/off cycling at the threshold speed.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 598
	 */
	int8_t disableFan1AtSpeedHysteresis;
	/**
	 * Hysteresis below the disable-at-speed threshold before the fan is allowed back on. Prevents rapid on/off cycling at the threshold speed.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 599
	 */
	int8_t disableFan2AtSpeedHysteresis;
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 600 bit 0 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 600 bit 1 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable PWM mode for Fan 1. When enabled, the fan output is driven by the PWM curve instead of on/off relay logic.
	offset 600 bit 2 */
	bool fan1PwmEnabled : 1 {};
	/**
	 * Enable PWM mode for Fan 2. When enabled, the fan output is driven by the PWM curve instead of on/off relay logic.
	offset 600 bit 3 */
	bool fan2PwmEnabled : 1 {};
	/**
	offset 600 bit 4 */
	bool unusedBit_173_4 : 1 {};
	/**
	offset 600 bit 5 */
	bool unusedBit_173_5 : 1 {};
	/**
	offset 600 bit 6 */
	bool unusedBit_173_6 : 1 {};
	/**
	offset 600 bit 7 */
	bool unusedBit_173_7 : 1 {};
	/**
	offset 600 bit 8 */
	bool unusedBit_173_8 : 1 {};
	/**
	offset 600 bit 9 */
	bool unusedBit_173_9 : 1 {};
	/**
	offset 600 bit 10 */
	bool unusedBit_173_10 : 1 {};
	/**
	offset 600 bit 11 */
	bool unusedBit_173_11 : 1 {};
	/**
	offset 600 bit 12 */
	bool unusedBit_173_12 : 1 {};
	/**
	offset 600 bit 13 */
	bool unusedBit_173_13 : 1 {};
	/**
	offset 600 bit 14 */
	bool unusedBit_173_14 : 1 {};
	/**
	offset 600 bit 15 */
	bool unusedBit_173_15 : 1 {};
	/**
	offset 600 bit 16 */
	bool unusedBit_173_16 : 1 {};
	/**
	offset 600 bit 17 */
	bool unusedBit_173_17 : 1 {};
	/**
	offset 600 bit 18 */
	bool unusedBit_173_18 : 1 {};
	/**
	offset 600 bit 19 */
	bool unusedBit_173_19 : 1 {};
	/**
	offset 600 bit 20 */
	bool unusedBit_173_20 : 1 {};
	/**
	offset 600 bit 21 */
	bool unusedBit_173_21 : 1 {};
	/**
	offset 600 bit 22 */
	bool unusedBit_173_22 : 1 {};
	/**
	offset 600 bit 23 */
	bool unusedBit_173_23 : 1 {};
	/**
	offset 600 bit 24 */
	bool unusedBit_173_24 : 1 {};
	/**
	offset 600 bit 25 */
	bool unusedBit_173_25 : 1 {};
	/**
	offset 600 bit 26 */
	bool unusedBit_173_26 : 1 {};
	/**
	offset 600 bit 27 */
	bool unusedBit_173_27 : 1 {};
	/**
	offset 600 bit 28 */
	bool unusedBit_173_28 : 1 {};
	/**
	offset 600 bit 29 */
	bool unusedBit_173_29 : 1 {};
	/**
	offset 600 bit 30 */
	bool unusedBit_173_30 : 1 {};
	/**
	offset 600 bit 31 */
	bool unusedBit_173_31 : 1 {};
	/**
	 * Fan 1 PWM frequency
	 * units: Hz
	 * offset 604
	 */
	uint16_t fan1PwmFrequency;
	/**
	 * Fan 2 PWM frequency
	 * units: Hz
	 * offset 606
	 */
	uint16_t fan2PwmFrequency;
	/**
	 * Fan 1 PWM curve temperature bins
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 608
	 */
	int16_t fan1TempBins[FAN_PWM_CURVE_SIZE] = {};
	/**
	 * Fan 1 speed demand (0% off, 100% full speed)
	 * units: %
	 * offset 624
	 */
	uint8_t fan1PwmValues[FAN_PWM_CURVE_SIZE] = {};
	/**
	 * Fan 2 PWM curve temperature bins
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 632
	 */
	int16_t fan2TempBins[FAN_PWM_CURVE_SIZE] = {};
	/**
	 * Fan 2 speed demand (0% off, 100% full speed)
	 * units: %
	 * offset 648
	 */
	uint8_t fan2PwmValues[FAN_PWM_CURVE_SIZE] = {};
	/**
	 * PWM duty cycle meaning "fan off" (0% demand). May be greater than the full-speed duty to describe hardware that drives the fan with an inverted PWM signal (e.g. NPN transistor + pull-up: high duty = off, low duty = full speed) (Fan 1)
	 * units: %
	 * offset 656
	 */
	uint8_t fan1MinPwm;
	/**
	 * PWM duty cycle meaning "fan full speed" (100% demand) (Fan 1)
	 * units: %
	 * offset 657
	 */
	uint8_t fan1MaxPwm;
	/**
	 * PWM duty cycle meaning "fan off" (0% demand). May be greater than the full-speed duty to describe hardware that drives the fan with an inverted PWM signal (e.g. NPN transistor + pull-up: high duty = off, low duty = full speed) (Fan 2)
	 * units: %
	 * offset 658
	 */
	uint8_t fan2MinPwm;
	/**
	 * PWM duty cycle meaning "fan full speed" (100% demand) (Fan 2)
	 * units: %
	 * offset 659
	 */
	uint8_t fan2MaxPwm;
	/**
	 * Time to ramp speed demand from 0 to 100% (Fan 1)
	 * units: s
	 * offset 660
	 */
	float fan1SoftStartSec;
	/**
	 * Time to ramp speed demand from 0 to 100% (Fan 2)
	 * units: s
	 * offset 664
	 */
	float fan2SoftStartSec;
	/**
	 * A/C assistance: Disabled, compressor Relay, or high-side Pressure independent of compressor state. Pressure mode requires an A/C pressure sensor.
	 * offset 668
	 */
	fan_ac_mode_e fan1AcMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 669
	 */
	uint8_t alignmentFill_at_669[1] = {};
	/**
	 * Pressure for relay fan turn-on or 100% PWM demand. Must exceed the off threshold.
	 * units: kPa
	 * offset 670
	 */
	uint16_t fan1AcPressureOn;
	/**
	 * Pressure below which relay assistance turns off, or at which PWM pressure demand reaches zero.
	 * units: kPa
	 * offset 672
	 */
	uint16_t fan1AcPressureOff;
	/**
	 * A/C assistance: Disabled, compressor Relay, or high-side Pressure independent of compressor state. Pressure mode requires an A/C pressure sensor.
	 * offset 674
	 */
	fan_ac_mode_e fan2AcMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 675
	 */
	uint8_t alignmentFill_at_675[1] = {};
	/**
	 * Pressure for relay fan turn-on or 100% PWM demand. Must exceed the off threshold.
	 * units: kPa
	 * offset 676
	 */
	uint16_t fan2AcPressureOn;
	/**
	 * Pressure below which relay assistance turns off, or at which PWM pressure demand reaches zero.
	 * units: kPa
	 * offset 678
	 */
	uint16_t fan2AcPressureOff;
	/**
	 * offset 680
	 */
	int wizardPanelToShow;
	/**
	 * offset 684
	 */
	output_pin_e acrPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 686
	 */
	uint8_t alignmentFill_at_686[2] = {};
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	 * units: revs/km
	 * offset 688
	 */
	float driveWheelRevPerKm;
	/**
	 * CANbus thread period in ms
	 * units: ms
	 * offset 692
	 */
	int canSleepPeriodMs;
	/**
	 * units: index
	 * offset 696
	 */
	int byFirmwareVersion;
	/**
	 * First analog throttle body, first sensor. See also pedalPositionAdcChannel
	 * Analog TPS inputs have 200Hz low-pass cutoff.
	 * offset 700
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 701
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 702
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * offset 703
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 704
	 */
	float idle_derivativeFilterLoss;
	/**
	 * offset 708
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	 * units: %
	 * offset 720
	 */
	float airByRpmTaper;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	 * units: %
	 * offset 724
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 725
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 726
	 */
	uint8_t acrRevolutions;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 727
	 */
	uint8_t alignmentFill_at_727[1] = {};
	/**
	 * offset 728
	 */
	int calibrationBirthday;
	/**
	 * units: volts
	 * offset 732
	 */
	float adcVcc;
	/**
	 * Magic engine phase: we compare instant MAP at X to instant MAP at x+360 angle in one complete cycle
	 * units: Deg
	 * offset 736
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 740
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 748
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 768
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * TPS/PPS error threshold
	 * units: %
	 * offset 769
	 */
	scaled_channel<uint8_t, 10, 1> etbSplit;
	/**
	 * offset 770
	 */
	Gpio tle6240_cs;
	/**
	 * offset 772
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * offset 773
	 */
	pin_output_mode_e mc33810_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 774
	 */
	uint8_t alignmentFill_at_774[2] = {};
	/**
	 * @see hasBaroSensor
	 * offset 776
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 788
	 */
	idle_hardware_s idle;
	/**
	 * Ignition timing to remove when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 800
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	 * units: deg/s
	 * offset 801
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 802
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * offset 803
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	 * units: ratio
	 * offset 804
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 806
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	 * units: count
	 * offset 807
	 */
	uint8_t vssToothCount;
	/**
	 * Allows you to change the default load axis used for the VE table, which is typically MAP (manifold absolute pressure).
	 * offset 808
	 */
	ve_override_e idleVeOverrideMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 809
	 */
	uint8_t alignmentFill_at_809[1] = {};
	/**
	 * offset 810
	 */
	Gpio l9779_cs;
	/**
	 * offset 812
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 836
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 860
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 861
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 862
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 864
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * How many consecutive VVT gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 865
	 */
	int8_t gapVvtTrackingLengthOverride;
	/**
	 * Check engine light, also malfunction indicator light. Solid while ignition is on and the engine is stopped, blinks active error codes.
	 * offset 866
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 868
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 869
	 */
	uint8_t alignmentFill_at_869[1] = {};
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 870
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 872
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 874
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 875
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 876
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT] = {};
	/**
	 * offset 884
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 885
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 886
	 */
	Gpio debugTriggerSync;
	/**
	 * offset 888
	 */
	Gpio debugTriggerState;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 890
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 891
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 892
	 */
	Gpio mc33972_cs;
	/**
	 * offset 894
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 895
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 896
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 897
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 898
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 899
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 900
	 */
	Gpio mc33810_cs[C_MC33810_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 902
	 */
	uint8_t alignmentFill_at_902[2] = {};
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 904
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 908
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT] = {};
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	 * units: us
	 * offset 912
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * Ratio between the wheels and your transmission output.
	 * units: ratio
	 * offset 914
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 916
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 918
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 919
	 */
	uint8_t alignmentFill_at_919[1] = {};
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 920
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * units: g/s
	 * offset 924
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 926
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * offset 928
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 929
	 */
	uint8_t alignmentFill_at_929[1] = {};
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 930
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 932
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * units: RPM
	 * offset 933
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionMinRpm;
	/**
	 * units: %
	 * offset 934
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionMinLoad;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 935
	 */
	uint8_t alignmentFill_at_935[1] = {};
	/**
	offset 936 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 936 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 936 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	offset 936 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 936 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 936 bit 5 */
	bool rethrowHardFault : 1 {};
	/**
	offset 936 bit 6 */
	bool verboseQuad : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 936 bit 7 */
	bool useStepperIdle : 1 {};
	/**
	offset 936 bit 8 */
	bool lambdaProtectionEnable : 1 {};
	/**
	offset 936 bit 9 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	offset 936 bit 10 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 936 bit 11 */
	bool externalRusEfiGdiModule : 1 {};
	/**
	 * Sample MAP during only one cylinder's intake per engine cycle instead of every cylinder.
	 * Enable for individual throttle bodies, where the MAP sensor reads a single runner with its own pressure pulses. Leave disabled for a shared plenum or single throttle so every intake event is averaged together for a smoother reading.
	offset 936 bit 12 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 936 bit 13 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, RPM is estimated from ~90 degrees of rotation using tooth timestamps collected even before trigger sync, and fuel/ignition scheduling starts as soon as the trigger syncs (sequential ignition temporarily runs as wasted spark until full phase sync). As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 936 bit 14 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * Enable fuel cutoff in the selected Overrun, Decel or Both mode. Coolant, MAP, tuning and Lua guards apply to every mode.
	offset 936 bit 15 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 936 bit 16 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 936 bit 17 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 936 bit 18 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 936 bit 19 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 936 bit 20 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 936 bit 21 */
	bool pauseEtbControl : 1 {};
	/**
	offset 936 bit 22 */
	bool verboseKLine : 1 {};
	/**
	offset 936 bit 23 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 936 bit 24 */
	bool enableAemXSeries : 1 {};
	/**
	offset 936 bit 25 */
	bool modeledFlowIdle : 1 {};
	/**
	offset 936 bit 26 */
	bool isTuningDetectorEnabled : 1 {};
	/**
	offset 936 bit 27 */
	bool useAbsolutePressureForLagTime : 1 {};
	/**
	 * When returning to idle from coasting, start the closed-loop RPM target elevated by the 'RPM upper limit' idle detection threshold, then ramp it down to the normal target over the 'Ramp target duration'. Helps prevent RPM from dipping below the idle target on return to idle.
	offset 936 bit 28 */
	bool idleReturnTargetRamp : 1 {};
	/**
	offset 936 bit 29 */
	bool useInjectorFlowLinearizationTable : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 936 bit 30 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 936 bit 31 */
	bool multisparkEnable : 1 {};
	/**
	 * Enables absolute ignition timing control during launch (sets timing to the "Absolute Timing at Launch" value).
	offset 940 bit 0 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 940 bit 1 */
	bool canInputBCM : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	offset 940 bit 2 */
	bool consumeObdSensors : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 940 bit 3 */
	bool enableCanVss : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 940 bit 4 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 940 bit 5 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 940 bit 6 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 940 bit 7 */
	bool verboseVVTDecoding : 1 {};
	/**
	offset 940 bit 8 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * When set to yes, it enables intake air temperature-based corrections for Alpha-N tuning strategies.
	offset 940 bit 9 */
	bool alphaNUseIat : 1 {};
	/**
	offset 940 bit 10 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 940 bit 11 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 940 bit 12 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 940 bit 13 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 940 bit 14 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 940 bit 15 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 940 bit 16 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 940 bit 17 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 940 bit 18 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 940 bit 19 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 940 bit 20 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 940 bit 21 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 940 bit 22 */
	bool tcuEnabled : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coils
	offset 940 bit 23 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 940 bit 24 */
	bool verboseCan2 : 1 {};
	/**
	offset 940 bit 25 */
	bool unusedBit_365_25 : 1 {};
	/**
	offset 940 bit 26 */
	bool unusedBit_365_26 : 1 {};
	/**
	offset 940 bit 27 */
	bool unusedBit_365_27 : 1 {};
	/**
	offset 940 bit 28 */
	bool unusedBit_365_28 : 1 {};
	/**
	offset 940 bit 29 */
	bool unusedBit_365_29 : 1 {};
	/**
	offset 940 bit 30 */
	bool unusedBit_365_30 : 1 {};
	/**
	offset 940 bit 31 */
	bool unusedBit_365_31 : 1 {};
	/**
	 * offset 944
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT] = {};
	/**
	 * offset 952
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * Time after ignition turn-off before the main relay is disabled.
	 * units: s
	 * offset 953
	 */
	uint8_t mainRelayDisableTime;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 954
	 */
	uint8_t alignmentFill_at_954[2] = {};
	/**
	 * offset 956
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	 * units: v
	 * offset 960
	 */
	uint8_t mc33_hvolt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 961
	 */
	uint8_t alignmentFill_at_961[1] = {};
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 962
	 */
	uint16_t minimumBoostClosedLoopMap;
	/**
	 * The percentage of ignition events to cut when entering the launch control window (e.g., at Launch RPM minus Launch Control Window).
	 * units: %
	 * offset 964
	 */
	int8_t initialIgnitionCutPercent;
	/**
	 * The percentage of ignition events to cut when the engine speed reaches the end of the corrections RPM (Launch RPM minus Launch Corrections End RPM). Between the start of the window and the end of corrections RPM, the cut percentage interpolates linearly from initial to final cut percentage.
	 * units: %
	 * offset 965
	 */
	int8_t finalIgnitionCutPercentBeforeLaunch;
	/**
	 * offset 966
	 */
	gppwm_channel_e boostOpenLoopYAxis;
	/**
	 * offset 967
	 */
	spi_device_e l9779spiDevice;
	/**
	 * offset 968
	 */
	imu_type_e imuType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 969
	 */
	uint8_t alignmentFill_at_969[1] = {};
	/**
	 * How far above idle speed do we consider idling, i.e. coasting detection threshold.
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	 * units: RPM
	 * offset 970
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	 * units: ms
	 * offset 972
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 974
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 976
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 978
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 980
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 982
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 983
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 984
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT] = {};
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 1000
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * Since torque reduction pin is usually shared with launch control, most people have an RPM where behavior under that is Launch Control, over that is Flat Shift/Torque Reduction
	 * units: rpm
	 * offset 1002
	 */
	uint16_t torqueReductionArmingRpm;
	/**
	 * offset 1004
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 1005
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	 * units: :1
	 * offset 1006
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	 * units: %
	 * offset 1007
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	 * units: hz
	 * offset 1008
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 1010
	 */
	adc_channel_e idlePositionChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1011
	 */
	uint8_t alignmentFill_at_1011[1] = {};
	/**
	 * The RPM difference below the Launch RPM at which corrections (timing retard interpolation and/or ignition cut ramp) reach their final/maximum target. For example, if Launch RPM is 4000, and this is 50, corrections reach their final target at 3950 RPM.
	 * units: RPM
	 * offset 1012
	 */
	uint16_t launchCorrectionsEndRpm;
	/**
	 * offset 1014
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 1016
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1017
	 */
	uint8_t alignmentFill_at_1017[1] = {};
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 1018
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 1020
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * units: RPM
	 * offset 1022
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionRestoreRpm;
	/**
	 * offset 1023
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	 * units: count
	 * offset 1024
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	 * units: %
	 * offset 1028
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * units: %
	 * offset 1030
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * Closed voltage for secondary throttle position sensor
	 * offset 1032
	 */
	tps_limit_t tps1SecondaryMin;
	/**
	 * Fully opened voltage for secondary throttle position sensor
	 * offset 1034
	 */
	tps_limit_t tps1SecondaryMax;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	 * units: Seconds
	 * offset 1036
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 1038
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 1040
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1041
	 */
	uint8_t alignmentFill_at_1041[1] = {};
	/**
	 * offset 1042
	 */
	output_pin_e acRelayPin;
	/**
	 * units: %
	 * offset 1044
	 */
	uint8_t lambdaProtectionMinTps;
	/**
	 * Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed
	 * units: s
	 * offset 1045
	 */
	scaled_channel<uint8_t, 10, 1> lambdaProtectionTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1046
	 */
	uint8_t alignmentFill_at_1046[2] = {};
	/**
	 * offset 1048
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT] = {};
	/**
	 * offset 1080
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 1082
	 */
	Gpio spi1misoPin;
	/**
	 * offset 1084
	 */
	Gpio spi1sckPin;
	/**
	 * offset 1086
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 1088
	 */
	Gpio spi2misoPin;
	/**
	 * offset 1090
	 */
	Gpio spi2sckPin;
	/**
	 * offset 1092
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 1094
	 */
	Gpio spi3misoPin;
	/**
	 * offset 1096
	 */
	Gpio spi3sckPin;
	/**
	 * UNUSED
	 * Will remove in 2026 for sure
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 1098
	 */
	Gpio cdmInputPin;
	/**
	 * offset 1100
	 */
	uart_device_e consoleUartDevice;
	/**
	 * offset 1101
	 */
	can_broadcast_channel_e canBroadcastUseChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1102
	 */
	uint8_t alignmentFill_at_1102[2] = {};
	/**
	 * offset 1104
	 */
	dc_io etbIo[ETB_COUNT] = {};
	/**
	 * offset 1120
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 1122
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 1124
	 */
	pid_s boostPid;
	/**
	 * offset 1144
	 */
	boostType_e boostType;
	/**
	 * offset 1145
	 */
	pin_input_mode_e ignitionKeyDigitalPinMode;
	/**
	 * offset 1146
	 */
	switch_input_pin_e ignitionKeyDigitalPin;
	/**
	 * units: Hz
	 * offset 1148
	 */
	int boostPwmFrequency;
	/**
	 * offset 1152
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1153
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * offset 1154
	 */
	cranking_condition_e crankingCondition;
	/**
	 * TPS history window for additive AE. The detector selects an adjacent-sample step within this window. A longer window keeps an older step eligible longer. Zero disables additive AE. MAP Prediction derives its history from the duration curve. Lua ignores this setting.
	 * units: sec
	 * offset 1155
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Deceleration is detected when the largest signed TPS change between adjacent samples in the history is below the negative of this threshold. Units are TPS percentage points per sample (50 ms). A zero or positive change elsewhere in the history can mask a throttle-closing step.
	 * units: %/sample
	 * offset 1156
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Scale fuel reduction from the selected TPS closing step in additive modes. Larger values increase reduction. RPM and flex multipliers also apply. Fractional-pump processing can suppress the reduction. This setting is independent of DFCO and ignored by MAP Prediction and Lua.
	 * units: coeff
	 * offset 1160
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * Select transient enrichment strategy. MS Adder uses milliseconds. Percent Adder uses fractions. MAP Prediction corrects Speed Density MAP. Lua supplies extra g/cylinder/cycle with setAccelEnrichment(grams), refreshed before 200 ms. Wall wetting is independent.
	 * offset 1164
	 */
	accel_enrichment_mode_e accelEnrichmentMode;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	 * units: sec
	 * offset 1165
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Pause closed loop fueling after acceleration fuel occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel accel.
	 * units: sec
	 * offset 1166
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterAccelTime;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1167
	 */
	uint8_t alignmentFill_at_1167[1] = {};
	/**
	 * Launch disabled above this speed if setting is above zero
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 1168
	 */
	int launchSpeedThreshold;
	/**
	 * The RPM window before the Launch RPM where launch control strategies (like retard/cut) begin to activate. For example, if Launch RPM is 4000 and Window is 500, activation starts at 3500 RPM.
	 * units: RPM
	 * offset 1172
	 */
	int launchRpmWindow;
	/**
	 * units: ms
	 * offset 1176
	 */
	float triggerEventsTimeoutMs;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '100%' means no filtering, 98% would be some filtering.
	 * units: percent
	 * offset 1180
	 */
	float ppsExpAverageAlpha;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 1184
	 */
	float mapExpAverageAlpha;
	/**
	 * offset 1188
	 */
	float magicNumberAvailableForDevTricks;
	/**
	 * offset 1192
	 */
	float turbochargerFilter;
	/**
	 * offset 1196
	 */
	int launchTpsThreshold;
	/**
	 * offset 1200
	 */
	float launchActivateDelay;
	/**
	 * offset 1204
	 */
	stft_s stft;
	/**
	 * offset 1232
	 */
	ltft_s ltft;
	/**
	 * offset 1248
	 */
	dc_io stepperDcIo[DC_PER_STEPPER] = {};
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1264
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1296
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1328
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1360
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT] = {};
	/**
	 * offset 1372
	 */
	dc_function_e etbFunctions[ETB_COUNT] = {};
	/**
	 * offset 1374
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1375
	 */
	uint8_t alignmentFill_at_1375[1] = {};
	/**
	 * offset 1376
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1378
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1379
	 */
	uint8_t alignmentFill_at_1379[1] = {};
	/**
	 * offset 1380
	 */
	Gpio drv8860_miso;
	/**
	 * offset 1382
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1398
	 */
	uint8_t alignmentFill_at_1398[2] = {};
	/**
	 * Angle between cam sensor and VVT zero position
	 * units: value
	 * offset 1400
	 */
	float vvtOffsets[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1416
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT] = {};
	/**
	 * offset 1464
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS] = {};
	/**
	 * Closed voltage for secondary throttle position sensor
	 * offset 1528
	 */
	tps_limit_t tps2SecondaryMin;
	/**
	 * Fully opened voltage for secondary throttle position sensor
	 * offset 1530
	 */
	tps_limit_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1532 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1532 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Write SD card log even when powered by USB
	offset 1532 bit 2 */
	bool alwaysWriteSdCard : 1 {};
	/**
	 * Second harmonic (aka double) is usually quieter background noise
	offset 1532 bit 3 */
	bool knockDetectionUseDoubleFrequency : 1 {};
	/**
	 * Unlocking only via rusEFI console using 'unlock PICODEBUG' command. Use 'reset to default firmware' if pincode is lost.
	offset 1532 bit 4 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1532 bit 5 */
	bool silentTriggerError : 1 {};
	/**
	offset 1532 bit 6 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1532 bit 7 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write. See also can1ListenMode
	offset 1532 bit 8 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1532 bit 9 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1532 bit 10 */
	bool enableOilPressureProtect : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1532 bit 11 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1532 bit 12 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1532 bit 13 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1532 bit 14 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	 * When enabled, this option cuts the fuel supply when the RPM limit is reached. Cutting fuel provides a smoother limiting action; however, it may lead to slightly higher combustion chamber temperatures since unburned fuel is not present to cool the combustion process.
	offset 1532 bit 15 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * When selected, this option cuts the spark to limit RPM. Cutting spark can produce flames from the exhaust due to unburned fuel igniting in the exhaust system. Additionally, this unburned fuel can help cool the combustion chamber, which may be beneficial in high-performance applications.
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1532 bit 16 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1532 bit 17 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * Enables or disables ignition/spark cut during launch control.
	offset 1532 bit 18 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1532 bit 19 */
	bool torqueReductionEnabled : 1 {};
	/**
	 * When we sync cam sensor is that first or second full engine revolution of the four stroke cycle?
	offset 1532 bit 20 */
	bool camSyncOnSecondCrankRevolution : 1 {};
	/**
	offset 1532 bit 21 */
	bool limitTorqueReductionTime : 1 {};
	/**
	 * Are you a developer troubleshooting TS over CAN ISO/TP?
	offset 1532 bit 22 */
	bool verboseIsoTp : 1 {};
	/**
	 * In this mode only trigger events go into engine sniffer and not coils/injectors etc
	offset 1532 bit 23 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1532 bit 24 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1532 bit 25 */
	bool skippedWheelOnCam : 1 {};
	/**
	offset 1532 bit 26 */
	bool unusedBit_513_26 : 1 {};
	/**
	offset 1532 bit 27 */
	bool unusedBit_513_27 : 1 {};
	/**
	offset 1532 bit 28 */
	bool unusedBit_513_28 : 1 {};
	/**
	offset 1532 bit 29 */
	bool unusedBit_513_29 : 1 {};
	/**
	offset 1532 bit 30 */
	bool unusedBit_513_30 : 1 {};
	/**
	offset 1532 bit 31 */
	bool unusedBit_513_31 : 1 {};
	/**
	 * A/C button input
	 * offset 1536
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1538
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	 * units: %
	 * offset 1539
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1540
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1541
	 */
	uint8_t alignmentFill_at_1541[3] = {};
	/**
	offset 1544 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1544 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1544 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1544 bit 3 */
	bool complexWallModel : 1 {};
	/**
	 * RPM is measured based on last 720 degrees while instant RPM is measured based on the last 90 degrees of crank revolution
	offset 1544 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1544 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra advance at low idle speeds will prevent stalling and extra retard at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1544 bit 6 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1544 bit 7 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1544 bit 8 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 1544 bit 9 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	offset 1544 bit 10 */
	bool hondaK : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1544 bit 11 */
	bool twoWireBatchIgnition : 1 {};
	/**
	 * Read MAP sensor on ECU start-up to use as baro value.
	offset 1544 bit 12 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1544 bit 13 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT and PID idle).
	 * You probably don't need this.
	offset 1544 bit 14 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable flex-fuel compensation for engine start. When on (and a flex fuel sensor is present) the cranking coolant multiplier and the priming pulse mass each come from a 2D table over coolant and ethanol % (crankingFuelFlexTable / primeFlexTable, 4-row ethanol axis) instead of their 1D coolant curves. When off, the 1D curves (crankingFuelCoef / primeValues) are used.
	offset 1544 bit 15 */
	bool flexCranking : 1 {};
	/**
	 * Shared CLT/ethanol compensation for additive TPS AE and wall-film tau/beta. AE multiplier applies only to MS/Percent Adder. tau/beta apply in every AE strategy and Basic/Advanced. Requires a registered ethanol sensor. Does not control base stoichiometry, cranking or priming.
	offset 1544 bit 16 */
	bool flexFuelTransientComp : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1544 bit 17 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1544 bit 18 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Gradually interpolates the ignition timing from the base timing table value down to the target "Absolute Timing at Launch" value, starting from the beginning of the launch window.
	offset 1544 bit 19 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1544 bit 20 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1544 bit 21 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors.
	 * WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1544 bit 22 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1544 bit 23 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1544 bit 24 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1544 bit 25 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	offset 1544 bit 26 */
	bool stepper_dc_use_two_wires : 1 {};
	/**
	offset 1544 bit 27 */
	bool watchOutForLinearTime : 1 {};
	/**
	 * Only write the SD log while trigger conditions are met (start/stop). Off = always log, the current behavior.
	offset 1544 bit 28 */
	bool sdCardConditionalLogging : 1 {};
	/**
	 * Compensated MAP: in Speed Density mode, normalize MAP by barometric pressure before it is used as a table load axis.
	 * MAP_ref = MAP / (baro / 101.325 kPa) feeds the VE lookup and the fuel/spark load axes, so the same table cells are hit regardless of altitude (WOT reads ~100 kPa at any elevation).
	 * The physical air mass calculation still uses actual MAP. Requires a barometric pressure sensor; without a valid baro reading no compensation is applied.
	 * Works together with the Barometric pressure correction table, which serves a different goal: this setting keeps table lookups stable across altitude, while the baro table multiplies fueling for exhaust-side scavenging effects. Either or both can be used.
	offset 1544 bit 29 */
	bool useCompensatedMap : 1 {};
	/**
	offset 1544 bit 30 */
	bool unusedBit_554_30 : 1 {};
	/**
	offset 1544 bit 31 */
	bool unusedBit_554_31 : 1 {};
	/**
	 * Start logging at/above this RPM
	 * units: rpm
	 * offset 1548
	 */
	uint16_t sdLogStartRpm;
	/**
	 * Stop logging below this RPM. Set below 'start' for hysteresis
	 * units: rpm
	 * offset 1550
	 */
	uint16_t sdLogStopRpm;
	/**
	 * Keep logging this many seconds after RPM drops below the stop threshold
	 * units: sec
	 * offset 1552
	 */
	uint8_t sdLogStopDelay;
	/**
	 * Also require TPS at/above this to start logging (0 = ignore)
	 * units: %
	 * offset 1553
	 */
	uint8_t sdLogMinTps;
	/**
	 * Also require MAP at/above this to start logging (0 = ignore)
	 * units: kPa
	 * offset 1554
	 */
	uint16_t sdLogMinMap;
	/**
	 * Also require vehicle speed at/above this to start logging (0 = ignore)
	 * units: kph
	 * offset 1556
	 */
	uint8_t sdLogMinVss;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1557
	 */
	uint8_t alignmentFill_at_1557[1] = {};
	/**
	 * Optional toggle button to start/stop logging (press on, press off)
	 * offset 1558
	 */
	switch_input_pin_e sdLogTriggerPin;
	/**
	 * offset 1560
	 */
	pin_input_mode_e sdLogTriggerPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1561
	 */
	uint8_t alignmentFill_at_1561[3] = {};
	/**
	 * units: count
	 * offset 1564
	 */
	uint32_t engineChartSize;
	/**
	 * units: mult
	 * offset 1568
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * Idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	 * units: RPM
	 * offset 1572
	 */
	int16_t acIdleRpmTarget;
	/**
	 * set warningPeriod X
	 * units: seconds
	 * offset 1574
	 */
	int16_t warningPeriod;
	/**
	 * units: angle
	 * offset 1576
	 */
	float knockDetectionWindowStart;
	/**
	 * units: ms
	 * offset 1580
	 */
	float idleStepperReactionTime;
	/**
	 * units: count
	 * offset 1584
	 */
	int idleStepperTotalSteps;
	/**
	 * Pedal position to realize that we need to reduce torque when the trigger pin is triggered
	 * offset 1588
	 */
	int torqueReductionArmingApp;
	/**
	 * Reference Torque value
	 * units: Nm
	 * offset 1592
	 */
	float referenceTorqueForGenerator;
	/**
	 * kPa/psi value at which Reference Torque is archived
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1596
	 */
	float referenceMapForGenerator;
	/**
	 * offset 1600
	 */
	float referenceVeForGenerator;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1604
	 */
	float tachPulseDuractionMs;
	/**
	 * Evaporation time constant for the Basic fuel-film model, in seconds. Higher values make the film respond more slowly. This is an exponential time constant, not the time for all deposited fuel to evaporate. Effective values below 0.01 bypass compensation.
	 * units: Seconds
	 * offset 1608
	 */
	float wwaeTau;
	/**
	 * offset 1612
	 */
	pid_s alternatorControl;
	/**
	 * offset 1632
	 */
	pid_s etb;
	/**
	 * RPM range above upper limit for extra air taper
	 * units: RPM
	 * offset 1652
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1654
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed voltage for primary throttle position sensor
	 * offset 1656
	 */
	tps_limit_t tps2Min;
	/**
	 * Fully opened voltage for primary throttle position sensor
	 * offset 1658
	 */
	tps_limit_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1660
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1662
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1663
	 */
	uint8_t alignmentFill_at_1663[1] = {};
	/**
	 * offset 1664
	 */
	Gpio mc33816_flag0;
	/**
	 * offset 1666
	 */
	scaled_channel<uint16_t, 1000, 1> tachPulsePerRev;
	/**
	 * kPa/psi value which is too low to be true
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1668
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa/psi value which is too high to be true
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1672
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	 * units: ms
	 * offset 1676
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	 * units: ms
	 * offset 1678
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1680
	 */
	pid_s idleRpmPid;
	/**
	 * Fraction of injected fuel deposited on the wall in the Basic model. 0.20 represents 20%. Larger values increase modeled deposition. Effective values below 0.01 bypass compensation. Downstream controller limits still apply.
	 * units: Fraction
	 * offset 1700
	 */
	float wwaeBeta;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1704
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1706
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1708
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT] = {};
	/**
	 * offset 1712
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1714
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * units: volts
	 * offset 1716
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * units: volts
	 * offset 1720
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	 * units: seconds
	 * offset 1724
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * larger value = larger intake manifold volume
	 * offset 1726
	 */
	uint16_t mafFilterParameter;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	 * units: RPM
	 * offset 1728
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1730
	 */
	uint8_t alignmentFill_at_1730[2] = {};
	/**
	 * See Over/Undervoltage Shutdown/Retry bit in documentation
	offset 1732 bit 0 */
	bool mc33810DisableRecoveryMode : 1 {};
	/**
	offset 1732 bit 1 */
	bool mc33810Gpgd0Mode : 1 {};
	/**
	offset 1732 bit 2 */
	bool mc33810Gpgd1Mode : 1 {};
	/**
	offset 1732 bit 3 */
	bool mc33810Gpgd2Mode : 1 {};
	/**
	offset 1732 bit 4 */
	bool mc33810Gpgd3Mode : 1 {};
	/**
	 * Send out board statistics
	offset 1732 bit 5 */
	bool enableExtendedCanBroadcast : 1 {};
	/**
	 * global_can_data performance hack
	offset 1732 bit 6 */
	bool luaCanRxWorkaround : 1 {};
	/**
	offset 1732 bit 7 */
	bool flexSensorInverted : 1 {};
	/**
	offset 1732 bit 8 */
	bool useHardSkipInTraction : 1 {};
	/**
	 * Use Aux Speed 1 as one of speeds for wheel slip ratio?
	offset 1732 bit 9 */
	bool useAuxSpeedForSlipRatio : 1 {};
	/**
	 * VSS and Aux Speed 1 or Aux Speed 1 with Aux Speed 2?
	offset 1732 bit 10 */
	bool useVssAsSecondWheelSpeed : 1 {};
	/**
	offset 1732 bit 11 */
	bool is_enabled_spi_5 : 1 {};
	/**
	offset 1732 bit 12 */
	bool is_enabled_spi_6 : 1 {};
	/**
	 * AEM X-Series EGT gauge kit or rusEFI EGT sensor from Wideband controller
	offset 1732 bit 13 */
	bool enableAemXSeriesEgt : 1 {};
	/**
	offset 1732 bit 14 */
	bool startRequestPinInverted : 1 {};
	/**
	offset 1732 bit 15 */
	bool tcu_rangeSensorPulldown : 1 {};
	/**
	offset 1732 bit 16 */
	bool devBit01 : 1 {};
	/**
	offset 1732 bit 17 */
	bool devBit0 : 1 {};
	/**
	offset 1732 bit 18 */
	bool devBit1 : 1 {};
	/**
	offset 1732 bit 19 */
	bool devBit2 : 1 {};
	/**
	offset 1732 bit 20 */
	bool devBit3 : 1 {};
	/**
	offset 1732 bit 21 */
	bool devBit4 : 1 {};
	/**
	offset 1732 bit 22 */
	bool devBit5 : 1 {};
	/**
	offset 1732 bit 23 */
	bool devBit6 : 1 {};
	/**
	offset 1732 bit 24 */
	bool devBit7 : 1 {};
	/**
	offset 1732 bit 25 */
	bool invertExhaustCamVVTSignal : 1 {};
	/**
	 * "Available via TS Plugin see https://rusefi.com/s/knock"
	offset 1732 bit 26 */
	bool enableKnockSpectrogram : 1 {};
	/**
	offset 1732 bit 27 */
	bool enableKnockSpectrogramFilter : 1 {};
	/**
	offset 1732 bit 28 */
	bool unusedBit_635_28 : 1 {};
	/**
	offset 1732 bit 29 */
	bool unusedBit_635_29 : 1 {};
	/**
	offset 1732 bit 30 */
	bool unusedBit_635_30 : 1 {};
	/**
	offset 1732 bit 31 */
	bool unusedBit_635_31 : 1 {};
	/**
	 * This value is an added for base idle value. Idle Value added when coasting and transitioning into idle.
	 * units: percent
	 * offset 1736
	 */
	int16_t iacByTpsTaper;
	/**
	 * offset 1738
	 */
	Gpio accelerometerCsPin;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 1740
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 1741
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1742
	 */
	uint8_t alignmentFill_at_1742[2] = {};
	/**
	 * Acceleration is detected when the largest signed TPS change between adjacent samples in the history exceeds this threshold. Units are TPS percentage points per sample (50 ms), not percent per second or total change over the lookback window. A change from 10% to 20% is 10 percentage points.
	 * units: %/sample
	 * offset 1744
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1748
	 */
	brain_input_pin_e auxSpeedSensorInputPin[AUX_SPEED_SENSOR_COUNT] = {};
	/**
	 * offset 1752
	 */
	uint8_t totalGearsCount;
	/**
	 * Selects which point of the nominal injector pulse follows the Injection Timing table: End, Start, or Center.
	 * Table angles are crank degrees relative to each cylinder's compression TDC. Negative = before TDC, positive = after TDC.
	 * End/Center modes advance the start by the full/half pulse duration. See Injection Timing table help for examples.
	 * offset 1753
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1754
	 */
	debug_mode_e debugMode;
	/**
	 * Additional idle % when fan #1 is active
	 * units: %
	 * offset 1755
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * Band rate for primary TTL
	 * units: BPs
	 * offset 1756
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * units: volts
	 * offset 1760
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	 * units: volts
	 * offset 1764
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * offset 1768
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1769
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1770
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1771
	 */
	load_override_e afrOverrideMode;
	/**
	 * units: A
	 * offset 1772
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	 * units: A
	 * offset 1773
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	 * units: us
	 * offset 1774
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	 * units: ms
	 * offset 1775
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1776 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (default, cc/min) or mass flow (g/s).
	offset 1776 bit 1 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1776 bit 2 */
	bool kLineDoHondaSend : 1 {};
	/**
	 * ListenMode is about acknowledging CAN traffic on the protocol level. Different from canWriteEnabled
	offset 1776 bit 3 */
	bool can1ListenMode : 1 {};
	/**
	offset 1776 bit 4 */
	bool can2ListenMode : 1 {};
	/**
	offset 1776 bit 5 */
	bool unusedBit_666_5 : 1 {};
	/**
	offset 1776 bit 6 */
	bool unusedBit_666_6 : 1 {};
	/**
	offset 1776 bit 7 */
	bool unusedBit_666_7 : 1 {};
	/**
	offset 1776 bit 8 */
	bool unusedBit_666_8 : 1 {};
	/**
	offset 1776 bit 9 */
	bool unusedBit_666_9 : 1 {};
	/**
	offset 1776 bit 10 */
	bool unusedBit_666_10 : 1 {};
	/**
	offset 1776 bit 11 */
	bool unusedBit_666_11 : 1 {};
	/**
	offset 1776 bit 12 */
	bool unusedBit_666_12 : 1 {};
	/**
	offset 1776 bit 13 */
	bool unusedBit_666_13 : 1 {};
	/**
	offset 1776 bit 14 */
	bool unusedBit_666_14 : 1 {};
	/**
	offset 1776 bit 15 */
	bool unusedBit_666_15 : 1 {};
	/**
	offset 1776 bit 16 */
	bool unusedBit_666_16 : 1 {};
	/**
	offset 1776 bit 17 */
	bool unusedBit_666_17 : 1 {};
	/**
	offset 1776 bit 18 */
	bool unusedBit_666_18 : 1 {};
	/**
	offset 1776 bit 19 */
	bool unusedBit_666_19 : 1 {};
	/**
	offset 1776 bit 20 */
	bool unusedBit_666_20 : 1 {};
	/**
	offset 1776 bit 21 */
	bool unusedBit_666_21 : 1 {};
	/**
	offset 1776 bit 22 */
	bool unusedBit_666_22 : 1 {};
	/**
	offset 1776 bit 23 */
	bool unusedBit_666_23 : 1 {};
	/**
	offset 1776 bit 24 */
	bool unusedBit_666_24 : 1 {};
	/**
	offset 1776 bit 25 */
	bool unusedBit_666_25 : 1 {};
	/**
	offset 1776 bit 26 */
	bool unusedBit_666_26 : 1 {};
	/**
	offset 1776 bit 27 */
	bool unusedBit_666_27 : 1 {};
	/**
	offset 1776 bit 28 */
	bool unusedBit_666_28 : 1 {};
	/**
	offset 1776 bit 29 */
	bool unusedBit_666_29 : 1 {};
	/**
	offset 1776 bit 30 */
	bool unusedBit_666_30 : 1 {};
	/**
	offset 1776 bit 31 */
	bool unusedBit_666_31 : 1 {};
	/**
	 * Angle of tooth detection within engine phase cycle
	 * units: angle
	 * offset 1780
	 */
	uint16_t camDecoder2jzPosition;
	/**
	 * offset 1782
	 */
	mc33810maxDwellTimer_e mc33810maxDwellTimer;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1783
	 */
	uint8_t alignmentFill_at_1783[1] = {};
	/**
	 * Duration of each test pulse
	 * units: ms
	 * offset 1784
	 */
	scaled_channel<uint16_t, 100, 1> benchTestOnTime;
	/**
	 * units: %
	 * offset 1786
	 */
	uint8_t lambdaProtectionRestoreTps;
	/**
	 * units: %
	 * offset 1787
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionRestoreLoad;
	/**
	 * offset 1788
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1789
	 */
	uint8_t alignmentFill_at_1789[1] = {};
	/**
	 * offset 1790
	 */
	Gpio can2TxPin;
	/**
	 * offset 1792
	 */
	Gpio can2RxPin;
	/**
	 * offset 1794
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1795
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1796
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1797
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1798
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1800
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1801
	 */
	uint8_t alignmentFill_at_1801[3] = {};
	/**
	 * Specifies the boost pressure allowed before triggering a cut. Setting this to 0 will DISABLE overboost cut.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1804
	 */
	float boostCutPressure;
	/**
	 * units: kg/h
	 * offset 1808
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16] = {};
	/**
	 * units: ratio
	 * offset 1824
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16] = {};
	/**
	 * Fixed timing, useful for TDC testing
	 * units: deg
	 * offset 1840
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1844
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1848
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * units: value
	 * offset 1852
	 */
	float egoValueShift;
	/**
	 * VVT output solenoid pin for this cam
	 * offset 1856
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1864
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMinTps;
	/**
	 * offset 1865
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMaxTps;
	/**
	 * offset 1866
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMinTps;
	/**
	 * offset 1867
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMaxTps;
	/**
	 * offset 1868
	 */
	pwm_freq_t vvtOutputFrequency;
	/**
	 * Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.
	 * units: deg BTDC
	 * offset 1870
	 */
	int8_t minimumIgnitionTiming;
	/**
	 * Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC
	 * units: deg BTDC
	 * offset 1871
	 */
	int8_t maximumIgnitionTiming;
	/**
	 * units: Hz
	 * offset 1872
	 */
	int alternatorPwmFrequency;
	/**
	 * offset 1876
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK] = {};
	/**
	 * Additional idle % when fan #2 is active
	 * units: %
	 * offset 1878
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	 * units: sec
	 * offset 1879
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1880
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT] = {};
	/**
	 * offset 1888
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 1912
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1913
	 */
	pin_output_mode_e accelerometerCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 *  HPFP fuel mass compensation = manual mode for GDI engines
	 * offset 1914
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1915
	 */
	uint8_t alignmentFill_at_1915[1] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa/50.7psi here.
	 * This is gauge pressure/in reference to atmospheric.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1916
	 */
	float fuelReferencePressure;
	/**
	 * offset 1920
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1952
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * units: Deg
	 * offset 1984
	 */
	int16_t knockSamplingDuration;
	/**
	 * units: Hz
	 * offset 1986
	 */
	int16_t etbFreq;
	/**
	 * offset 1988
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 2008
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	 * units: %
	 * offset 2009
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	 * units: %
	 * offset 2010
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 2011
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder ignition and fueling timing correction for uneven engines
	 * units: deg
	 * offset 2012
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: seconds
	 * offset 2060
	 */
	float idlePidActivationTime;
	/**
	 * Minimum coolant temperature to activate VVT
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 2064
	 */
	int16_t vvtControlMinClt;
	/**
	 * offset 2066
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 2067
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 2068
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 2069
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 2070
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 2071
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 2072
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 2073
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 2074
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 2075
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 2076
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 2078
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 2080
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2082
	 */
	uint8_t alignmentFill_at_2082[2] = {};
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 2084
	 */
	pid_s auxPid[CAMS_PER_BANK] = {};
	/**
	 * offset 2124
	 */
	float injectorCorrectionPolynomial[8] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 2156
	 */
	scaled_channel<int16_t, 1, 1> primeBins[PRIME_CURVE_COUNT] = {};
	/**
	 * offset 2172
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2192
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2193
	 */
	uint8_t alignmentFill_at_2193[1] = {};
	/**
	 * offset 2194
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2196
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2198
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2199
	 */
	uint8_t alignmentFill_at_2199[1] = {};
	/**
	 * offset 2200
	 */
	Gpio mc33816_cs;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2202
	 */
	uint8_t alignmentFill_at_2202[2] = {};
	/**
	 * units: hz
	 * offset 2204
	 */
	float auxFrequencyFilter;
	/**
	 * offset 2208
	 */
	sent_input_pin_e sentInputPins[SENT_INPUT_COUNT] = {};
	/**
	 * This sets the RPM above which fuel cut is active.
	 * units: rpm
	 * offset 2210
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	 * units: rpm
	 * offset 2212
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	 * units: %
	 * offset 2214
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 2216
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * units: %
	 * offset 2218
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 2220
	 */
	int16_t coastingFuelCutMap;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2222
	 */
	uint8_t alignmentFill_at_2222[2] = {};
	/**
	 * offset 2224
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2244
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * offset 2264
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT] = {};
	/**
	 * offset 2360
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT] = {};
	/**
	 * offset 2424
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT] = {};
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2552
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2556
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	 * units: kg/h
	 * offset 2560
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2564
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2568
	 */
	float tChargeAirDecrLimit;
	/**
	 * iTerm min value
	 * offset 2572
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2574
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2576
	 */
	pid_s idleTimingPid;
	/**
	 * When entering idle, and the PID settings are aggressive, it's good to make a soft entry upon entering closed loop
	 * offset 2596
	 */
	float idleTimingSoftEntryTime;
	/**
	 * offset 2600
	 */
	pin_input_mode_e torqueReductionTriggerPinMode;
	/**
	 * offset 2601
	 */
	torqueReductionActivationMode_e torqueReductionActivationMode;
	/**
	 * Spread each additive enrichment portion evenly across this number of engine cycles. Higher values reduce delivery per cycle and spread it longer. Period 1 and divisor 1 disable fractional spreading. Ignored by MAP Prediction and Lua.
	 * units: cycles
	 * offset 2602
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * Divide accumulated additive enrichment into diminishing portions. 1 delivers a full portion. Larger values reduce the initial delivery and extend the tail. With period 1 and divisor 2: 1/2, 1/4, 1/8. Ignored by MAP Prediction and Lua.
	 * units: coef
	 * offset 2604
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2608
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2609
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2610
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2612
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	 * units: :1
	 * offset 2613
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2614
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position. Can also be interpreted as the maximum allowed TPS% Opening for Idle Control.
	 * units: %
	 * offset 2616
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2620
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: mg
	 * offset 2632
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[PRIME_CURVE_COUNT] = {};
	/**
	 * Ethanol % axis (Y) for primeFlexTable.
	 * units: %
	 * offset 2640
	 */
	uint8_t primeFlexBins[PRIME_FLEX_SIZE] = {};
	/**
	 * Priming pulse fuel mass as a function of coolant (X axis, shared primeBins) and ethanol % (Y axis, primeFlexBins). Used instead of primeValues when flexCranking is enabled and a flex sensor is present.
	 * units: mg
	 * offset 2644
	 */
	scaled_channel<uint8_t, 1, 5> primeFlexTable[PRIME_FLEX_SIZE][PRIME_CURVE_COUNT] = {};
	/**
	 * Trigger comparator center point voltage
	 * units: V
	 * offset 2676
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	 * units: V
	 * offset 2677
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	 * units: V
	 * offset 2678
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	 * units: RPM
	 * offset 2679
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * units: ratio
	 * offset 2680
	 */
	scaled_channel<uint16_t, 100, 1> tractionControlSlipBins[TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * units: RPM
	 * offset 2692
	 */
	uint8_t tractionControlSpeedBins[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE] = {};
	/**
	 * offset 2698
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2699
	 */
	uint8_t alignmentFill_at_2699[1] = {};
	/**
	 * offset 2700
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS] = {};
	/**
	 * Boost Current
	 * units: mA
	 * offset 3132
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	 * units: mA
	 * offset 3134
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	 * units: mA
	 * offset 3136
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	 * units: us
	 * offset 3138
	 */
	uint16_t mc33_t_max_boost;
	/**
	 * units: us
	 * offset 3140
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	 * units: us
	 * offset 3142
	 */
	uint16_t mc33_t_peak_tot;
	/**
	 * units: us
	 * offset 3144
	 */
	uint16_t mc33_t_bypass;
	/**
	 * units: us
	 * offset 3146
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	 * units: us
	 * offset 3148
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 3150
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 3151
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 3152
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 3153
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3159
	 */
	uint8_t alignmentFill_at_3159[1] = {};
	/**
	 * units: ratio
	 * offset 3160
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3232
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH] = {};
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	 * units: rpm
	 * offset 3304
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	 * units: sec
	 * offset 3305
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	 * units: sec
	 * offset 3306
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * offset 3307
	 */
	tChargeMode_e tChargeMode;
	/**
	 * units: mg
	 * offset 3308
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	 * Threshold in ETB error (target vs. actual) above which the jam timer is started. If the timer reaches the time specified in the jam detection timeout period, the throttle is considered jammed, and engine operation limited.
	 * units: %
	 * offset 3310
	 */
	uint8_t etbJamDetectThreshold;
	/**
	 * units: lobes/cam
	 * offset 3311
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 3312
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Low engine speed for A/C. Larger engines can survive lower values
	 * units: RPM
	 * offset 3313
	 */
	scaled_channel<int8_t, 1, 10> acLowRpmLimit;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	 * units: deg
	 * offset 3314
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3315
	 */
	uint8_t alignmentFill_at_3315[1] = {};
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	 * units: cc
	 * offset 3316
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	 * https://rusefi.com/forum/viewtopic.php?t=2192
	 * units: deg
	 * offset 3318
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 3319
	 */
	uint8_t issFilterReciprocal;
	/**
	 * units: %/kPa
	 * offset 3320
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	 * units: %/kPa/lobe
	 * offset 3322
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * iTerm min value
	 * offset 3324
	 */
	int16_t hpfpPid_iTermMin;
	/**
	 * iTerm max value
	 * offset 3326
	 */
	int16_t hpfpPid_iTermMax;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	 * units: kPa/s
	 * offset 3328
	 */
	uint16_t hpfpTargetDecay;
	/**
	 * offset 3330
	 */
	output_pin_e stepper_raw_output[4] = {};
	/**
	 * units: ratio
	 * offset 3338
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[TCU_GEAR_COUNT] = {};
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	 * units: ms
	 * offset 3358
	 */
	uint16_t vvtActivationDelayMs;
	/**
	 * offset 3360
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3361
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * During revolution where ACR should be disabled at what specific angle to disengage
	 * units: deg
	 * offset 3362
	 */
	uint16_t acrDisablePhase;
	/**
	 * offset 3364
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3384
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 3404
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 3406
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3407
	 */
	uint8_t alignmentFill_at_3407[1] = {};
	/**
	 * offset 3408
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 3410
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3411
	 */
	uint8_t alignmentFill_at_3411[1] = {};
	/**
	 * offset 3412
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 3414
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 3416
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3417
	 */
	uint8_t alignmentFill_at_3417[1] = {};
	/**
	 * offset 3418
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 3420
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 3422
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3423
	 */
	uint8_t alignmentFill_at_3423[1] = {};
	/**
	 * offset 3424
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * offset 3426
	 */
	output_pin_e acrPin2;
	/**
	 * Set a minimum allowed target position to avoid slamming/driving against the hard mechanical stop in the throttle.
	 * units: %
	 * offset 3428
	 */
	scaled_channel<uint8_t, 10, 1> etbMinimumPosition;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3429
	 */
	uint8_t alignmentFill_at_3429[1] = {};
	/**
	 * offset 3430
	 */
	uint16_t tuneHidingKey;
	/**
	 * Individual characters are accessible using vin(index) Lua function
	 * offset 3432
	 */
	vin_number_t vinNumber;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 3449
	 */
	int8_t torqueReductionActivationTemperature;
	/**
	 * Absolute: Sensor reads ~100 kPa (14.7 psi) with engine off and no fuel pressure.
	 * Gauge: Sensor reads 0 with engine off and no fuel pressure (most common standard 0-10 bar / 0-150 psi sensors).
	 * Differential: Sensor is connected to intake manifold vacuum and measures pressure difference directly.
	 * offset 3450
	 */
	fuel_pressure_sensor_mode_e fuelPressureSensorMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3451
	 */
	uint8_t alignmentFill_at_3451[1] = {};
	/**
	 * offset 3452
	 */
	switch_input_pin_e luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * units: rpm
	 * offset 3468
	 */
	int16_t ALSMinRPM;
	/**
	 * units: rpm
	 * offset 3470
	 */
	int16_t ALSMaxRPM;
	/**
	 * units: sec
	 * offset 3472
	 */
	int16_t ALSMaxDuration;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 3474
	 */
	int8_t ALSMinCLT;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 3475
	 */
	int8_t ALSMaxCLT;
	/**
	 * offset 3476
	 */
	uint8_t alsMinTimeBetween;
	/**
	 * offset 3477
	 */
	uint8_t alsEtbPosition;
	/**
	 * units: %
	 * offset 3478
	 */
	uint8_t acRelayAlternatorDutyAdder;
	/**
	 * If you have digital SENT TPS sensor please select type. For analog TPS leave None
	 * offset 3479
	 */
	SentEtbType sentEtbType;
	/**
	 * offset 3480
	 */
	uint16_t customSentTpsMin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3482
	 */
	uint8_t alignmentFill_at_3482[2] = {};
	/**
	 * units: %
	 * offset 3484
	 */
	int ALSIdleAdd;
	/**
	 * units: %
	 * offset 3488
	 */
	int ALSEtbAdd;
	/**
	 * offset 3492
	 */
	float ALSSkipRatio;
	/**
	 * Hysterisis: if Pressure High Disable is 240kpa, and acPressureEnableHyst is 20, when the ECU sees 240kpa, A/C will be disabled, and stay disabled until 240-20=220kpa is reached
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3496
	 */
	scaled_channel<uint8_t, 2, 1> acPressureEnableHyst;
	/**
	 * offset 3497
	 */
	pin_input_mode_e ALSActivatePinMode;
	/**
	 * For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	 * units: %
	 * offset 3498
	 */
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	/**
	 * For Toyota ETCS-i, use ~69%
	 * units: %
	 * offset 3499
	 */
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	/**
	 * offset 3500
	 */
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * offset 3508
	 */
	uint16_t customSentTpsMax;
	/**
	 * offset 3510
	 */
	uint16_t kLineBaudRate;
	/**
	 * offset 3512
	 */
	CanGpioType canGpioType;
	/**
	 * offset 3513
	 */
	UiMode uiMode;
	/**
	 * Crank angle ATDC of first lobe peak
	 * units: deg
	 * offset 3514
	 */
	int16_t hpfpPeakPos;
	/**
	 * units: us
	 * offset 3516
	 */
	int16_t kLinePeriodUs;
	/**
	 * Degrees of timing REMOVED from actual timing during soft RPM limit window
	 * units: deg
	 * offset 3518
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitTimingRetard;
	/**
	 * % of fuel ADDED during window
	 * units: %
	 * offset 3519
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitFuelAdded;
	/**
	 * Sets a buffer below the RPM hard limit, helping avoid rapid cycling of cut actions by defining a range within which RPM must drop before cut actions are re-enabled.
	 * Hysterisis: if the hard limit is 7200rpm and rpmHardLimitHyst is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached
	 * units: RPM
	 * offset 3520
	 */
	scaled_channel<uint8_t, 1, 10> rpmHardLimitHyst;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3521
	 */
	uint8_t alignmentFill_at_3521[1] = {};
	/**
	 * Time between bench test pulses
	 * units: ms
	 * offset 3522
	 */
	scaled_channel<uint16_t, 10, 1> benchTestOffTime;
	/**
	 * Defines a pressure range below the cut limit at which boost can resume, providing smoother control over boost cut actions.
	 * For example: if hard cut is 240kpa, and boost cut hysteresis is 20, when the ECU sees 240kpa, fuel/ign will cut, and stay cut until 240-20=220kpa is reached
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3524
	 */
	scaled_channel<uint8_t, 2, 1> boostCutPressureHyst;
	/**
	 * Boost duty cycle modified by gear
	 * units: %
	 * offset 3525
	 */
	scaled_channel<int8_t, 2, 1> gearBasedOpenLoopBoostAdder[TCU_GEAR_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3535
	 */
	uint8_t alignmentFill_at_3535[1] = {};
	/**
	 * How many test bench pulses do you want
	 * offset 3536
	 */
	uint32_t benchTestCount;
	/**
	 * How long initial idle adder is held before starting to decay.
	 * units: seconds
	 * offset 3540
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsHoldTime;
	/**
	 * How long it takes to remove initial IAC adder to return to normal idle.
	 * units: seconds
	 * offset 3541
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsDecayTime;
	/**
	 * offset 3542
	 */
	switch_input_pin_e tcu_rangeInput[RANGE_INPUT_COUNT] = {};
	/**
	 * offset 3554
	 */
	pin_input_mode_e tcu_rangeInputMode[RANGE_INPUT_COUNT] = {};
	/**
	 * Scale the reported vehicle speed value from CAN. Example: Parameter set to 1.1, CAN VSS reports 50kph, ECU will report 55kph instead.
	 * units: ratio
	 * offset 3560
	 */
	scaled_channel<uint16_t, 10000, 1> canVssScaling;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3562
	 */
	uint8_t alignmentFill_at_3562[2] = {};
	/**
	 * offset 3564
	 */
	ThermistorConf oilTempSensor;
	/**
	 * offset 3596
	 */
	ThermistorConf fuelTempSensor;
	/**
	 * offset 3628
	 */
	ThermistorConf ambientTempSensor;
	/**
	 * offset 3660
	 */
	ThermistorConf compressorDischargeTemperature;
	/**
	 * Place the sensor before the throttle, but after any turbocharger/supercharger and intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3692
	 */
	adc_channel_e throttleInletPressureChannel;
	/**
	 * Place the sensor after the turbocharger/supercharger, but before any intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3693
	 */
	adc_channel_e compressorDischargePressureChannel;
	/**
	 * offset 3694
	 */
	Gpio dacOutputPins[DAC_OUTPUT_COUNT] = {};
	/**
	 * offset 3698
	 */
	output_pin_e speedometerOutputPin;
	/**
	 * Number of speedometer pulses per kilometer travelled.
	 * offset 3700
	 */
	uint16_t speedometerPulsePerKm;
	/**
	 * Frequency used by the "Test Speedo" bench test button, pulsed for a fixed duration.
	 * units: Hz
	 * offset 3702
	 */
	uint16_t speedometerBenchTestFrequency;
	/**
	 * offset 3704
	 */
	uint8_t simulatorCamPosition[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 3708
	 */
	adc_channel_e ignKeyAdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3709
	 */
	uint8_t alignmentFill_at_3709[3] = {};
	/**
	 * offset 3712
	 */
	float ignKeyAdcDivider;
	/**
	 * offset 3716
	 */
	pin_mode_e spi6MisoMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3717
	 */
	uint8_t alignmentFill_at_3717[3] = {};
	/**
	 * units: ratio
	 * offset 3720
	 */
	float triggerVVTGapOverrideFrom[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3736
	 */
	float triggerVVTGapOverrideTo[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: %
	 * offset 3752
	 */
	int8_t tractionControlEtbDrop[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * This sets an immediate limit on injector duty cycle. If this threshold is reached, the system will immediately cut the injectors.
	 * units: %
	 * offset 3788
	 */
	uint8_t maxInjectorDutyInstant;
	/**
	 * This limit allows injectors to operate up to the specified duty cycle percentage for a short period (as defined by the delay). After this delay, if the duty cycle remains above the limit, it will trigger a cut.
	 * units: %
	 * offset 3789
	 */
	uint8_t maxInjectorDutySustained;
	/**
	 * Timeout period for duty cycle over the sustained limit to trigger duty cycle protection.
	 * units: sec
	 * offset 3790
	 */
	scaled_channel<uint8_t, 10, 1> maxInjectorDutySustainedTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3791
	 */
	uint8_t alignmentFill_at_3791[1] = {};
	/**
	 * offset 3792
	 */
	output_pin_e injectionPinsStage2[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: Deg
	 * offset 3816
	 */
	int8_t tractionControlTimingDrop[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * units: %
	 * offset 3852
	 */
	int8_t tractionControlIgnitionSkip[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * offset 3888
	 */
	float auxSpeed1Multiplier;
	/**
	 * offset 3892
	 */
	float brakeMeanEffectivePressureDifferential;
	/**
	 * offset 3896
	 */
	Gpio spi4mosiPin;
	/**
	 * offset 3898
	 */
	Gpio spi4misoPin;
	/**
	 * offset 3900
	 */
	Gpio spi4sckPin;
	/**
	 * offset 3902
	 */
	Gpio spi5mosiPin;
	/**
	 * offset 3904
	 */
	Gpio spi5misoPin;
	/**
	 * offset 3906
	 */
	Gpio spi5sckPin;
	/**
	 * offset 3908
	 */
	Gpio spi6mosiPin;
	/**
	 * offset 3910
	 */
	Gpio spi6misoPin;
	/**
	 * offset 3912
	 */
	Gpio spi6sckPin;
	/**
	 * offset 3914
	 */
	pin_mode_e spi4SckMode;
	/**
	 * offset 3915
	 */
	pin_mode_e spi4MosiMode;
	/**
	 * offset 3916
	 */
	pin_mode_e spi4MisoMode;
	/**
	 * offset 3917
	 */
	pin_mode_e spi5SckMode;
	/**
	 * offset 3918
	 */
	pin_mode_e spi5MosiMode;
	/**
	 * offset 3919
	 */
	pin_mode_e spi5MisoMode;
	/**
	 * offset 3920
	 */
	pin_mode_e spi6SckMode;
	/**
	 * offset 3921
	 */
	pin_mode_e spi6MosiMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3922
	 */
	uint8_t alignmentFill_at_3922[2] = {};
	/**
	 * Secondary TTL channel baud rate
	 * units: BPs
	 * offset 3924
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 3928
	 */
	Gpio camSimulatorPin;
	/**
	 * offset 3930
	 */
	pin_output_mode_e camSimulatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3931
	 */
	uint8_t alignmentFill_at_3931[1] = {};
	/**
	 * offset 3932
	 */
	int anotherCiTest;
	/**
	 * offset 3936
	 */
	uint32_t device_uid[3] = {};
	/**
	 * offset 3948
	 */
	adc_channel_e tcu_rangeAnalogInput[RANGE_INPUT_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3954
	 */
	uint8_t alignmentFill_at_3954[2] = {};
	/**
	 * units: Ohm
	 * offset 3956
	 */
	float tcu_rangeSensorBiasResistor;
	/**
	 * offset 3960
	 */
	MsIoBox_config_s msIoBox0;
	/**
	 * Nominal coil charge current, 0.25A step
	 * units: A
	 * offset 3964
	 */
	scaled_channel<uint8_t, 4, 1> mc33810Nomi;
	/**
	 * Maximum coil charge current, 1A step
	 * units: A
	 * offset 3965
	 */
	uint8_t mc33810Maxi;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3966
	 */
	uint8_t alignmentFill_at_3966[2] = {};
	/**
	 * offset 3968
	 */
	linear_sensor_s acPressure;
	/**
	 * value of A/C pressure in kPa/psi before that compressor is disengaged
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3988
	 */
	uint16_t minAcPressure;
	/**
	 * value of A/C pressure in kPa/psi after that compressor is disengaged
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3990
	 */
	uint16_t maxAcPressure;
	/**
	 * Delay before cutting fuel due to low oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 3992
	 */
	scaled_channel<uint8_t, 10, 1> minimumOilPressureTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3993
	 */
	uint8_t alignmentFill_at_3993[3] = {};
	/**
	 * offset 3996
	 */
	linear_sensor_s auxLinear3;
	/**
	 * offset 4016
	 */
	linear_sensor_s auxLinear4;
	/**
	 * offset 4036
	 */
	float engineShutDownPeriod;
	/**
	 * Below TPS value all knock suppression will be disabled.
	 * units: %
	 * offset 4040
	 */
	scaled_channel<uint8_t, 1, 1> knockSuppressMinTps;
	/**
	 * Fuel to odd when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 4041
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimAggression;
	/**
	 * After a knock event, reapply fuel at this rate.
	 * units: 1%/s
	 * offset 4042
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimReapplyRate;
	/**
	 * Maximum Amount of Fuel trim when knock
	 * units: %
	 * offset 4043
	 */
	scaled_channel<uint8_t, 1, 1> knockFuelTrim;
	/**
	 * units: sense
	 * offset 4044
	 */
	float knockSpectrumSensitivity;
	/**
	 * "Estimated knock frequency, ignore cylinderBore if this one > 0"
	 * units: Hz
	 * offset 4048
	 */
	float knockFrequency;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 4052
	 */
	injector_compensation_mode_e secondaryInjectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4053
	 */
	uint8_t alignmentFill_at_4053[3] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 4056
	 */
	float secondaryInjectorFuelReferencePressure;
	/**
	 * SENT input connected to ETB
	 * offset 4060
	 */
	SentInput EtbSentInput;
	/**
	 * SENT input used for high pressure fuel sensor
	 * offset 4061
	 */
	SentInput FuelHighPressureSentInput;
	/**
	 * If you have SENT High Pressure Fuel Sensor please select type. For analog TPS leave None
	 * offset 4062
	 */
	SentFuelHighPressureType FuelHighPressureSentType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4063
	 */
	uint8_t alignmentFill_at_4063[1] = {};
	/**
	offset 4064 bit 0 */
	bool nitrousControlEnabled : 1 {};
	/**
	offset 4064 bit 1 */
	bool vvlControlEnabled : 1 {};
	/**
	 * By default the idle solenoid is switched off whenever the engine is not turning, to be quieter and save power. Enable this to keep driving it to the position the idle controller asks for, which at zero RPM is the cranking curve for the current coolant temperature - for valves which need to rest somewhere other than de-energized. The valve is only driven for a minute after the engine stops turning, then switched off anyway to protect the coil and the battery.
	offset 4064 bit 2 */
	bool keepIdleSolenoidWhenStopped : 1 {};
	/**
	 * Include CAN bus in trace using Elmue SLCAN prefixes (CAN1: none, CAN2: &, CAN3: $). Disable for standard SLCAN clients such as SavvyCAN or slcand. Reconnect the sniffer after changing this setting.
	offset 4064 bit 3 */
	bool canSnifferIncludeBus : 1 {};
	/**
	offset 4064 bit 4 */
	bool unusedBit_Fancy5 : 1 {};
	/**
	offset 4064 bit 5 */
	bool unusedBit_Fancy6 : 1 {};
	/**
	offset 4064 bit 6 */
	bool unusedBit_Fancy7 : 1 {};
	/**
	offset 4064 bit 7 */
	bool unusedBit_Fancy8 : 1 {};
	/**
	offset 4064 bit 8 */
	bool unusedBit_Fancy9 : 1 {};
	/**
	offset 4064 bit 9 */
	bool unusedBit_Fancy10 : 1 {};
	/**
	offset 4064 bit 10 */
	bool unusedBit_Fancy11 : 1 {};
	/**
	offset 4064 bit 11 */
	bool unusedBit_Fancy12 : 1 {};
	/**
	offset 4064 bit 12 */
	bool unusedBit_Fancy13 : 1 {};
	/**
	offset 4064 bit 13 */
	bool unusedBit_Fancy14 : 1 {};
	/**
	offset 4064 bit 14 */
	bool unusedBit_Fancy15 : 1 {};
	/**
	offset 4064 bit 15 */
	bool unusedBit_Fancy16 : 1 {};
	/**
	 * Driveshaft reference for gear detection. Main Vehicle Speed uses wheel revolutions/km and final drive ratio. Output Shaft Speed reads shaft RPM directly and requires an OSS provider.
	offset 4064 bit 16 */
	bool gearDetectionUseOutputShaftSpeed : 1 {};
	/**
	 * RPM source when Speed Source is Output Shaft Speed. Main Vehicle Speed retains automatic input-shaft selection when that sensor is registered.
	offset 4064 bit 17 */
	bool gearDetectionRpmSourceIsInputShaftSpeed : 1 {};
	/**
	offset 4064 bit 18 */
	bool unusedBit_Fancy19 : 1 {};
	/**
	offset 4064 bit 19 */
	bool unusedBit_Fancy20 : 1 {};
	/**
	offset 4064 bit 20 */
	bool unusedBit_Fancy21 : 1 {};
	/**
	 * Compare the selected Slip RPM Source with expected RPM for the detected gear. Requires Speed Source = Output Shaft Speed, a valid gear and the minimum vehicle speed. Wheel-based references cannot identify transmission slip.
	offset 4064 bit 21 */
	bool transmissionSlipDetectionEnabled : 1 {};
	/**
	 * Input Shaft Speed measures internal gearbox slip. Engine RPM also includes clutch or torque-converter slip. Accurate gear ratios are required.
	offset 4064 bit 22 */
	bool transmissionSlipRpmSourceIsInputShaftSpeed : 1 {};
	/**
	offset 4064 bit 23 */
	bool unusedBit_Fancy24 : 1 {};
	/**
	offset 4064 bit 24 */
	bool unusedBit_Fancy25 : 1 {};
	/**
	offset 4064 bit 25 */
	bool unusedBit_Fancy26 : 1 {};
	/**
	offset 4064 bit 26 */
	bool unusedBit_Fancy27 : 1 {};
	/**
	offset 4064 bit 27 */
	bool unusedBit_Fancy28 : 1 {};
	/**
	offset 4064 bit 28 */
	bool unusedBit_Fancy29 : 1 {};
	/**
	offset 4064 bit 29 */
	bool unusedBit_Fancy30 : 1 {};
	/**
	offset 4064 bit 30 */
	bool unusedBit_Fancy31 : 1 {};
	/**
	offset 4064 bit 31 */
	bool unusedBit_Fancy32 : 1 {};
	/**
	 * offset 4068
	 */
	nitrous_arming_method_e nitrousControlArmingMethod;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4069
	 */
	uint8_t alignmentFill_at_4069[1] = {};
	/**
	 * Pin that activates nitrous control
	 * offset 4070
	 */
	switch_input_pin_e nitrousControlTriggerPin;
	/**
	 * offset 4072
	 */
	pin_input_mode_e nitrousControlTriggerPinMode;
	/**
	 * offset 4073
	 */
	lua_gauge_e nitrousLuaGauge;
	/**
	 * offset 4074
	 */
	lua_gauge_meaning_e nitrousLuaGaugeMeaning;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4075
	 */
	uint8_t alignmentFill_at_4075[1] = {};
	/**
	 * offset 4076
	 */
	float nitrousLuaGaugeArmingValue;
	/**
	 * offset 4080
	 */
	int nitrousMinimumTps;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 4084
	 */
	int16_t nitrousMinimumClt;
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 4086
	 */
	int16_t nitrousMaximumMap;
	/**
	 * units: afr
	 * offset 4088
	 */
	scaled_channel<uint8_t, 10, 1> nitrousMaximumAfr;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4089
	 */
	uint8_t alignmentFill_at_4089[1] = {};
	/**
	 * units: rpm
	 * offset 4090
	 */
	uint16_t nitrousActivationRpm;
	/**
	 * units: rpm
	 * offset 4092
	 */
	uint16_t nitrousDeactivationRpm;
	/**
	 * units: rpm
	 * offset 4094
	 */
	uint16_t nitrousDeactivationRpmWindow;
	/**
	 * Retard timing by this amount during DFCO. Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: deg
	 * offset 4096
	 */
	uint8_t dfcoRetardDeg;
	/**
	 * Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: s
	 * offset 4097
	 */
	scaled_channel<uint8_t, 10, 1> dfcoRetardRampInTime;
	/**
	 * offset 4098
	 */
	output_pin_e nitrousRelayPin;
	/**
	 * offset 4100
	 */
	pin_output_mode_e nitrousRelayPinMode;
	/**
	 * units: %
	 * offset 4101
	 */
	int8_t nitrousFuelAdderPercent;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4102
	 */
	uint8_t alignmentFill_at_4102[2] = {};
	/**
	 * Retard timing to remove from actual final timing (after all corrections) due to additional air.
	 * units: deg
	 * offset 4104
	 */
	float nitrousIgnitionRetard;
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 4108
	 */
	uint16_t nitrousMinimumVehicleSpeed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4110
	 */
	uint8_t alignmentFill_at_4110[2] = {};
	/**
	 * Exponential Average Alpha filtering parameter
	 * offset 4112
	 */
	float fuelLevelAveragingAlpha;
	/**
	 * How often do we update fuel level gauge
	 * units: seconds
	 * offset 4116
	 */
	float fuelLevelUpdatePeriodSec;
	/**
	 * Error below specified value
	 * units: v
	 * offset 4120
	 */
	float fuelLevelLowThresholdVoltage;
	/**
	 * Error above specified value
	 * units: v
	 * offset 4124
	 */
	float fuelLevelHighThresholdVoltage;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 4128
	 */
	float afrExpAverageAlpha;
	/**
	 * Compensates for trigger delay due to belt stretch, or other electromechanical issues. beware that raising this value advances ignition timing!
	 * units: uS
	 * offset 4132
	 */
	scaled_channel<uint8_t, 1, 1> sparkHardwareLatencyCorrection;
	/**
	 * Delay before cutting fuel due to extra high oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 4133
	 */
	scaled_channel<uint8_t, 10, 1> maxOilPressureTimeout;
	/**
	 * units: kg/h
	 * offset 4134
	 */
	scaled_channel<uint16_t, 100, 1> idleFlowEstimateFlow[8] = {};
	/**
	 * units: %
	 * offset 4150
	 */
	scaled_channel<uint8_t, 2, 1> idleFlowEstimatePosition[8] = {};
	/**
	 * units: mg
	 * offset 4158
	 */
	int8_t airmassToTimingBins[8] = {};
	/**
	 * units: deg
	 * offset 4166
	 */
	int8_t airmassToTimingValues[8] = {};
	/**
	 * Time for the idle RPM target to ramp down from the elevated return-to-idle value (normal target + 'RPM upper limit') to the normal target. Longer duration gives a gentler, slower settle to idle. Only used when 'Ramp target on return to idle' is enabled.
	 * units: seconds
	 * offset 4174
	 */
	scaled_channel<uint8_t, 10, 1> idleReturnTargetRampDuration;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4175
	 */
	uint8_t alignmentFill_at_4175[1] = {};
	/**
	 * Voltage when the wastegate is fully open
	 * units: v
	 * offset 4176
	 */
	float wastegatePositionOpenedVoltage;
	/**
	 * Voltage when the wastegate is closed
	 * units: v
	 * offset 4180
	 */
	float wastegatePositionClosedVoltage;
	/**
	 * offset 4184
	 */
	wbo_s canWbo[CAN_WBO_COUNT] = {};
	/**
	 * offset 4200
	 */
	output_pin_e vvlRelayPin;
	/**
	 * offset 4202
	 */
	pin_output_mode_e vvlRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4203
	 */
	uint8_t alignmentFill_at_4203[1] = {};
	/**
	 * offset 4204
	 */
	vvl_s vvlController;
	/**
	 * offset 4228
	 */
	rotational_idle_s rotationalIdleController;
	/**
	 * Launch RPM Threshold: when above 0, launch only engages if the activation switch (button/clutch) is pressed at or below this RPM, and stays latched while held - even past this RPM. This lets a standing launch (switch pressed low, revved up) coexist with flat shift / torque reduction (switch blipped high during an upshift). 0 disables the gate (legacy behavior).
	 * units: rpm
	 * offset 4264
	 */
	uint16_t launchRpmThreshold;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4266
	 */
	uint8_t alignmentFill_at_4266[2] = {};
	/**
	 * Misfire Detection: master enable. Active at idle only. Latches check-engine light (P0300) once the count threshold is reached.
	offset 4268 bit 0 */
	bool misfireDetectionEnabled : 1 {};
	/**
	offset 4268 bit 1 */
	bool unusedBit_1089_1 : 1 {};
	/**
	offset 4268 bit 2 */
	bool unusedBit_1089_2 : 1 {};
	/**
	offset 4268 bit 3 */
	bool unusedBit_1089_3 : 1 {};
	/**
	offset 4268 bit 4 */
	bool unusedBit_1089_4 : 1 {};
	/**
	offset 4268 bit 5 */
	bool unusedBit_1089_5 : 1 {};
	/**
	offset 4268 bit 6 */
	bool unusedBit_1089_6 : 1 {};
	/**
	offset 4268 bit 7 */
	bool unusedBit_1089_7 : 1 {};
	/**
	offset 4268 bit 8 */
	bool unusedBit_1089_8 : 1 {};
	/**
	offset 4268 bit 9 */
	bool unusedBit_1089_9 : 1 {};
	/**
	offset 4268 bit 10 */
	bool unusedBit_1089_10 : 1 {};
	/**
	offset 4268 bit 11 */
	bool unusedBit_1089_11 : 1 {};
	/**
	offset 4268 bit 12 */
	bool unusedBit_1089_12 : 1 {};
	/**
	offset 4268 bit 13 */
	bool unusedBit_1089_13 : 1 {};
	/**
	offset 4268 bit 14 */
	bool unusedBit_1089_14 : 1 {};
	/**
	offset 4268 bit 15 */
	bool unusedBit_1089_15 : 1 {};
	/**
	offset 4268 bit 16 */
	bool unusedBit_1089_16 : 1 {};
	/**
	offset 4268 bit 17 */
	bool unusedBit_1089_17 : 1 {};
	/**
	offset 4268 bit 18 */
	bool unusedBit_1089_18 : 1 {};
	/**
	offset 4268 bit 19 */
	bool unusedBit_1089_19 : 1 {};
	/**
	offset 4268 bit 20 */
	bool unusedBit_1089_20 : 1 {};
	/**
	offset 4268 bit 21 */
	bool unusedBit_1089_21 : 1 {};
	/**
	offset 4268 bit 22 */
	bool unusedBit_1089_22 : 1 {};
	/**
	offset 4268 bit 23 */
	bool unusedBit_1089_23 : 1 {};
	/**
	offset 4268 bit 24 */
	bool unusedBit_1089_24 : 1 {};
	/**
	offset 4268 bit 25 */
	bool unusedBit_1089_25 : 1 {};
	/**
	offset 4268 bit 26 */
	bool unusedBit_1089_26 : 1 {};
	/**
	offset 4268 bit 27 */
	bool unusedBit_1089_27 : 1 {};
	/**
	offset 4268 bit 28 */
	bool unusedBit_1089_28 : 1 {};
	/**
	offset 4268 bit 29 */
	bool unusedBit_1089_29 : 1 {};
	/**
	offset 4268 bit 30 */
	bool unusedBit_1089_30 : 1 {};
	/**
	offset 4268 bit 31 */
	bool unusedBit_1089_31 : 1 {};
	/**
	 * Misfire Detection: minimum flagged firings within the recent-firings window before a misfire is counted.
	 * units: events
	 * offset 4272
	 */
	uint8_t misfireConsecutiveCount;
	/**
	 * Misfire Detection: sliding window size in firings across all cylinders.
	 * units: firings
	 * offset 4273
	 */
	uint8_t misfireWindowFirings;
	/**
	 * Misfire Detection: total counted misfires before the MIL latches. 0 = monitor-only.
	 * units: events
	 * offset 4274
	 */
	uint16_t misfireCountThreshold;
	/**
	 * Misfire Detection: threshold multiplier (baseline + K * wobble). Default 3.0.
	 * offset 4276
	 */
	float misfireK;
	/**
	 * Misfire Detection: window start, degrees after each cylinder's TDC.
	 * units: deg
	 * offset 4280
	 */
	float misfireWindowStart;
	/**
	 * Misfire Detection: window end, degrees after each cylinder's TDC.
	 * units: deg
	 * offset 4284
	 */
	float misfireWindowEnd;
	/**
	 * Misfire Detection: EMA alpha when segment is slowing (above baseline).
	 * offset 4288
	 */
	float misfireEmaAlphaDecel;
	/**
	 * Misfire Detection: EMA alpha when segment is recovering (below baseline).
	 * offset 4292
	 */
	float misfireEmaAlphaAccel;
	/**
	 * Misfire Detection: wobble EMA alpha when spread is increasing.
	 * offset 4296
	 */
	float misfireWobbleAlphaRise;
	/**
	 * Misfire Detection: wobble EMA alpha when spread is decreasing.
	 * offset 4300
	 */
	float misfireWobbleAlphaFall;
	/**
	 * Misfire Detection: firings to wait after entering idle before flagging starts. 0 = immediate.
	 * units: cycles
	 * offset 4304
	 */
	uint16_t misfireSettleCycles;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4306
	 */
	uint8_t alignmentFill_at_4306[2] = {};
	/**
	 * offset 4308
	 */
	i2c_config_s i2c[I2C_BUS_TOTAL_COUNT] = {};
	/**
	 * offset 4356
	 */
	pca9685_config_s pca9685[PCA9685_COUNT] = {};
	/**
	 * offset 4364
	 */
	can_sniffer_channel_s canSniffer[EFI_CAN_BUS_COUNT] = {};
	/**
	 * Bus for Tx
	 * offset 4372
	 */
	can_bus_channel_e canSnifferTxBus;
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Full MLG' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio. 'DTC Freeze Frame' will write one frame MLG log and short CSV trigger log on every DTC
	 * offset 4373
	 */
	SDLoggerMode sdLoggerMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4374
	 */
	uint8_t alignmentFill_at_4374[2] = {};
	/**
	 * Dwell Duty Mode: when enabled, ignores the RPM/voltage dwell tables and computes dwell as a fixed percentage of the time between consecutive ignition pulses. Required for Ford TFI modules that expect a 50% duty cycle square wave.
	offset 4376 bit 0 */
	bool dwellDutyModeEnabled : 1 {};
	/**
	offset 4376 bit 1 */
	bool unusedBit_1139_1 : 1 {};
	/**
	offset 4376 bit 2 */
	bool unusedBit_1139_2 : 1 {};
	/**
	offset 4376 bit 3 */
	bool unusedBit_1139_3 : 1 {};
	/**
	offset 4376 bit 4 */
	bool unusedBit_1139_4 : 1 {};
	/**
	offset 4376 bit 5 */
	bool unusedBit_1139_5 : 1 {};
	/**
	offset 4376 bit 6 */
	bool unusedBit_1139_6 : 1 {};
	/**
	offset 4376 bit 7 */
	bool unusedBit_1139_7 : 1 {};
	/**
	offset 4376 bit 8 */
	bool unusedBit_1139_8 : 1 {};
	/**
	offset 4376 bit 9 */
	bool unusedBit_1139_9 : 1 {};
	/**
	offset 4376 bit 10 */
	bool unusedBit_1139_10 : 1 {};
	/**
	offset 4376 bit 11 */
	bool unusedBit_1139_11 : 1 {};
	/**
	offset 4376 bit 12 */
	bool unusedBit_1139_12 : 1 {};
	/**
	offset 4376 bit 13 */
	bool unusedBit_1139_13 : 1 {};
	/**
	offset 4376 bit 14 */
	bool unusedBit_1139_14 : 1 {};
	/**
	offset 4376 bit 15 */
	bool unusedBit_1139_15 : 1 {};
	/**
	offset 4376 bit 16 */
	bool unusedBit_1139_16 : 1 {};
	/**
	offset 4376 bit 17 */
	bool unusedBit_1139_17 : 1 {};
	/**
	offset 4376 bit 18 */
	bool unusedBit_1139_18 : 1 {};
	/**
	offset 4376 bit 19 */
	bool unusedBit_1139_19 : 1 {};
	/**
	offset 4376 bit 20 */
	bool unusedBit_1139_20 : 1 {};
	/**
	offset 4376 bit 21 */
	bool unusedBit_1139_21 : 1 {};
	/**
	offset 4376 bit 22 */
	bool unusedBit_1139_22 : 1 {};
	/**
	offset 4376 bit 23 */
	bool unusedBit_1139_23 : 1 {};
	/**
	offset 4376 bit 24 */
	bool unusedBit_1139_24 : 1 {};
	/**
	offset 4376 bit 25 */
	bool unusedBit_1139_25 : 1 {};
	/**
	offset 4376 bit 26 */
	bool unusedBit_1139_26 : 1 {};
	/**
	offset 4376 bit 27 */
	bool unusedBit_1139_27 : 1 {};
	/**
	offset 4376 bit 28 */
	bool unusedBit_1139_28 : 1 {};
	/**
	offset 4376 bit 29 */
	bool unusedBit_1139_29 : 1 {};
	/**
	offset 4376 bit 30 */
	bool unusedBit_1139_30 : 1 {};
	/**
	offset 4376 bit 31 */
	bool unusedBit_1139_31 : 1 {};
	/**
	 * Dwell Duty Mode: percentage of the inter-spark interval used as coil dwell time. 50 = half the interval between pulses (standard TFI target).
	 * units: %
	 * offset 4380
	 */
	uint8_t dwellDutyPercent;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4381
	 */
	uint8_t alignmentFill_at_4381[3] = {};
	/**
	 * Signed idle position offset when Lua reports Park or Neutral via setParkNeutral(boolean). Negative values reduce opening, positive values increase it. Added to running open-loop idle, not target RPM. Zero disables the correction.
	 * units: %
	 * offset 4384
	 */
	float idleParkNeutralOffset;
	/**
	 * Below this vehicle speed, transmission slip is invalid. Zero is reset to the 5 km/h default.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 4388
	 */
	uint8_t transmissionSlipMinVss;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4389
	 */
	uint8_t alignmentFill_at_4389[1] = {};
	/**
	 * Output-shaft pulse input. Leave unassigned for a Lua/CAN provider using Sensor.new("OutputShaftSpeed") in RPM. Use a separate input from the main vehicle speed sensor.
	 * offset 4390
	 */
	brain_input_pin_e outputShaftSpeedSensorPin;
	/**
	 * Pulses per output-shaft revolution.
	 * units: count
	 * offset 4392
	 */
	uint8_t outputShaftSpeedSensorTeeth;
	/**
	 * Output-shaft low-pass filter reciprocal. Higher values average more teeth.
	 * offset 4393
	 */
	uint8_t ossFilterReciprocal;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4394
	 */
	uint8_t alignmentFill_at_4394[2] = {};
	/**
	 * Hold a captured RPM window while moving with the activation switch or Lua trigger held.
	offset 4396 bit 0 */
	bool rollingLaunchEnabled : 1 {};
	/**
	offset 4396 bit 1 */
	bool unusedBit_1180_1 : 1 {};
	/**
	offset 4396 bit 2 */
	bool unusedBit_1180_2 : 1 {};
	/**
	offset 4396 bit 3 */
	bool unusedBit_1180_3 : 1 {};
	/**
	offset 4396 bit 4 */
	bool unusedBit_1180_4 : 1 {};
	/**
	offset 4396 bit 5 */
	bool unusedBit_1180_5 : 1 {};
	/**
	offset 4396 bit 6 */
	bool unusedBit_1180_6 : 1 {};
	/**
	offset 4396 bit 7 */
	bool unusedBit_1180_7 : 1 {};
	/**
	offset 4396 bit 8 */
	bool unusedBit_1180_8 : 1 {};
	/**
	offset 4396 bit 9 */
	bool unusedBit_1180_9 : 1 {};
	/**
	offset 4396 bit 10 */
	bool unusedBit_1180_10 : 1 {};
	/**
	offset 4396 bit 11 */
	bool unusedBit_1180_11 : 1 {};
	/**
	offset 4396 bit 12 */
	bool unusedBit_1180_12 : 1 {};
	/**
	offset 4396 bit 13 */
	bool unusedBit_1180_13 : 1 {};
	/**
	offset 4396 bit 14 */
	bool unusedBit_1180_14 : 1 {};
	/**
	offset 4396 bit 15 */
	bool unusedBit_1180_15 : 1 {};
	/**
	offset 4396 bit 16 */
	bool unusedBit_1180_16 : 1 {};
	/**
	offset 4396 bit 17 */
	bool unusedBit_1180_17 : 1 {};
	/**
	offset 4396 bit 18 */
	bool unusedBit_1180_18 : 1 {};
	/**
	offset 4396 bit 19 */
	bool unusedBit_1180_19 : 1 {};
	/**
	offset 4396 bit 20 */
	bool unusedBit_1180_20 : 1 {};
	/**
	offset 4396 bit 21 */
	bool unusedBit_1180_21 : 1 {};
	/**
	offset 4396 bit 22 */
	bool unusedBit_1180_22 : 1 {};
	/**
	offset 4396 bit 23 */
	bool unusedBit_1180_23 : 1 {};
	/**
	offset 4396 bit 24 */
	bool unusedBit_1180_24 : 1 {};
	/**
	offset 4396 bit 25 */
	bool unusedBit_1180_25 : 1 {};
	/**
	offset 4396 bit 26 */
	bool unusedBit_1180_26 : 1 {};
	/**
	offset 4396 bit 27 */
	bool unusedBit_1180_27 : 1 {};
	/**
	offset 4396 bit 28 */
	bool unusedBit_1180_28 : 1 {};
	/**
	offset 4396 bit 29 */
	bool unusedBit_1180_29 : 1 {};
	/**
	offset 4396 bit 30 */
	bool unusedBit_1180_30 : 1 {};
	/**
	offset 4396 bit 31 */
	bool unusedBit_1180_31 : 1 {};
	/**
	 * Hardware activation switch, OR-ed with setRollingLaunchTrigger(1). Unassigned allows Lua-only activation.
	 * offset 4400
	 */
	switch_input_pin_e rollingLaunchActivatePin;
	/**
	 * offset 4402
	 */
	pin_input_mode_e rollingLaunchActivatePinMode;
	/**
	 * Ignition timing removed while armed. This is relative retard, not an absolute timing target.
	 * units: deg
	 * offset 4403
	 */
	uint8_t rollingLaunchTimingRetard;
	/**
	 * Additional fuel while armed. Removed immediately when disarmed.
	 * units: %
	 * offset 4404
	 */
	uint8_t rollingLaunchFuelAdderPercent;
	/**
	 * Time for the timing retard to decay to zero after release or loss of an arming gate. Zero removes retard immediately.
	 * units: s
	 * offset 4405
	 */
	scaled_channel<uint8_t, 10, 1> rollingLaunchRampOutTime;
	/**
	 * RPM above the captured arming RPM where spark skip reaches 100%. Zero requests a hard cut at the captured RPM.
	 * units: rpm
	 * offset 4406
	 */
	uint16_t rollingLaunchRpmWindow;
	/**
	 * Minimum RPM while the trigger is held. Crossing this gate can arm without another button press.
	 * units: rpm
	 * offset 4408
	 */
	uint16_t rollingLaunchMinArmRpm;
	/**
	 * Maximum captured target RPM. The normal rev limiter remains independent.
	 * units: rpm
	 * offset 4410
	 */
	uint16_t rollingLaunchMaxRpm;
	/**
	 * Minimum vehicle speed while the trigger is held. Requires valid vehicle speed.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 4412
	 */
	uint16_t rollingLaunchMinVss;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4414
	 */
	uint8_t alignmentFill_at_4414[2] = {};
	/**
	 * Exponential smoothing applied independently to TPS1 and TPS2 on each resolved sensor read.
	 * 1 passes readings through. Lower values add smoothing and feedback delay. This is not a fixed-rate filter.
	 * offset 4416
	 */
	float tpsExpAverageAlpha;
	/**
	 * If TPS changes by at least this much within tpsExpAverageResetTimeMs, the ExpAverage filter resets and jumps straight to the new raw value instead of smoothing into it. Set to 0 to disable.
	 * units: %
	 * offset 4420
	 */
	float tpsExpAverageResetThreshold;
	/**
	 * Time window for tpsExpAverageResetThreshold - a change of that size within this many milliseconds triggers an immediate ExpAverage reset. Set to 0 to disable.
	 * units: ms
	 * offset 4424
	 */
	float tpsExpAverageResetTimeMs;
	/**
	 * Pull ignition timing while the TPS acceleration-enrichment threshold is exceeded, then decay the retard to zero. This uses the TPS delta and lookback, not acoustic knock detection.
	offset 4428 bit 0 */
	bool burstKnockEnabled : 1 {};
	/**
	offset 4428 bit 1 */
	bool unusedBit_1225_1 : 1 {};
	/**
	offset 4428 bit 2 */
	bool unusedBit_1225_2 : 1 {};
	/**
	offset 4428 bit 3 */
	bool unusedBit_1225_3 : 1 {};
	/**
	offset 4428 bit 4 */
	bool unusedBit_1225_4 : 1 {};
	/**
	offset 4428 bit 5 */
	bool unusedBit_1225_5 : 1 {};
	/**
	offset 4428 bit 6 */
	bool unusedBit_1225_6 : 1 {};
	/**
	offset 4428 bit 7 */
	bool unusedBit_1225_7 : 1 {};
	/**
	offset 4428 bit 8 */
	bool unusedBit_1225_8 : 1 {};
	/**
	offset 4428 bit 9 */
	bool unusedBit_1225_9 : 1 {};
	/**
	offset 4428 bit 10 */
	bool unusedBit_1225_10 : 1 {};
	/**
	offset 4428 bit 11 */
	bool unusedBit_1225_11 : 1 {};
	/**
	offset 4428 bit 12 */
	bool unusedBit_1225_12 : 1 {};
	/**
	offset 4428 bit 13 */
	bool unusedBit_1225_13 : 1 {};
	/**
	offset 4428 bit 14 */
	bool unusedBit_1225_14 : 1 {};
	/**
	offset 4428 bit 15 */
	bool unusedBit_1225_15 : 1 {};
	/**
	offset 4428 bit 16 */
	bool unusedBit_1225_16 : 1 {};
	/**
	offset 4428 bit 17 */
	bool unusedBit_1225_17 : 1 {};
	/**
	offset 4428 bit 18 */
	bool unusedBit_1225_18 : 1 {};
	/**
	offset 4428 bit 19 */
	bool unusedBit_1225_19 : 1 {};
	/**
	offset 4428 bit 20 */
	bool unusedBit_1225_20 : 1 {};
	/**
	offset 4428 bit 21 */
	bool unusedBit_1225_21 : 1 {};
	/**
	offset 4428 bit 22 */
	bool unusedBit_1225_22 : 1 {};
	/**
	offset 4428 bit 23 */
	bool unusedBit_1225_23 : 1 {};
	/**
	offset 4428 bit 24 */
	bool unusedBit_1225_24 : 1 {};
	/**
	offset 4428 bit 25 */
	bool unusedBit_1225_25 : 1 {};
	/**
	offset 4428 bit 26 */
	bool unusedBit_1225_26 : 1 {};
	/**
	offset 4428 bit 27 */
	bool unusedBit_1225_27 : 1 {};
	/**
	offset 4428 bit 28 */
	bool unusedBit_1225_28 : 1 {};
	/**
	offset 4428 bit 29 */
	bool unusedBit_1225_29 : 1 {};
	/**
	offset 4428 bit 30 */
	bool unusedBit_1225_30 : 1 {};
	/**
	offset 4428 bit 31 */
	bool unusedBit_1225_31 : 1 {};
	/**
	 * Linear decay time measured from the last above-threshold TPS sample. Zero removes retard as soon as the transient ends.
	 * units: s
	 * offset 4432
	 */
	scaled_channel<uint8_t, 10, 1> burstKnockDecayTime;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4433
	 */
	uint8_t alignmentFill_at_4433[3] = {};
	/**
	 * units: rpm
	 * offset 4436
	 */
	float burstKnockRpmBins[BURST_KNOCK_RPM_SIZE] = {};
	/**
	 * TPS acceleration-enrichment delta divided by its configured lookback (minimum 0.001 seconds).
	 * units: %/s
	 * offset 4468
	 */
	float burstKnockTpsRateBins[BURST_KNOCK_TPS_RATE_SIZE] = {};
	/**
	 * Ignition timing to remove during the transient. Zero disables retard for that cell.
	 * units: deg
	 * offset 4500
	 */
	float burstKnockRetardTable[BURST_KNOCK_TPS_RATE_SIZE][BURST_KNOCK_RPM_SIZE] = {};
	/**
	 * Classify engine operating state and recognize shifts. State-based STFT is separately enabled below.
	offset 4756 bit 0 */
	bool useEngineStateMachine : 1 {};
	/**
	offset 4756 bit 1 */
	bool unusedBit_1262_1 : 1 {};
	/**
	offset 4756 bit 2 */
	bool unusedBit_1262_2 : 1 {};
	/**
	offset 4756 bit 3 */
	bool unusedBit_1262_3 : 1 {};
	/**
	offset 4756 bit 4 */
	bool unusedBit_1262_4 : 1 {};
	/**
	offset 4756 bit 5 */
	bool unusedBit_1262_5 : 1 {};
	/**
	offset 4756 bit 6 */
	bool unusedBit_1262_6 : 1 {};
	/**
	offset 4756 bit 7 */
	bool unusedBit_1262_7 : 1 {};
	/**
	offset 4756 bit 8 */
	bool unusedBit_1262_8 : 1 {};
	/**
	offset 4756 bit 9 */
	bool unusedBit_1262_9 : 1 {};
	/**
	offset 4756 bit 10 */
	bool unusedBit_1262_10 : 1 {};
	/**
	offset 4756 bit 11 */
	bool unusedBit_1262_11 : 1 {};
	/**
	offset 4756 bit 12 */
	bool unusedBit_1262_12 : 1 {};
	/**
	offset 4756 bit 13 */
	bool unusedBit_1262_13 : 1 {};
	/**
	offset 4756 bit 14 */
	bool unusedBit_1262_14 : 1 {};
	/**
	offset 4756 bit 15 */
	bool unusedBit_1262_15 : 1 {};
	/**
	offset 4756 bit 16 */
	bool unusedBit_1262_16 : 1 {};
	/**
	offset 4756 bit 17 */
	bool unusedBit_1262_17 : 1 {};
	/**
	offset 4756 bit 18 */
	bool unusedBit_1262_18 : 1 {};
	/**
	offset 4756 bit 19 */
	bool unusedBit_1262_19 : 1 {};
	/**
	offset 4756 bit 20 */
	bool unusedBit_1262_20 : 1 {};
	/**
	offset 4756 bit 21 */
	bool unusedBit_1262_21 : 1 {};
	/**
	offset 4756 bit 22 */
	bool unusedBit_1262_22 : 1 {};
	/**
	offset 4756 bit 23 */
	bool unusedBit_1262_23 : 1 {};
	/**
	offset 4756 bit 24 */
	bool unusedBit_1262_24 : 1 {};
	/**
	offset 4756 bit 25 */
	bool unusedBit_1262_25 : 1 {};
	/**
	offset 4756 bit 26 */
	bool unusedBit_1262_26 : 1 {};
	/**
	offset 4756 bit 27 */
	bool unusedBit_1262_27 : 1 {};
	/**
	offset 4756 bit 28 */
	bool unusedBit_1262_28 : 1 {};
	/**
	offset 4756 bit 29 */
	bool unusedBit_1262_29 : 1 {};
	/**
	offset 4756 bit 30 */
	bool unusedBit_1262_30 : 1 {};
	/**
	offset 4756 bit 31 */
	bool unusedBit_1262_31 : 1 {};
	/**
	 * Engine SM: TPS threshold above which the engine is at Wide Open Throttle.
	 * units: %
	 * offset 4760
	 */
	uint8_t smWotTpsThreshold;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4761
	 */
	uint8_t alignmentFill_at_4761[3] = {};
	/**
	 * Engine SM: drive STFT region selection from the live SM state (Idle/Overrun/WOT/Cruise) instead of the RPM/load region thresholds. The per-region STFT settings (time constant, max add/remove) are reused unchanged.
	offset 4764 bit 0 */
	bool stateBasedStft : 1 {};
	/**
	 * Engine SM: a second clutch switch is also wired (e.g. both Clutch-Up and Clutch-Down, not just whichever is selected below). Lets shift detection open the window on the earliest available press edge and close it on the earliest available release edge, instead of relying solely on the configured switch's own edges.
	offset 4764 bit 1 */
	bool smSecondClutchSwitchAvailable : 1 {};
	/**
	 * Engine SM: while the shift latch (smShiftLatchTimeMs) is active, a fresh Clutch-Down full-press resets it back to the full duration instead of letting it expire -- keeps Upshifting/Downshifting asserted through a flurry of quick consecutive shifts. Requires smSecondClutchSwitchAvailable (both Clutch-Up and Clutch-Down wired).
	offset 4764 bit 2 */
	bool smRelatchOnClutchDown : 1 {};
	/**
	offset 4764 bit 3 */
	bool unusedBit_1298_3 : 1 {};
	/**
	offset 4764 bit 4 */
	bool unusedBit_1298_4 : 1 {};
	/**
	offset 4764 bit 5 */
	bool unusedBit_1298_5 : 1 {};
	/**
	offset 4764 bit 6 */
	bool unusedBit_1298_6 : 1 {};
	/**
	offset 4764 bit 7 */
	bool unusedBit_1298_7 : 1 {};
	/**
	offset 4764 bit 8 */
	bool unusedBit_1298_8 : 1 {};
	/**
	offset 4764 bit 9 */
	bool unusedBit_1298_9 : 1 {};
	/**
	offset 4764 bit 10 */
	bool unusedBit_1298_10 : 1 {};
	/**
	offset 4764 bit 11 */
	bool unusedBit_1298_11 : 1 {};
	/**
	offset 4764 bit 12 */
	bool unusedBit_1298_12 : 1 {};
	/**
	offset 4764 bit 13 */
	bool unusedBit_1298_13 : 1 {};
	/**
	offset 4764 bit 14 */
	bool unusedBit_1298_14 : 1 {};
	/**
	offset 4764 bit 15 */
	bool unusedBit_1298_15 : 1 {};
	/**
	offset 4764 bit 16 */
	bool unusedBit_1298_16 : 1 {};
	/**
	offset 4764 bit 17 */
	bool unusedBit_1298_17 : 1 {};
	/**
	offset 4764 bit 18 */
	bool unusedBit_1298_18 : 1 {};
	/**
	offset 4764 bit 19 */
	bool unusedBit_1298_19 : 1 {};
	/**
	offset 4764 bit 20 */
	bool unusedBit_1298_20 : 1 {};
	/**
	offset 4764 bit 21 */
	bool unusedBit_1298_21 : 1 {};
	/**
	offset 4764 bit 22 */
	bool unusedBit_1298_22 : 1 {};
	/**
	offset 4764 bit 23 */
	bool unusedBit_1298_23 : 1 {};
	/**
	offset 4764 bit 24 */
	bool unusedBit_1298_24 : 1 {};
	/**
	offset 4764 bit 25 */
	bool unusedBit_1298_25 : 1 {};
	/**
	offset 4764 bit 26 */
	bool unusedBit_1298_26 : 1 {};
	/**
	offset 4764 bit 27 */
	bool unusedBit_1298_27 : 1 {};
	/**
	offset 4764 bit 28 */
	bool unusedBit_1298_28 : 1 {};
	/**
	offset 4764 bit 29 */
	bool unusedBit_1298_29 : 1 {};
	/**
	offset 4764 bit 30 */
	bool unusedBit_1298_30 : 1 {};
	/**
	offset 4764 bit 31 */
	bool unusedBit_1298_31 : 1 {};
	/**
	 * Engine SM: Clutch switch that signals an upshift. 'Clutch Up' opens the shift window when the pedal leaves rest (falling edge, drivetrain still locked) and closes it when the pedal returns to rest
	 * units: 'Clutch Down' opens the window on full press and closes it on release. Flat-shift torque reduction always overrides this.
	 * offset 4768
	 */
	sm_clutch_switch_e smUpshiftClutchSwitch;
	/**
	 * Engine SM: Clutch switch that signals a downshift.
	 * offset 4769
	 */
	sm_clutch_switch_e smDownshiftClutchSwitch;
	/**
	 * Engine SM: Signal source for the shift-direction accumulator. RPM Rate: accumulates positive/negative based on engine RPM rate of change. VSS Rate: accumulates based on vehicle speed rate of change (needs VSS sensor).
	 * offset 4770
	 */
	sm_shift_detection_mode_e smShiftDetectionMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4771
	 */
	uint8_t alignmentFill_at_4771[1] = {};
	/**
	 * Engine SM: RPM/s rate threshold to enter the Accelerating state. Also the accumulator gain normalizer while accelerating -- at exactly this rate the accumulator climbs at smAccumulatorGain pct/s, faster rates climb proportionally faster. 0 disables the Accelerating state. In VSS Rate mode this numeric value also normalizes the accumulator signal in km/h/s.
	 * units: RPM/s
	 * offset 4772
	 */
	int16_t smAccelRateThreshold;
	/**
	 * Engine SM: RPM/s rate threshold to enter the Decelerating state (RPM falling faster than this). Also the accumulator normalizer in Overrun. 0 disables the Decelerating state. In VSS Rate mode this numeric value also normalizes the accumulator signal in km/h/s.
	 * units: RPM/s
	 * offset 4774
	 */
	int16_t smDecelRateThreshold;
	/**
	 * Engine SM: Time window used to compute RPM rate of change (RPM/s, "RPM/t") for Accelerating/Decelerating state detection and the RPM-rate shift-accumulator signal. Larger values smooth out sensor/tooth noise at the cost of slower response, smaller values track faster but are noisier. Internally floored to one slow-callback tick (50 ms). smAccelRateThreshold/smDecelRateThreshold stay in RPM/s regardless of this setting and do not need re-tuning when it changes.
	 * units: ms
	 * offset 4776
	 */
	uint16_t smRpmRateWindowMs;
	/**
	 * Engine SM: once shift direction is confirmed, hold Upshifting/Downshifting true for at least this long. 0 = no latch.
	 * units: ms
	 * offset 4778
	 */
	uint16_t smShiftLatchTimeMs;
	/**
	 * Engine SM: time to hold the Transient state after the accel-enrichment threshold drops. 0 = no hold.
	 * units: ms
	 * offset 4780
	 */
	uint16_t smTransientHoldTimeMs;
	/**
	 * Engine SM: VSS Rate mode only -- minimum vehicle speed. Below this the accumulator decays instead of accumulating. 0 = no minimum.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 4782
	 */
	uint16_t smShiftMinVss;
	/**
	 * Engine SM: Accumulator must reach or exceed this value (0..100) to confirm an upshift. The accumulator runs from -100 to +100.
	 * offset 4784
	 */
	int8_t smAccumulatorUpshiftThreshold;
	/**
	 * Engine SM: Accumulator must reach or fall below the negative of this value (0..100) to confirm a downshift.
	 * offset 4785
	 */
	int8_t smAccumulatorDownshiftThreshold;
	/**
	 * Engine SM: once the Accelerating rate threshold is met, hold the Accelerating state for at least this long after the rate drops back below threshold. A fresh threshold crossing during the hold resets it back to the full duration. Broken immediately by a lift into the Coasting/Overrun phase, so a stale hold can never mask Overrun/DFCO. 0 = no hold (raw signal, legacy behavior).
	 * units: ms
	 * offset 4786
	 */
	uint16_t smAccelHoldMs;
	/**
	 * Engine SM: Accumulator climb rate in %/s per 1x threshold ratio. At exactly the threshold rate, the accumulator changes by this many %/s. Larger = faster response.
	 * units: %/s
	 * offset 4788
	 */
	float smAccumulatorGain;
	/**
	 * Engine SM: Rate at which the accumulator decays toward 0 (%/s) when the signal is within the �threshold dead-band. Larger = faster decay.
	 * units: %/s
	 * offset 4792
	 */
	float smAccumulatorDecayRate;
	/**
	 * Engine SM: once continuously in the Accelerating state for this long, a still-negative accumulator (left over from a prior decel run) is forced to 0 instead of waiting for it to decay/climb across zero on its own. Symmetrically, once continuously in Decelerating or Overrun for this long, a still-positive accumulator is forced to 0. Lets a same-gear direction reversal (e.g. a long overrun immediately followed by acceleration) shed a large stale opposite-sign accumulator quickly, rather than needing an equally long run the other way to null it out and risking a stale shift-direction call (e.g. a spurious downshift blip on clutch-in right after tipping back into the throttle). 0 = disabled (legacy decay/cross-only behavior).
	 * units: ms
	 * offset 4796
	 */
	uint16_t smAccumulatorSnapTimeMs;
	/**
	 * Engine Temperature Overlay: CLT below this is Cold engine. Between this and smHotTempThreshold is Operating temperature.
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 4798
	 */
	int16_t smColdTempThreshold;
	/**
	 * Engine Temperature Overlay: CLT above this is Hot engine. Below smColdTempThreshold is Cold, between both is Operating temperature.
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 4800
	 */
	int16_t smHotTempThreshold;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4802
	 */
	uint8_t alignmentFill_at_4802[2] = {};
	/**
	 * Add an AFR offset according to continuous time in the Engine State Machine WOT state. Requires Enable Engine State Machine. Negative values richen; positive values lean. Leaving WOT resets the timer and offset.
	offset 4804 bit 0 */
	bool wotEnrichmentEnabled : 1 {};
	/**
	offset 4804 bit 1 */
	bool unusedBit_1347_1 : 1 {};
	/**
	offset 4804 bit 2 */
	bool unusedBit_1347_2 : 1 {};
	/**
	offset 4804 bit 3 */
	bool unusedBit_1347_3 : 1 {};
	/**
	offset 4804 bit 4 */
	bool unusedBit_1347_4 : 1 {};
	/**
	offset 4804 bit 5 */
	bool unusedBit_1347_5 : 1 {};
	/**
	offset 4804 bit 6 */
	bool unusedBit_1347_6 : 1 {};
	/**
	offset 4804 bit 7 */
	bool unusedBit_1347_7 : 1 {};
	/**
	offset 4804 bit 8 */
	bool unusedBit_1347_8 : 1 {};
	/**
	offset 4804 bit 9 */
	bool unusedBit_1347_9 : 1 {};
	/**
	offset 4804 bit 10 */
	bool unusedBit_1347_10 : 1 {};
	/**
	offset 4804 bit 11 */
	bool unusedBit_1347_11 : 1 {};
	/**
	offset 4804 bit 12 */
	bool unusedBit_1347_12 : 1 {};
	/**
	offset 4804 bit 13 */
	bool unusedBit_1347_13 : 1 {};
	/**
	offset 4804 bit 14 */
	bool unusedBit_1347_14 : 1 {};
	/**
	offset 4804 bit 15 */
	bool unusedBit_1347_15 : 1 {};
	/**
	offset 4804 bit 16 */
	bool unusedBit_1347_16 : 1 {};
	/**
	offset 4804 bit 17 */
	bool unusedBit_1347_17 : 1 {};
	/**
	offset 4804 bit 18 */
	bool unusedBit_1347_18 : 1 {};
	/**
	offset 4804 bit 19 */
	bool unusedBit_1347_19 : 1 {};
	/**
	offset 4804 bit 20 */
	bool unusedBit_1347_20 : 1 {};
	/**
	offset 4804 bit 21 */
	bool unusedBit_1347_21 : 1 {};
	/**
	offset 4804 bit 22 */
	bool unusedBit_1347_22 : 1 {};
	/**
	offset 4804 bit 23 */
	bool unusedBit_1347_23 : 1 {};
	/**
	offset 4804 bit 24 */
	bool unusedBit_1347_24 : 1 {};
	/**
	offset 4804 bit 25 */
	bool unusedBit_1347_25 : 1 {};
	/**
	offset 4804 bit 26 */
	bool unusedBit_1347_26 : 1 {};
	/**
	offset 4804 bit 27 */
	bool unusedBit_1347_27 : 1 {};
	/**
	offset 4804 bit 28 */
	bool unusedBit_1347_28 : 1 {};
	/**
	offset 4804 bit 29 */
	bool unusedBit_1347_29 : 1 {};
	/**
	offset 4804 bit 30 */
	bool unusedBit_1347_30 : 1 {};
	/**
	offset 4804 bit 31 */
	bool unusedBit_1347_31 : 1 {};
	/**
	 * Continuous time in WOT, capped at 30 seconds.
	 * units: s
	 * offset 4808
	 */
	float wotEnrichmentTimeBins[WOT_ENRICHMENT_SIZE] = {};
	/**
	 * Offset added to the target AFR after the fuel-dependent stoichiometric conversion. This is an AFR change, not a fuel percentage.
	 * units: afr
	 * offset 4840
	 */
	float wotEnrichmentAfrAdder[WOT_ENRICHMENT_SIZE] = {};
	/**
	 * VVT Advanced Mode: master enable. Replaces the fixed PID offset (Hold Duty) with an oil-pressure-based base duty curve, and the fixed PID P factor with a distance-from-target P factor curve.
	offset 4872 bit 0 */
	bool vvtAdvancedModeEnabled : 1 {};
	/**
	offset 4872 bit 1 */
	bool unusedBit_1381_1 : 1 {};
	/**
	offset 4872 bit 2 */
	bool unusedBit_1381_2 : 1 {};
	/**
	offset 4872 bit 3 */
	bool unusedBit_1381_3 : 1 {};
	/**
	offset 4872 bit 4 */
	bool unusedBit_1381_4 : 1 {};
	/**
	offset 4872 bit 5 */
	bool unusedBit_1381_5 : 1 {};
	/**
	offset 4872 bit 6 */
	bool unusedBit_1381_6 : 1 {};
	/**
	offset 4872 bit 7 */
	bool unusedBit_1381_7 : 1 {};
	/**
	offset 4872 bit 8 */
	bool unusedBit_1381_8 : 1 {};
	/**
	offset 4872 bit 9 */
	bool unusedBit_1381_9 : 1 {};
	/**
	offset 4872 bit 10 */
	bool unusedBit_1381_10 : 1 {};
	/**
	offset 4872 bit 11 */
	bool unusedBit_1381_11 : 1 {};
	/**
	offset 4872 bit 12 */
	bool unusedBit_1381_12 : 1 {};
	/**
	offset 4872 bit 13 */
	bool unusedBit_1381_13 : 1 {};
	/**
	offset 4872 bit 14 */
	bool unusedBit_1381_14 : 1 {};
	/**
	offset 4872 bit 15 */
	bool unusedBit_1381_15 : 1 {};
	/**
	offset 4872 bit 16 */
	bool unusedBit_1381_16 : 1 {};
	/**
	offset 4872 bit 17 */
	bool unusedBit_1381_17 : 1 {};
	/**
	offset 4872 bit 18 */
	bool unusedBit_1381_18 : 1 {};
	/**
	offset 4872 bit 19 */
	bool unusedBit_1381_19 : 1 {};
	/**
	offset 4872 bit 20 */
	bool unusedBit_1381_20 : 1 {};
	/**
	offset 4872 bit 21 */
	bool unusedBit_1381_21 : 1 {};
	/**
	offset 4872 bit 22 */
	bool unusedBit_1381_22 : 1 {};
	/**
	offset 4872 bit 23 */
	bool unusedBit_1381_23 : 1 {};
	/**
	offset 4872 bit 24 */
	bool unusedBit_1381_24 : 1 {};
	/**
	offset 4872 bit 25 */
	bool unusedBit_1381_25 : 1 {};
	/**
	offset 4872 bit 26 */
	bool unusedBit_1381_26 : 1 {};
	/**
	offset 4872 bit 27 */
	bool unusedBit_1381_27 : 1 {};
	/**
	offset 4872 bit 28 */
	bool unusedBit_1381_28 : 1 {};
	/**
	offset 4872 bit 29 */
	bool unusedBit_1381_29 : 1 {};
	/**
	offset 4872 bit 30 */
	bool unusedBit_1381_30 : 1 {};
	/**
	offset 4872 bit 31 */
	bool unusedBit_1381_31 : 1 {};
	/**
	 * VVT Advanced Mode: intake signed target-minus-position axis after control-direction inversion, shared by the P factor curve below.
	 * units: deg
	 * offset 4876
	 */
	float vvtAdvDistanceBinsIntake[VVT_ADV_DISTANCE_SIZE] = {};
	/**
	 * VVT Advanced Mode: intake PID P factor at each distance from target (curve Y). Replaces the fixed Intake PID "P factor" field. Entered/displayed x100 for friendlier decimal precision (e.g. 100 = 1.000 actual P factor, 0.1 = 0.001 actual).
	 * units: x100
	 * offset 4912
	 */
	float vvtAdvPFactorIntake[VVT_ADV_DISTANCE_SIZE] = {};
	/**
	 * VVT Advanced Mode: intake oil pressure axis (curve X), shared by the base duty curve below.
	 * units: kPa
	 * offset 4948
	 */
	float vvtAdvOilPressureBinsIntake[VVT_ADV_OILP_SIZE] = {};
	/**
	 * VVT Advanced Mode: intake base duty cycle at each oil pressure (curve Y). Evaluated at 0 kPa if oil pressure is unavailable.
	 * units: %
	 * offset 4972
	 */
	float vvtAdvDutyIntake[VVT_ADV_OILP_SIZE] = {};
	/**
	 * VVT Advanced Mode: exhaust signed target-minus-position axis after control-direction inversion, shared by the P factor curve below.
	 * units: deg
	 * offset 4996
	 */
	float vvtAdvDistanceBinsExhaust[VVT_ADV_DISTANCE_SIZE] = {};
	/**
	 * VVT Advanced Mode: exhaust PID P factor at each distance from target (curve Y). Replaces the fixed Exhaust PID "P factor" field. Entered/displayed x100 for friendlier decimal precision (e.g. 100 = 1.000 actual P factor, 0.1 = 0.001 actual).
	 * units: x100
	 * offset 5032
	 */
	float vvtAdvPFactorExhaust[VVT_ADV_DISTANCE_SIZE] = {};
	/**
	 * VVT Advanced Mode: exhaust oil pressure axis (curve X), shared by the base duty curve below.
	 * units: kPa
	 * offset 5068
	 */
	float vvtAdvOilPressureBinsExhaust[VVT_ADV_OILP_SIZE] = {};
	/**
	 * VVT Advanced Mode: exhaust base duty cycle at each oil pressure (curve Y). Evaluated at 0 kPa if oil pressure is unavailable.
	 * units: %
	 * offset 5092
	 */
	float vvtAdvDutyExhaust[VVT_ADV_OILP_SIZE] = {};
	/**
	 * Intake VVT integral minimum. Both integral limits zero restore defaults of -1000 and 1000.
	 * units: %
	 * offset 5116
	 */
	int16_t vvtIntake_iTermMin;
	/**
	 * Intake VVT integral maximum. Applies in both basic and advanced modes.
	 * units: %
	 * offset 5118
	 */
	int16_t vvtIntake_iTermMax;
	/**
	 * Exhaust VVT integral minimum. Both integral limits zero restore defaults of -1000 and 1000.
	 * units: %
	 * offset 5120
	 */
	int16_t vvtExhaust_iTermMin;
	/**
	 * Exhaust VVT integral maximum. Applies in both basic and advanced modes.
	 * units: %
	 * offset 5122
	 */
	int16_t vvtExhaust_iTermMax;
	/**
	 * Overrun uses the TPS, RPM and vehicle-speed thresholds below. Decel requires Enable Engine State Machine and its Decelerating state, or RPM falling faster than the Decel RPM rate threshold when no vehicle-speed sensor is configured. Decel does not use the overrun TPS/RPM/speed/gear gates. Both accepts either path. Coolant, MAP, tuning and Lua guards apply in all modes. Active state-machine shifts suppress fuel cut.
	 * offset 5124
	 */
	dfco_fuel_cut_mode_e dfcoFuelCutMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5125
	 */
	uint8_t alignmentFill_at_5125[3] = {};
	/**
	 * Require a valid DetectedGear greater than zero for the Overrun path. Neutral, unknown or invalid gear prevents overrun fuel cut. Configure gear detection before enabling. Does not restrict the Decel path.
	offset 5128 bit 0 */
	bool coastingFuelCutRequiresGear : 1 {};
	/**
	offset 5128 bit 1 */
	bool unusedBit_1427_1 : 1 {};
	/**
	offset 5128 bit 2 */
	bool unusedBit_1427_2 : 1 {};
	/**
	offset 5128 bit 3 */
	bool unusedBit_1427_3 : 1 {};
	/**
	offset 5128 bit 4 */
	bool unusedBit_1427_4 : 1 {};
	/**
	offset 5128 bit 5 */
	bool unusedBit_1427_5 : 1 {};
	/**
	offset 5128 bit 6 */
	bool unusedBit_1427_6 : 1 {};
	/**
	offset 5128 bit 7 */
	bool unusedBit_1427_7 : 1 {};
	/**
	offset 5128 bit 8 */
	bool unusedBit_1427_8 : 1 {};
	/**
	offset 5128 bit 9 */
	bool unusedBit_1427_9 : 1 {};
	/**
	offset 5128 bit 10 */
	bool unusedBit_1427_10 : 1 {};
	/**
	offset 5128 bit 11 */
	bool unusedBit_1427_11 : 1 {};
	/**
	offset 5128 bit 12 */
	bool unusedBit_1427_12 : 1 {};
	/**
	offset 5128 bit 13 */
	bool unusedBit_1427_13 : 1 {};
	/**
	offset 5128 bit 14 */
	bool unusedBit_1427_14 : 1 {};
	/**
	offset 5128 bit 15 */
	bool unusedBit_1427_15 : 1 {};
	/**
	offset 5128 bit 16 */
	bool unusedBit_1427_16 : 1 {};
	/**
	offset 5128 bit 17 */
	bool unusedBit_1427_17 : 1 {};
	/**
	offset 5128 bit 18 */
	bool unusedBit_1427_18 : 1 {};
	/**
	offset 5128 bit 19 */
	bool unusedBit_1427_19 : 1 {};
	/**
	offset 5128 bit 20 */
	bool unusedBit_1427_20 : 1 {};
	/**
	offset 5128 bit 21 */
	bool unusedBit_1427_21 : 1 {};
	/**
	offset 5128 bit 22 */
	bool unusedBit_1427_22 : 1 {};
	/**
	offset 5128 bit 23 */
	bool unusedBit_1427_23 : 1 {};
	/**
	offset 5128 bit 24 */
	bool unusedBit_1427_24 : 1 {};
	/**
	offset 5128 bit 25 */
	bool unusedBit_1427_25 : 1 {};
	/**
	offset 5128 bit 26 */
	bool unusedBit_1427_26 : 1 {};
	/**
	offset 5128 bit 27 */
	bool unusedBit_1427_27 : 1 {};
	/**
	offset 5128 bit 28 */
	bool unusedBit_1427_28 : 1 {};
	/**
	offset 5128 bit 29 */
	bool unusedBit_1427_29 : 1 {};
	/**
	offset 5128 bit 30 */
	bool unusedBit_1427_30 : 1 {};
	/**
	offset 5128 bit 31 */
	bool unusedBit_1427_31 : 1 {};
	/**
	 * Single relay preserves normal priming and engine-motion control. Dual stages a second relay. PWM regulates low fuel pressure. Burn changes to mode, pins or PWM frequency before use.
	 * offset 5132
	 */
	fuel_pump_mode_e fuelPumpMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5133
	 */
	uint8_t alignmentFill_at_5133[1] = {};
	/**
	 * Secondary pump relay output, used in Dual relay mode.
	 * offset 5134
	 */
	output_pin_e fuelPump2Pin;
	/**
	 * Secondary pump output polarity.
	 * offset 5136
	 */
	pin_output_mode_e fuelPump2PinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5137
	 */
	uint8_t alignmentFill_at_5137[1] = {};
	/**
	 * Secondary pump engages only when RPM, fueling load and TPS all exceed their thresholds.
	 * units: rpm
	 * offset 5138
	 */
	uint16_t secondaryFpActivationRpm;
	/**
	 * Fueling-load threshold for the secondary pump. Speed Density uses MAP in kPa, other fueling algorithms use their own load scale.
	 * units: load
	 * offset 5140
	 */
	uint16_t secondaryFpActivationLoad;
	/**
	 * TPS threshold for the secondary pump.
	 * units: %
	 * offset 5142
	 */
	uint8_t secondaryFpActivationTps;
	/**
	 * Secondary pump releases when any input falls below its activation threshold minus hysteresis.
	 * units: %
	 * offset 5143
	 */
	uint8_t secondaryFpTpsHysteresis;
	/**
	 * RPM release hysteresis for the secondary pump.
	 * units: rpm
	 * offset 5144
	 */
	uint16_t secondaryFpRpmHysteresis;
	/**
	 * Fueling-load release hysteresis for the secondary pump.
	 * units: load
	 * offset 5146
	 */
	uint16_t secondaryFpLoadHysteresis;
	/**
	 * PWM command frequency. Use a compatible pump controller or power driver and its specified frequency. Burn frequency changes.
	 * units: Hz
	 * offset 5148
	 */
	uint16_t fuelPumpPwmFrequency;
	/**
	 * Minimum duty while pressure control is active, including relief or an invalid control result. An inactive pump always receives zero duty.
	 * units: %
	 * offset 5150
	 */
	uint8_t fuelPumpMinDuty;
	/**
	 * Maximum running duty and key-on prime duty.
	 * units: %
	 * offset 5151
	 */
	uint8_t fuelPumpMaxDuty;
	/**
	 * Fuel-pressure PID correction added to the base-duty map. Output limits are signed duty corrections, not total pump duty.
	 * offset 5152
	 */
	pid_s fuelPumpControl;
	/**
	 * Minimum integral correction.
	 * units: %
	 * offset 5172
	 */
	int16_t fuelPump_iTermMin;
	/**
	 * Maximum integral correction.
	 * units: %
	 * offset 5174
	 */
	int16_t fuelPump_iTermMax;
	/**
	 * Hold minimum active duty during low-demand overpressure. Intended for returnless systems. Feedback resumes below the recovery threshold or when fuel demand increases.
	offset 5176 bit 0 */
	bool fuelPumpAggressiveRelief : 1 {};
	/**
	offset 5176 bit 1 */
	bool unusedBit_1476_1 : 1 {};
	/**
	offset 5176 bit 2 */
	bool unusedBit_1476_2 : 1 {};
	/**
	offset 5176 bit 3 */
	bool unusedBit_1476_3 : 1 {};
	/**
	offset 5176 bit 4 */
	bool unusedBit_1476_4 : 1 {};
	/**
	offset 5176 bit 5 */
	bool unusedBit_1476_5 : 1 {};
	/**
	offset 5176 bit 6 */
	bool unusedBit_1476_6 : 1 {};
	/**
	offset 5176 bit 7 */
	bool unusedBit_1476_7 : 1 {};
	/**
	offset 5176 bit 8 */
	bool unusedBit_1476_8 : 1 {};
	/**
	offset 5176 bit 9 */
	bool unusedBit_1476_9 : 1 {};
	/**
	offset 5176 bit 10 */
	bool unusedBit_1476_10 : 1 {};
	/**
	offset 5176 bit 11 */
	bool unusedBit_1476_11 : 1 {};
	/**
	offset 5176 bit 12 */
	bool unusedBit_1476_12 : 1 {};
	/**
	offset 5176 bit 13 */
	bool unusedBit_1476_13 : 1 {};
	/**
	offset 5176 bit 14 */
	bool unusedBit_1476_14 : 1 {};
	/**
	offset 5176 bit 15 */
	bool unusedBit_1476_15 : 1 {};
	/**
	offset 5176 bit 16 */
	bool unusedBit_1476_16 : 1 {};
	/**
	offset 5176 bit 17 */
	bool unusedBit_1476_17 : 1 {};
	/**
	offset 5176 bit 18 */
	bool unusedBit_1476_18 : 1 {};
	/**
	offset 5176 bit 19 */
	bool unusedBit_1476_19 : 1 {};
	/**
	offset 5176 bit 20 */
	bool unusedBit_1476_20 : 1 {};
	/**
	offset 5176 bit 21 */
	bool unusedBit_1476_21 : 1 {};
	/**
	offset 5176 bit 22 */
	bool unusedBit_1476_22 : 1 {};
	/**
	offset 5176 bit 23 */
	bool unusedBit_1476_23 : 1 {};
	/**
	offset 5176 bit 24 */
	bool unusedBit_1476_24 : 1 {};
	/**
	offset 5176 bit 25 */
	bool unusedBit_1476_25 : 1 {};
	/**
	offset 5176 bit 26 */
	bool unusedBit_1476_26 : 1 {};
	/**
	offset 5176 bit 27 */
	bool unusedBit_1476_27 : 1 {};
	/**
	offset 5176 bit 28 */
	bool unusedBit_1476_28 : 1 {};
	/**
	offset 5176 bit 29 */
	bool unusedBit_1476_29 : 1 {};
	/**
	offset 5176 bit 30 */
	bool unusedBit_1476_30 : 1 {};
	/**
	offset 5176 bit 31 */
	bool unusedBit_1476_31 : 1 {};
	/**
	 * Relief is eligible below this injected mass per cycle.
	 * units: mg
	 * offset 5180
	 */
	uint16_t fuelPumpReliefMaxInjectedMass;
	/**
	 * Engage relief above target pressure plus this value.
	 * units: kPa
	 * offset 5182
	 */
	uint16_t fuelPumpReliefEngageOverpressure;
	/**
	 * Release relief at or below target plus this value. Set below the engagement threshold.
	 * units: kPa
	 * offset 5184
	 */
	uint16_t fuelPumpReliefRecoverOverpressure;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5186
	 */
	uint8_t alignmentFill_at_5186[2] = {};
};
static_assert(sizeof(engine_configuration_s) == 5188);

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
	scaled_channel<int16_t, 1, 1> table[BLEND_TABLE_COUNT][BLEND_TABLE_COUNT] = {};
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

// start of blend_table_s_BLEND_PRECISION
struct blend_table_s_BLEND_PRECISION {
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
static_assert(sizeof(blend_table_s_BLEND_PRECISION) == 188);

// start of blend_table_s_TARGET_AFR_BLEND_PRECISION
struct blend_table_s_TARGET_AFR_BLEND_PRECISION {
	/**
	 * offset 0
	 */
	scaled_channel<int16_t, 100, 1> table[BLEND_TABLE_COUNT][BLEND_TABLE_COUNT] = {};
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
static_assert(sizeof(blend_table_s_TARGET_AFR_BLEND_PRECISION) == 188);

// start of KnockGain
struct KnockGain {
	/**
	 * units: dB
	 * offset 0
	 */
	int8_t table[6][6] = {};
};
static_assert(sizeof(KnockGain) == 36);

// start of persistent_config_s
struct persistent_config_s {
	/**
	 * offset 0
	 */
	engine_configuration_s engineConfiguration;
	/**
	 * offset 5188
	 */
	float tmfTable[TMF_RATIO_SIZE][TMF_SIZE] = {};
	/**
	 * offset 5204
	 */
	float tmfRatioBins[TMF_RATIO_SIZE] = {};
	/**
	 * offset 5212
	 */
	float tmfOpeningBins[TMF_SIZE] = {};
	/**
	 * units: mult
	 * offset 5220
	 */
	float postCrankingFactor[CRANKING_ENRICH_CLT_COUNT][CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: count
	 * offset 5364
	 */
	uint16_t postCrankingDurationBins[CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 5376
	 */
	int16_t postCrankingCLTBins[CRANKING_ENRICH_CLT_COUNT] = {};
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	 * units: target TPS position
	 * offset 5388
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: ETB duty cycle bias
	 * offset 5420
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * target Wastegate value, 0 to 100%
	 * units: target DC position
	 * offset 5452
	 */
	int8_t dcWastegateBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: DC wastegate duty cycle bias
	 * offset 5460
	 */
	scaled_channel<int16_t, 100, 1> dcWastegateBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * units: %
	 * offset 5476
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE] = {};
	/**
	 * units: Load
	 * offset 5540
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5548
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_RPM_SIZE] = {};
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	 * units: RPM
	 * offset 5556
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: ms
	 * offset 5572
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE] = {};
	/**
	 * CLT-based target RPM for automatic idle controller
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 5588
	 */
	scaled_channel<int16_t, 1, 1> cltIdleRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * See idleRpmPid
	 * units: RPM
	 * offset 5620
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE] = {};
	/**
	 * units: deg
	 * offset 5636
	 */
	scaled_channel<int16_t, 10, 1> ignitionCltCorrTable[CLT_TIMING_LOAD_AXIS_SIZE][CLT_TIMING_TEMP_AXIS_SIZE] = {};
	/**
	 * CLT-based timing correction
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 5686
	 */
	scaled_channel<int16_t, 1, 1> ignitionCltCorrTempBins[CLT_TIMING_TEMP_AXIS_SIZE] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 5696
	 */
	scaled_channel<uint8_t, 1, 5> ignitionCltCorrLoadBins[CLT_TIMING_LOAD_AXIS_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5701
	 */
	uint8_t alignmentFill_at_5701[3] = {};
	/**
	 * units: x
	 * offset 5704
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 5768
	 */
	float scriptCurve1[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 5832
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 5896
	 */
	float scriptCurve2[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 5960
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5992
	 */
	float scriptCurve3[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 6024
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 6056
	 */
	float scriptCurve4[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 6088
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 6120
	 */
	float scriptCurve5[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 6152
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 6184
	 */
	float scriptCurve6[SCRIPT_CURVE_8] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 6216
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE] = {};
	/**
	 * units: RPM
	 * offset 6232
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE] = {};
	/**
	 * units: ratio
	 * offset 6248
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE] = {};
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * units: Ratio
	 * offset 6312
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 6344
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: RPM
	 * offset 6376
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: deg
	 * offset 6384
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: RPM
	 * offset 6392
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: %
	 * offset 6408
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE] = {};
	/**
	 * offset 6424
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * units: RPM
	 * offset 6488
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT] = {};
	/**
	 * offset 6496
	 */
	uint16_t boostOpenLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * offset 6512
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * offset 6576
	 */
	uint16_t boostClosedLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * units: %
	 * offset 6592
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 6656
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 6664
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_RPM_SIZE] = {};
	/**
	 * CLT-based cranking position %. The values in this curve represent a percentage of the ETB Maximum angle. e.g. If "ETB Idle Maximum Angle" is 10, a value of 70 means 7% ETB Position.
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 6672
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based cranking position %. The values in this curve represent a percentage of the ETB Maximum angle. e.g. If "ETB Idle Maximum Angle" is 10, a value of 70 means 7% ETB Position.
	 * units: percent
	 * offset 6704
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 6736
	 */
	float afterCrankingIACtaperDurationBins[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	 * Should be 100 once tune is better
	 * units: cycles
	 * offset 6760
	 */
	uint16_t afterCrankingIACtaperDuration[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: RPM
	 * offset 6772
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: deg
	 * offset 6780
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 6812
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE_RPM] = {};
	/**
	 * units: load
	 * offset 6816
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE] = {};
	/**
	 * units: %
	 * offset 6820
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE_RPM] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 6852
	 */
	float cltFuelCorrBins[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 6916
	 */
	float cltFuelCorr[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 6980
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 7044
	 */
	float iatFuelCorr[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 7108
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 7140
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: counter
	 * offset 7172
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 7204
	 */
	int16_t crankingCycleFuelCltBins[CRANKING_CYCLE_CLT_SIZE] = {};
	/**
	 * Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for IAT, TPS ect, to give the final cranking pulse width.
	 * A reasonable starting point is 60mg per liter per cylinder.
	 * ex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.
	 * units: mg
	 * offset 7212
	 */
	float crankingCycleBaseFuel[CRANKING_CYCLE_CLT_SIZE][CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 7340
	 */
	float cltIdleCorrBins[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: %
	 * offset 7364
	 */
	float cltIdleCorrTable[CLT_IDLE_TABLE_RPM_SIZE][CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * units: Target RPM
	 * offset 7460
	 */
	scaled_channel<uint8_t, 1, 100> rpmIdleCorrBins[CLT_IDLE_TABLE_RPM_SIZE] = {};
	/**
	 * Long Term Idle Trim (LTIT) multiplicativo para idle open loop
	 * units: %
	 * offset 7464
	 */
	scaled_channel<uint16_t, 10, 1> ltitTable[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	 * units: kg/hour
	 * offset 7476
	 */
	float mafDecoding[MAF_DECODING_COUNT] = {};
	/**
	 * units: V
	 * offset 7604
	 */
	float mafDecodingBins[MAF_DECODING_COUNT] = {};
	/**
	 * units: deg
	 * offset 7732
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IAT_IGN_CORR_LOAD_COUNT][IAT_IGN_CORR_TEMP_COUNT] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 7860
	 */
	int8_t ignitionIatCorrTempBins[IAT_IGN_CORR_TEMP_COUNT] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 7868
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[IAT_IGN_CORR_LOAD_COUNT] = {};
	/**
	 * Crank degrees relative to each cylinder's compression TDC: 0 = TDC, negative = before, positive = after.
	 * Injection timing control mode selects End, Start, or Center of the nominal pulse.
	 * On a four-stroke engine angles wrap every 720 degrees: -400 = +320, -250 = +470, +400 = -320. Valve timing depends on the camshaft and VVT.
	 * units: deg
	 * offset 7876
	 */
	int16_t injectionPhase[INJ_PHASE_LOAD_COUNT][INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 8388
	 */
	uint16_t injPhaseLoadBins[INJ_PHASE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 8420
	 */
	uint16_t injPhaseRpmBins[INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: onoff
	 * offset 8452
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT] = {};
	/**
	 * This table represents MAP at a given TPS vs RPM, which we use if our MAP sensor has failed, or if we are using MAP Prediciton. 
	 *  This table should be a direct representation of MAP, you can tune it manually by disconnecting MAP sensor, and filling out the table with values that match an external gauge that shows MAP.
	 * Additionally, you can also use MLV to get the map values and/or generate the table for you
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 8512
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[MAP_EST_LOAD_COUNT][MAP_EST_RPM_COUNT] = {};
	/**
	 * units: % TPS
	 * offset 9024
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[MAP_EST_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 9056
	 */
	uint16_t mapEstimateRpmBins[MAP_EST_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 9088
	 */
	int8_t vvtTable1[VVT_TABLE_SIZE][VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 9152
	 */
	uint16_t vvtTable1LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 9168
	 */
	uint16_t vvtTable1RpmBins[VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 9184
	 */
	int8_t vvtTable2[VVT_TABLE_SIZE][VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 9248
	 */
	uint16_t vvtTable2LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 9264
	 */
	uint16_t vvtTable2RpmBins[VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 9280
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(ignLoadUnitLabels, ignLoadUnitIdx)}
	 * offset 9792
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 9824
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT] = {};
	/**
	 * units: %
	 * offset 9856
	 */
	scaled_channel<uint16_t, 10, 1> veTable[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(veLoadUnitLabels, veLoadUnitIdx)}
	 * offset 10368
	 */
	uint16_t veLoadBins[VE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 10400
	 */
	uint16_t veRpmBins[VE_RPM_COUNT] = {};
	/**
	 * units: {useLambdaOnInterface ? "lambda" : "afr"}
	 * offset 10432
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(afrLoadUnitLabels, afrLoadUnitIdx)}
	 * offset 10688
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 10720
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT] = {};
	/**
	 * X: previous TPS. Y: new TPS. MS Adder values are milliseconds. Percent Adder values are fractions: 0.20 adds 20% in sequential injection with neutral corrections. Injection-mode scaling also applies. MAP Prediction and Lua ignore this table. Changing modes reinterprets existing values.
	 * units: ms/frac
	 * offset 10752
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: %
	 * offset 11008
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: %
	 * offset 11040
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: value
	 * offset 11072
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8] = {};
	/**
	 * units: L
	 * offset 11328
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 11344
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: value
	 * offset 11360
	 */
	float scriptTable2[TABLE_2_LOAD_SIZE][TABLE_2_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 11616
	 */
	int16_t scriptTable2LoadBins[TABLE_2_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 11632
	 */
	int16_t scriptTable2RpmBins[TABLE_2_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 11648
	 */
	uint8_t scriptTable3[TABLE_3_LOAD_SIZE][TABLE_3_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 11712
	 */
	int16_t scriptTable3LoadBins[TABLE_3_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 11728
	 */
	int16_t scriptTable3RpmBins[TABLE_3_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 11744
	 */
	uint8_t scriptTable4[TABLE_4_LOAD_SIZE][TABLE_4_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 11824
	 */
	int16_t scriptTable4LoadBins[TABLE_4_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 11840
	 */
	int16_t scriptTable4RpmBins[TABLE_4_RPM_SIZE] = {};
	/**
	 * units: {bitStringValue(ignLoadUnitLabels, ignLoadUnitIdx)}
	 * offset 11860
	 */
	uint16_t ignTrimLoadBins[IGN_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 11868
	 */
	uint16_t ignTrimRpmBins[IGN_TRIM_SIZE] = {};
	/**
	 * offset 11876
	 */
	ign_cyl_trim_s ignTrims[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: {bitStringValue(veLoadUnitLabels, veLoadUnitIdx)}
	 * offset 12068
	 */
	uint16_t fuelTrimLoadBins[FUEL_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 12076
	 */
	uint16_t fuelTrimRpmBins[FUEL_TRIM_SIZE] = {};
	/**
	 * offset 12084
	 */
	fuel_cyl_trim_s fuelTrims[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: ratio
	 * offset 12276
	 */
	scaled_channel<uint16_t, 100, 1> unusedCrankingFuelCoefE100[CRANKING_CURVE_SIZE] = {};
	/**
	 * Ethanol % axis (Y) for crankingFuelFlexTable.
	 * units: %
	 * offset 12292
	 */
	uint8_t crankingFuelFlexBins[CRANKING_FLEX_SIZE] = {};
	/**
	 * Cranking coolant multiplier as a function of coolant (X axis, shared crankingFuelBins) and ethanol % (Y axis, crankingFuelFlexBins). Used instead of crankingFuelCoef when flexCranking is enabled and a flex sensor is present.
	 * units: mult
	 * offset 12296
	 */
	scaled_channel<uint8_t, 50, 1> crankingFuelFlexTable[CRANKING_FLEX_SIZE][CRANKING_CURVE_SIZE] = {};
	/**
	 * units: Airmass
	 * offset 12328
	 */
	scaled_channel<uint8_t, 1, 5> tcu_pcAirmassBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12336
	 */
	uint8_t tcu_pcValsR[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12344
	 */
	uint8_t tcu_pcValsN[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12352
	 */
	uint8_t tcu_pcVals1[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12360
	 */
	uint8_t tcu_pcVals2[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12368
	 */
	uint8_t tcu_pcVals3[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12376
	 */
	uint8_t tcu_pcVals4[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12384
	 */
	uint8_t tcu_pcVals12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12392
	 */
	uint8_t tcu_pcVals23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12400
	 */
	uint8_t tcu_pcVals34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12408
	 */
	uint8_t tcu_pcVals21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12416
	 */
	uint8_t tcu_pcVals32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 12424
	 */
	uint8_t tcu_pcVals43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: TPS
	 * offset 12432
	 */
	uint8_t tcu_tccTpsBins[8] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 12440
	 */
	uint8_t tcu_tccLockSpeed[8] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 12448
	 */
	uint8_t tcu_tccUnlockSpeed[8] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 12456
	 */
	uint8_t tcu_32SpeedBins[8] = {};
	/**
	 * units: %
	 * offset 12464
	 */
	uint8_t tcu_32Vals[8] = {};
	/**
	 * units: %
	 * offset 12472
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[ETB2_TRIM_SIZE][ETB2_TRIM_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 12508
	 */
	uint8_t throttle2TrimTpsBins[ETB2_TRIM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 12514
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[ETB2_TRIM_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 12520
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[KNOCK_TABLE_SIZE][KNOCK_TABLE_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 12556
	 */
	uint8_t maxKnockRetardLoadBins[KNOCK_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 12562
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[KNOCK_TABLE_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 12568
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 12600
	 */
	uint16_t alsIgnRetardLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 12608
	 */
	uint16_t alsIgnRetardrpmBins[ALS_RPM_SIZE] = {};
	/**
	 * units: percent
	 * offset 12616
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 12648
	 */
	uint16_t alsFuelAdjustmentLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 12656
	 */
	uint16_t alsFuelAdjustmentrpmBins[ALS_RPM_SIZE] = {};
	/**
	 * units: ratio
	 * offset 12664
	 */
	scaled_channel<int16_t, 1, 10> ALSIgnSkipTable[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 12696
	 */
	uint16_t alsIgnSkipLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 12704
	 */
	uint16_t alsIgnSkiprpmBins[ALS_RPM_SIZE] = {};
	/**
	 * offset 12712
	 */
	blend_table_s_BLEND_PRECISION ignBlends[IGN_BLEND_COUNT] = {};
	/**
	 * offset 13464
	 */
	blend_table_s_BLEND_PRECISION veBlends[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 14216
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaBins[THR_EST_SIZE] = {};
	/**
	 * In units of g/s normalized to choked flow conditions
	 * units: g/s
	 * offset 14240
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaValues[THR_EST_SIZE] = {};
	/**
	 * offset 14264
	 */
	blend_table_s_BLEND_PRECISION boostOpenLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 14640
	 */
	blend_table_s_BLEND_PRECISION boostClosedLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * units: level
	 * offset 15016
	 */
	float tcu_rangeP[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15040
	 */
	float tcu_rangeR[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15064
	 */
	float tcu_rangeN[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15088
	 */
	float tcu_rangeD[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15112
	 */
	float tcu_rangeM[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15136
	 */
	float tcu_rangeM3[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15160
	 */
	float tcu_rangeM2[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15184
	 */
	float tcu_rangeM1[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15208
	 */
	float tcu_rangePlus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15232
	 */
	float tcu_rangeMinus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 15256
	 */
	float tcu_rangeLow[RANGE_INPUT_COUNT] = {};
	/**
	 * units: lambda
	 * offset 15280
	 */
	scaled_channel<uint8_t, 100, 1> lambdaMaxDeviationTable[LAM_SIZE][LAM_RPM_SIZE] = {};
	/**
	 * units: {bitStringValue(afrLoadUnitLabels, afrLoadUnitIdx)}
	 * offset 15296
	 */
	uint16_t lambdaMaxDeviationLoadBins[LAM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 15304
	 */
	uint16_t lambdaMaxDeviationRpmBins[LAM_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 15312
	 */
	uint8_t injectorStagingTable[INJ_STAGING_COUNT][INJ_STAGING_RPM_SIZE] = {};
	/**
	 * units: {bitStringValue(afrLoadUnitLabels, afrLoadUnitIdx)}
	 * offset 15348
	 */
	uint16_t injectorStagingLoadBins[INJ_STAGING_COUNT] = {};
	/**
	 * units: RPM
	 * offset 15360
	 */
	uint16_t injectorStagingRpmBins[INJ_STAGING_RPM_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 15372
	 */
	int16_t wwCltBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 15388
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 15396
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 15404
	 */
	uint8_t wwMapBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 15412
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 15420
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: %
	 * offset 15428
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: deg
	 * offset 15444
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: volts
	 * offset 15460
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: ms
	 * offset 15468
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: kPa
	 * offset 15484
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE] = {};
	/**
	 * units: load
	 * offset 15684
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 15704
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: %
	 * offset 15714
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_RPM_SIZE] = {};
	/**
	 * units: cc/lobe
	 * offset 15814
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: RPM
	 * offset 15834
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 15844
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensation[HPFP_FUEL_MASS_COMPENSATION_SIZE][HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 15972
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensationFuelMass[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: bar
	 * offset 15988
	 */
	scaled_channel<uint16_t, 10, 1> hpfpFuelMassCompensationFuelPressure[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: ms
	 * offset 16004
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearization[FLOW_LINEARIZATION_PRESSURE_SIZE][FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 16516
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearizationFuelMassBins[FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: bar
	 * offset 16548
	 */
	scaled_channel<uint16_t, 10, 1> injectorFlowLinearizationPressureBins[FLOW_LINEARIZATION_PRESSURE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 16580
	 */
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	 * units: dB
	 * offset 16612
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 16628
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * Multiplier applied to additive TPS enrichment at each RPM. 1.00 is neutral, 0.50 halves and 2.00 doubles. Ignored by MAP Prediction and Lua.
	 * units: multiplier
	 * offset 16632
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: RPM
	 * offset 16636
	 */
	scaled_channel<uint8_t, 1, 50> predictiveMapBlendDurationBins[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * Transition duration and total event time limit from estimated MAP toward current measured MAP, interpolated at current RPM. Rising estimated MAP restarts blending without extending the event time limit. Prediction may exit earlier. The largest duration in this curve also sizes TPS history. Used only by MAP Prediction.
	 * units: second
	 * offset 16640
	 */
	scaled_channel<uint8_t, 50, 1> predictiveMapBlendDurationValues[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * Coolant temperature axis for the flex-fuel transient compensation tables
	 * units: C
	 * offset 16644
	 */
	int16_t flexTransientCltBins[FLEX_TRANSIENT_CLT_SIZE] = {};
	/**
	 * Ethanol percentage axis for the flex-fuel transient compensation tables
	 * units: %
	 * offset 16660
	 */
	uint8_t flexTransientEthanolBins[FLEX_TRANSIENT_ETH_SIZE] = {};
	/**
	 * Additive TPS AE multiplier over CLT (X) and ethanol (Y), enabled by Flex fuel transient compensation. 1.00 is neutral. Ignored by MAP Prediction and Lua. CLT/ethanol bins are shared with both wall-film flex tables.
	 * units: mult
	 * offset 16668
	 */
	scaled_channel<uint8_t, 50, 1> flexAeMult[FLEX_TRANSIENT_ETH_SIZE][FLEX_TRANSIENT_CLT_SIZE] = {};
	/**
	 * Multiplier of wall-film evaporation time constant over CLT (X) and ethanol (Y). Above 1 makes evaporation slower. Applies with every AE strategy and Basic/Advanced. CLT/ethanol bins are shared with the other flex transient tables.
	 * units: mult
	 * offset 16732
	 */
	scaled_channel<uint8_t, 50, 1> flexWwTauMult[FLEX_TRANSIENT_ETH_SIZE][FLEX_TRANSIENT_CLT_SIZE] = {};
	/**
	 * Multiplier of wall-film deposition fraction over CLT (X) and ethanol (Y). Above 1 increases modeled deposition. Applies with every AE strategy and Basic/Advanced. CLT/ethanol bins are shared with the other flex transient tables.
	 * units: mult
	 * offset 16796
	 */
	scaled_channel<uint8_t, 50, 1> flexWwBetaMult[FLEX_TRANSIENT_ETH_SIZE][FLEX_TRANSIENT_CLT_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 16860
	 */
	scaled_channel<int16_t, 1, 1> cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 16868
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: volt
	 * offset 16876
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: %
	 * offset 16892
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: volts
	 * offset 16900
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: multiplier
	 * offset 16908
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 16916
	 */
	scaled_channel<uint8_t, 1, 1> tcu_shiftTpsBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 16924
	 */
	uint8_t tcu_shiftSpeed12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 16932
	 */
	uint8_t tcu_shiftSpeed23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 16940
	 */
	uint8_t tcu_shiftSpeed34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 16948
	 */
	uint8_t tcu_shiftSpeed21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 16956
	 */
	uint8_t tcu_shiftSpeed32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 16964
	 */
	uint8_t tcu_shiftSpeed43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: ms
	 * offset 16972
	 */
	float tcu_shiftTime;
	/**
	 * units: volts
	 * offset 16976
	 */
	scaled_channel<int16_t, 10, 1> alternatorVoltageTargetTable[ALTERNATOR_VOLTAGE_TARGET_SIZE][ALTERNATOR_VOLTAGE_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 17008
	 */
	uint16_t alternatorVoltageTargetLoadBins[ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 17016
	 */
	uint16_t alternatorVoltageTargetRpmBins[ALTERNATOR_VOLTAGE_RPM_SIZE] = {};
	/**
	 * units: C
	 * offset 17024
	 */
	float cltBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 17044
	 */
	float cltBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 17064
	 */
	float iatBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 17084
	 */
	float iatBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 17104
	 */
	float cltBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 17124
	 */
	float cltBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 17144
	 */
	float iatBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 17164
	 */
	float iatBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * "Minimum Battery Voltage"
	 * units: V
	 * offset 17184
	 */
	scaled_channel<uint8_t, 10, 1> cel_battery_min_v;
	/**
	 * "Maximum Battery Voltage"
	 * units: V
	 * offset 17185
	 */
	scaled_channel<uint8_t, 10, 1> cel_battery_max_v;
	/**
	 * "Minimum MAP V"
	 * units: V
	 * offset 17186
	 */
	scaled_channel<uint8_t, 50, 1> cel_map_min_v;
	/**
	 * "Maximum MAP V"
	 * units: V
	 * offset 17187
	 */
	scaled_channel<uint8_t, 50, 1> cel_map_max_v;
	/**
	 * "Minimum IAT V"
	 * units: V
	 * offset 17188
	 */
	scaled_channel<uint8_t, 50, 1> cel_iat_min_v;
	/**
	 * "Maximum IAT V"
	 * units: V
	 * offset 17189
	 */
	scaled_channel<uint8_t, 50, 1> cel_iat_max_v;
	/**
	 * "Minimum TPS V"
	 * units: V
	 * offset 17190
	 */
	scaled_channel<uint8_t, 50, 1> cel_tps_min_v;
	/**
	 * "Maximum TPS V"
	 * units: V
	 * offset 17191
	 */
	scaled_channel<uint8_t, 50, 1> cel_tps_max_v;
	/**
	 * units: RPM
	 * offset 17192
	 */
	scaled_channel<uint8_t, 1, 100> minimumOilPressureBins[8] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 17200
	 */
	scaled_channel<uint8_t, 1, 10> minimumOilPressureValues[8] = {};
	/**
	 * offset 17208
	 */
	blend_table_s_TARGET_AFR_BLEND_PRECISION targetAfrBlends[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * @@DYNO_RPM_STEP_TOOLTIP@@
	 * units: Rpm
	 * offset 17584
	 */
	scaled_channel<uint8_t, 1, 1> dynoRpmStep;
	/**
	 * @@DYNO_SAE_TEMPERATURE_C_TOOLTIP@@
	 * units: C
	 * offset 17585
	 */
	scaled_channel<int8_t, 1, 1> dynoSaeTemperatureC;
	/**
	 * @@DYNO_SAE_RELATIVE_HUMIDITY_TOOLTIP@@
	 * units: %
	 * offset 17586
	 */
	scaled_channel<uint8_t, 1, 1> dynoSaeRelativeHumidity;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 17587
	 */
	uint8_t alignmentFill_at_17587[1] = {};
	/**
	 * @@DYNO_SAE_BARO_TOOLTIP@@
	 * units: KPa
	 * offset 17588
	 */
	scaled_channel<float, 1, 1> dynoSaeBaro;
	/**
	 * @@DYNO_CAR_WHEEL_DIA_INCH_TOOLTIP@@
	 * units: Inch
	 * offset 17592
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelDiaInch;
	/**
	 * @@DYNO_CAR_WHEEL_ASPECT_RATIO_TOOLTIP@@
	 * units: Aspect Ratio (height)
	 * offset 17593
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelAspectRatio;
	/**
	 * @@DYNO_CAR_WHEEL_TIRE_WIDTH_TOOLTIP@@
	 * units: Width mm
	 * offset 17594
	 */
	scaled_channel<int16_t, 1, 1> dynoCarWheelTireWidthMm;
	/**
	 * @@DYNO_CAR_GEAR_PRIMARY_REDUCTION_TOOLTIP@@
	 * units: Units
	 * offset 17596
	 */
	scaled_channel<float, 1, 1> dynoCarGearPrimaryReduction;
	/**
	 * @@DYNO_CAR_GEAR_RATIO_TOOLTIP@@
	 * units: Units
	 * offset 17600
	 */
	scaled_channel<float, 1, 1> dynoCarGearRatio;
	/**
	 * @@DYNO_CAR_GEAR_FINAL_DRIVE_TOOLTIP@@
	 * units: Units
	 * offset 17604
	 */
	scaled_channel<float, 1, 1> dynoCarGearFinalDrive;
	/**
	 * @@DYNO_CAR_CAR_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 17608
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCarMassKg;
	/**
	 * @@DYNO_CAR_CARGO_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 17610
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCargoMassKg;
	/**
	 * @@DYNO_CAR_COEFF_OF_DRAG_TOOLTIP@@
	 * units: Coeff
	 * offset 17612
	 */
	scaled_channel<float, 1, 1> dynoCarCoeffOfDrag;
	/**
	 * @@DYNO_CAR_FRONTAL_AREA_TOOLTIP@@
	 * units: m2
	 * offset 17616
	 */
	scaled_channel<float, 1, 1> dynoCarFrontalAreaM2;
	/**
	 * units: deg
	 * offset 17620
	 */
	scaled_channel<int8_t, 10, 1> trailingSparkTable[TRAILING_SPARK_SIZE][TRAILING_SPARK_RPM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 17636
	 */
	scaled_channel<uint8_t, 1, 50> trailingSparkRpmBins[TRAILING_SPARK_RPM_SIZE] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 17640
	 */
	scaled_channel<uint8_t, 1, 5> trailingSparkLoadBins[TRAILING_SPARK_SIZE] = {};
	/**
	 * units: RPM
	 * offset 17644
	 */
	scaled_channel<uint8_t, 1, 100> maximumOilPressureBins[4] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 17648
	 */
	scaled_channel<uint8_t, 1, 10> maximumOilPressureValues[4] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 17652
	 */
	gppwm_channel_e torqueReductionCutXaxis;
	/**
	 * How many % of ignition events will be cut
	 * units: %
	 * offset 17653
	 */
	int8_t torqueReductionIgnitionCutTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 17665
	 */
	uint8_t alignmentFill_at_17665[1] = {};
	/**
	 * offset 17666
	 */
	int16_t torqueReductionCutXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 17678
	 */
	int8_t torqueReductionCutGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 17680
	 */
	gppwm_channel_e torqueReductionTimeXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 17681
	 */
	uint8_t alignmentFill_at_17681[3] = {};
	/**
	 * For how long after the pin has been triggered will the cut/reduction stay active. After that, even if the pin is still triggered, torque is re-introduced
	 * units: ms
	 * offset 17684
	 */
	float torqueReductionTimeTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 17732
	 */
	int16_t torqueReductionTimeXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 17744
	 */
	int8_t torqueReductionTimeGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 17746
	 */
	gppwm_channel_e torqueReductionIgnitionRetardXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 17747
	 */
	uint8_t alignmentFill_at_17747[1] = {};
	/**
	 * How many degrees of timing advance will be reduced during the Torque Reduction Time
	 * units: deg
	 * offset 17748
	 */
	float torqueReductionIgnitionRetardTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 17796
	 */
	int16_t torqueReductionIgnitionRetardXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 17808
	 */
	int8_t torqueReductionIgnitionRetardGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 17810
	 */
	uint8_t alignmentFill_at_17810[2] = {};
	/**
	offset 17812 bit 0 */
	bool wizardNumberOfCylinders : 1 {};
	/**
	offset 17812 bit 1 */
	bool wizardFiringOrder : 1 {};
	/**
	offset 17812 bit 2 */
	bool wizardMapSensorType : 1 {};
	/**
	offset 17812 bit 3 */
	bool wizardCrankTrigger : 1 {};
	/**
	offset 17812 bit 4 */
	bool wizardCamTrigger : 1 {};
	/**
	offset 17812 bit 5 */
	bool wizardInjectorFlow : 1 {};
	/**
	offset 17812 bit 6 */
	bool wizardDisplacement : 1 {};
	/**
	offset 17812 bit 7 */
	bool wizardCltSensor : 1 {};
	/**
	offset 17812 bit 8 */
	bool wizardTps : 1 {};
	/**
	offset 17812 bit 9 */
	bool wizardIgnitionOutputs : 1 {};
	/**
	offset 17812 bit 10 */
	bool wizardInjectorOutputs : 1 {};
	/**
	offset 17812 bit 11 */
	bool wizardVeTable : 1 {};
	/**
	offset 17812 bit 12 */
	bool boardUseTempPullUp : 1 {};
	/**
	offset 17812 bit 13 */
	bool boardUse2stepPullDown : 1 {};
	/**
	offset 17812 bit 14 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 17812 bit 15 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 17812 bit 16 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 17812 bit 17 */
	bool boardUseH1PullDown : 1 {};
	/**
	offset 17812 bit 18 */
	bool boardUseH2PullDown : 1 {};
	/**
	offset 17812 bit 19 */
	bool boardUseH3PullDown : 1 {};
	/**
	offset 17812 bit 20 */
	bool boardUseH4PullDown : 1 {};
	/**
	offset 17812 bit 21 */
	bool boardUseH5PullDown : 1 {};
	/**
	offset 17812 bit 22 */
	bool boardUseFlexPullDown : 1 {};
	/**
	offset 17812 bit 23 */
	bool unusedBit_308_23 : 1 {};
	/**
	offset 17812 bit 24 */
	bool unusedBit_308_24 : 1 {};
	/**
	offset 17812 bit 25 */
	bool unusedBit_308_25 : 1 {};
	/**
	offset 17812 bit 26 */
	bool unusedBit_308_26 : 1 {};
	/**
	offset 17812 bit 27 */
	bool unusedBit_308_27 : 1 {};
	/**
	offset 17812 bit 28 */
	bool unusedBit_308_28 : 1 {};
	/**
	offset 17812 bit 29 */
	bool unusedBit_308_29 : 1 {};
	/**
	offset 17812 bit 30 */
	bool unusedBit_308_30 : 1 {};
	/**
	offset 17812 bit 31 */
	bool unusedBit_308_31 : 1 {};
	/**
	 * offset 17816
	 */
	scaled_channel<uint8_t, 1, 10> knockGainLoadBins[6] = {};
	/**
	 * units: RPM
	 * offset 17822
	 */
	scaled_channel<uint8_t, 1, 100> knockGainRpmBins[6] = {};
	/**
	 * offset 17828
	 */
	KnockGain knockGains[MAX_CYLINDER_COUNT] = {};
	/**
	 * Low fuel pressure target using the same pressure reference as the low-pressure sensor.
	 * units: kPa
	 * offset 18260
	 */
	scaled_channel<uint16_t, 10, 1> fuelPressureTargetTable[FP_PRESSURE_TABLE_SIZE][FP_PRESSURE_RPM_SIZE] = {};
	/**
	 * Fueling-load axis. Speed Density uses MAP in kPa.
	 * units: load
	 * offset 18292
	 */
	uint16_t fuelPressureTargetLoadBins[FP_PRESSURE_TABLE_SIZE] = {};
	/**
	 * units: rpm
	 * offset 18300
	 */
	uint16_t fuelPressureTargetRpmBins[FP_PRESSURE_RPM_SIZE] = {};
	/**
	 * Base duty before PID correction. Also used without pressure feedback when the sensor is absent or invalid.
	 * units: %
	 * offset 18308
	 */
	scaled_channel<uint16_t, 10, 1> fuelPumpBaseDutyTable[FP_DUTY_TABLE_SIZE][FP_DUTY_RPM_SIZE] = {};
	/**
	 * Target-pressure axis for the base-duty map.
	 * units: kPa
	 * offset 18340
	 */
	uint16_t fuelPumpBaseDutyFpBins[FP_DUTY_TABLE_SIZE] = {};
	/**
	 * units: rpm
	 * offset 18348
	 */
	uint16_t fuelPumpBaseDutyRpmBins[FP_DUTY_RPM_SIZE] = {};
};
static_assert(sizeof(persistent_config_s) == 18356);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt
