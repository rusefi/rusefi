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
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 0
	 */
	float tempC_1;
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
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
	 * Turn on this fan when AC is on.
	offset 276 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 276 bit 5 */
	bool enableFan2WithAc : 1 {};
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
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
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
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 584
	 */
	int16_t fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
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
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 592
	 */
	int16_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 594
	 */
	int16_t fan2OffTemperature;
	/**
	 * offset 596
	 */
	int8_t disableFan1AtSpeed;
	/**
	 * offset 597
	 */
	int8_t disableFan2AtSpeed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 598
	 */
	uint8_t alignmentFill_at_598[2] = {};
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
	bool unusedBit_172_4 : 1 {};
	/**
	offset 600 bit 5 */
	bool unusedBit_172_5 : 1 {};
	/**
	offset 600 bit 6 */
	bool unusedBit_172_6 : 1 {};
	/**
	offset 600 bit 7 */
	bool unusedBit_172_7 : 1 {};
	/**
	offset 600 bit 8 */
	bool unusedBit_172_8 : 1 {};
	/**
	offset 600 bit 9 */
	bool unusedBit_172_9 : 1 {};
	/**
	offset 600 bit 10 */
	bool unusedBit_172_10 : 1 {};
	/**
	offset 600 bit 11 */
	bool unusedBit_172_11 : 1 {};
	/**
	offset 600 bit 12 */
	bool unusedBit_172_12 : 1 {};
	/**
	offset 600 bit 13 */
	bool unusedBit_172_13 : 1 {};
	/**
	offset 600 bit 14 */
	bool unusedBit_172_14 : 1 {};
	/**
	offset 600 bit 15 */
	bool unusedBit_172_15 : 1 {};
	/**
	offset 600 bit 16 */
	bool unusedBit_172_16 : 1 {};
	/**
	offset 600 bit 17 */
	bool unusedBit_172_17 : 1 {};
	/**
	offset 600 bit 18 */
	bool unusedBit_172_18 : 1 {};
	/**
	offset 600 bit 19 */
	bool unusedBit_172_19 : 1 {};
	/**
	offset 600 bit 20 */
	bool unusedBit_172_20 : 1 {};
	/**
	offset 600 bit 21 */
	bool unusedBit_172_21 : 1 {};
	/**
	offset 600 bit 22 */
	bool unusedBit_172_22 : 1 {};
	/**
	offset 600 bit 23 */
	bool unusedBit_172_23 : 1 {};
	/**
	offset 600 bit 24 */
	bool unusedBit_172_24 : 1 {};
	/**
	offset 600 bit 25 */
	bool unusedBit_172_25 : 1 {};
	/**
	offset 600 bit 26 */
	bool unusedBit_172_26 : 1 {};
	/**
	offset 600 bit 27 */
	bool unusedBit_172_27 : 1 {};
	/**
	offset 600 bit 28 */
	bool unusedBit_172_28 : 1 {};
	/**
	offset 600 bit 29 */
	bool unusedBit_172_29 : 1 {};
	/**
	offset 600 bit 30 */
	bool unusedBit_172_30 : 1 {};
	/**
	offset 600 bit 31 */
	bool unusedBit_172_31 : 1 {};
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
	 * Fan 1 PWM curve output values
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
	 * Fan 2 PWM curve output values
	 * units: %
	 * offset 648
	 */
	uint8_t fan2PwmValues[FAN_PWM_CURVE_SIZE] = {};
	/**
	 * Minimum PWM output clamp (Fan 1)
	 * units: %
	 * offset 656
	 */
	uint8_t fan1MinPwm;
	/**
	 * Maximum PWM output clamp (Fan 1)
	 * units: %
	 * offset 657
	 */
	uint8_t fan1MaxPwm;
	/**
	 * Minimum PWM output clamp (Fan 2)
	 * units: %
	 * offset 658
	 */
	uint8_t fan2MinPwm;
	/**
	 * Maximum PWM output clamp (Fan 2)
	 * units: %
	 * offset 659
	 */
	uint8_t fan2MaxPwm;
	/**
	 * PWM adder when AC compressor is active (Fan 1)
	 * units: %
	 * offset 660
	 */
	uint8_t fan1AcAdder;
	/**
	 * PWM adder when AC compressor is active (Fan 2)
	 * units: %
	 * offset 661
	 */
	uint8_t fan2AcAdder;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 662
	 */
	uint8_t alignmentFill_at_662[2] = {};
	/**
	 * Soft-start ramp time ? how long to ramp from 0 to target PWM (Fan 1)
	 * units: s
	 * offset 664
	 */
	float fan1SoftStartSec;
	/**
	 * Soft-start ramp time ? how long to ramp from 0 to target PWM (Fan 2)
	 * units: s
	 * offset 668
	 */
	float fan2SoftStartSec;
	/**
	 * offset 672
	 */
	int wizardPanelToShow;
	/**
	 * offset 676
	 */
	output_pin_e acrPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 678
	 */
	uint8_t alignmentFill_at_678[2] = {};
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	 * units: revs/km
	 * offset 680
	 */
	float driveWheelRevPerKm;
	/**
	 * CANbus thread period in ms
	 * units: ms
	 * offset 684
	 */
	int canSleepPeriodMs;
	/**
	 * units: index
	 * offset 688
	 */
	int byFirmwareVersion;
	/**
	 * First analog throttle body, first sensor. See also pedalPositionAdcChannel
	 * Analog TPS inputs have 200Hz low-pass cutoff.
	 * offset 692
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 693
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 694
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * offset 695
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 696
	 */
	float idle_derivativeFilterLoss;
	/**
	 * offset 700
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	 * units: %
	 * offset 712
	 */
	float airByRpmTaper;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	 * units: %
	 * offset 716
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 717
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 718
	 */
	uint8_t acrRevolutions;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 719
	 */
	uint8_t alignmentFill_at_719[1] = {};
	/**
	 * offset 720
	 */
	int calibrationBirthday;
	/**
	 * units: volts
	 * offset 724
	 */
	float adcVcc;
	/**
	 * Magic engine phase: we compare instant MAP at X to instant MAP at x+360 angle in one complete cycle
	 * units: Deg
	 * offset 728
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 732
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 740
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 760
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * TPS/PPS error threshold
	 * units: %
	 * offset 761
	 */
	scaled_channel<uint8_t, 10, 1> etbSplit;
	/**
	 * offset 762
	 */
	Gpio tle6240_cs;
	/**
	 * offset 764
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * offset 765
	 */
	pin_output_mode_e mc33810_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 766
	 */
	uint8_t alignmentFill_at_766[2] = {};
	/**
	 * @see hasBaroSensor
	 * offset 768
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 780
	 */
	idle_hardware_s idle;
	/**
	 * Ignition timing to remove when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 792
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	 * units: deg/s
	 * offset 793
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 794
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * offset 795
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	 * units: ratio
	 * offset 796
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 798
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	 * units: count
	 * offset 799
	 */
	uint8_t vssToothCount;
	/**
	 * Allows you to change the default load axis used for the VE table, which is typically MAP (manifold absolute pressure).
	 * offset 800
	 */
	ve_override_e idleVeOverrideMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 801
	 */
	uint8_t alignmentFill_at_801[1] = {};
	/**
	 * offset 802
	 */
	Gpio l9779_cs;
	/**
	 * offset 804
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 828
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 852
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 853
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 854
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 856
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * How many consecutive VVT gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 857
	 */
	int8_t gapVvtTrackingLengthOverride;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 858
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 860
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 861
	 */
	uint8_t alignmentFill_at_861[1] = {};
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 862
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 864
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 866
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 867
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 868
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT] = {};
	/**
	 * offset 876
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 877
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 878
	 */
	Gpio debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 880
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 881
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 882
	 */
	Gpio mc33972_cs;
	/**
	 * offset 884
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 885
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 886
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 887
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 888
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 889
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 890
	 */
	Gpio mc33810_cs[C_MC33810_COUNT] = {};
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 892
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 896
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT] = {};
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	 * units: us
	 * offset 900
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * Ratio between the wheels and your transmission output.
	 * units: ratio
	 * offset 902
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 904
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 906
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 907
	 */
	uint8_t alignmentFill_at_907[1] = {};
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 908
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * units: g/s
	 * offset 912
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 914
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * offset 916
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 917
	 */
	uint8_t alignmentFill_at_917[1] = {};
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 918
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 920
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * units: RPM
	 * offset 921
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionMinRpm;
	/**
	 * units: %
	 * offset 922
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionMinLoad;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 923
	 */
	uint8_t alignmentFill_at_923[1] = {};
	/**
	offset 924 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 924 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 924 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	offset 924 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 924 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 924 bit 5 */
	bool rethrowHardFault : 1 {};
	/**
	offset 924 bit 6 */
	bool verboseQuad : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 924 bit 7 */
	bool useStepperIdle : 1 {};
	/**
	offset 924 bit 8 */
	bool lambdaProtectionEnable : 1 {};
	/**
	offset 924 bit 9 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	offset 924 bit 10 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 924 bit 11 */
	bool externalRusEfiGdiModule : 1 {};
	/**
	 * Useful for individual intakes
	offset 924 bit 12 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 924 bit 13 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 924 bit 14 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 924 bit 15 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 924 bit 16 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 924 bit 17 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 924 bit 18 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 924 bit 19 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 924 bit 20 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 924 bit 21 */
	bool pauseEtbControl : 1 {};
	/**
	offset 924 bit 22 */
	bool verboseKLine : 1 {};
	/**
	offset 924 bit 23 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 924 bit 24 */
	bool enableAemXSeries : 1 {};
	/**
	offset 924 bit 25 */
	bool modeledFlowIdle : 1 {};
	/**
	offset 924 bit 26 */
	bool isTuningDetectorEnabled : 1 {};
	/**
	offset 924 bit 27 */
	bool useAbsolutePressureForLagTime : 1 {};
	/**
	offset 924 bit 28 */
	bool unusedBit_311_30 : 1 {};
	/**
	offset 924 bit 29 */
	bool unusedBit_311_31 : 1 {};
	/**
	offset 924 bit 30 */
	bool unusedBit_330_30 : 1 {};
	/**
	offset 924 bit 31 */
	bool unusedBit_330_31 : 1 {};
	/**
	 * offset 928
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT] = {};
	/**
	 * offset 936
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 937
	 */
	uint8_t alignmentFill_at_937[3] = {};
	/**
	 * offset 940
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	 * units: v
	 * offset 944
	 */
	uint8_t mc33_hvolt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 945
	 */
	uint8_t alignmentFill_at_945[1] = {};
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 946
	 */
	uint16_t minimumBoostClosedLoopMap;
	/**
	 * units: %
	 * offset 948
	 */
	int8_t initialIgnitionCutPercent;
	/**
	 * units: %
	 * offset 949
	 */
	int8_t finalIgnitionCutPercentBeforeLaunch;
	/**
	 * offset 950
	 */
	gppwm_channel_e boostOpenLoopYAxis;
	/**
	 * offset 951
	 */
	spi_device_e l9779spiDevice;
	/**
	 * offset 952
	 */
	imu_type_e imuType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 953
	 */
	uint8_t alignmentFill_at_953[1] = {};
	/**
	 * How far above idle speed do we consider idling, i.e. coasting detection threshold.
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	 * units: RPM
	 * offset 954
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	 * units: ms
	 * offset 956
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 958
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 960
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 962
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 964
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 966
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 967
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 968
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT] = {};
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 984
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * Since torque reduction pin is usually shared with launch control, most people have an RPM where behavior under that is Launch Control, over that is Flat Shift/Torque Reduction
	 * units: rpm
	 * offset 986
	 */
	uint16_t torqueReductionArmingRpm;
	/**
	 * offset 988
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 989
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	 * units: :1
	 * offset 990
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	 * units: %
	 * offset 991
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	 * units: hz
	 * offset 992
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 994
	 */
	adc_channel_e idlePositionChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 995
	 */
	uint8_t alignmentFill_at_995[1] = {};
	/**
	 * offset 996
	 */
	uint16_t launchCorrectionsEndRpm;
	/**
	 * offset 998
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 1000
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1001
	 */
	uint8_t alignmentFill_at_1001[1] = {};
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 1002
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 1004
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * units: RPM
	 * offset 1006
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionRestoreRpm;
	/**
	 * offset 1007
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	 * units: count
	 * offset 1008
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	 * units: %
	 * offset 1012
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * units: %
	 * offset 1014
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * Closed voltage for secondary throttle position sensor
	 * offset 1016
	 */
	tps_limit_t tps1SecondaryMin;
	/**
	 * Fully opened voltage for secondary throttle position sensor
	 * offset 1018
	 */
	tps_limit_t tps1SecondaryMax;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	 * units: Seconds
	 * offset 1020
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 1022
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 1024
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1025
	 */
	uint8_t alignmentFill_at_1025[1] = {};
	/**
	 * offset 1026
	 */
	output_pin_e acRelayPin;
	/**
	 * units: %
	 * offset 1028
	 */
	uint8_t lambdaProtectionMinTps;
	/**
	 * Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed
	 * units: s
	 * offset 1029
	 */
	scaled_channel<uint8_t, 10, 1> lambdaProtectionTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1030
	 */
	uint8_t alignmentFill_at_1030[2] = {};
	/**
	 * offset 1032
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT] = {};
	/**
	 * offset 1064
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 1066
	 */
	Gpio spi1misoPin;
	/**
	 * offset 1068
	 */
	Gpio spi1sckPin;
	/**
	 * offset 1070
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 1072
	 */
	Gpio spi2misoPin;
	/**
	 * offset 1074
	 */
	Gpio spi2sckPin;
	/**
	 * offset 1076
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 1078
	 */
	Gpio spi3misoPin;
	/**
	 * offset 1080
	 */
	Gpio spi3sckPin;
	/**
	 * UNUSED
	 * Will remove in 2026 for sure
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 1082
	 */
	Gpio cdmInputPin;
	/**
	 * offset 1084
	 */
	uart_device_e consoleUartDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1085
	 */
	uint8_t alignmentFill_at_1085[3] = {};
	/**
	 * Ramp the idle target down from the entry threshold over N seconds when returning to idle. Helps prevent overshooting (below) the idle target while returning to idle from coasting.
	offset 1088 bit 0 */
	bool idleReturnTargetRamp : 1 {};
	/**
	offset 1088 bit 1 */
	bool useInjectorFlowLinearizationTable : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 1088 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 1088 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 1088 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 1088 bit 5 */
	bool canInputBCM : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	offset 1088 bit 6 */
	bool consumeObdSensors : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 1088 bit 7 */
	bool enableCanVss : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 1088 bit 8 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 1088 bit 9 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 1088 bit 10 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 1088 bit 11 */
	bool verboseVVTDecoding : 1 {};
	/**
	offset 1088 bit 12 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * When set to yes, it enables intake air temperature-based corrections for Alpha-N tuning strategies.
	offset 1088 bit 13 */
	bool alphaNUseIat : 1 {};
	/**
	offset 1088 bit 14 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 1088 bit 15 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 1088 bit 16 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 1088 bit 17 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 1088 bit 18 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 1088 bit 19 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 1088 bit 20 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 1088 bit 21 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 1088 bit 22 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 1088 bit 23 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 1088 bit 24 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 1088 bit 25 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 1088 bit 26 */
	bool tcuEnabled : 1 {};
	/**
	offset 1088 bit 27 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coils
	offset 1088 bit 28 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 1088 bit 29 */
	bool verboseCan2 : 1 {};
	/**
	offset 1088 bit 30 */
	bool unusedBit_427_30 : 1 {};
	/**
	offset 1088 bit 31 */
	bool unusedBit_427_31 : 1 {};
	/**
	 * offset 1092
	 */
	dc_io etbIo[ETB_COUNT] = {};
	/**
	 * offset 1108
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 1110
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 1112
	 */
	pid_s boostPid;
	/**
	 * offset 1132
	 */
	boostType_e boostType;
	/**
	 * offset 1133
	 */
	pin_input_mode_e ignitionKeyDigitalPinMode;
	/**
	 * offset 1134
	 */
	Gpio ignitionKeyDigitalPin;
	/**
	 * units: Hz
	 * offset 1136
	 */
	int boostPwmFrequency;
	/**
	 * offset 1140
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1141
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * offset 1142
	 */
	cranking_condition_e crankingCondition;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	 * units: sec
	 * offset 1143
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	 * units: roc
	 * offset 1144
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	 * units: coeff
	 * offset 1148
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * Selects the acceleration enrichment strategy.
	 * offset 1152
	 */
	accel_enrichment_mode_e accelEnrichmentMode;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	 * units: sec
	 * offset 1153
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Pause closed loop fueling after acceleration fuel occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel accel.
	 * units: sec
	 * offset 1154
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterAccelTime;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1155
	 */
	uint8_t alignmentFill_at_1155[1] = {};
	/**
	 * Launch disabled above this speed if setting is above zero
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 1156
	 */
	int launchSpeedThreshold;
	/**
	 * Starting Launch RPM window to activate (subtracts from Launch RPM)
	 * units: RPM
	 * offset 1160
	 */
	int launchRpmWindow;
	/**
	 * units: ms
	 * offset 1164
	 */
	float triggerEventsTimeoutMs;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '100%' means no filtering, 98% would be some filtering.
	 * units: percent
	 * offset 1168
	 */
	float ppsExpAverageAlpha;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 1172
	 */
	float mapExpAverageAlpha;
	/**
	 * offset 1176
	 */
	float magicNumberAvailableForDevTricks;
	/**
	 * offset 1180
	 */
	float turbochargerFilter;
	/**
	 * offset 1184
	 */
	int launchTpsThreshold;
	/**
	 * offset 1188
	 */
	float launchActivateDelay;
	/**
	 * offset 1192
	 */
	stft_s stft;
	/**
	 * offset 1220
	 */
	ltft_s ltft;
	/**
	 * offset 1236
	 */
	dc_io stepperDcIo[DC_PER_STEPPER] = {};
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1252
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1284
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1316
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1348
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT] = {};
	/**
	 * offset 1360
	 */
	dc_function_e etbFunctions[ETB_COUNT] = {};
	/**
	 * offset 1362
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1363
	 */
	uint8_t alignmentFill_at_1363[1] = {};
	/**
	 * offset 1364
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1366
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1367
	 */
	uint8_t alignmentFill_at_1367[1] = {};
	/**
	 * offset 1368
	 */
	Gpio drv8860_miso;
	/**
	 * offset 1370
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1386
	 */
	uint8_t alignmentFill_at_1386[2] = {};
	/**
	 * Angle between cam sensor and VVT zero position
	 * units: value
	 * offset 1388
	 */
	float vvtOffsets[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1404
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT] = {};
	/**
	 * offset 1452
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS] = {};
	/**
	 * Closed voltage for secondary throttle position sensor
	 * offset 1516
	 */
	tps_limit_t tps2SecondaryMin;
	/**
	 * Fully opened voltage for secondary throttle position sensor
	 * offset 1518
	 */
	tps_limit_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1520 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1520 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Write SD card log even when powered by USB
	offset 1520 bit 2 */
	bool alwaysWriteSdCard : 1 {};
	/**
	 * Second harmonic (aka double) is usually quieter background noise
	offset 1520 bit 3 */
	bool knockDetectionUseDoubleFrequency : 1 {};
	/**
	 * Unlocking only via rusEFI console using 'unlock PICODEBUG' command. Use 'reset to default firmware' if pincode is lost.
	offset 1520 bit 4 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1520 bit 5 */
	bool silentTriggerError : 1 {};
	/**
	offset 1520 bit 6 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1520 bit 7 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write. See also can1ListenMode
	offset 1520 bit 8 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1520 bit 9 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1520 bit 10 */
	bool enableOilPressureProtect : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1520 bit 11 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1520 bit 12 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1520 bit 13 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1520 bit 14 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	 * When enabled, this option cuts the fuel supply when the RPM limit is reached. Cutting fuel provides a smoother limiting action; however, it may lead to slightly higher combustion chamber temperatures since unburned fuel is not present to cool the combustion process.
	offset 1520 bit 15 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * When selected, this option cuts the spark to limit RPM. Cutting spark can produce flames from the exhaust due to unburned fuel igniting in the exhaust system. Additionally, this unburned fuel can help cool the combustion chamber, which may be beneficial in high-performance applications.
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1520 bit 16 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1520 bit 17 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1520 bit 18 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1520 bit 19 */
	bool torqueReductionEnabled : 1 {};
	/**
	 * When we sync cam sensor is that first or second full engine revolution of the four stroke cycle?
	offset 1520 bit 20 */
	bool camSyncOnSecondCrankRevolution : 1 {};
	/**
	offset 1520 bit 21 */
	bool limitTorqueReductionTime : 1 {};
	/**
	 * Are you a developer troubleshooting TS over CAN ISO/TP?
	offset 1520 bit 22 */
	bool verboseIsoTp : 1 {};
	/**
	 * In this mode only trigger events go into engine sniffer and not coils/injectors etc
	offset 1520 bit 23 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1520 bit 24 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1520 bit 25 */
	bool skippedWheelOnCam : 1 {};
	/**
	offset 1520 bit 26 */
	bool unusedBit_503_26 : 1 {};
	/**
	offset 1520 bit 27 */
	bool unusedBit_503_27 : 1 {};
	/**
	offset 1520 bit 28 */
	bool unusedBit_503_28 : 1 {};
	/**
	offset 1520 bit 29 */
	bool unusedBit_503_29 : 1 {};
	/**
	offset 1520 bit 30 */
	bool unusedBit_503_30 : 1 {};
	/**
	offset 1520 bit 31 */
	bool unusedBit_503_31 : 1 {};
	/**
	 * A/C button input
	 * offset 1524
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1526
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	 * units: %
	 * offset 1527
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1528
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1529
	 */
	uint8_t alignmentFill_at_1529[3] = {};
	/**
	offset 1532 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1532 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1532 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1532 bit 3 */
	bool complexWallModel : 1 {};
	/**
	 * RPM is measured based on last 720 degrees while instant RPM is measured based on the last 90 degrees of crank revolution
	offset 1532 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1532 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra advance at low idle speeds will prevent stalling and extra retard at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1532 bit 6 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1532 bit 7 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1532 bit 8 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 1532 bit 9 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	offset 1532 bit 10 */
	bool hondaK : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1532 bit 11 */
	bool twoWireBatchIgnition : 1 {};
	/**
	 * Read MAP sensor on ECU start-up to use as baro value.
	offset 1532 bit 12 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1532 bit 13 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT and PID idle).
	 * You probably don't need this.
	offset 1532 bit 14 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1532 bit 15 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1532 bit 16 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1532 bit 17 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1532 bit 18 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1532 bit 19 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1532 bit 20 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors.
	 * WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1532 bit 21 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1532 bit 22 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1532 bit 23 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1532 bit 24 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	offset 1532 bit 25 */
	bool sdTriggerLog : 1 {};
	/**
	offset 1532 bit 26 */
	bool stepper_dc_use_two_wires : 1 {};
	/**
	offset 1532 bit 27 */
	bool watchOutForLinearTime : 1 {};
	/**
	offset 1532 bit 28 */
	bool unusedBit_542_28 : 1 {};
	/**
	offset 1532 bit 29 */
	bool unusedBit_542_29 : 1 {};
	/**
	offset 1532 bit 30 */
	bool unusedBit_542_30 : 1 {};
	/**
	offset 1532 bit 31 */
	bool unusedBit_542_31 : 1 {};
	/**
	 * units: count
	 * offset 1536
	 */
	uint32_t engineChartSize;
	/**
	 * units: mult
	 * offset 1540
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * Idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	 * units: RPM
	 * offset 1544
	 */
	int16_t acIdleRpmTarget;
	/**
	 * set warningPeriod X
	 * units: seconds
	 * offset 1546
	 */
	int16_t warningPeriod;
	/**
	 * units: angle
	 * offset 1548
	 */
	float knockDetectionWindowStart;
	/**
	 * units: ms
	 * offset 1552
	 */
	float idleStepperReactionTime;
	/**
	 * units: count
	 * offset 1556
	 */
	int idleStepperTotalSteps;
	/**
	 * Pedal position to realize that we need to reduce torque when the trigger pin is triggered
	 * offset 1560
	 */
	int torqueReductionArmingApp;
	/**
	 * Reference Torque value
	 * units: Nm
	 * offset 1564
	 */
	float referenceTorqueForGenerator;
	/**
	 * kPa/psi value at which Reference Torque is archived
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1568
	 */
	float referenceMapForGenerator;
	/**
	 * offset 1572
	 */
	float referenceVeForGenerator;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1576
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	 * units: Seconds
	 * offset 1580
	 */
	float wwaeTau;
	/**
	 * offset 1584
	 */
	pid_s alternatorControl;
	/**
	 * offset 1604
	 */
	pid_s etb;
	/**
	 * RPM range above upper limit for extra air taper
	 * units: RPM
	 * offset 1624
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1626
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed voltage for primary throttle position sensor
	 * offset 1628
	 */
	tps_limit_t tps2Min;
	/**
	 * Fully opened voltage for primary throttle position sensor
	 * offset 1630
	 */
	tps_limit_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1632
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1634
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1635
	 */
	uint8_t alignmentFill_at_1635[1] = {};
	/**
	 * offset 1636
	 */
	Gpio mc33816_flag0;
	/**
	 * offset 1638
	 */
	scaled_channel<uint16_t, 1000, 1> tachPulsePerRev;
	/**
	 * kPa/psi value which is too low to be true
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1640
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa/psi value which is too high to be true
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1644
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	 * units: ms
	 * offset 1648
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	 * units: ms
	 * offset 1650
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1652
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	 * units: Fraction
	 * offset 1672
	 */
	float wwaeBeta;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1676
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1678
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1680
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT] = {};
	/**
	 * offset 1684
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1686
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * units: volts
	 * offset 1688
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * units: volts
	 * offset 1692
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	 * units: seconds
	 * offset 1696
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * larger value = larger intake manifold volume
	 * offset 1698
	 */
	uint16_t mafFilterParameter;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	 * units: RPM
	 * offset 1700
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1702
	 */
	uint8_t alignmentFill_at_1702[2] = {};
	/**
	 * See Over/Undervoltage Shutdown/Retry bit in documentation
	offset 1704 bit 0 */
	bool mc33810DisableRecoveryMode : 1 {};
	/**
	offset 1704 bit 1 */
	bool mc33810Gpgd0Mode : 1 {};
	/**
	offset 1704 bit 2 */
	bool mc33810Gpgd1Mode : 1 {};
	/**
	offset 1704 bit 3 */
	bool mc33810Gpgd2Mode : 1 {};
	/**
	offset 1704 bit 4 */
	bool mc33810Gpgd3Mode : 1 {};
	/**
	 * Send out board statistics
	offset 1704 bit 5 */
	bool enableExtendedCanBroadcast : 1 {};
	/**
	 * global_can_data performance hack
	offset 1704 bit 6 */
	bool luaCanRxWorkaround : 1 {};
	/**
	offset 1704 bit 7 */
	bool flexSensorInverted : 1 {};
	/**
	offset 1704 bit 8 */
	bool useHardSkipInTraction : 1 {};
	/**
	 * Use Aux Speed 1 as one of speeds for wheel slip ratio?
	offset 1704 bit 9 */
	bool useAuxSpeedForSlipRatio : 1 {};
	/**
	 * VSS and Aux Speed 1 or Aux Speed 1 with Aux Speed 2?
	offset 1704 bit 10 */
	bool useVssAsSecondWheelSpeed : 1 {};
	/**
	offset 1704 bit 11 */
	bool is_enabled_spi_5 : 1 {};
	/**
	offset 1704 bit 12 */
	bool is_enabled_spi_6 : 1 {};
	/**
	 * AEM X-Series EGT gauge kit or rusEFI EGT sensor from Wideband controller
	offset 1704 bit 13 */
	bool enableAemXSeriesEgt : 1 {};
	/**
	offset 1704 bit 14 */
	bool startRequestPinInverted : 1 {};
	/**
	offset 1704 bit 15 */
	bool tcu_rangeSensorPulldown : 1 {};
	/**
	offset 1704 bit 16 */
	bool devBit01 : 1 {};
	/**
	offset 1704 bit 17 */
	bool devBit0 : 1 {};
	/**
	offset 1704 bit 18 */
	bool devBit1 : 1 {};
	/**
	offset 1704 bit 19 */
	bool devBit2 : 1 {};
	/**
	offset 1704 bit 20 */
	bool devBit3 : 1 {};
	/**
	offset 1704 bit 21 */
	bool devBit4 : 1 {};
	/**
	offset 1704 bit 22 */
	bool devBit5 : 1 {};
	/**
	offset 1704 bit 23 */
	bool devBit6 : 1 {};
	/**
	offset 1704 bit 24 */
	bool devBit7 : 1 {};
	/**
	offset 1704 bit 25 */
	bool invertExhaustCamVVTSignal : 1 {};
	/**
	 * "Available via TS Plugin see https://rusefi.com/s/knock"
	offset 1704 bit 26 */
	bool enableKnockSpectrogram : 1 {};
	/**
	offset 1704 bit 27 */
	bool enableKnockSpectrogramFilter : 1 {};
	/**
	offset 1704 bit 28 */
	bool unusedBit_615_28 : 1 {};
	/**
	offset 1704 bit 29 */
	bool unusedBit_615_29 : 1 {};
	/**
	offset 1704 bit 30 */
	bool unusedBit_615_30 : 1 {};
	/**
	offset 1704 bit 31 */
	bool unusedBit_615_31 : 1 {};
	/**
	 * This value is an added for base idle value. Idle Value added when coasting and transitioning into idle.
	 * units: percent
	 * offset 1708
	 */
	int16_t iacByTpsTaper;
	/**
	 * offset 1710
	 */
	Gpio accelerometerCsPin;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 1712
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 1713
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1714
	 */
	uint8_t alignmentFill_at_1714[2] = {};
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	 * units: roc
	 * offset 1716
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1720
	 */
	brain_input_pin_e auxSpeedSensorInputPin[AUX_SPEED_SENSOR_COUNT] = {};
	/**
	 * offset 1724
	 */
	uint8_t totalGearsCount;
	/**
	 * Defines when fuel is injected relative to the intake valve opening. Options include End of Injection or other timing references.
	 * offset 1725
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1726
	 */
	debug_mode_e debugMode;
	/**
	 * Additional idle % when fan #1 is active
	 * units: %
	 * offset 1727
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * Band rate for primary TTL
	 * units: BPs
	 * offset 1728
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * units: volts
	 * offset 1732
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	 * units: volts
	 * offset 1736
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * offset 1740
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1741
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1742
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1743
	 */
	load_override_e afrOverrideMode;
	/**
	 * units: A
	 * offset 1744
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	 * units: A
	 * offset 1745
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	 * units: us
	 * offset 1746
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	 * units: ms
	 * offset 1747
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1748 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Allow OpenBLT on Primary CAN
	offset 1748 bit 1 */
	bool canOpenBLT : 1 {};
	/**
	 * Allow OpenBLT on Secondary CAN
	offset 1748 bit 2 */
	bool can2OpenBLT : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (default, cc/min) or mass flow (g/s).
	offset 1748 bit 3 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1748 bit 4 */
	bool boardUseCanTerminator : 1 {};
	/**
	offset 1748 bit 5 */
	bool kLineDoHondaSend : 1 {};
	/**
	 * ListenMode is about acknowledging CAN traffic on the protocol level. Different from canWriteEnabled
	offset 1748 bit 6 */
	bool can1ListenMode : 1 {};
	/**
	offset 1748 bit 7 */
	bool can2ListenMode : 1 {};
	/**
	offset 1748 bit 8 */
	bool unusedBit_649_8 : 1 {};
	/**
	offset 1748 bit 9 */
	bool unusedBit_649_9 : 1 {};
	/**
	offset 1748 bit 10 */
	bool unusedBit_649_10 : 1 {};
	/**
	offset 1748 bit 11 */
	bool unusedBit_649_11 : 1 {};
	/**
	offset 1748 bit 12 */
	bool unusedBit_649_12 : 1 {};
	/**
	offset 1748 bit 13 */
	bool unusedBit_649_13 : 1 {};
	/**
	offset 1748 bit 14 */
	bool unusedBit_649_14 : 1 {};
	/**
	offset 1748 bit 15 */
	bool unusedBit_649_15 : 1 {};
	/**
	offset 1748 bit 16 */
	bool unusedBit_649_16 : 1 {};
	/**
	offset 1748 bit 17 */
	bool unusedBit_649_17 : 1 {};
	/**
	offset 1748 bit 18 */
	bool unusedBit_649_18 : 1 {};
	/**
	offset 1748 bit 19 */
	bool unusedBit_649_19 : 1 {};
	/**
	offset 1748 bit 20 */
	bool unusedBit_649_20 : 1 {};
	/**
	offset 1748 bit 21 */
	bool unusedBit_649_21 : 1 {};
	/**
	offset 1748 bit 22 */
	bool unusedBit_649_22 : 1 {};
	/**
	offset 1748 bit 23 */
	bool unusedBit_649_23 : 1 {};
	/**
	offset 1748 bit 24 */
	bool unusedBit_649_24 : 1 {};
	/**
	offset 1748 bit 25 */
	bool unusedBit_649_25 : 1 {};
	/**
	offset 1748 bit 26 */
	bool unusedBit_649_26 : 1 {};
	/**
	offset 1748 bit 27 */
	bool unusedBit_649_27 : 1 {};
	/**
	offset 1748 bit 28 */
	bool unusedBit_649_28 : 1 {};
	/**
	offset 1748 bit 29 */
	bool unusedBit_649_29 : 1 {};
	/**
	offset 1748 bit 30 */
	bool unusedBit_649_30 : 1 {};
	/**
	offset 1748 bit 31 */
	bool unusedBit_649_31 : 1 {};
	/**
	 * Angle of tooth detection within engine phase cycle
	 * units: angle
	 * offset 1752
	 */
	uint16_t camDecoder2jzPosition;
	/**
	 * offset 1754
	 */
	mc33810maxDwellTimer_e mc33810maxDwellTimer;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1755
	 */
	uint8_t alignmentFill_at_1755[1] = {};
	/**
	 * Duration of each test pulse
	 * units: ms
	 * offset 1756
	 */
	scaled_channel<uint16_t, 100, 1> benchTestOnTime;
	/**
	 * units: %
	 * offset 1758
	 */
	uint8_t lambdaProtectionRestoreTps;
	/**
	 * units: %
	 * offset 1759
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionRestoreLoad;
	/**
	 * offset 1760
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1761
	 */
	uint8_t alignmentFill_at_1761[1] = {};
	/**
	 * offset 1762
	 */
	Gpio can2TxPin;
	/**
	 * offset 1764
	 */
	Gpio can2RxPin;
	/**
	 * offset 1766
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1767
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1768
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1769
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1770
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1772
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1773
	 */
	uint8_t alignmentFill_at_1773[3] = {};
	/**
	 * Specifies the boost pressure allowed before triggering a cut. Setting this to 0 will DISABLE overboost cut.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1776
	 */
	float boostCutPressure;
	/**
	 * units: kg/h
	 * offset 1780
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16] = {};
	/**
	 * units: ratio
	 * offset 1796
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16] = {};
	/**
	 * Fixed timing, useful for TDC testing
	 * units: deg
	 * offset 1812
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1816
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1820
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * units: value
	 * offset 1824
	 */
	float egoValueShift;
	/**
	 * VVT output solenoid pin for this cam
	 * offset 1828
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1836
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMinTps;
	/**
	 * offset 1837
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMaxTps;
	/**
	 * offset 1838
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMinTps;
	/**
	 * offset 1839
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMaxTps;
	/**
	 * offset 1840
	 */
	pwm_freq_t vvtOutputFrequency;
	/**
	 * Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.
	 * units: deg BTDC
	 * offset 1842
	 */
	int8_t minimumIgnitionTiming;
	/**
	 * Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC
	 * units: deg BTDC
	 * offset 1843
	 */
	int8_t maximumIgnitionTiming;
	/**
	 * units: Hz
	 * offset 1844
	 */
	int alternatorPwmFrequency;
	/**
	 * offset 1848
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK] = {};
	/**
	 * Additional idle % when fan #2 is active
	 * units: %
	 * offset 1850
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	 * units: sec
	 * offset 1851
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1852
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT] = {};
	/**
	 * offset 1860
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 1884
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1885
	 */
	pin_output_mode_e accelerometerCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 *  HPFP fuel mass compensation = manual mode for GDI engines
	 * offset 1886
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1887
	 */
	uint8_t alignmentFill_at_1887[1] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa/50.7psi here.
	 * This is gauge pressure/in reference to atmospheric.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 1888
	 */
	float fuelReferencePressure;
	/**
	 * offset 1892
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1924
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * units: Deg
	 * offset 1956
	 */
	int16_t knockSamplingDuration;
	/**
	 * units: Hz
	 * offset 1958
	 */
	int16_t etbFreq;
	/**
	 * offset 1960
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 1980
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	 * units: %
	 * offset 1981
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	 * units: %
	 * offset 1982
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 1983
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder ignition and fueling timing correction for uneven engines
	 * units: deg
	 * offset 1984
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: seconds
	 * offset 2032
	 */
	float idlePidActivationTime;
	/**
	 * Minimum coolant temperature to activate VVT
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 2036
	 */
	int16_t vvtControlMinClt;
	/**
	 * offset 2038
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 2039
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 2040
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 2041
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 2042
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 2043
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 2044
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 2045
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 2046
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 2047
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 2048
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 2050
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 2052
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2054
	 */
	uint8_t alignmentFill_at_2054[2] = {};
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 2056
	 */
	pid_s auxPid[CAMS_PER_BANK] = {};
	/**
	 * offset 2096
	 */
	float injectorCorrectionPolynomial[8] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 2128
	 */
	scaled_channel<int16_t, 1, 1> primeBins[PRIME_CURVE_COUNT] = {};
	/**
	 * offset 2144
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2164
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2165
	 */
	uint8_t alignmentFill_at_2165[1] = {};
	/**
	 * offset 2166
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2168
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2170
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2171
	 */
	uint8_t alignmentFill_at_2171[1] = {};
	/**
	 * offset 2172
	 */
	Gpio mc33816_cs;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2174
	 */
	uint8_t alignmentFill_at_2174[2] = {};
	/**
	 * units: hz
	 * offset 2176
	 */
	float auxFrequencyFilter;
	/**
	 * offset 2180
	 */
	sent_input_pin_e sentInputPins[SENT_INPUT_COUNT] = {};
	/**
	 * This sets the RPM above which fuel cut is active.
	 * units: rpm
	 * offset 2182
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	 * units: rpm
	 * offset 2184
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	 * units: %
	 * offset 2186
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 2188
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * units: %
	 * offset 2190
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 2192
	 */
	int16_t coastingFuelCutMap;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2194
	 */
	uint8_t alignmentFill_at_2194[2] = {};
	/**
	 * offset 2196
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2216
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * offset 2236
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT] = {};
	/**
	 * offset 2332
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT] = {};
	/**
	 * offset 2396
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT] = {};
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2524
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2528
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	 * units: kg/h
	 * offset 2532
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2536
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2540
	 */
	float tChargeAirDecrLimit;
	/**
	 * iTerm min value
	 * offset 2544
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2546
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2548
	 */
	pid_s idleTimingPid;
	/**
	 * When entering idle, and the PID settings are aggressive, it's good to make a soft entry upon entering closed loop
	 * offset 2568
	 */
	float idleTimingSoftEntryTime;
	/**
	 * offset 2572
	 */
	pin_input_mode_e torqueReductionTriggerPinMode;
	/**
	 * offset 2573
	 */
	torqueReductionActivationMode_e torqueReductionActivationMode;
	/**
	 * A delay in cycles between fuel-enrich. portions
	 * units: cycles
	 * offset 2574
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	 * units: coef
	 * offset 2576
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2580
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2581
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2582
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2584
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	 * units: :1
	 * offset 2585
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2586
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position. Can also be interpreted as the maximum allowed TPS% Opening for Idle Control.
	 * units: %
	 * offset 2588
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2592
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: mg
	 * offset 2604
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[PRIME_CURVE_COUNT] = {};
	/**
	 * Trigger comparator center point voltage
	 * units: V
	 * offset 2612
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	 * units: V
	 * offset 2613
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	 * units: V
	 * offset 2614
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	 * units: RPM
	 * offset 2615
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * units: ratio
	 * offset 2616
	 */
	scaled_channel<uint16_t, 100, 1> tractionControlSlipBins[TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * units: RPM
	 * offset 2628
	 */
	uint8_t tractionControlSpeedBins[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE] = {};
	/**
	 * offset 2634
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2635
	 */
	uint8_t alignmentFill_at_2635[1] = {};
	/**
	 * offset 2636
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS] = {};
	/**
	 * Boost Current
	 * units: mA
	 * offset 3068
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	 * units: mA
	 * offset 3070
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	 * units: mA
	 * offset 3072
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	 * units: us
	 * offset 3074
	 */
	uint16_t mc33_t_max_boost;
	/**
	 * units: us
	 * offset 3076
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	 * units: us
	 * offset 3078
	 */
	uint16_t mc33_t_peak_tot;
	/**
	 * units: us
	 * offset 3080
	 */
	uint16_t mc33_t_bypass;
	/**
	 * units: us
	 * offset 3082
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	 * units: us
	 * offset 3084
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 3086
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 3087
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 3088
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 3089
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3095
	 */
	uint8_t alignmentFill_at_3095[1] = {};
	/**
	 * units: ratio
	 * offset 3096
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3168
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH] = {};
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	 * units: rpm
	 * offset 3240
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	 * units: sec
	 * offset 3241
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	 * units: sec
	 * offset 3242
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * offset 3243
	 */
	tChargeMode_e tChargeMode;
	/**
	 * units: mg
	 * offset 3244
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	 * Threshold in ETB error (target vs. actual) above which the jam timer is started. If the timer reaches the time specified in the jam detection timeout period, the throttle is considered jammed, and engine operation limited.
	 * units: %
	 * offset 3246
	 */
	uint8_t etbJamDetectThreshold;
	/**
	 * units: lobes/cam
	 * offset 3247
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 3248
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Low engine speed for A/C. Larger engines can survive lower values
	 * units: RPM
	 * offset 3249
	 */
	scaled_channel<int8_t, 1, 10> acLowRpmLimit;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	 * units: deg
	 * offset 3250
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3251
	 */
	uint8_t alignmentFill_at_3251[1] = {};
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	 * units: cc
	 * offset 3252
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	 * https://rusefi.com/forum/viewtopic.php?t=2192
	 * units: deg
	 * offset 3254
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 3255
	 */
	uint8_t issFilterReciprocal;
	/**
	 * units: %/kPa
	 * offset 3256
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	 * units: %/kPa/lobe
	 * offset 3258
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * iTerm min value
	 * offset 3260
	 */
	int16_t hpfpPid_iTermMin;
	/**
	 * iTerm max value
	 * offset 3262
	 */
	int16_t hpfpPid_iTermMax;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	 * units: kPa/s
	 * offset 3264
	 */
	uint16_t hpfpTargetDecay;
	/**
	 * offset 3266
	 */
	output_pin_e stepper_raw_output[4] = {};
	/**
	 * units: ratio
	 * offset 3274
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[TCU_GEAR_COUNT] = {};
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	 * units: ms
	 * offset 3294
	 */
	uint16_t vvtActivationDelayMs;
	/**
	 * offset 3296
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3297
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * During revolution where ACR should be disabled at what specific angle to disengage
	 * units: deg
	 * offset 3298
	 */
	uint16_t acrDisablePhase;
	/**
	 * offset 3300
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3320
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 3340
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 3342
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3343
	 */
	uint8_t alignmentFill_at_3343[1] = {};
	/**
	 * offset 3344
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 3346
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3347
	 */
	uint8_t alignmentFill_at_3347[1] = {};
	/**
	 * offset 3348
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 3350
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 3352
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3353
	 */
	uint8_t alignmentFill_at_3353[1] = {};
	/**
	 * offset 3354
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 3356
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 3358
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3359
	 */
	uint8_t alignmentFill_at_3359[1] = {};
	/**
	 * offset 3360
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * offset 3362
	 */
	output_pin_e acrPin2;
	/**
	 * Set a minimum allowed target position to avoid slamming/driving against the hard mechanical stop in the throttle.
	 * units: %
	 * offset 3364
	 */
	scaled_channel<uint8_t, 10, 1> etbMinimumPosition;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3365
	 */
	uint8_t alignmentFill_at_3365[1] = {};
	/**
	 * offset 3366
	 */
	uint16_t tuneHidingKey;
	/**
	 * Individual characters are accessible using vin(index) Lua function
	 * offset 3368
	 */
	vin_number_t vinNumber;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 3385
	 */
	int8_t torqueReductionActivationTemperature;
	/**
	 * offset 3386
	 */
	fuel_pressure_sensor_mode_e fuelPressureSensorMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3387
	 */
	uint8_t alignmentFill_at_3387[1] = {};
	/**
	 * offset 3388
	 */
	switch_input_pin_e luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * units: rpm
	 * offset 3404
	 */
	int16_t ALSMinRPM;
	/**
	 * units: rpm
	 * offset 3406
	 */
	int16_t ALSMaxRPM;
	/**
	 * units: sec
	 * offset 3408
	 */
	int16_t ALSMaxDuration;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 3410
	 */
	int8_t ALSMinCLT;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 3411
	 */
	int8_t ALSMaxCLT;
	/**
	 * offset 3412
	 */
	uint8_t alsMinTimeBetween;
	/**
	 * offset 3413
	 */
	uint8_t alsEtbPosition;
	/**
	 * units: %
	 * offset 3414
	 */
	uint8_t acRelayAlternatorDutyAdder;
	/**
	 * If you have digital SENT TPS sensor please select type. For analog TPS leave None
	 * offset 3415
	 */
	SentEtbType sentEtbType;
	/**
	 * offset 3416
	 */
	uint16_t customSentTpsMin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3418
	 */
	uint8_t alignmentFill_at_3418[2] = {};
	/**
	 * units: %
	 * offset 3420
	 */
	int ALSIdleAdd;
	/**
	 * units: %
	 * offset 3424
	 */
	int ALSEtbAdd;
	/**
	 * offset 3428
	 */
	float ALSSkipRatio;
	/**
	 * Hysterisis: if Pressure High Disable is 240kpa, and acPressureEnableHyst is 20, when the ECU sees 240kpa, A/C will be disabled, and stay disabled until 240-20=220kpa is reached
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3432
	 */
	scaled_channel<uint8_t, 2, 1> acPressureEnableHyst;
	/**
	 * offset 3433
	 */
	pin_input_mode_e ALSActivatePinMode;
	/**
	 * For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	 * units: %
	 * offset 3434
	 */
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	/**
	 * For Toyota ETCS-i, use ~69%
	 * units: %
	 * offset 3435
	 */
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	/**
	 * offset 3436
	 */
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * offset 3444
	 */
	uint16_t customSentTpsMax;
	/**
	 * offset 3446
	 */
	uint16_t kLineBaudRate;
	/**
	 * offset 3448
	 */
	CanGpioType canGpioType;
	/**
	 * offset 3449
	 */
	UiMode uiMode;
	/**
	 * Crank angle ATDC of first lobe peak
	 * units: deg
	 * offset 3450
	 */
	int16_t hpfpPeakPos;
	/**
	 * units: us
	 * offset 3452
	 */
	int16_t kLinePeriodUs;
	/**
	 * Degrees of timing REMOVED from actual timing during soft RPM limit window
	 * units: deg
	 * offset 3454
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitTimingRetard;
	/**
	 * % of fuel ADDED during window
	 * units: %
	 * offset 3455
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitFuelAdded;
	/**
	 * Sets a buffer below the RPM hard limit, helping avoid rapid cycling of cut actions by defining a range within which RPM must drop before cut actions are re-enabled.
	 * Hysterisis: if the hard limit is 7200rpm and rpmHardLimitHyst is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached
	 * units: RPM
	 * offset 3456
	 */
	scaled_channel<uint8_t, 1, 10> rpmHardLimitHyst;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3457
	 */
	uint8_t alignmentFill_at_3457[1] = {};
	/**
	 * Time between bench test pulses
	 * units: ms
	 * offset 3458
	 */
	scaled_channel<uint16_t, 10, 1> benchTestOffTime;
	/**
	 * Defines a pressure range below the cut limit at which boost can resume, providing smoother control over boost cut actions.
	 * For example: if hard cut is 240kpa, and boost cut hysteresis is 20, when the ECU sees 240kpa, fuel/ign will cut, and stay cut until 240-20=220kpa is reached
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3460
	 */
	scaled_channel<uint8_t, 2, 1> boostCutPressureHyst;
	/**
	 * Boost duty cycle modified by gear
	 * units: %
	 * offset 3461
	 */
	scaled_channel<int8_t, 2, 1> gearBasedOpenLoopBoostAdder[TCU_GEAR_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3471
	 */
	uint8_t alignmentFill_at_3471[1] = {};
	/**
	 * How many test bench pulses do you want
	 * offset 3472
	 */
	uint32_t benchTestCount;
	/**
	 * How long initial idle adder is held before starting to decay.
	 * units: seconds
	 * offset 3476
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsHoldTime;
	/**
	 * How long it takes to remove initial IAC adder to return to normal idle.
	 * units: seconds
	 * offset 3477
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsDecayTime;
	/**
	 * offset 3478
	 */
	switch_input_pin_e tcu_rangeInput[RANGE_INPUT_COUNT] = {};
	/**
	 * offset 3490
	 */
	pin_input_mode_e tcu_rangeInputMode[RANGE_INPUT_COUNT] = {};
	/**
	 * Scale the reported vehicle speed value from CAN. Example: Parameter set to 1.1, CAN VSS reports 50kph, ECU will report 55kph instead.
	 * units: ratio
	 * offset 3496
	 */
	scaled_channel<uint16_t, 10000, 1> canVssScaling;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3498
	 */
	uint8_t alignmentFill_at_3498[2] = {};
	/**
	 * offset 3500
	 */
	ThermistorConf oilTempSensor;
	/**
	 * offset 3532
	 */
	ThermistorConf fuelTempSensor;
	/**
	 * offset 3564
	 */
	ThermistorConf ambientTempSensor;
	/**
	 * offset 3596
	 */
	ThermistorConf compressorDischargeTemperature;
	/**
	 * Place the sensor before the throttle, but after any turbocharger/supercharger and intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3628
	 */
	adc_channel_e throttleInletPressureChannel;
	/**
	 * Place the sensor after the turbocharger/supercharger, but before any intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3629
	 */
	adc_channel_e compressorDischargePressureChannel;
	/**
	 * offset 3630
	 */
	Gpio dacOutputPins[DAC_OUTPUT_COUNT] = {};
	/**
	 * offset 3634
	 */
	output_pin_e speedometerOutputPin;
	/**
	 * Number of speedometer pulses per kilometer travelled.
	 * offset 3636
	 */
	uint16_t speedometerPulsePerKm;
	/**
	 * offset 3638
	 */
	uint8_t simulatorCamPosition[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 3642
	 */
	adc_channel_e ignKeyAdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3643
	 */
	uint8_t alignmentFill_at_3643[1] = {};
	/**
	 * offset 3644
	 */
	float ignKeyAdcDivider;
	/**
	 * offset 3648
	 */
	pin_mode_e spi6MisoMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3649
	 */
	uint8_t alignmentFill_at_3649[3] = {};
	/**
	 * units: ratio
	 * offset 3652
	 */
	float triggerVVTGapOverrideFrom[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3668
	 */
	float triggerVVTGapOverrideTo[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: %
	 * offset 3684
	 */
	int8_t tractionControlEtbDrop[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * This sets an immediate limit on injector duty cycle. If this threshold is reached, the system will immediately cut the injectors.
	 * units: %
	 * offset 3720
	 */
	uint8_t maxInjectorDutyInstant;
	/**
	 * This limit allows injectors to operate up to the specified duty cycle percentage for a short period (as defined by the delay). After this delay, if the duty cycle remains above the limit, it will trigger a cut.
	 * units: %
	 * offset 3721
	 */
	uint8_t maxInjectorDutySustained;
	/**
	 * Timeout period for duty cycle over the sustained limit to trigger duty cycle protection.
	 * units: sec
	 * offset 3722
	 */
	scaled_channel<uint8_t, 10, 1> maxInjectorDutySustainedTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3723
	 */
	uint8_t alignmentFill_at_3723[1] = {};
	/**
	 * offset 3724
	 */
	output_pin_e injectionPinsStage2[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: Deg
	 * offset 3748
	 */
	int8_t tractionControlTimingDrop[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * units: %
	 * offset 3784
	 */
	int8_t tractionControlIgnitionSkip[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE][TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * offset 3820
	 */
	float auxSpeed1Multiplier;
	/**
	 * offset 3824
	 */
	float brakeMeanEffectivePressureDifferential;
	/**
	 * offset 3828
	 */
	Gpio spi4mosiPin;
	/**
	 * offset 3830
	 */
	Gpio spi4misoPin;
	/**
	 * offset 3832
	 */
	Gpio spi4sckPin;
	/**
	 * offset 3834
	 */
	Gpio spi5mosiPin;
	/**
	 * offset 3836
	 */
	Gpio spi5misoPin;
	/**
	 * offset 3838
	 */
	Gpio spi5sckPin;
	/**
	 * offset 3840
	 */
	Gpio spi6mosiPin;
	/**
	 * offset 3842
	 */
	Gpio spi6misoPin;
	/**
	 * offset 3844
	 */
	Gpio spi6sckPin;
	/**
	 * offset 3846
	 */
	pin_mode_e spi4SckMode;
	/**
	 * offset 3847
	 */
	pin_mode_e spi4MosiMode;
	/**
	 * offset 3848
	 */
	pin_mode_e spi4MisoMode;
	/**
	 * offset 3849
	 */
	pin_mode_e spi5SckMode;
	/**
	 * offset 3850
	 */
	pin_mode_e spi5MosiMode;
	/**
	 * offset 3851
	 */
	pin_mode_e spi5MisoMode;
	/**
	 * offset 3852
	 */
	pin_mode_e spi6SckMode;
	/**
	 * offset 3853
	 */
	pin_mode_e spi6MosiMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3854
	 */
	uint8_t alignmentFill_at_3854[2] = {};
	/**
	 * Secondary TTL channel baud rate
	 * units: BPs
	 * offset 3856
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 3860
	 */
	Gpio camSimulatorPin;
	/**
	 * offset 3862
	 */
	pin_output_mode_e camSimulatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3863
	 */
	uint8_t alignmentFill_at_3863[1] = {};
	/**
	 * offset 3864
	 */
	int anotherCiTest;
	/**
	 * offset 3868
	 */
	uint32_t device_uid[3] = {};
	/**
	 * offset 3880
	 */
	adc_channel_e tcu_rangeAnalogInput[RANGE_INPUT_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3886
	 */
	uint8_t alignmentFill_at_3886[2] = {};
	/**
	 * units: Ohm
	 * offset 3888
	 */
	float tcu_rangeSensorBiasResistor;
	/**
	 * offset 3892
	 */
	MsIoBox_config_s msIoBox0;
	/**
	 * Nominal coil charge current, 0.25A step
	 * units: A
	 * offset 3896
	 */
	scaled_channel<uint8_t, 4, 1> mc33810Nomi;
	/**
	 * Maximum coil charge current, 1A step
	 * units: A
	 * offset 3897
	 */
	uint8_t mc33810Maxi;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3898
	 */
	uint8_t alignmentFill_at_3898[2] = {};
	/**
	 * offset 3900
	 */
	linear_sensor_s acPressure;
	/**
	 * value of A/C pressure in kPa/psi before that compressor is disengaged
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3920
	 */
	uint16_t minAcPressure;
	/**
	 * value of A/C pressure in kPa/psi after that compressor is disengaged
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3922
	 */
	uint16_t maxAcPressure;
	/**
	 * Delay before cutting fuel due to low oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 3924
	 */
	scaled_channel<uint8_t, 10, 1> minimumOilPressureTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3925
	 */
	uint8_t alignmentFill_at_3925[3] = {};
	/**
	 * offset 3928
	 */
	linear_sensor_s auxLinear3;
	/**
	 * offset 3948
	 */
	linear_sensor_s auxLinear4;
	/**
	 * Below TPS value all knock suppression will be disabled.
	 * units: %
	 * offset 3968
	 */
	scaled_channel<uint8_t, 1, 1> knockSuppressMinTps;
	/**
	 * Fuel to odd when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 3969
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimAggression;
	/**
	 * After a knock event, reapply fuel at this rate.
	 * units: 1%/s
	 * offset 3970
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimReapplyRate;
	/**
	 * Fuel trim when knock, max 30%
	 * units: %
	 * offset 3971
	 */
	scaled_channel<uint8_t, 1, 1> knockFuelTrim;
	/**
	 * units: sense
	 * offset 3972
	 */
	float knockSpectrumSensitivity;
	/**
	 * "Estimated knock frequency, ignore cylinderBore if this one > 0"
	 * units: Hz
	 * offset 3976
	 */
	float knockFrequency;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 3980
	 */
	injector_compensation_mode_e secondaryInjectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3981
	 */
	uint8_t alignmentFill_at_3981[3] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 3984
	 */
	float secondaryInjectorFuelReferencePressure;
	/**
	 * SENT input connected to ETB
	 * offset 3988
	 */
	SentInput EtbSentInput;
	/**
	 * SENT input used for high pressure fuel sensor
	 * offset 3989
	 */
	SentInput FuelHighPressureSentInput;
	/**
	 * If you have SENT High Pressure Fuel Sensor please select type. For analog TPS leave None
	 * offset 3990
	 */
	SentFuelHighPressureType FuelHighPressureSentType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3991
	 */
	uint8_t alignmentFill_at_3991[1] = {};
	/**
	offset 3992 bit 0 */
	bool nitrousControlEnabled : 1 {};
	/**
	offset 3992 bit 1 */
	bool vvlControlEnabled : 1 {};
	/**
	offset 3992 bit 2 */
	bool unusedBit_Fancy3 : 1 {};
	/**
	offset 3992 bit 3 */
	bool unusedBit_Fancy4 : 1 {};
	/**
	offset 3992 bit 4 */
	bool unusedBit_Fancy5 : 1 {};
	/**
	offset 3992 bit 5 */
	bool unusedBit_Fancy6 : 1 {};
	/**
	offset 3992 bit 6 */
	bool unusedBit_Fancy7 : 1 {};
	/**
	offset 3992 bit 7 */
	bool unusedBit_Fancy8 : 1 {};
	/**
	offset 3992 bit 8 */
	bool unusedBit_Fancy9 : 1 {};
	/**
	offset 3992 bit 9 */
	bool unusedBit_Fancy10 : 1 {};
	/**
	offset 3992 bit 10 */
	bool unusedBit_Fancy11 : 1 {};
	/**
	offset 3992 bit 11 */
	bool unusedBit_Fancy12 : 1 {};
	/**
	offset 3992 bit 12 */
	bool unusedBit_Fancy13 : 1 {};
	/**
	offset 3992 bit 13 */
	bool unusedBit_Fancy14 : 1 {};
	/**
	offset 3992 bit 14 */
	bool unusedBit_Fancy15 : 1 {};
	/**
	offset 3992 bit 15 */
	bool unusedBit_Fancy16 : 1 {};
	/**
	offset 3992 bit 16 */
	bool unusedBit_Fancy17 : 1 {};
	/**
	offset 3992 bit 17 */
	bool unusedBit_Fancy18 : 1 {};
	/**
	offset 3992 bit 18 */
	bool unusedBit_Fancy19 : 1 {};
	/**
	offset 3992 bit 19 */
	bool unusedBit_Fancy20 : 1 {};
	/**
	offset 3992 bit 20 */
	bool unusedBit_Fancy21 : 1 {};
	/**
	offset 3992 bit 21 */
	bool unusedBit_Fancy22 : 1 {};
	/**
	offset 3992 bit 22 */
	bool unusedBit_Fancy23 : 1 {};
	/**
	offset 3992 bit 23 */
	bool unusedBit_Fancy24 : 1 {};
	/**
	offset 3992 bit 24 */
	bool unusedBit_Fancy25 : 1 {};
	/**
	offset 3992 bit 25 */
	bool unusedBit_Fancy26 : 1 {};
	/**
	offset 3992 bit 26 */
	bool unusedBit_Fancy27 : 1 {};
	/**
	offset 3992 bit 27 */
	bool unusedBit_Fancy28 : 1 {};
	/**
	offset 3992 bit 28 */
	bool unusedBit_Fancy29 : 1 {};
	/**
	offset 3992 bit 29 */
	bool unusedBit_Fancy30 : 1 {};
	/**
	offset 3992 bit 30 */
	bool unusedBit_Fancy31 : 1 {};
	/**
	offset 3992 bit 31 */
	bool unusedBit_Fancy32 : 1 {};
	/**
	 * offset 3996
	 */
	nitrous_arming_method_e nitrousControlArmingMethod;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3997
	 */
	uint8_t alignmentFill_at_3997[1] = {};
	/**
	 * Pin that activates nitrous control
	 * offset 3998
	 */
	switch_input_pin_e nitrousControlTriggerPin;
	/**
	 * offset 4000
	 */
	pin_input_mode_e nitrousControlTriggerPinMode;
	/**
	 * offset 4001
	 */
	lua_gauge_e nitrousLuaGauge;
	/**
	 * offset 4002
	 */
	lua_gauge_meaning_e nitrousLuaGaugeMeaning;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4003
	 */
	uint8_t alignmentFill_at_4003[1] = {};
	/**
	 * offset 4004
	 */
	float nitrousLuaGaugeArmingValue;
	/**
	 * offset 4008
	 */
	int nitrousMinimumTps;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 4012
	 */
	int16_t nitrousMinimumClt;
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 4014
	 */
	int16_t nitrousMaximumMap;
	/**
	 * units: afr
	 * offset 4016
	 */
	scaled_channel<uint8_t, 10, 1> nitrousMaximumAfr;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4017
	 */
	uint8_t alignmentFill_at_4017[1] = {};
	/**
	 * units: rpm
	 * offset 4018
	 */
	uint16_t nitrousActivationRpm;
	/**
	 * units: rpm
	 * offset 4020
	 */
	uint16_t nitrousDeactivationRpm;
	/**
	 * units: rpm
	 * offset 4022
	 */
	uint16_t nitrousDeactivationRpmWindow;
	/**
	 * Retard timing by this amount during DFCO. Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: deg
	 * offset 4024
	 */
	uint8_t dfcoRetardDeg;
	/**
	 * Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: s
	 * offset 4025
	 */
	scaled_channel<uint8_t, 10, 1> dfcoRetardRampInTime;
	/**
	 * offset 4026
	 */
	output_pin_e nitrousRelayPin;
	/**
	 * offset 4028
	 */
	pin_output_mode_e nitrousRelayPinMode;
	/**
	 * units: %
	 * offset 4029
	 */
	int8_t nitrousFuelAdderPercent;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4030
	 */
	uint8_t alignmentFill_at_4030[2] = {};
	/**
	 * Retard timing to remove from actual final timing (after all corrections) due to additional air.
	 * units: deg
	 * offset 4032
	 */
	float nitrousIgnitionRetard;
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 4036
	 */
	uint16_t nitrousMinimumVehicleSpeed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4038
	 */
	uint8_t alignmentFill_at_4038[2] = {};
	/**
	 * Exponential Average Alpha filtering parameter
	 * offset 4040
	 */
	float fuelLevelAveragingAlpha;
	/**
	 * How often do we update fuel level gauge
	 * units: seconds
	 * offset 4044
	 */
	float fuelLevelUpdatePeriodSec;
	/**
	 * Error below specified value
	 * units: v
	 * offset 4048
	 */
	float fuelLevelLowThresholdVoltage;
	/**
	 * Error above specified value
	 * units: v
	 * offset 4052
	 */
	float fuelLevelHighThresholdVoltage;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 4056
	 */
	float afrExpAverageAlpha;
	/**
	 * Compensates for trigger delay due to belt stretch, or other electromechanical issues. beware that raising this value advances ignition timing!
	 * units: uS
	 * offset 4060
	 */
	scaled_channel<uint8_t, 1, 1> sparkHardwareLatencyCorrection;
	/**
	 * Delay before cutting fuel due to extra high oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 4061
	 */
	scaled_channel<uint8_t, 10, 1> maxOilPressureTimeout;
	/**
	 * units: kg/h
	 * offset 4062
	 */
	scaled_channel<uint16_t, 100, 1> idleFlowEstimateFlow[8] = {};
	/**
	 * units: %
	 * offset 4078
	 */
	scaled_channel<uint8_t, 2, 1> idleFlowEstimatePosition[8] = {};
	/**
	 * units: mg
	 * offset 4086
	 */
	int8_t airmassToTimingBins[8] = {};
	/**
	 * units: deg
	 * offset 4094
	 */
	int8_t airmassToTimingValues[8] = {};
	/**
	 * idle return target ramp duration
	 * units: seconds
	 * offset 4102
	 */
	scaled_channel<uint8_t, 10, 1> idleReturnTargetRampDuration;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4103
	 */
	uint8_t alignmentFill_at_4103[1] = {};
	/**
	 * Voltage when the wastegate is fully open
	 * units: v
	 * offset 4104
	 */
	float wastegatePositionOpenedVoltage;
	/**
	 * Voltage when the wastegate is closed
	 * units: v
	 * offset 4108
	 */
	float wastegatePositionClosedVoltage;
	/**
	 * offset 4112
	 */
	wbo_s canWbo[CAN_WBO_COUNT] = {};
	/**
	 * offset 4128
	 */
	output_pin_e vvlRelayPin;
	/**
	 * offset 4130
	 */
	pin_output_mode_e vvlRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4131
	 */
	uint8_t alignmentFill_at_4131[1] = {};
	/**
	 * offset 4132
	 */
	vvl_s vvlController;
	/**
	 * offset 4156
	 */
	rotational_idle_s rotationalIdleController;
};
static_assert(sizeof(engine_configuration_s) == 4192);

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
	 * offset 4192
	 */
	float tmfTable[TMF_RATIO_SIZE][TMF_SIZE] = {};
	/**
	 * offset 4208
	 */
	float tmfRatioBins[TMF_RATIO_SIZE] = {};
	/**
	 * offset 4216
	 */
	float tmfOpeningBins[TMF_SIZE] = {};
	/**
	 * units: Nm
	 * offset 4224
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_RPM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 4260
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 4272
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE] = {};
	/**
	 * units: mult
	 * offset 4284
	 */
	float postCrankingFactor[CRANKING_ENRICH_CLT_COUNT][CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: count
	 * offset 4428
	 */
	uint16_t postCrankingDurationBins[CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 4440
	 */
	int16_t postCrankingCLTBins[CRANKING_ENRICH_CLT_COUNT] = {};
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	 * units: target TPS position
	 * offset 4452
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: ETB duty cycle bias
	 * offset 4484
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * target Wastegate value, 0 to 100%
	 * units: target DC position
	 * offset 4516
	 */
	int8_t dcWastegateBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: DC wastegate duty cycle bias
	 * offset 4524
	 */
	scaled_channel<int16_t, 100, 1> dcWastegateBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * units: %
	 * offset 4540
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE] = {};
	/**
	 * units: Load
	 * offset 4604
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE] = {};
	/**
	 * units: RPM
	 * offset 4612
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_RPM_SIZE] = {};
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	 * units: RPM
	 * offset 4620
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: ms
	 * offset 4636
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE] = {};
	/**
	 * CLT-based target RPM for automatic idle controller
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 4652
	 */
	scaled_channel<int16_t, 1, 1> cltIdleRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * See idleRpmPid
	 * units: RPM
	 * offset 4684
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE] = {};
	/**
	 * units: deg
	 * offset 4700
	 */
	scaled_channel<int16_t, 10, 1> ignitionCltCorrTable[CLT_TIMING_LOAD_AXIS_SIZE][CLT_TIMING_TEMP_AXIS_SIZE] = {};
	/**
	 * CLT-based timing correction
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 4750
	 */
	scaled_channel<int16_t, 1, 1> ignitionCltCorrTempBins[CLT_TIMING_TEMP_AXIS_SIZE] = {};
	/**
	 * units: Load
	 * offset 4760
	 */
	scaled_channel<uint8_t, 1, 5> ignitionCltCorrLoadBins[CLT_TIMING_LOAD_AXIS_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4765
	 */
	uint8_t alignmentFill_at_4765[3] = {};
	/**
	 * units: x
	 * offset 4768
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 4832
	 */
	float scriptCurve1[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 4896
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 4960
	 */
	float scriptCurve2[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 5024
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5056
	 */
	float scriptCurve3[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 5088
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5120
	 */
	float scriptCurve4[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 5152
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5184
	 */
	float scriptCurve5[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 5216
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5248
	 */
	float scriptCurve6[SCRIPT_CURVE_8] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 5280
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5296
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE] = {};
	/**
	 * units: ratio
	 * offset 5312
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE] = {};
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * units: Ratio
	 * offset 5376
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 5408
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: RPM
	 * offset 5440
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: deg
	 * offset 5448
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: RPM
	 * offset 5456
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: %
	 * offset 5472
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE] = {};
	/**
	 * offset 5488
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * units: RPM
	 * offset 5552
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT] = {};
	/**
	 * offset 5560
	 */
	uint16_t boostOpenLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * offset 5576
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * offset 5640
	 */
	uint16_t boostClosedLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * units: %
	 * offset 5656
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 5720
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5728
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_RPM_SIZE] = {};
	/**
	 * CLT-based cranking position %. The values in this curve represent a percentage of the ETB Maximum angle. e.g. If "ETB Idle Maximum Angle" is 10, a value of 70 means 7% ETB Position.
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 5736
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based cranking position %. The values in this curve represent a percentage of the ETB Maximum angle. e.g. If "ETB Idle Maximum Angle" is 10, a value of 70 means 7% ETB Position.
	 * units: percent
	 * offset 5768
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 5800
	 */
	float afterCrankingIACtaperDurationBins[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	 * Should be 100 once tune is better
	 * units: cycles
	 * offset 5824
	 */
	uint16_t afterCrankingIACtaperDuration[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: RPM
	 * offset 5836
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: deg
	 * offset 5844
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5876
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE_RPM] = {};
	/**
	 * units: load
	 * offset 5880
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE] = {};
	/**
	 * units: %
	 * offset 5884
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE_RPM] = {};
	/**
	 * offset 5916
	 */
	lua_script_t luaScript;
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 45916
	 */
	float cltFuelCorrBins[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 45980
	 */
	float cltFuelCorr[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 46044
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 46108
	 */
	float iatFuelCorr[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 46172
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 46204
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: counter
	 * offset 46236
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 46268
	 */
	int16_t crankingCycleFuelCltBins[CRANKING_CYCLE_CLT_SIZE] = {};
	/**
	 * Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for IAT, TPS ect, to give the final cranking pulse width.
	 * A reasonable starting point is 60mg per liter per cylinder.
	 * ex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.
	 * units: mg
	 * offset 46276
	 */
	float crankingCycleBaseFuel[CRANKING_CYCLE_CLT_SIZE][CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 46404
	 */
	float cltIdleCorrBins[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: %
	 * offset 46428
	 */
	float cltIdleCorrTable[CLT_IDLE_TABLE_RPM_SIZE][CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * units: Target RPM
	 * offset 46524
	 */
	scaled_channel<uint8_t, 1, 100> rpmIdleCorrBins[CLT_IDLE_TABLE_RPM_SIZE] = {};
	/**
	 * Long Term Idle Trim (LTIT) multiplicativo para idle open loop
	 * units: %
	 * offset 46528
	 */
	scaled_channel<uint16_t, 10, 1> ltitTable[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	 * units: kg/hour
	 * offset 46540
	 */
	float mafDecoding[MAF_DECODING_COUNT] = {};
	/**
	 * units: V
	 * offset 46668
	 */
	float mafDecodingBins[MAF_DECODING_COUNT] = {};
	/**
	 * units: deg
	 * offset 46796
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IAT_IGN_CORR_LOAD_COUNT][IAT_IGN_CORR_TEMP_COUNT] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 46924
	 */
	int8_t ignitionIatCorrTempBins[IAT_IGN_CORR_TEMP_COUNT] = {};
	/**
	 * units: Load
	 * offset 46932
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[IAT_IGN_CORR_LOAD_COUNT] = {};
	/**
	 * units: deg
	 * offset 46940
	 */
	int16_t injectionPhase[INJ_PHASE_LOAD_COUNT][INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: Load
	 * offset 47452
	 */
	uint16_t injPhaseLoadBins[INJ_PHASE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 47484
	 */
	uint16_t injPhaseRpmBins[INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: onoff
	 * offset 47516
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT] = {};
	/**
	 * This table represents MAP at a given TPS vs RPM, which we use if our MAP sensor has failed, or if we are using MAP Prediciton. 
	 *  This table should be a direct representation of MAP, you can tune it manually by disconnecting MAP sensor, and filling out the table with values that match an external gauge that shows MAP.
	 * Additionally, you can also use MLV to get the map values and/or generate the table for you
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 47576
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[MAP_EST_LOAD_COUNT][MAP_EST_RPM_COUNT] = {};
	/**
	 * units: % TPS
	 * offset 48088
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[MAP_EST_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 48120
	 */
	uint16_t mapEstimateRpmBins[MAP_EST_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 48152
	 */
	int8_t vvtTable1[VVT_TABLE_SIZE][VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 48216
	 */
	uint16_t vvtTable1LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 48232
	 */
	uint16_t vvtTable1RpmBins[VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 48248
	 */
	int8_t vvtTable2[VVT_TABLE_SIZE][VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 48312
	 */
	uint16_t vvtTable2LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 48328
	 */
	uint16_t vvtTable2RpmBins[VVT_TABLE_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 48344
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(ignLoadUnitLabels, ignLoadUnitIdxPcv)}
	 * offset 48856
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 48888
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT] = {};
	/**
	 * units: %
	 * offset 48920
	 */
	scaled_channel<uint16_t, 10, 1> veTable[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(veLoadUnitLabels, veLoadUnitIdxPcv)}
	 * offset 49432
	 */
	uint16_t veLoadBins[VE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 49464
	 */
	uint16_t veRpmBins[VE_RPM_COUNT] = {};
	/**
	 * units: {useLambdaOnInterface ? "lambda" : "afr"}
	 * offset 49496
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT] = {};
	/**
	 * offset 49752
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 49784
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 49816
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: %
	 * offset 50072
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: %
	 * offset 50104
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: value
	 * offset 50136
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8] = {};
	/**
	 * units: L
	 * offset 50392
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 50408
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: value
	 * offset 50424
	 */
	float scriptTable2[TABLE_2_LOAD_SIZE][TABLE_2_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 50680
	 */
	int16_t scriptTable2LoadBins[TABLE_2_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 50696
	 */
	int16_t scriptTable2RpmBins[TABLE_2_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 50712
	 */
	uint8_t scriptTable3[TABLE_3_LOAD_SIZE][TABLE_3_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 50776
	 */
	int16_t scriptTable3LoadBins[TABLE_3_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 50792
	 */
	int16_t scriptTable3RpmBins[TABLE_3_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 50808
	 */
	uint8_t scriptTable4[TABLE_4_LOAD_SIZE][TABLE_4_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 50888
	 */
	int16_t scriptTable4LoadBins[TABLE_4_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 50904
	 */
	int16_t scriptTable4RpmBins[TABLE_4_RPM_SIZE] = {};
	/**
	 * offset 50924
	 */
	uint16_t ignTrimLoadBins[IGN_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 50932
	 */
	uint16_t ignTrimRpmBins[IGN_TRIM_SIZE] = {};
	/**
	 * offset 50940
	 */
	ign_cyl_trim_s ignTrims[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 51132
	 */
	uint16_t fuelTrimLoadBins[FUEL_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 51140
	 */
	uint16_t fuelTrimRpmBins[FUEL_TRIM_SIZE] = {};
	/**
	 * offset 51148
	 */
	fuel_cyl_trim_s fuelTrims[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: ratio
	 * offset 51340
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: Airmass
	 * offset 51356
	 */
	scaled_channel<uint8_t, 1, 5> tcu_pcAirmassBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51364
	 */
	uint8_t tcu_pcValsR[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51372
	 */
	uint8_t tcu_pcValsN[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51380
	 */
	uint8_t tcu_pcVals1[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51388
	 */
	uint8_t tcu_pcVals2[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51396
	 */
	uint8_t tcu_pcVals3[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51404
	 */
	uint8_t tcu_pcVals4[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51412
	 */
	uint8_t tcu_pcVals12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51420
	 */
	uint8_t tcu_pcVals23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51428
	 */
	uint8_t tcu_pcVals34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51436
	 */
	uint8_t tcu_pcVals21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51444
	 */
	uint8_t tcu_pcVals32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51452
	 */
	uint8_t tcu_pcVals43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: TPS
	 * offset 51460
	 */
	uint8_t tcu_tccTpsBins[8] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 51468
	 */
	uint8_t tcu_tccLockSpeed[8] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 51476
	 */
	uint8_t tcu_tccUnlockSpeed[8] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 51484
	 */
	uint8_t tcu_32SpeedBins[8] = {};
	/**
	 * units: %
	 * offset 51492
	 */
	uint8_t tcu_32Vals[8] = {};
	/**
	 * units: %
	 * offset 51500
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[ETB2_TRIM_SIZE][ETB2_TRIM_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 51536
	 */
	uint8_t throttle2TrimTpsBins[ETB2_TRIM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51542
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[ETB2_TRIM_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 51548
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[KNOCK_TABLE_SIZE][KNOCK_TABLE_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 51584
	 */
	uint8_t maxKnockRetardLoadBins[KNOCK_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51590
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[KNOCK_TABLE_RPM_SIZE] = {};
	/**
	 * units: deg
	 * offset 51596
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51628
	 */
	uint16_t alsIgnRetardLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51636
	 */
	uint16_t alsIgnRetardrpmBins[ALS_RPM_SIZE] = {};
	/**
	 * units: percent
	 * offset 51644
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51676
	 */
	uint16_t alsFuelAdjustmentLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51684
	 */
	uint16_t alsFuelAdjustmentrpmBins[ALS_RPM_SIZE] = {};
	/**
	 * units: ratio
	 * offset 51692
	 */
	scaled_channel<int16_t, 1, 10> ALSIgnSkipTable[ALS_SIZE][ALS_RPM_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51724
	 */
	uint16_t alsIgnSkipLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51732
	 */
	uint16_t alsIgnSkiprpmBins[ALS_RPM_SIZE] = {};
	/**
	 * offset 51740
	 */
	blend_table_s ignBlends[IGN_BLEND_COUNT] = {};
	/**
	 * offset 52492
	 */
	blend_table_s veBlends[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 53244
	 */
	scaled_channel<uint16_t, 10, 1> secondVeTable[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(veLoadUnitLabels, veLoadUnitIdxPcv)}
	 * offset 53756
	 */
	uint16_t secondVeLoadBins[VE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 53788
	 */
	uint16_t secondVeRpmBins[VE_RPM_COUNT] = {};
	/**
	 * Pin that activates the second VE table (hard switch, overrides blend)
	 * offset 53820
	 */
	switch_input_pin_e secondVeTableInput;
	/**
	 * offset 53822
	 */
	pin_input_mode_e secondVeTableInputMode;
	/**
	 * Input that controls the blend ratio between primary and second VE table
	 * offset 53823
	 */
	gppwm_channel_e secondVeBlendParameter;
	/**
	 * offset 53824
	 */
	scaled_channel<int16_t, 10, 1> secondVeBlendBins[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: %
	 * offset 53840
	 */
	scaled_channel<uint8_t, 2, 1> secondVeBlendValues[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: deg
	 * offset 53848
	 */
	scaled_channel<int16_t, 10, 1> secondIgnitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(ignLoadUnitLabels, ignLoadUnitIdxPcv)}
	 * offset 54360
	 */
	uint16_t secondIgnitionLoadBins[IGN_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 54392
	 */
	uint16_t secondIgnitionRpmBins[IGN_RPM_COUNT] = {};
	/**
	 * Pin that activates the second ignition table (hard switch, overrides blend)
	 * offset 54424
	 */
	switch_input_pin_e secondIgnitionTableInput;
	/**
	 * offset 54426
	 */
	pin_input_mode_e secondIgnitionTableInputMode;
	/**
	 * Input that controls the blend ratio between primary and second ignition table
	 * offset 54427
	 */
	gppwm_channel_e secondIgnitionBlendParameter;
	/**
	 * offset 54428
	 */
	scaled_channel<int16_t, 10, 1> secondIgnitionBlendBins[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: %
	 * offset 54444
	 */
	scaled_channel<uint8_t, 2, 1> secondIgnitionBlendValues[BLEND_FACTOR_SIZE] = {};
	/**
	 * units: %
	 * offset 54452
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaBins[THR_EST_SIZE] = {};
	/**
	 * In units of g/s normalized to choked flow conditions
	 * units: g/s
	 * offset 54476
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaValues[THR_EST_SIZE] = {};
	/**
	 * offset 54500
	 */
	blend_table_s boostOpenLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 54876
	 */
	blend_table_s boostClosedLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * units: level
	 * offset 55252
	 */
	float tcu_rangeP[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55276
	 */
	float tcu_rangeR[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55300
	 */
	float tcu_rangeN[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55324
	 */
	float tcu_rangeD[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55348
	 */
	float tcu_rangeM[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55372
	 */
	float tcu_rangeM3[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55396
	 */
	float tcu_rangeM2[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55420
	 */
	float tcu_rangeM1[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55444
	 */
	float tcu_rangePlus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55468
	 */
	float tcu_rangeMinus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 55492
	 */
	float tcu_rangeLow[RANGE_INPUT_COUNT] = {};
	/**
	 * units: lambda
	 * offset 55516
	 */
	scaled_channel<uint8_t, 100, 1> lambdaMaxDeviationTable[LAM_SIZE][LAM_RPM_SIZE] = {};
	/**
	 * offset 55532
	 */
	uint16_t lambdaMaxDeviationLoadBins[LAM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55540
	 */
	uint16_t lambdaMaxDeviationRpmBins[LAM_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 55548
	 */
	uint8_t injectorStagingTable[INJ_STAGING_COUNT][INJ_STAGING_RPM_SIZE] = {};
	/**
	 * offset 55584
	 */
	uint16_t injectorStagingLoadBins[INJ_STAGING_COUNT] = {};
	/**
	 * units: RPM
	 * offset 55596
	 */
	uint16_t injectorStagingRpmBins[INJ_STAGING_RPM_SIZE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 55608
	 */
	int16_t wwCltBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 55624
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 55632
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 55640
	 */
	uint8_t wwMapBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 55648
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 55656
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: %
	 * offset 55664
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: deg
	 * offset 55680
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: volts
	 * offset 55696
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: ms
	 * offset 55704
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: kPa
	 * offset 55720
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE] = {};
	/**
	 * units: load
	 * offset 55920
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55940
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: %
	 * offset 55950
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_RPM_SIZE] = {};
	/**
	 * units: cc/lobe
	 * offset 56050
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: RPM
	 * offset 56070
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 56080
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensation[HPFP_FUEL_MASS_COMPENSATION_SIZE][HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 56208
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensationFuelMass[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: bar
	 * offset 56224
	 */
	scaled_channel<uint16_t, 10, 1> hpfpFuelMassCompensationFuelPressure[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: ms
	 * offset 56240
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearization[FLOW_LINEARIZATION_PRESSURE_SIZE][FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 56752
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearizationFuelMassBins[FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: bar
	 * offset 56784
	 */
	scaled_channel<uint16_t, 10, 1> injectorFlowLinearizationPressureBins[FLOW_LINEARIZATION_PRESSURE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 56816
	 */
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	 * units: dB
	 * offset 56848
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 56864
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: multiplier
	 * offset 56868
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: RPM
	 * offset 56872
	 */
	scaled_channel<uint8_t, 1, 50> predictiveMapBlendDurationBins[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: second
	 * offset 56876
	 */
	scaled_channel<uint8_t, 50, 1> predictiveMapBlendDurationValues[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: {bitStringValue(unitsLabels, useMetricOnInterface)}
	 * offset 56880
	 */
	scaled_channel<int16_t, 1, 1> cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 56888
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: volt
	 * offset 56896
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: %
	 * offset 56912
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: volts
	 * offset 56920
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: multiplier
	 * offset 56928
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 56936
	 */
	scaled_channel<uint8_t, 1, 1> tcu_shiftTpsBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 56944
	 */
	uint8_t tcu_shiftSpeed12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 56952
	 */
	uint8_t tcu_shiftSpeed23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 56960
	 */
	uint8_t tcu_shiftSpeed34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 56968
	 */
	uint8_t tcu_shiftSpeed21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 56976
	 */
	uint8_t tcu_shiftSpeed32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: {bitStringValue(velocityUnitsLabels, useMetricOnInterface)}
	 * offset 56984
	 */
	uint8_t tcu_shiftSpeed43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: ms
	 * offset 56992
	 */
	float tcu_shiftTime;
	/**
	 * units: volts
	 * offset 56996
	 */
	scaled_channel<int16_t, 10, 1> alternatorVoltageTargetTable[ALTERNATOR_VOLTAGE_TARGET_SIZE][ALTERNATOR_VOLTAGE_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 57028
	 */
	uint16_t alternatorVoltageTargetLoadBins[ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 57036
	 */
	uint16_t alternatorVoltageTargetRpmBins[ALTERNATOR_VOLTAGE_RPM_SIZE] = {};
	/**
	 * units: C
	 * offset 57044
	 */
	float cltBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 57064
	 */
	float cltBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 57084
	 */
	float iatBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 57104
	 */
	float iatBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 57124
	 */
	float cltBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 57144
	 */
	float cltBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 57164
	 */
	float iatBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 57184
	 */
	float iatBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * "Minimum Battery Voltage"
	 * units: #
	 * offset 57204
	 */
	scaled_channel<uint8_t, 10, 1> cel_battery_min_v;
	/**
	 * "Maximum Battery Voltage"
	 * units: #
	 * offset 57205
	 */
	scaled_channel<uint8_t, 10, 1> cel_battery_max_v;
	/**
	 * "Minimum MAP V"
	 * units: V
	 * offset 57206
	 */
	scaled_channel<uint8_t, 50, 1> cel_map_min_v;
	/**
	 * "Maximum MAP V"
	 * units: V
	 * offset 57207
	 */
	scaled_channel<uint8_t, 50, 1> cel_map_max_v;
	/**
	 * "Minimum IAT V"
	 * units: V
	 * offset 57208
	 */
	scaled_channel<uint8_t, 50, 1> cel_iat_min_v;
	/**
	 * "Maximum IAT V"
	 * units: V
	 * offset 57209
	 */
	scaled_channel<uint8_t, 50, 1> cel_iat_max_v;
	/**
	 * "Minimum TPS V"
	 * units: V
	 * offset 57210
	 */
	scaled_channel<uint8_t, 50, 1> cel_tps_min_v;
	/**
	 * "Maximum TPS V"
	 * units: V
	 * offset 57211
	 */
	scaled_channel<uint8_t, 50, 1> cel_tps_max_v;
	/**
	 * units: RPM
	 * offset 57212
	 */
	scaled_channel<uint8_t, 1, 100> minimumOilPressureBins[8] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 57220
	 */
	scaled_channel<uint8_t, 1, 10> minimumOilPressureValues[8] = {};
	/**
	 * offset 57228
	 */
	blend_table_s targetAfrBlends[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * @@DYNO_RPM_STEP_TOOLTIP@@
	 * units: Rpm
	 * offset 57604
	 */
	scaled_channel<uint8_t, 1, 1> dynoRpmStep;
	/**
	 * @@DYNO_SAE_TEMPERATURE_C_TOOLTIP@@
	 * units: C
	 * offset 57605
	 */
	scaled_channel<int8_t, 1, 1> dynoSaeTemperatureC;
	/**
	 * @@DYNO_SAE_RELATIVE_HUMIDITY_TOOLTIP@@
	 * units: %
	 * offset 57606
	 */
	scaled_channel<uint8_t, 1, 1> dynoSaeRelativeHumidity;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 57607
	 */
	uint8_t alignmentFill_at_57607[1] = {};
	/**
	 * @@DYNO_SAE_BARO_TOOLTIP@@
	 * units: KPa
	 * offset 57608
	 */
	scaled_channel<float, 1, 1> dynoSaeBaro;
	/**
	 * @@DYNO_CAR_WHEEL_DIA_INCH_TOOLTIP@@
	 * units: Inch
	 * offset 57612
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelDiaInch;
	/**
	 * @@DYNO_CAR_WHEEL_ASPECT_RATIO_TOOLTIP@@
	 * units: Aspect Ratio (height)
	 * offset 57613
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelAspectRatio;
	/**
	 * @@DYNO_CAR_WHEEL_TIRE_WIDTH_TOOLTIP@@
	 * units: Width mm
	 * offset 57614
	 */
	scaled_channel<int16_t, 1, 1> dynoCarWheelTireWidthMm;
	/**
	 * @@DYNO_CAR_GEAR_PRIMARY_REDUCTION_TOOLTIP@@
	 * units: Units
	 * offset 57616
	 */
	scaled_channel<float, 1, 1> dynoCarGearPrimaryReduction;
	/**
	 * @@DYNO_CAR_GEAR_RATIO_TOOLTIP@@
	 * units: Units
	 * offset 57620
	 */
	scaled_channel<float, 1, 1> dynoCarGearRatio;
	/**
	 * @@DYNO_CAR_GEAR_FINAL_DRIVE_TOOLTIP@@
	 * units: Units
	 * offset 57624
	 */
	scaled_channel<float, 1, 1> dynoCarGearFinalDrive;
	/**
	 * @@DYNO_CAR_CAR_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 57628
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCarMassKg;
	/**
	 * @@DYNO_CAR_CARGO_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 57630
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCargoMassKg;
	/**
	 * @@DYNO_CAR_COEFF_OF_DRAG_TOOLTIP@@
	 * units: Coeff
	 * offset 57632
	 */
	scaled_channel<float, 1, 1> dynoCarCoeffOfDrag;
	/**
	 * @@DYNO_CAR_FRONTAL_AREA_TOOLTIP@@
	 * units: m2
	 * offset 57636
	 */
	scaled_channel<float, 1, 1> dynoCarFrontalAreaM2;
	/**
	 * units: deg
	 * offset 57640
	 */
	scaled_channel<int8_t, 10, 1> trailingSparkTable[TRAILING_SPARK_SIZE][TRAILING_SPARK_RPM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 57656
	 */
	scaled_channel<uint8_t, 1, 50> trailingSparkRpmBins[TRAILING_SPARK_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 57660
	 */
	scaled_channel<uint8_t, 1, 5> trailingSparkLoadBins[TRAILING_SPARK_SIZE] = {};
	/**
	 * units: RPM
	 * offset 57664
	 */
	scaled_channel<uint8_t, 1, 100> maximumOilPressureBins[4] = {};
	/**
	 * units: {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}
	 * offset 57668
	 */
	scaled_channel<uint8_t, 1, 10> maximumOilPressureValues[4] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 57672
	 */
	gppwm_channel_e torqueReductionCutXaxis;
	/**
	 * How many % of ignition events will be cut
	 * units: %
	 * offset 57673
	 */
	int8_t torqueReductionIgnitionCutTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 57685
	 */
	uint8_t alignmentFill_at_57685[1] = {};
	/**
	 * offset 57686
	 */
	int16_t torqueReductionCutXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 57698
	 */
	int8_t torqueReductionCutGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 57700
	 */
	gppwm_channel_e torqueReductionTimeXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 57701
	 */
	uint8_t alignmentFill_at_57701[3] = {};
	/**
	 * For how long after the pin has been triggered will the cut/reduction stay active. After that, even if the pin is still triggered, torque is re-introduced
	 * units: ms
	 * offset 57704
	 */
	float torqueReductionTimeTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 57752
	 */
	int16_t torqueReductionTimeXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 57764
	 */
	int8_t torqueReductionTimeGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 57766
	 */
	gppwm_channel_e torqueReductionIgnitionRetardXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 57767
	 */
	uint8_t alignmentFill_at_57767[1] = {};
	/**
	 * How many degrees of timing advance will be reduced during the Torque Reduction Time
	 * units: deg
	 * offset 57768
	 */
	float torqueReductionIgnitionRetardTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 57816
	 */
	int16_t torqueReductionIgnitionRetardXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 57828
	 */
	int8_t torqueReductionIgnitionRetardGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 57830
	 */
	uint8_t alignmentFill_at_57830[2] = {};
	/**
	offset 57832 bit 0 */
	bool wizardNumberOfCylinders : 1 {};
	/**
	offset 57832 bit 1 */
	bool wizardFiringOrder : 1 {};
	/**
	offset 57832 bit 2 */
	bool wizardMapSensorType : 1 {};
	/**
	offset 57832 bit 3 */
	bool wizardCrankTrigger : 1 {};
	/**
	offset 57832 bit 4 */
	bool wizardCamTrigger : 1 {};
	/**
	offset 57832 bit 5 */
	bool wizardInjectorFlow : 1 {};
	/**
	offset 57832 bit 6 */
	bool boardUseTempPullUp : 1 {};
	/**
	offset 57832 bit 7 */
	bool boardUse2stepPullDown : 1 {};
	/**
	offset 57832 bit 8 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 57832 bit 9 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 57832 bit 10 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 57832 bit 11 */
	bool boardUseH1PullDown : 1 {};
	/**
	offset 57832 bit 12 */
	bool boardUseH2PullDown : 1 {};
	/**
	offset 57832 bit 13 */
	bool boardUseH3PullDown : 1 {};
	/**
	offset 57832 bit 14 */
	bool boardUseH4PullDown : 1 {};
	/**
	offset 57832 bit 15 */
	bool boardUseH5PullDown : 1 {};
	/**
	offset 57832 bit 16 */
	bool boardUseFlexPullDown : 1 {};
	/**
	offset 57832 bit 17 */
	bool unusedBit_315_17 : 1 {};
	/**
	offset 57832 bit 18 */
	bool unusedBit_315_18 : 1 {};
	/**
	offset 57832 bit 19 */
	bool unusedBit_315_19 : 1 {};
	/**
	offset 57832 bit 20 */
	bool unusedBit_315_20 : 1 {};
	/**
	offset 57832 bit 21 */
	bool unusedBit_315_21 : 1 {};
	/**
	offset 57832 bit 22 */
	bool unusedBit_315_22 : 1 {};
	/**
	offset 57832 bit 23 */
	bool unusedBit_315_23 : 1 {};
	/**
	offset 57832 bit 24 */
	bool unusedBit_315_24 : 1 {};
	/**
	offset 57832 bit 25 */
	bool unusedBit_315_25 : 1 {};
	/**
	offset 57832 bit 26 */
	bool unusedBit_315_26 : 1 {};
	/**
	offset 57832 bit 27 */
	bool unusedBit_315_27 : 1 {};
	/**
	offset 57832 bit 28 */
	bool unusedBit_315_28 : 1 {};
	/**
	offset 57832 bit 29 */
	bool unusedBit_315_29 : 1 {};
	/**
	offset 57832 bit 30 */
	bool unusedBit_315_30 : 1 {};
	/**
	offset 57832 bit 31 */
	bool unusedBit_315_31 : 1 {};
};
static_assert(sizeof(persistent_config_s) == 57836);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt
