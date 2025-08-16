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
	 * Below this temperature, correction is disabled.
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
	 * Delay after starting the engine before beginning closed loop correction.
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
	 * Enables lambda sensor long term fuel corrections learning
	offset 0 bit 0 */
	bool enabled : 1 {};
	/**
	 * Enables applying long term fuel corrections
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
	scaled_channel<uint16_t, 1, 1> timeConstant;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2] = {};
};
static_assert(sizeof(ltft_s) == 12);

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
	 * units: kPa
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
	 * https://github.com/rusefi/rusefi/wiki/All-Supported-Triggers
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
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	 * units: deg C
	 * offset 13
	 */
	uint8_t maxAcClt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 14
	 */
	uint8_t alignmentFill_at_14[2] = {};
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
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * units: ADC
	 * offset 276
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * units: ADC
	 * offset 278
	 */
	int16_t tpsMax;
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
	 * Zero value means do not detect tuning
	 * units: seconds
	 * offset 505
	 */
	uint8_t tuningDetector;
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
	 * Lua: Allows for custom fuel calculations using Lua scripting, enabling highly specific tuning applications where the other strategies don’t apply.
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
	 * units: kph
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
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: deg C
	 * offset 579
	 */
	uint8_t fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: deg C
	 * offset 580
	 */
	uint8_t fanOffTemperature;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 581
	 */
	uint8_t alignmentFill_at_581[1] = {};
	/**
	 * offset 582
	 */
	output_pin_e fan2Pin;
	/**
	 * offset 584
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: deg C
	 * offset 585
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: deg C
	 * offset 586
	 */
	uint8_t fan2OffTemperature;
	/**
	 * offset 587
	 */
	int8_t disableFan1AtSpeed;
	/**
	 * offset 588
	 */
	int8_t disableFan2AtSpeed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 589
	 */
	uint8_t alignmentFill_at_589[3] = {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 592 bit 0 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 592 bit 1 */
	bool disableFan2WhenStopped : 1 {};
	/**
	offset 592 bit 2 */
	bool unusedBit_167_2 : 1 {};
	/**
	offset 592 bit 3 */
	bool unusedBit_167_3 : 1 {};
	/**
	offset 592 bit 4 */
	bool unusedBit_167_4 : 1 {};
	/**
	offset 592 bit 5 */
	bool unusedBit_167_5 : 1 {};
	/**
	offset 592 bit 6 */
	bool unusedBit_167_6 : 1 {};
	/**
	offset 592 bit 7 */
	bool unusedBit_167_7 : 1 {};
	/**
	offset 592 bit 8 */
	bool unusedBit_167_8 : 1 {};
	/**
	offset 592 bit 9 */
	bool unusedBit_167_9 : 1 {};
	/**
	offset 592 bit 10 */
	bool unusedBit_167_10 : 1 {};
	/**
	offset 592 bit 11 */
	bool unusedBit_167_11 : 1 {};
	/**
	offset 592 bit 12 */
	bool unusedBit_167_12 : 1 {};
	/**
	offset 592 bit 13 */
	bool unusedBit_167_13 : 1 {};
	/**
	offset 592 bit 14 */
	bool unusedBit_167_14 : 1 {};
	/**
	offset 592 bit 15 */
	bool unusedBit_167_15 : 1 {};
	/**
	offset 592 bit 16 */
	bool unusedBit_167_16 : 1 {};
	/**
	offset 592 bit 17 */
	bool unusedBit_167_17 : 1 {};
	/**
	offset 592 bit 18 */
	bool unusedBit_167_18 : 1 {};
	/**
	offset 592 bit 19 */
	bool unusedBit_167_19 : 1 {};
	/**
	offset 592 bit 20 */
	bool unusedBit_167_20 : 1 {};
	/**
	offset 592 bit 21 */
	bool unusedBit_167_21 : 1 {};
	/**
	offset 592 bit 22 */
	bool unusedBit_167_22 : 1 {};
	/**
	offset 592 bit 23 */
	bool unusedBit_167_23 : 1 {};
	/**
	offset 592 bit 24 */
	bool unusedBit_167_24 : 1 {};
	/**
	offset 592 bit 25 */
	bool unusedBit_167_25 : 1 {};
	/**
	offset 592 bit 26 */
	bool unusedBit_167_26 : 1 {};
	/**
	offset 592 bit 27 */
	bool unusedBit_167_27 : 1 {};
	/**
	offset 592 bit 28 */
	bool unusedBit_167_28 : 1 {};
	/**
	offset 592 bit 29 */
	bool unusedBit_167_29 : 1 {};
	/**
	offset 592 bit 30 */
	bool unusedBit_167_30 : 1 {};
	/**
	offset 592 bit 31 */
	bool unusedBit_167_31 : 1 {};
	/**
	 * offset 596
	 */
	output_pin_e acrPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 598
	 */
	uint8_t alignmentFill_at_598[2] = {};
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	 * units: revs/km
	 * offset 600
	 */
	float driveWheelRevPerKm;
	/**
	 * CANbus thread period in ms
	 * units: ms
	 * offset 604
	 */
	int canSleepPeriodMs;
	/**
	 * units: index
	 * offset 608
	 */
	int byFirmwareVersion;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
	 * Analog TPS inputs have 200Hz low-pass cutoff.
	 * offset 612
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 613
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 614
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * offset 615
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 616
	 */
	float idle_derivativeFilterLoss;
	/**
	 * offset 620
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	 * units: %
	 * offset 632
	 */
	float airByRpmTaper;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	 * units: %
	 * offset 636
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 637
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 638
	 */
	uint8_t acrRevolutions;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 639
	 */
	uint8_t alignmentFill_at_639[1] = {};
	/**
	 * offset 640
	 */
	int calibrationBirthday;
	/**
	 * units: volts
	 * offset 644
	 */
	float adcVcc;
	/**
	 * Magic engine phase: we compare instant MAP at X to instant MAP at x+360 angle in one complete cycle
	 * units: Deg
	 * offset 648
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 652
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 660
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 680
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * TPS/PPS error threshold
	 * units: %
	 * offset 681
	 */
	scaled_channel<uint8_t, 10, 1> etbSplit;
	/**
	 * offset 682
	 */
	Gpio tle6240_cs;
	/**
	 * offset 684
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * offset 685
	 */
	pin_output_mode_e mc33810_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 686
	 */
	uint8_t alignmentFill_at_686[2] = {};
	/**
	 * @see hasBaroSensor
	 * offset 688
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 700
	 */
	idle_hardware_s idle;
	/**
	 * Ignition timing to remove when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 712
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	 * units: deg/s
	 * offset 713
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 714
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * offset 715
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	 * units: ratio
	 * offset 716
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 718
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	 * units: count
	 * offset 719
	 */
	uint8_t vssToothCount;
	/**
	 * Override the Y axis (load) value used for only the Idle VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 720
	 */
	ve_override_e idleVeOverrideMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 721
	 */
	uint8_t alignmentFill_at_721[1] = {};
	/**
	 * offset 722
	 */
	Gpio l9779_cs;
	/**
	 * offset 724
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 748
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 772
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 773
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 774
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 776
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * How many consecutive VVT gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 777
	 */
	int8_t gapVvtTrackingLengthOverride;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 778
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 780
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 781
	 */
	uint8_t alignmentFill_at_781[1] = {};
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 782
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 784
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 786
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 787
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 788
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT] = {};
	/**
	 * offset 796
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 797
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 798
	 */
	Gpio debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 800
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 801
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 802
	 */
	Gpio mc33972_cs;
	/**
	 * offset 804
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 805
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 806
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 807
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 808
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 809
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 810
	 */
	Gpio mc33810_cs[C_MC33810_COUNT] = {};
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 812
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 816
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT] = {};
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	 * units: us
	 * offset 820
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * Ratio between the wheels and your transmission output.
	 * units: ratio
	 * offset 822
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 824
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 826
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 827
	 */
	uint8_t alignmentFill_at_827[1] = {};
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 828
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * units: g/s
	 * offset 832
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 834
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * offset 836
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 837
	 */
	uint8_t alignmentFill_at_837[1] = {};
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 838
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 840
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * units: RPM
	 * offset 841
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionMinRpm;
	/**
	 * units: %
	 * offset 842
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionMinLoad;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 843
	 */
	uint8_t alignmentFill_at_843[1] = {};
	/**
	offset 844 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 844 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 844 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 844 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 844 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 844 bit 5 */
	bool rethrowHardFault : 1 {};
	/**
	offset 844 bit 6 */
	bool verboseQuad : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 844 bit 7 */
	bool useStepperIdle : 1 {};
	/**
	offset 844 bit 8 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 844 bit 9 */
	bool lambdaProtectionEnable : 1 {};
	/**
	offset 844 bit 10 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	offset 844 bit 11 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 844 bit 12 */
	bool externalRusEfiGdiModule : 1 {};
	/**
	offset 844 bit 13 */
	bool unusedFlipWboChannels : 1 {};
	/**
	 * Useful for individual intakes
	offset 844 bit 14 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 844 bit 15 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 844 bit 16 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 844 bit 17 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 844 bit 18 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 844 bit 19 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 844 bit 20 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 844 bit 21 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 844 bit 22 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 844 bit 23 */
	bool pauseEtbControl : 1 {};
	/**
	offset 844 bit 24 */
	bool verboseKLine : 1 {};
	/**
	offset 844 bit 25 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 844 bit 26 */
	bool enableAemXSeries : 1 {};
	/**
	offset 844 bit 27 */
	bool modeledFlowIdle : 1 {};
	/**
	offset 844 bit 28 */
	bool unusedBit_309_28 : 1 {};
	/**
	offset 844 bit 29 */
	bool unusedBit_309_29 : 1 {};
	/**
	offset 844 bit 30 */
	bool unusedBit_309_30 : 1 {};
	/**
	offset 844 bit 31 */
	bool unusedBit_309_31 : 1 {};
	/**
	 * offset 848
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT] = {};
	/**
	 * offset 856
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 857
	 */
	uint8_t alignmentFill_at_857[3] = {};
	/**
	 * offset 860
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	 * units: v
	 * offset 864
	 */
	uint8_t mc33_hvolt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 865
	 */
	uint8_t alignmentFill_at_865[1] = {};
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 866
	 */
	uint16_t minimumBoostClosedLoopMap;
	/**
	 * units: %
	 * offset 868
	 */
	int8_t initialIgnitionCutPercent;
	/**
	 * units: %
	 * offset 869
	 */
	int8_t finalIgnitionCutPercentBeforeLaunch;
	/**
	 * offset 870
	 */
	gppwm_channel_e boostOpenLoopYAxis;
	/**
	 * offset 871
	 */
	spi_device_e l9779spiDevice;
	/**
	 * offset 872
	 */
	imu_type_e imuType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 873
	 */
	uint8_t alignmentFill_at_873[1] = {};
	/**
	 * How far above idle speed do we consider idling, i.e. coasting detection threshold.
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	 * units: RPM
	 * offset 874
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	 * units: ms
	 * offset 876
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 878
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 880
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 882
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 884
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 886
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 887
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 888
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT] = {};
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 904
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * Since torque reduction pin is usually shared with launch control, most people have an RPM where behavior under that is Launch Control, over that is Flat Shift/Torque Reduction
	 * units: rpm
	 * offset 906
	 */
	uint16_t torqueReductionArmingRpm;
	/**
	 * offset 908
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 909
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	 * units: :1
	 * offset 910
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	 * units: %
	 * offset 911
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	 * units: hz
	 * offset 912
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 914
	 */
	adc_channel_e idlePositionChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 915
	 */
	uint8_t alignmentFill_at_915[1] = {};
	/**
	 * offset 916
	 */
	uint16_t launchCorrectionsEndRpm;
	/**
	 * offset 918
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 920
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 921
	 */
	uint8_t alignmentFill_at_921[1] = {};
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 922
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 924
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * units: RPM
	 * offset 926
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionRestoreRpm;
	/**
	 * offset 927
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	 * units: count
	 * offset 928
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	 * units: %
	 * offset 932
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * units: %
	 * offset 934
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * units: ADC
	 * offset 936
	 */
	uint16_t tps1SecondaryMin;
	/**
	 * units: ADC
	 * offset 938
	 */
	uint16_t tps1SecondaryMax;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	 * units: Seconds
	 * offset 940
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 942
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 944
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 945
	 */
	uint8_t alignmentFill_at_945[1] = {};
	/**
	 * offset 946
	 */
	output_pin_e acRelayPin;
	/**
	 * units: %
	 * offset 948
	 */
	uint8_t lambdaProtectionMinTps;
	/**
	 * Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed
	 * units: s
	 * offset 949
	 */
	scaled_channel<uint8_t, 10, 1> lambdaProtectionTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 950
	 */
	uint8_t alignmentFill_at_950[2] = {};
	/**
	 * offset 952
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT] = {};
	/**
	 * offset 984
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 986
	 */
	Gpio spi1misoPin;
	/**
	 * offset 988
	 */
	Gpio spi1sckPin;
	/**
	 * offset 990
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 992
	 */
	Gpio spi2misoPin;
	/**
	 * offset 994
	 */
	Gpio spi2sckPin;
	/**
	 * offset 996
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 998
	 */
	Gpio spi3misoPin;
	/**
	 * offset 1000
	 */
	Gpio spi3sckPin;
	/**
	 * UNUSED
	 * Will remove in 2026 for sure
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 1002
	 */
	Gpio cdmInputPin;
	/**
	 * offset 1004
	 */
	uart_device_e consoleUartDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1005
	 */
	uint8_t alignmentFill_at_1005[3] = {};
	/**
	 * Ramp the idle target down from the entry threshold over N seconds when returning to idle. Helps prevent overshooting (below) the idle target while returning to idle from coasting.
	offset 1008 bit 0 */
	bool idleReturnTargetRamp : 1 {};
	/**
	offset 1008 bit 1 */
	bool useInjectorFlowLinearizationTable : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 1008 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 1008 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 1008 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 1008 bit 5 */
	bool canInputBCM : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	offset 1008 bit 6 */
	bool consumeObdSensors : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 1008 bit 7 */
	bool enableCanVss : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 1008 bit 8 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 1008 bit 9 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 1008 bit 10 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 1008 bit 11 */
	bool verboseVVTDecoding : 1 {};
	/**
	offset 1008 bit 12 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * In Alpha-N mode, compensate for air temperature.
	offset 1008 bit 13 */
	bool alphaNUseIat : 1 {};
	/**
	offset 1008 bit 14 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 1008 bit 15 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 1008 bit 16 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 1008 bit 17 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 1008 bit 18 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 1008 bit 19 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 1008 bit 20 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 1008 bit 21 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 1008 bit 22 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 1008 bit 23 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 1008 bit 24 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 1008 bit 25 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 1008 bit 26 */
	bool tcuEnabled : 1 {};
	/**
	offset 1008 bit 27 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coils
	offset 1008 bit 28 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 1008 bit 29 */
	bool verboseCan2 : 1 {};
	/**
	offset 1008 bit 30 */
	bool unusedBit_408_30 : 1 {};
	/**
	offset 1008 bit 31 */
	bool unusedBit_408_31 : 1 {};
	/**
	 * offset 1012
	 */
	dc_io etbIo[ETB_COUNT] = {};
	/**
	 * offset 1028
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 1030
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 1032
	 */
	pid_s boostPid;
	/**
	 * offset 1052
	 */
	boostType_e boostType;
	/**
	 * offset 1053
	 */
	pin_input_mode_e ignitionKeyDigitalPinMode;
	/**
	 * offset 1054
	 */
	Gpio ignitionKeyDigitalPin;
	/**
	 * units: Hz
	 * offset 1056
	 */
	int boostPwmFrequency;
	/**
	 * offset 1060
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1061
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * offset 1062
	 */
	cranking_condition_e crankingCondition;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	 * units: sec
	 * offset 1063
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	 * units: roc
	 * offset 1064
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	 * units: coeff
	 * offset 1068
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * Time to blend from predicted MAP back to real MAP. Used only in Predictive MAP mode.
	 * units: s
	 * offset 1072
	 */
	float mapPredictionBlendDuration;
	/**
	 * Selects the acceleration enrichment strategy.
	 * offset 1076
	 */
	accel_enrichment_mode_e accelEnrichmentMode;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	 * units: sec
	 * offset 1077
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Pause closed loop fueling after acceleration fuel occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel accel.
	 * units: sec
	 * offset 1078
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterAccelTime;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1079
	 */
	uint8_t alignmentFill_at_1079[1] = {};
	/**
	 * Launch disabled above this speed if setting is above zero
	 * units: Kph
	 * offset 1080
	 */
	int launchSpeedThreshold;
	/**
	 * Starting Launch RPM window to activate (subtracts from Launch RPM)
	 * units: RPM
	 * offset 1084
	 */
	int launchRpmWindow;
	/**
	 * units: ms
	 * offset 1088
	 */
	float triggerEventsTimeoutMs;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 1092
	 */
	float ppsExpAverageAlpha;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 1096
	 */
	float mapExpAverageAlpha;
	/**
	 * offset 1100
	 */
	float magicNumberAvailableForDevTricks;
	/**
	 * offset 1104
	 */
	float turbochargerFilter;
	/**
	 * offset 1108
	 */
	int launchTpsThreshold;
	/**
	 * offset 1112
	 */
	float launchActivateDelay;
	/**
	 * offset 1116
	 */
	stft_s stft;
	/**
	 * offset 1144
	 */
	ltft_s ltft;
	/**
	 * offset 1156
	 */
	dc_io stepperDcIo[DC_PER_STEPPER] = {};
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1172
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1204
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1236
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1268
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT] = {};
	/**
	 * offset 1280
	 */
	dc_function_e etbFunctions[ETB_COUNT] = {};
	/**
	 * offset 1282
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1283
	 */
	uint8_t alignmentFill_at_1283[1] = {};
	/**
	 * offset 1284
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1286
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1287
	 */
	uint8_t alignmentFill_at_1287[1] = {};
	/**
	 * offset 1288
	 */
	Gpio drv8860_miso;
	/**
	 * offset 1290
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1306
	 */
	uint8_t alignmentFill_at_1306[2] = {};
	/**
	 * Angle between cam sensor and VVT zero position
	 * units: value
	 * offset 1308
	 */
	float vvtOffsets[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1324
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT] = {};
	/**
	 * offset 1372
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS] = {};
	/**
	 * units: ADC
	 * offset 1436
	 */
	uint16_t tps2SecondaryMin;
	/**
	 * units: ADC
	 * offset 1438
	 */
	uint16_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1440 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1440 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Write SD card log even when powered by USB
	offset 1440 bit 2 */
	bool alwaysWriteSdCard : 1 {};
	/**
	 * Second harmonic (aka double) is usually quieter background noise
	offset 1440 bit 3 */
	bool knockDetectionUseDoubleFrequency : 1 {};
	/**
	offset 1440 bit 4 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1440 bit 5 */
	bool silentTriggerError : 1 {};
	/**
	offset 1440 bit 6 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1440 bit 7 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write. See also can1ListenMode
	offset 1440 bit 8 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1440 bit 9 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1440 bit 10 */
	bool enableOilPressureProtect : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1440 bit 11 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1440 bit 12 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1440 bit 13 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1440 bit 14 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	 * When enabled, this option cuts the fuel supply when the RPM limit is reached. Cutting fuel provides a smoother limiting action; however, it may lead to slightly higher combustion chamber temperatures since unburned fuel is not present to cool the combustion process.
	offset 1440 bit 15 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1440 bit 16 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1440 bit 17 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1440 bit 18 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1440 bit 19 */
	bool torqueReductionEnabled : 1 {};
	/**
	offset 1440 bit 20 */
	bool camSyncOnSecondCrankRevolution : 1 {};
	/**
	offset 1440 bit 21 */
	bool limitTorqueReductionTime : 1 {};
	/**
	 * Are you a developer troubleshooting TS over CAN ISO/TP?
	offset 1440 bit 22 */
	bool verboseIsoTp : 1 {};
	/**
	 * In this mode only trigger events go into engine sniffer and not coils/injectors etc
	offset 1440 bit 23 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1440 bit 24 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1440 bit 25 */
	bool skippedWheelOnCam : 1 {};
	/**
	offset 1440 bit 26 */
	bool unusedBit_485_26 : 1 {};
	/**
	offset 1440 bit 27 */
	bool unusedBit_485_27 : 1 {};
	/**
	offset 1440 bit 28 */
	bool unusedBit_485_28 : 1 {};
	/**
	offset 1440 bit 29 */
	bool unusedBit_485_29 : 1 {};
	/**
	offset 1440 bit 30 */
	bool unusedBit_485_30 : 1 {};
	/**
	offset 1440 bit 31 */
	bool unusedBit_485_31 : 1 {};
	/**
	 * A/C button input
	 * offset 1444
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1446
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	 * units: %
	 * offset 1447
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1448
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1449
	 */
	uint8_t alignmentFill_at_1449[3] = {};
	/**
	offset 1452 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1452 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1452 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1452 bit 3 */
	bool complexWallModel : 1 {};
	/**
	 * RPM is measured based on last 720 degrees while instant RPM is measured based on the last 90 degrees of crank revolution
	offset 1452 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1452 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra advance at low idle speeds will prevent stalling and extra retard at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1452 bit 6 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1452 bit 7 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1452 bit 8 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 1452 bit 9 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	offset 1452 bit 10 */
	bool hondaK : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1452 bit 11 */
	bool twoWireBatchIgnition : 1 {};
	/**
	 * Read MAP sensor on ECU start-up to use as baro value.
	offset 1452 bit 12 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1452 bit 13 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT and PID idle).
	 * You probably don't need this.
	offset 1452 bit 14 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1452 bit 15 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1452 bit 16 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1452 bit 17 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1452 bit 18 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1452 bit 19 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1452 bit 20 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors.
	 * WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1452 bit 21 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1452 bit 22 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1452 bit 23 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1452 bit 24 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	offset 1452 bit 25 */
	bool sdTriggerLog : 1 {};
	/**
	offset 1452 bit 26 */
	bool stepper_dc_use_two_wires : 1 {};
	/**
	offset 1452 bit 27 */
	bool watchOutForLinearTime : 1 {};
	/**
	offset 1452 bit 28 */
	bool unusedBit_524_28 : 1 {};
	/**
	offset 1452 bit 29 */
	bool unusedBit_524_29 : 1 {};
	/**
	offset 1452 bit 30 */
	bool unusedBit_524_30 : 1 {};
	/**
	offset 1452 bit 31 */
	bool unusedBit_524_31 : 1 {};
	/**
	 * units: count
	 * offset 1456
	 */
	uint32_t engineChartSize;
	/**
	 * units: mult
	 * offset 1460
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * Idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	 * units: RPM
	 * offset 1464
	 */
	int16_t acIdleRpmTarget;
	/**
	 * set warningPeriod X
	 * units: seconds
	 * offset 1466
	 */
	int16_t warningPeriod;
	/**
	 * units: angle
	 * offset 1468
	 */
	float knockDetectionWindowStart;
	/**
	 * units: ms
	 * offset 1472
	 */
	float idleStepperReactionTime;
	/**
	 * units: count
	 * offset 1476
	 */
	int idleStepperTotalSteps;
	/**
	 * Pedal position to realize that we need to reduce torque when the trigger pin is uuuh triggered
	 * offset 1480
	 */
	int torqueReductionArmingApp;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1484
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	 * units: Seconds
	 * offset 1488
	 */
	float wwaeTau;
	/**
	 * offset 1492
	 */
	pid_s alternatorControl;
	/**
	 * offset 1512
	 */
	pid_s etb;
	/**
	 * RPM range above upper limit for extra air taper
	 * units: RPM
	 * offset 1532
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1534
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * units: ADC
	 * offset 1536
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * units: ADC
	 * offset 1538
	 */
	int16_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1540
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1542
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1543
	 */
	uint8_t alignmentFill_at_1543[1] = {};
	/**
	 * offset 1544
	 */
	Gpio mc33816_flag0;
	/**
	 * offset 1546
	 */
	scaled_channel<uint16_t, 1000, 1> tachPulsePerRev;
	/**
	 * kPa/psi value which is too low to be true
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 1548
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa/psi value which is too high to be true
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 1552
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	 * units: ms
	 * offset 1556
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	 * units: ms
	 * offset 1558
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1560
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	 * units: Fraction
	 * offset 1580
	 */
	float wwaeBeta;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1584
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1586
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1588
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT] = {};
	/**
	 * offset 1592
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1594
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * units: volts
	 * offset 1596
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * units: volts
	 * offset 1600
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	 * units: seconds
	 * offset 1604
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	 * units: RPM
	 * offset 1606
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * See Over/Undervoltage Shutdown/Retry bit in documentation
	offset 1608 bit 0 */
	bool mc33810DisableRecoveryMode : 1 {};
	/**
	offset 1608 bit 1 */
	bool mc33810Gpgd0Mode : 1 {};
	/**
	offset 1608 bit 2 */
	bool mc33810Gpgd1Mode : 1 {};
	/**
	offset 1608 bit 3 */
	bool mc33810Gpgd2Mode : 1 {};
	/**
	offset 1608 bit 4 */
	bool mc33810Gpgd3Mode : 1 {};
	/**
	 * Send out board statistics
	offset 1608 bit 5 */
	bool enableExtendedCanBroadcast : 1 {};
	/**
	 * global_can_data performance hack
	offset 1608 bit 6 */
	bool luaCanRxWorkaround : 1 {};
	/**
	offset 1608 bit 7 */
	bool flexSensorInverted : 1 {};
	/**
	offset 1608 bit 8 */
	bool useHardSkipInTraction : 1 {};
	/**
	 * Use Aux Speed 1 as one of speeds for wheel slip ratio?
	offset 1608 bit 9 */
	bool useAuxSpeedForSlipRatio : 1 {};
	/**
	 * VSS and Aux Speed 1 or Aux Speed 1 with Aux Speed 2?
	offset 1608 bit 10 */
	bool useVssAsSecondWheelSpeed : 1 {};
	/**
	offset 1608 bit 11 */
	bool is_enabled_spi_5 : 1 {};
	/**
	offset 1608 bit 12 */
	bool is_enabled_spi_6 : 1 {};
	/**
	 * AEM X-Series EGT gauge kit or rusEFI EGT sensor from Wideband controller
	offset 1608 bit 13 */
	bool enableAemXSeriesEgt : 1 {};
	/**
	offset 1608 bit 14 */
	bool startRequestPinInverted : 1 {};
	/**
	offset 1608 bit 15 */
	bool tcu_rangeSensorPulldown : 1 {};
	/**
	offset 1608 bit 16 */
	bool devBit01 : 1 {};
	/**
	offset 1608 bit 17 */
	bool devBit0 : 1 {};
	/**
	offset 1608 bit 18 */
	bool devBit1 : 1 {};
	/**
	offset 1608 bit 19 */
	bool devBit2 : 1 {};
	/**
	offset 1608 bit 20 */
	bool devBit3 : 1 {};
	/**
	offset 1608 bit 21 */
	bool devBit4 : 1 {};
	/**
	offset 1608 bit 22 */
	bool devBit5 : 1 {};
	/**
	offset 1608 bit 23 */
	bool devBit6 : 1 {};
	/**
	offset 1608 bit 24 */
	bool devBit7 : 1 {};
	/**
	offset 1608 bit 25 */
	bool invertExhaustCamVVTSignal : 1 {};
	/**
	 * "Available via TS Plugin see https://rusefi.com/s/knock"
	offset 1608 bit 26 */
	bool enableKnockSpectrogram : 1 {};
	/**
	offset 1608 bit 27 */
	bool enableKnockSpectrogramFilter : 1 {};
	/**
	offset 1608 bit 28 */
	bool unusedBit_592_28 : 1 {};
	/**
	offset 1608 bit 29 */
	bool unusedBit_592_29 : 1 {};
	/**
	offset 1608 bit 30 */
	bool unusedBit_592_30 : 1 {};
	/**
	offset 1608 bit 31 */
	bool unusedBit_592_31 : 1 {};
	/**
	 * This value is an added for base idle value. Idle Value added when coasting and transitioning into idle.
	 * units: percent
	 * offset 1612
	 */
	int16_t iacByTpsTaper;
	/**
	 * offset 1614
	 */
	Gpio accelerometerCsPin;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: kph
	 * offset 1616
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: kph
	 * offset 1617
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1618
	 */
	uint8_t alignmentFill_at_1618[2] = {};
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	 * units: roc
	 * offset 1620
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1624
	 */
	brain_input_pin_e auxSpeedSensorInputPin[AUX_SPEED_SENSOR_COUNT] = {};
	/**
	 * offset 1628
	 */
	uint8_t totalGearsCount;
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 1629
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1630
	 */
	debug_mode_e debugMode;
	/**
	 * Additional idle % when fan #1 is active
	 * units: %
	 * offset 1631
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * Band rate for primary TTL
	 * units: BPs
	 * offset 1632
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * units: volts
	 * offset 1636
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	 * units: volts
	 * offset 1640
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * offset 1644
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1645
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1646
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1647
	 */
	load_override_e afrOverrideMode;
	/**
	 * units: A
	 * offset 1648
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	 * units: A
	 * offset 1649
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	 * units: us
	 * offset 1650
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	 * units: ms
	 * offset 1651
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1652 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Allow OpenBLT on Primary CAN
	offset 1652 bit 1 */
	bool canOpenBLT : 1 {};
	/**
	 * Allow OpenBLT on Secondary CAN
	offset 1652 bit 2 */
	bool can2OpenBLT : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (default, cc/min) or mass flow (g/s).
	offset 1652 bit 3 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1652 bit 4 */
	bool boardUseCanTerminator : 1 {};
	/**
	offset 1652 bit 5 */
	bool kLineDoHondaSend : 1 {};
	/**
	 * ListenMode is about acknowledging CAN traffic on the protocol level. Different from canWriteEnabled
	offset 1652 bit 6 */
	bool can1ListenMode : 1 {};
	/**
	offset 1652 bit 7 */
	bool can2ListenMode : 1 {};
	/**
	offset 1652 bit 8 */
	bool unusedBit_626_8 : 1 {};
	/**
	offset 1652 bit 9 */
	bool unusedBit_626_9 : 1 {};
	/**
	offset 1652 bit 10 */
	bool unusedBit_626_10 : 1 {};
	/**
	offset 1652 bit 11 */
	bool unusedBit_626_11 : 1 {};
	/**
	offset 1652 bit 12 */
	bool unusedBit_626_12 : 1 {};
	/**
	offset 1652 bit 13 */
	bool unusedBit_626_13 : 1 {};
	/**
	offset 1652 bit 14 */
	bool unusedBit_626_14 : 1 {};
	/**
	offset 1652 bit 15 */
	bool unusedBit_626_15 : 1 {};
	/**
	offset 1652 bit 16 */
	bool unusedBit_626_16 : 1 {};
	/**
	offset 1652 bit 17 */
	bool unusedBit_626_17 : 1 {};
	/**
	offset 1652 bit 18 */
	bool unusedBit_626_18 : 1 {};
	/**
	offset 1652 bit 19 */
	bool unusedBit_626_19 : 1 {};
	/**
	offset 1652 bit 20 */
	bool unusedBit_626_20 : 1 {};
	/**
	offset 1652 bit 21 */
	bool unusedBit_626_21 : 1 {};
	/**
	offset 1652 bit 22 */
	bool unusedBit_626_22 : 1 {};
	/**
	offset 1652 bit 23 */
	bool unusedBit_626_23 : 1 {};
	/**
	offset 1652 bit 24 */
	bool unusedBit_626_24 : 1 {};
	/**
	offset 1652 bit 25 */
	bool unusedBit_626_25 : 1 {};
	/**
	offset 1652 bit 26 */
	bool unusedBit_626_26 : 1 {};
	/**
	offset 1652 bit 27 */
	bool unusedBit_626_27 : 1 {};
	/**
	offset 1652 bit 28 */
	bool unusedBit_626_28 : 1 {};
	/**
	offset 1652 bit 29 */
	bool unusedBit_626_29 : 1 {};
	/**
	offset 1652 bit 30 */
	bool unusedBit_626_30 : 1 {};
	/**
	offset 1652 bit 31 */
	bool unusedBit_626_31 : 1 {};
	/**
	 * Angle of tooth detection within engine phase cycle
	 * units: angle
	 * offset 1656
	 */
	uint8_t camDecoder2jzPosition;
	/**
	 * offset 1657
	 */
	mc33810maxDwellTimer_e mc33810maxDwellTimer;
	/**
	 * Duration of each test pulse
	 * units: ms
	 * offset 1658
	 */
	scaled_channel<uint16_t, 100, 1> benchTestOnTime;
	/**
	 * units: %
	 * offset 1660
	 */
	uint8_t lambdaProtectionRestoreTps;
	/**
	 * units: %
	 * offset 1661
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionRestoreLoad;
	/**
	 * offset 1662
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1663
	 */
	uint8_t alignmentFill_at_1663[1] = {};
	/**
	 * offset 1664
	 */
	Gpio can2TxPin;
	/**
	 * offset 1666
	 */
	Gpio can2RxPin;
	/**
	 * offset 1668
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1669
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1670
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1671
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1672
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1674
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1675
	 */
	uint8_t alignmentFill_at_1675[1] = {};
	/**
	 * Specifies the boost pressure allowed before triggering a cut. Setting this to 0 will DISABLE overboost cut.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 1676
	 */
	float boostCutPressure;
	/**
	 * units: kg/h
	 * offset 1680
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16] = {};
	/**
	 * units: ratio
	 * offset 1696
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16] = {};
	/**
	 * Fixed timing, useful for TDC testing
	 * units: deg
	 * offset 1712
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1716
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1720
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * units: value
	 * offset 1724
	 */
	float egoValueShift;
	/**
	 * VVT output solenoid pin for this cam
	 * offset 1728
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1736
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMinTps;
	/**
	 * offset 1737
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMaxTps;
	/**
	 * offset 1738
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMinTps;
	/**
	 * offset 1739
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMaxTps;
	/**
	 * offset 1740
	 */
	pwm_freq_t vvtOutputFrequency;
	/**
	 * Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.
	 * units: deg BTDC
	 * offset 1742
	 */
	int8_t minimumIgnitionTiming;
	/**
	 * Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC
	 * units: deg BTDC
	 * offset 1743
	 */
	int8_t maximumIgnitionTiming;
	/**
	 * units: Hz
	 * offset 1744
	 */
	int alternatorPwmFrequency;
	/**
	 * offset 1748
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK] = {};
	/**
	 * Additional idle % when fan #2 is active
	 * units: %
	 * offset 1750
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	 * units: sec
	 * offset 1751
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1752
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT] = {};
	/**
	 * offset 1760
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 1784
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1785
	 */
	pin_output_mode_e accelerometerCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 *  HPFP fuel mass compensation = manual mode for GDI engines
	 * offset 1786
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1787
	 */
	uint8_t alignmentFill_at_1787[1] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa/50.7psi here.
	 * This is gauge pressure/in reference to atmospheric.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 1788
	 */
	float fuelReferencePressure;
	/**
	 * offset 1792
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1824
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * units: Deg
	 * offset 1856
	 */
	int16_t knockSamplingDuration;
	/**
	 * units: Hz
	 * offset 1858
	 */
	int16_t etbFreq;
	/**
	 * offset 1860
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 1880
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	 * units: %
	 * offset 1881
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	 * units: %
	 * offset 1882
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 1883
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder ignition and fueling timing correction for uneven engines
	 * units: deg
	 * offset 1884
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: seconds
	 * offset 1932
	 */
	float idlePidActivationTime;
	/**
	 * Minimum coolant temperature to activate VVT
	 * units: deg C
	 * offset 1936
	 */
	int8_t vvtControlMinClt;
	/**
	 * offset 1937
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 1938
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 1939
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 1940
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 1941
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 1942
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 1943
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 1944
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 1945
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 1946
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1947
	 */
	uint8_t alignmentFill_at_1947[1] = {};
	/**
	 * ResetB
	 * offset 1948
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 1950
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 1952
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1954
	 */
	uint8_t alignmentFill_at_1954[2] = {};
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 1956
	 */
	pid_s auxPid[CAMS_PER_BANK] = {};
	/**
	 * offset 1996
	 */
	float injectorCorrectionPolynomial[8] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 2028
	 */
	scaled_channel<int16_t, 1, 1> primeBins[PRIME_CURVE_COUNT] = {};
	/**
	 * offset 2044
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2064
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2065
	 */
	uint8_t alignmentFill_at_2065[1] = {};
	/**
	 * offset 2066
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2068
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2070
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2071
	 */
	uint8_t alignmentFill_at_2071[1] = {};
	/**
	 * offset 2072
	 */
	Gpio mc33816_cs;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2074
	 */
	uint8_t alignmentFill_at_2074[2] = {};
	/**
	 * units: hz
	 * offset 2076
	 */
	float auxFrequencyFilter;
	/**
	 * offset 2080
	 */
	sent_input_pin_e sentInputPins[SENT_INPUT_COUNT] = {};
	/**
	 * This sets the RPM above which fuel cut is active.
	 * units: rpm
	 * offset 2082
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	 * units: rpm
	 * offset 2084
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	 * units: %
	 * offset 2086
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 2088
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * units: %
	 * offset 2090
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 2092
	 */
	int16_t coastingFuelCutMap;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2094
	 */
	uint8_t alignmentFill_at_2094[2] = {};
	/**
	 * offset 2096
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2116
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * offset 2136
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT] = {};
	/**
	 * offset 2232
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT] = {};
	/**
	 * offset 2296
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT] = {};
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2424
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2428
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	 * units: kg/h
	 * offset 2432
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2436
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2440
	 */
	float tChargeAirDecrLimit;
	/**
	 * iTerm min value
	 * offset 2444
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2446
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2448
	 */
	pid_s idleTimingPid;
	/**
	 * When entering idle, and the PID settings are aggressive, it's good to make a soft entry upon entering closed loop
	 * offset 2468
	 */
	float idleTimingSoftEntryTime;
	/**
	 * offset 2472
	 */
	pin_input_mode_e torqueReductionTriggerPinMode;
	/**
	 * offset 2473
	 */
	torqueReductionActivationMode_e torqueReductionActivationMode;
	/**
	 * A delay in cycles between fuel-enrich. portions
	 * units: cycles
	 * offset 2474
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	 * units: coef
	 * offset 2476
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2480
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2481
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2482
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2484
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	 * units: :1
	 * offset 2485
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2486
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	 * units: %
	 * offset 2488
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2492
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: mg
	 * offset 2504
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[PRIME_CURVE_COUNT] = {};
	/**
	 * Trigger comparator center point voltage
	 * units: V
	 * offset 2512
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	 * units: V
	 * offset 2513
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	 * units: V
	 * offset 2514
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	 * units: RPM
	 * offset 2515
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * units: ratio
	 * offset 2516
	 */
	scaled_channel<uint16_t, 100, 1> tractionControlSlipBins[TRACTION_CONTROL_ETB_DROP_SLIP_SIZE] = {};
	/**
	 * units: RPM
	 * offset 2528
	 */
	uint8_t tractionControlSpeedBins[TRACTION_CONTROL_ETB_DROP_SPEED_SIZE] = {};
	/**
	 * offset 2534
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2535
	 */
	uint8_t alignmentFill_at_2535[1] = {};
	/**
	 * offset 2536
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS] = {};
	/**
	 * Boost Current
	 * units: mA
	 * offset 2968
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	 * units: mA
	 * offset 2970
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	 * units: mA
	 * offset 2972
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	 * units: us
	 * offset 2974
	 */
	uint16_t mc33_t_max_boost;
	/**
	 * units: us
	 * offset 2976
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	 * units: us
	 * offset 2978
	 */
	uint16_t mc33_t_peak_tot;
	/**
	 * units: us
	 * offset 2980
	 */
	uint16_t mc33_t_bypass;
	/**
	 * units: us
	 * offset 2982
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	 * units: us
	 * offset 2984
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 2986
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 2987
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 2988
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 2989
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2995
	 */
	uint8_t alignmentFill_at_2995[1] = {};
	/**
	 * units: ratio
	 * offset 2996
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3068
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH] = {};
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	 * units: rpm
	 * offset 3140
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	 * units: sec
	 * offset 3141
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	 * units: sec
	 * offset 3142
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * offset 3143
	 */
	tChargeMode_e tChargeMode;
	/**
	 * units: mg
	 * offset 3144
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	 * Threshold in ETB error (target vs. actual) above which the jam timer is started. If the timer reaches the time specified in the jam detection timeout period, the throttle is considered jammed, and engine operation limited.
	 * units: %
	 * offset 3146
	 */
	uint8_t etbJamDetectThreshold;
	/**
	 * units: lobes/cam
	 * offset 3147
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 3148
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Low engine speed for A/C. Larger engines can survive lower values
	 * units: RPM
	 * offset 3149
	 */
	scaled_channel<int8_t, 1, 10> acLowRpmLimit;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	 * units: deg
	 * offset 3150
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3151
	 */
	uint8_t alignmentFill_at_3151[1] = {};
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	 * units: cc
	 * offset 3152
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	 * https://rusefi.com/forum/viewtopic.php?t=2192
	 * units: deg
	 * offset 3154
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 3155
	 */
	uint8_t issFilterReciprocal;
	/**
	 * units: %/kPa
	 * offset 3156
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	 * units: %/kPa/lobe
	 * offset 3158
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * iTerm min value
	 * offset 3160
	 */
	int16_t hpfpPid_iTermMin;
	/**
	 * iTerm max value
	 * offset 3162
	 */
	int16_t hpfpPid_iTermMax;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	 * units: kPa/s
	 * offset 3164
	 */
	uint16_t hpfpTargetDecay;
	/**
	 * offset 3166
	 */
	output_pin_e stepper_raw_output[4] = {};
	/**
	 * units: ratio
	 * offset 3174
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[TCU_GEAR_COUNT] = {};
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	 * units: ms
	 * offset 3194
	 */
	uint16_t vvtActivationDelayMs;
	/**
	 * offset 3196
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3197
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * During revolution where ACR should be disabled at what specific angle to disengage
	 * units: deg
	 * offset 3198
	 */
	uint16_t acrDisablePhase;
	/**
	 * offset 3200
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3220
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 3240
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 3242
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3243
	 */
	uint8_t alignmentFill_at_3243[1] = {};
	/**
	 * offset 3244
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 3246
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3247
	 */
	uint8_t alignmentFill_at_3247[1] = {};
	/**
	 * offset 3248
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 3250
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 3252
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3253
	 */
	uint8_t alignmentFill_at_3253[1] = {};
	/**
	 * offset 3254
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 3256
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 3258
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3259
	 */
	uint8_t alignmentFill_at_3259[1] = {};
	/**
	 * offset 3260
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * offset 3262
	 */
	output_pin_e acrPin2;
	/**
	 * Set a minimum allowed target position to avoid slamming/driving against the hard mechanical stop in the throttle.
	 * units: %
	 * offset 3264
	 */
	scaled_channel<uint8_t, 10, 1> etbMinimumPosition;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3265
	 */
	uint8_t alignmentFill_at_3265[1] = {};
	/**
	 * offset 3266
	 */
	uint16_t tuneHidingKey;
	/**
	 * Individual characters are accessible using vin(index) Lua function
	 * offset 3268
	 */
	vin_number_t vinNumber;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 3285
	 */
	int8_t torqueReductionActivationTemperature;
	/**
	 * offset 3286
	 */
	fuel_pressure_sensor_mode_e fuelPressureSensorMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3287
	 */
	uint8_t alignmentFill_at_3287[1] = {};
	/**
	 * offset 3288
	 */
	switch_input_pin_e luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * units: rpm
	 * offset 3304
	 */
	int16_t ALSMinRPM;
	/**
	 * units: rpm
	 * offset 3306
	 */
	int16_t ALSMaxRPM;
	/**
	 * units: sec
	 * offset 3308
	 */
	int16_t ALSMaxDuration;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 3310
	 */
	int8_t ALSMinCLT;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 3311
	 */
	int8_t ALSMaxCLT;
	/**
	 * offset 3312
	 */
	uint8_t alsMinTimeBetween;
	/**
	 * offset 3313
	 */
	uint8_t alsEtbPosition;
	/**
	 * units: %
	 * offset 3314
	 */
	uint8_t acRelayAlternatorDutyAdder;
	/**
	 * If you have SENT TPS sensor please select type. For analog TPS leave None
	 * offset 3315
	 */
	SentEtbType sentEtbType;
	/**
	 * offset 3316
	 */
	uint16_t customSentTpsMin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3318
	 */
	uint8_t alignmentFill_at_3318[2] = {};
	/**
	 * units: %
	 * offset 3320
	 */
	int ALSIdleAdd;
	/**
	 * units: %
	 * offset 3324
	 */
	int ALSEtbAdd;
	/**
	 * offset 3328
	 */
	float ALSSkipRatio;
	/**
	 * Hysterisis: if Pressure High Disable is 240kpa, and acPressureEnableHyst is 20, when the ECU sees 240kpa, A/C will be disabled, and stay disabled until 240-20=220kpa is reached
	 * units: kPa (absolute)
	 * offset 3332
	 */
	scaled_channel<uint8_t, 2, 1> acPressureEnableHyst;
	/**
	 * offset 3333
	 */
	pin_input_mode_e ALSActivatePinMode;
	/**
	 * For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	 * units: %
	 * offset 3334
	 */
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	/**
	 * For Toyota ETCS-i, use ~69%
	 * units: %
	 * offset 3335
	 */
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	/**
	 * offset 3336
	 */
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * offset 3344
	 */
	uint16_t customSentTpsMax;
	/**
	 * offset 3346
	 */
	uint16_t kLineBaudRate;
	/**
	 * offset 3348
	 */
	CanGpioType canGpioType;
	/**
	 * offset 3349
	 */
	UiMode uiMode;
	/**
	 * Crank angle ATDC of first lobe peak
	 * units: deg
	 * offset 3350
	 */
	int16_t hpfpPeakPos;
	/**
	 * units: us
	 * offset 3352
	 */
	int16_t kLinePeriodUs;
	/**
	 * Window that the correction will be added throughout (example, if rpm limit is 7000, and rpmSoftLimitWindowSize is 200, the corrections activate at 6800RPM, creating a 200rpm window)
	 * units: RPM
	 * offset 3354
	 */
	scaled_channel<uint8_t, 1, 10> rpmSoftLimitWindowSize;
	/**
	 * Degrees of timing REMOVED from actual timing during soft RPM limit window
	 * units: deg
	 * offset 3355
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitTimingRetard;
	/**
	 * % of fuel ADDED during window
	 * units: %
	 * offset 3356
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitFuelAdded;
	/**
	 * Sets a buffer below the RPM hard limit, helping avoid rapid cycling of cut actions by defining a range within which RPM must drop before cut actions are re-enabled.
	 * Hysterisis: if the hard limit is 7200rpm and rpmHardLimitHyst is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached
	 * units: RPM
	 * offset 3357
	 */
	scaled_channel<uint8_t, 1, 10> rpmHardLimitHyst;
	/**
	 * Time between bench test pulses
	 * units: ms
	 * offset 3358
	 */
	scaled_channel<uint16_t, 10, 1> benchTestOffTime;
	/**
	 * Hysterisis: if hard cut is 240kpa, and boostCutPressureHyst is 20, when the ECU sees 240kpa, fuel/ign will cut, and stay cut until 240-20=220kpa is reached
	 * units: kPa (absolute)
	 * offset 3360
	 */
	scaled_channel<uint8_t, 2, 1> boostCutPressureHyst;
	/**
	 * Boost duty cycle modified by gear
	 * units: %
	 * offset 3361
	 */
	scaled_channel<int8_t, 2, 1> gearBasedOpenLoopBoostAdder[TCU_GEAR_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3371
	 */
	uint8_t alignmentFill_at_3371[1] = {};
	/**
	 * How many test bench pulses do you want
	 * offset 3372
	 */
	uint32_t benchTestCount;
	/**
	 * How long initial idle adder is held before starting to decay.
	 * units: seconds
	 * offset 3376
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsHoldTime;
	/**
	 * How long it takes to remove initial IAC adder to return to normal idle.
	 * units: seconds
	 * offset 3377
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsDecayTime;
	/**
	 * offset 3378
	 */
	switch_input_pin_e tcu_rangeInput[RANGE_INPUT_COUNT] = {};
	/**
	 * offset 3390
	 */
	pin_input_mode_e tcu_rangeInputMode[RANGE_INPUT_COUNT] = {};
	/**
	 * Scale the reported vehicle speed value from CAN. Example: Parameter set to 1.1, CAN VSS reports 50kph, ECU will report 55kph instead.
	 * units: ratio
	 * offset 3396
	 */
	scaled_channel<uint16_t, 10000, 1> canVssScaling;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3398
	 */
	uint8_t alignmentFill_at_3398[2] = {};
	/**
	 * offset 3400
	 */
	ThermistorConf oilTempSensor;
	/**
	 * offset 3432
	 */
	ThermistorConf fuelTempSensor;
	/**
	 * offset 3464
	 */
	ThermistorConf ambientTempSensor;
	/**
	 * offset 3496
	 */
	ThermistorConf compressorDischargeTemperature;
	/**
	 * Place the sensor before the throttle, but after any turbocharger/supercharger and intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3528
	 */
	adc_channel_e throttleInletPressureChannel;
	/**
	 * Place the sensor after the turbocharger/supercharger, but before any intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3529
	 */
	adc_channel_e compressorDischargePressureChannel;
	/**
	 * offset 3530
	 */
	Gpio dacOutputPins[DAC_OUTPUT_COUNT] = {};
	/**
	 * offset 3534
	 */
	output_pin_e speedometerOutputPin;
	/**
	 * Number of speedometer pulses per kilometer travelled.
	 * offset 3536
	 */
	uint16_t speedometerPulsePerKm;
	/**
	 * offset 3538
	 */
	uint8_t simulatorCamPosition[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 3542
	 */
	adc_channel_e ignKeyAdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3543
	 */
	uint8_t alignmentFill_at_3543[1] = {};
	/**
	 * offset 3544
	 */
	float ignKeyAdcDivider;
	/**
	 * offset 3548
	 */
	pin_mode_e spi6MisoMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3549
	 */
	uint8_t alignmentFill_at_3549[3] = {};
	/**
	 * units: ratio
	 * offset 3552
	 */
	float triggerVVTGapOverrideFrom[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3568
	 */
	float triggerVVTGapOverrideTo[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: %
	 * offset 3584
	 */
	int8_t tractionControlEtbDrop[TRACTION_CONTROL_ETB_DROP_SLIP_SIZE][TRACTION_CONTROL_ETB_DROP_SPEED_SIZE] = {};
	/**
	 * This sets an immediate limit on injector duty cycle. If this threshold is reached, the system will immediately cut the injectors.
	 * units: %
	 * offset 3620
	 */
	uint8_t maxInjectorDutyInstant;
	/**
	 * This limit allows injectors to operate up to the specified duty cycle percentage for a short period (as defined by the delay). After this delay, if the duty cycle remains above the limit, it will trigger a cut.
	 * units: %
	 * offset 3621
	 */
	uint8_t maxInjectorDutySustained;
	/**
	 * Timeout period for duty cycle over the sustained limit to trigger duty cycle protection.
	 * units: sec
	 * offset 3622
	 */
	scaled_channel<uint8_t, 10, 1> maxInjectorDutySustainedTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3623
	 */
	uint8_t alignmentFill_at_3623[1] = {};
	/**
	 * offset 3624
	 */
	output_pin_e injectionPinsStage2[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: Deg
	 * offset 3648
	 */
	int8_t tractionControlTimingDrop[TRACTION_CONTROL_ETB_DROP_SLIP_SIZE][TRACTION_CONTROL_ETB_DROP_SPEED_SIZE] = {};
	/**
	 * units: %
	 * offset 3684
	 */
	int8_t tractionControlIgnitionSkip[TRACTION_CONTROL_ETB_DROP_SLIP_SIZE][TRACTION_CONTROL_ETB_DROP_SPEED_SIZE] = {};
	/**
	 * offset 3720
	 */
	float auxSpeed1Multiplier;
	/**
	 * offset 3724
	 */
	float brakeMeanEffectivePressureDifferential;
	/**
	 * offset 3728
	 */
	Gpio spi4mosiPin;
	/**
	 * offset 3730
	 */
	Gpio spi4misoPin;
	/**
	 * offset 3732
	 */
	Gpio spi4sckPin;
	/**
	 * offset 3734
	 */
	Gpio spi5mosiPin;
	/**
	 * offset 3736
	 */
	Gpio spi5misoPin;
	/**
	 * offset 3738
	 */
	Gpio spi5sckPin;
	/**
	 * offset 3740
	 */
	Gpio spi6mosiPin;
	/**
	 * offset 3742
	 */
	Gpio spi6misoPin;
	/**
	 * offset 3744
	 */
	Gpio spi6sckPin;
	/**
	 * offset 3746
	 */
	pin_mode_e spi4SckMode;
	/**
	 * offset 3747
	 */
	pin_mode_e spi4MosiMode;
	/**
	 * offset 3748
	 */
	pin_mode_e spi4MisoMode;
	/**
	 * offset 3749
	 */
	pin_mode_e spi5SckMode;
	/**
	 * offset 3750
	 */
	pin_mode_e spi5MosiMode;
	/**
	 * offset 3751
	 */
	pin_mode_e spi5MisoMode;
	/**
	 * offset 3752
	 */
	pin_mode_e spi6SckMode;
	/**
	 * offset 3753
	 */
	pin_mode_e spi6MosiMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3754
	 */
	uint8_t alignmentFill_at_3754[2] = {};
	/**
	 * Secondary TTL channel baud rate
	 * units: BPs
	 * offset 3756
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 3760
	 */
	Gpio camSimulatorPin;
	/**
	 * offset 3762
	 */
	pin_output_mode_e camSimulatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3763
	 */
	uint8_t alignmentFill_at_3763[1] = {};
	/**
	 * offset 3764
	 */
	int anotherCiTest;
	/**
	 * offset 3768
	 */
	uint32_t device_uid[3] = {};
	/**
	 * offset 3780
	 */
	adc_channel_e tcu_rangeAnalogInput[RANGE_INPUT_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3786
	 */
	uint8_t alignmentFill_at_3786[2] = {};
	/**
	 * units: Ohm
	 * offset 3788
	 */
	float tcu_rangeSensorBiasResistor;
	/**
	 * offset 3792
	 */
	MsIoBox_config_s msIoBox0;
	/**
	 * Nominal coil charge current, 0.25A step
	 * units: A
	 * offset 3796
	 */
	scaled_channel<uint8_t, 4, 1> mc33810Nomi;
	/**
	 * Maximum coil charge current, 1A step
	 * units: A
	 * offset 3797
	 */
	uint8_t mc33810Maxi;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3798
	 */
	uint8_t alignmentFill_at_3798[2] = {};
	/**
	 * offset 3800
	 */
	linear_sensor_s acPressure;
	/**
	 * value of A/C pressure in kPa/psi before that compressor is disengaged
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3820
	 */
	uint16_t minAcPressure;
	/**
	 * value of A/C pressure in kPa/psi after that compressor is disengaged
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3822
	 */
	uint16_t maxAcPressure;
	/**
	 * Delay before cutting fuel due to low oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 3824
	 */
	scaled_channel<uint8_t, 10, 1> minimumOilPressureTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3825
	 */
	uint8_t alignmentFill_at_3825[3] = {};
	/**
	 * offset 3828
	 */
	linear_sensor_s auxLinear3;
	/**
	 * offset 3848
	 */
	linear_sensor_s auxLinear4;
	/**
	 * Below TPS value all knock suppression will be disabled.
	 * units: %
	 * offset 3868
	 */
	scaled_channel<uint8_t, 1, 1> knockSuppressMinTps;
	/**
	 * Fuel to odd when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 3869
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimAggression;
	/**
	 * After a knock event, reapply fuel at this rate.
	 * units: 1%/s
	 * offset 3870
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimReapplyRate;
	/**
	 * Fuel trim when knock, max 30%
	 * units: %
	 * offset 3871
	 */
	scaled_channel<uint8_t, 1, 1> knockFuelTrim;
	/**
	 * units: sense
	 * offset 3872
	 */
	float knockSpectrumSensitivity;
	/**
	 * "Estimated knock frequency, ignore cylinderBore if this one > 0"
	 * units: Hz
	 * offset 3876
	 */
	float knockFrequency;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 3880
	 */
	injector_compensation_mode_e secondaryInjectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3881
	 */
	uint8_t alignmentFill_at_3881[3] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	 * units: kPa
	 * offset 3884
	 */
	float secondaryInjectorFuelReferencePressure;
	/**
	 * SENT input connected to ETB
	 * offset 3888
	 */
	SentInput EtbSentInput;
	/**
	 * SENT input used for high pressure fuel sensor
	 * offset 3889
	 */
	SentInput FuelHighPressureSentInput;
	/**
	 * If you have SENT High Pressure Fuel Sensor please select type. For analog TPS leave None
	 * offset 3890
	 */
	SentFuelHighPressureType FuelHighPressureSentType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3891
	 */
	uint8_t alignmentFill_at_3891[1] = {};
	/**
	offset 3892 bit 0 */
	bool nitrousControlEnabled : 1 {};
	/**
	offset 3892 bit 1 */
	bool unusedFancy2 : 1 {};
	/**
	offset 3892 bit 2 */
	bool unusedFancy3 : 1 {};
	/**
	offset 3892 bit 3 */
	bool unusedFancy4 : 1 {};
	/**
	offset 3892 bit 4 */
	bool unusedFancy5 : 1 {};
	/**
	offset 3892 bit 5 */
	bool unusedFancy6 : 1 {};
	/**
	offset 3892 bit 6 */
	bool unusedFancy7 : 1 {};
	/**
	offset 3892 bit 7 */
	bool unusedFancy8 : 1 {};
	/**
	offset 3892 bit 8 */
	bool unusedFancy9 : 1 {};
	/**
	offset 3892 bit 9 */
	bool unusedFancy10 : 1 {};
	/**
	offset 3892 bit 10 */
	bool unusedFancy11 : 1 {};
	/**
	offset 3892 bit 11 */
	bool unusedFancy12 : 1 {};
	/**
	offset 3892 bit 12 */
	bool unusedFancy13 : 1 {};
	/**
	offset 3892 bit 13 */
	bool unusedFancy14 : 1 {};
	/**
	offset 3892 bit 14 */
	bool unusedFancy15 : 1 {};
	/**
	offset 3892 bit 15 */
	bool unusedFancy16 : 1 {};
	/**
	offset 3892 bit 16 */
	bool unusedFancy17 : 1 {};
	/**
	offset 3892 bit 17 */
	bool unusedFancy18 : 1 {};
	/**
	offset 3892 bit 18 */
	bool unusedFancy19 : 1 {};
	/**
	offset 3892 bit 19 */
	bool unusedFancy20 : 1 {};
	/**
	offset 3892 bit 20 */
	bool unusedFancy21 : 1 {};
	/**
	offset 3892 bit 21 */
	bool unusedFancy22 : 1 {};
	/**
	offset 3892 bit 22 */
	bool unusedFancy23 : 1 {};
	/**
	offset 3892 bit 23 */
	bool unusedFancy24 : 1 {};
	/**
	offset 3892 bit 24 */
	bool unusedFancy25 : 1 {};
	/**
	offset 3892 bit 25 */
	bool unusedFancy26 : 1 {};
	/**
	offset 3892 bit 26 */
	bool unusedFancy27 : 1 {};
	/**
	offset 3892 bit 27 */
	bool unusedFancy28 : 1 {};
	/**
	offset 3892 bit 28 */
	bool unusedFancy29 : 1 {};
	/**
	offset 3892 bit 29 */
	bool unusedFancy30 : 1 {};
	/**
	offset 3892 bit 30 */
	bool unusedFancy31 : 1 {};
	/**
	offset 3892 bit 31 */
	bool unusedFancy32 : 1 {};
	/**
	 * offset 3896
	 */
	nitrous_arming_method_e nitrousControlArmingMethod;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3897
	 */
	uint8_t alignmentFill_at_3897[1] = {};
	/**
	 * Pin that activates nitrous control
	 * offset 3898
	 */
	switch_input_pin_e nitrousControlTriggerPin;
	/**
	 * offset 3900
	 */
	pin_input_mode_e nitrousControlTriggerPinMode;
	/**
	 * offset 3901
	 */
	lua_gauge_e nitrousLuaGauge;
	/**
	 * offset 3902
	 */
	lua_gauge_meaning_e nitrousLuaGaugeMeaning;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3903
	 */
	uint8_t alignmentFill_at_3903[1] = {};
	/**
	 * offset 3904
	 */
	float nitrousLuaGaugeArmingValue;
	/**
	 * offset 3908
	 */
	int nitrousMinimumTps;
	/**
	 * units: deg C
	 * offset 3912
	 */
	uint8_t nitrousMinimumClt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3913
	 */
	uint8_t alignmentFill_at_3913[1] = {};
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 3914
	 */
	int16_t nitrousMaximumMap;
	/**
	 * units: afr
	 * offset 3916
	 */
	scaled_channel<uint8_t, 10, 1> nitrousMaximumAfr;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3917
	 */
	uint8_t alignmentFill_at_3917[1] = {};
	/**
	 * units: rpm
	 * offset 3918
	 */
	uint16_t nitrousActivationRpm;
	/**
	 * units: rpm
	 * offset 3920
	 */
	uint16_t nitrousDeactivationRpm;
	/**
	 * units: rpm
	 * offset 3922
	 */
	uint16_t nitrousDeactivationRpmWindow;
	/**
	 * Retard timing by this amount during DFCO. Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: deg
	 * offset 3924
	 */
	uint8_t dfcoRetardDeg;
	/**
	 * Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: s
	 * offset 3925
	 */
	scaled_channel<uint8_t, 10, 1> dfcoRetardRampInTime;
	/**
	 * offset 3926
	 */
	output_pin_e nitrousRelayPin;
	/**
	 * offset 3928
	 */
	pin_output_mode_e nitrousRelayPinMode;
	/**
	 * units: %
	 * offset 3929
	 */
	int8_t nitrousFuelAdderPercent;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3930
	 */
	uint8_t alignmentFill_at_3930[2] = {};
	/**
	 * Retard timing to remove from actual final timing (after all corrections) due to additional air.
	 * units: deg
	 * offset 3932
	 */
	float nitrousIgnitionRetard;
	/**
	 * units: Kph
	 * offset 3936
	 */
	uint16_t nitrousMinimumVehicleSpeed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3938
	 */
	uint8_t alignmentFill_at_3938[2] = {};
	/**
	 * Exponential Average Alpha filtering parameter
	 * offset 3940
	 */
	float fuelLevelAveragingAlpha;
	/**
	 * How often do we update fuel level gauge
	 * units: seconds
	 * offset 3944
	 */
	float fuelLevelUpdatePeriodSec;
	/**
	 * Error below specified value
	 * units: v
	 * offset 3948
	 */
	float fuelLevelLowThresholdVoltage;
	/**
	 * Error above specified value
	 * units: v
	 * offset 3952
	 */
	float fuelLevelHighThresholdVoltage;
	/**
	 * A higher alpha (closer to 1) means the EMA reacts more quickly to changes in the data.
	 * '1' means no filtering, 0.98 would be some filtering.
	 * offset 3956
	 */
	float afrExpAverageAlpha;
	/**
	 * Compensates for trigger delay due to belt stretch, or other electromechanical issues. beware that raising this value advances ignition timing!
	 * units: uS
	 * offset 3960
	 */
	scaled_channel<uint8_t, 1, 1> sparkHardwareLatencyCorrection;
	/**
	 * Delay before cutting fuel due to extra high oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 3961
	 */
	scaled_channel<uint8_t, 10, 1> maxOilPressureTimeout;
	/**
	 * units: kg/h
	 * offset 3962
	 */
	scaled_channel<uint16_t, 100, 1> idleFlowEstimateFlow[8] = {};
	/**
	 * units: %
	 * offset 3978
	 */
	scaled_channel<uint8_t, 2, 1> idleFlowEstimatePosition[8] = {};
	/**
	 * units: mg
	 * offset 3986
	 */
	int8_t airmassToTimingBins[8] = {};
	/**
	 * units: deg
	 * offset 3994
	 */
	int8_t airmassToTimingValues[8] = {};
	/**
	 * idle return target ramp duration
	 * units: seconds
	 * offset 4002
	 */
	scaled_channel<uint8_t, 10, 1> idleReturnTargetRampDuration;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4003
	 */
	uint8_t alignmentFill_at_4003[1] = {};
	/**
	 * Voltage when the wastegate is fully open
	 * units: v
	 * offset 4004
	 */
	float wastegatePositionOpenedVoltage;
	/**
	 * Voltage when the wastegate is closed
	 * units: v
	 * offset 4008
	 */
	float wastegatePositionClosedVoltage;
	/**
	 * offset 4012
	 */
	wbo_s canWbo[CAN_WBO_COUNT] = {};
};
static_assert(sizeof(engine_configuration_s) == 4028);

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
	 * offset 4028
	 */
	float tmfTable[TMF_RATIO_SIZE][TMF_SIZE] = {};
	/**
	 * offset 4044
	 */
	float tmfRatioBins[TMF_RATIO_SIZE] = {};
	/**
	 * offset 4052
	 */
	float tmfOpeningBins[TMF_SIZE] = {};
	/**
	 * units: Nm
	 * offset 4060
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_RPM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 4096
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_RPM_SIZE] = {};
	/**
	 * units: Load
	 * offset 4108
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE] = {};
	/**
	 * units: mult
	 * offset 4120
	 */
	float postCrankingFactor[CRANKING_ENRICH_CLT_COUNT][CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: count
	 * offset 4264
	 */
	uint16_t postCrankingDurationBins[CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 4276
	 */
	int16_t postCrankingCLTBins[CRANKING_ENRICH_CLT_COUNT] = {};
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	 * units: target TPS position
	 * offset 4288
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: ETB duty cycle bias
	 * offset 4320
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * target Wastegate value, 0 to 100%
	 * units: target DC position
	 * offset 4352
	 */
	int8_t dcWastegateBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: DC wastegate duty cycle bias
	 * offset 4360
	 */
	scaled_channel<int16_t, 100, 1> dcWastegateBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * units: %
	 * offset 4376
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE] = {};
	/**
	 * units: Load
	 * offset 4440
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE] = {};
	/**
	 * units: RPM
	 * offset 4448
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_RPM_SIZE] = {};
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	 * units: RPM
	 * offset 4456
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: ms
	 * offset 4472
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE] = {};
	/**
	 * CLT-based target RPM for automatic idle controller
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 4488
	 */
	scaled_channel<int16_t, 1, 1> cltIdleRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * See idleRpmPid
	 * units: RPM
	 * offset 4520
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE] = {};
	/**
	 * units: deg
	 * offset 4536
	 */
	scaled_channel<int16_t, 10, 1> ignitionCltCorrTable[CLT_TIMING_CURVE_SIZE][CLT_TIMING_CURVE_SIZE] = {};
	/**
	 * CLT-based timing correction
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 4586
	 */
	scaled_channel<int16_t, 1, 1> ignitionCltCorrTempBins[CLT_TIMING_CURVE_SIZE] = {};
	/**
	 * units: Load
	 * offset 4596
	 */
	scaled_channel<uint8_t, 1, 5> ignitionCltCorrLoadBins[CLT_TIMING_CURVE_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4601
	 */
	uint8_t alignmentFill_at_4601[3] = {};
	/**
	 * units: x
	 * offset 4604
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 4668
	 */
	float scriptCurve1[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 4732
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 4796
	 */
	float scriptCurve2[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 4860
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 4892
	 */
	float scriptCurve3[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 4924
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 4956
	 */
	float scriptCurve4[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 4988
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5020
	 */
	float scriptCurve5[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 5052
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5084
	 */
	float scriptCurve6[SCRIPT_CURVE_8] = {};
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 5116
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5132
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE] = {};
	/**
	 * units: ratio
	 * offset 5148
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE] = {};
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * units: Ratio
	 * offset 5212
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 5244
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: RPM
	 * offset 5276
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: deg
	 * offset 5284
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: RPM
	 * offset 5292
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: %
	 * offset 5308
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE] = {};
	/**
	 * offset 5324
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * units: RPM
	 * offset 5388
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT] = {};
	/**
	 * offset 5396
	 */
	uint16_t boostOpenLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * offset 5412
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * offset 5476
	 */
	uint16_t boostClosedLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * units: %
	 * offset 5492
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_RPM_SIZE] = {};
	/**
	 * units: %
	 * offset 5556
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5564
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_RPM_SIZE] = {};
	/**
	 * CLT-based cranking position % for simple manual idle controller
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 5572
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based cranking position % for simple manual idle controller
	 * units: percent
	 * offset 5604
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 5636
	 */
	float afterCrankingIACtaperDurationBins[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	 * Should be 100 once tune is better
	 * units: cycles
	 * offset 5660
	 */
	uint16_t afterCrankingIACtaperDuration[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: RPM
	 * offset 5672
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: deg
	 * offset 5680
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5712
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE] = {};
	/**
	 * units: load
	 * offset 5716
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE] = {};
	/**
	 * units: %
	 * offset 5720
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE] = {};
	/**
	 * offset 5752
	 */
	lua_script_t luaScript;
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 45752
	 */
	float cltFuelCorrBins[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 45816
	 */
	float cltFuelCorr[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 45880
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 45944
	 */
	float iatFuelCorr[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 46008
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 46040
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: counter
	 * offset 46072
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 46104
	 */
	int16_t crankingCycleFuelCltBins[CRANKING_CYCLE_CLT_SIZE] = {};
	/**
	 * Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for IAT, TPS ect, to give the final cranking pulse width.
	 * A reasonable starting point is 60mg per liter per cylinder.
	 * ex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.
	 * units: mg
	 * offset 46112
	 */
	float crankingCycleBaseFuel[CRANKING_CYCLE_CLT_SIZE][CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 46240
	 */
	float cltIdleCorrBins[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: %
	 * offset 46264
	 */
	float cltIdleCorrTable[CLT_IDLE_TABLE_RPM_SIZE][CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * units: Target RPM
	 * offset 46360
	 */
	scaled_channel<uint8_t, 1, 100> rpmIdleCorrBins[CLT_IDLE_TABLE_RPM_SIZE] = {};
	/**
	 * Long Term Idle Trim (LTIT) multiplicativo para idle open loop
	 * units: %
	 * offset 46364
	 */
	scaled_channel<uint16_t, 10, 1> ltitTable[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	 * units: kg/hour
	 * offset 46376
	 */
	float mafDecoding[MAF_DECODING_COUNT] = {};
	/**
	 * units: V
	 * offset 46504
	 */
	float mafDecodingBins[MAF_DECODING_COUNT] = {};
	/**
	 * units: deg
	 * offset 46632
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IAT_IGN_CORR_LOAD_COUNT][IAT_IGN_CORR_COUNT] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 46760
	 */
	int8_t ignitionIatCorrTempBins[IAT_IGN_CORR_COUNT] = {};
	/**
	 * units: Load
	 * offset 46768
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[IAT_IGN_CORR_LOAD_COUNT] = {};
	/**
	 * units: deg
	 * offset 46776
	 */
	int16_t injectionPhase[INJ_PHASE_LOAD_COUNT][INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: Load
	 * offset 47288
	 */
	uint16_t injPhaseLoadBins[INJ_PHASE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 47320
	 */
	uint16_t injPhaseRpmBins[INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: onoff
	 * offset 47352
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT] = {};
	/**
	 * This table represents MAP at a given TPS vs RPM, which we use if our MAP sensor has failed, or if we are using MAP Prediciton. 
	 *  This table should be a direct representation of MAP, you can tune it manually by disconnecting MAP sensor, and filling out the table with values that match an external gauge that shows MAP.
	 * Additionally, you can also use MLV to get the map values and/or generate the table for you
	 * units: kPa
	 * offset 47412
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[MAP_EST_LOAD_COUNT][MAP_EST_RPM_COUNT] = {};
	/**
	 * units: % TPS
	 * offset 47924
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[MAP_EST_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 47956
	 */
	uint16_t mapEstimateRpmBins[MAP_EST_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 47988
	 */
	int8_t vvtTable1[VVT_TABLE_SIZE][VVT_TABLE_SIZE] = {};
	/**
	 * units: L
	 * offset 48052
	 */
	uint16_t vvtTable1LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 48068
	 */
	uint16_t vvtTable1RpmBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: value
	 * offset 48084
	 */
	int8_t vvtTable2[VVT_TABLE_SIZE][VVT_TABLE_SIZE] = {};
	/**
	 * units: L
	 * offset 48148
	 */
	uint16_t vvtTable2LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 48164
	 */
	uint16_t vvtTable2RpmBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: deg
	 * offset 48180
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT] = {};
	/**
	 * units: Load
	 * offset 48692
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 48724
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT] = {};
	/**
	 * units: %
	 * offset 48756
	 */
	scaled_channel<uint16_t, 10, 1> veTable[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(fuelUnits, fuelAlgorithm) }
	 * offset 49268
	 */
	uint16_t veLoadBins[VE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 49300
	 */
	uint16_t veRpmBins[VE_RPM_COUNT] = {};
	/**
	 * units: lambda
	 * offset 49332
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT] = {};
	/**
	 * offset 49588
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 49620
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 49652
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: from
	 * offset 49908
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: to
	 * offset 49940
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: value
	 * offset 49972
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8] = {};
	/**
	 * units: L
	 * offset 50228
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 50244
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: value
	 * offset 50260
	 */
	float scriptTable2[TABLE_2_LOAD_SIZE][TABLE_2_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 50516
	 */
	int16_t scriptTable2LoadBins[TABLE_2_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 50532
	 */
	int16_t scriptTable2RpmBins[TABLE_2_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 50548
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8] = {};
	/**
	 * units: L
	 * offset 50612
	 */
	int16_t scriptTable3LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 50628
	 */
	int16_t scriptTable3RpmBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: value
	 * offset 50644
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][TABLE_4_RPM] = {};
	/**
	 * units: L
	 * offset 50724
	 */
	int16_t scriptTable4LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 50740
	 */
	int16_t scriptTable4RpmBins[TABLE_4_RPM] = {};
	/**
	 * offset 50760
	 */
	uint16_t ignTrimLoadBins[IGN_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 50768
	 */
	uint16_t ignTrimRpmBins[IGN_TRIM_SIZE] = {};
	/**
	 * offset 50776
	 */
	ign_cyl_trim_s ignTrims[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 50968
	 */
	uint16_t fuelTrimLoadBins[FUEL_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 50976
	 */
	uint16_t fuelTrimRpmBins[FUEL_TRIM_SIZE] = {};
	/**
	 * offset 50984
	 */
	fuel_cyl_trim_s fuelTrims[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: ratio
	 * offset 51176
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: Airmass
	 * offset 51192
	 */
	scaled_channel<uint8_t, 1, 5> tcu_pcAirmassBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51200
	 */
	uint8_t tcu_pcValsR[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51208
	 */
	uint8_t tcu_pcValsN[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51216
	 */
	uint8_t tcu_pcVals1[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51224
	 */
	uint8_t tcu_pcVals2[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51232
	 */
	uint8_t tcu_pcVals3[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51240
	 */
	uint8_t tcu_pcVals4[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51248
	 */
	uint8_t tcu_pcVals12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51256
	 */
	uint8_t tcu_pcVals23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51264
	 */
	uint8_t tcu_pcVals34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51272
	 */
	uint8_t tcu_pcVals21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51280
	 */
	uint8_t tcu_pcVals32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 51288
	 */
	uint8_t tcu_pcVals43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: TPS
	 * offset 51296
	 */
	uint8_t tcu_tccTpsBins[8] = {};
	/**
	 * units: MPH
	 * offset 51304
	 */
	uint8_t tcu_tccLockSpeed[8] = {};
	/**
	 * units: MPH
	 * offset 51312
	 */
	uint8_t tcu_tccUnlockSpeed[8] = {};
	/**
	 * units: KPH
	 * offset 51320
	 */
	uint8_t tcu_32SpeedBins[8] = {};
	/**
	 * units: %
	 * offset 51328
	 */
	uint8_t tcu_32Vals[8] = {};
	/**
	 * units: %
	 * offset 51336
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[ETB2_TRIM_SIZE][ETB2_TRIM_SIZE] = {};
	/**
	 * units: %
	 * offset 51372
	 */
	uint8_t throttle2TrimTpsBins[ETB2_TRIM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51378
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[ETB2_TRIM_SIZE] = {};
	/**
	 * units: deg
	 * offset 51384
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[KNOCK_TABLE_SIZE][KNOCK_TABLE_SIZE] = {};
	/**
	 * units: %
	 * offset 51420
	 */
	uint8_t maxKnockRetardLoadBins[KNOCK_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51426
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[KNOCK_TABLE_SIZE] = {};
	/**
	 * units: deg
	 * offset 51432
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[ALS_SIZE][ALS_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51464
	 */
	uint16_t alsIgnRetardLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51472
	 */
	uint16_t alsIgnRetardrpmBins[ALS_SIZE] = {};
	/**
	 * units: percent
	 * offset 51480
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[ALS_SIZE][ALS_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51512
	 */
	uint16_t alsFuelAdjustmentLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51520
	 */
	uint16_t alsFuelAdjustmentrpmBins[ALS_SIZE] = {};
	/**
	 * units: ratio
	 * offset 51528
	 */
	scaled_channel<int16_t, 1, 10> ALSIgnSkipTable[ALS_SIZE][ALS_SIZE] = {};
	/**
	 * units: TPS
	 * offset 51560
	 */
	uint16_t alsIgnSkipLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 51568
	 */
	uint16_t alsIgnSkiprpmBins[ALS_SIZE] = {};
	/**
	 * offset 51576
	 */
	blend_table_s ignBlends[IGN_BLEND_COUNT] = {};
	/**
	 * offset 52328
	 */
	blend_table_s veBlends[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 53080
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaBins[THR_EST_SIZE] = {};
	/**
	 * In units of g/s normalized to choked flow conditions
	 * units: g/s
	 * offset 53104
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaValues[THR_EST_SIZE] = {};
	/**
	 * offset 53128
	 */
	blend_table_s boostOpenLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 53504
	 */
	blend_table_s boostClosedLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * units: level
	 * offset 53880
	 */
	float tcu_rangeP[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 53904
	 */
	float tcu_rangeR[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 53928
	 */
	float tcu_rangeN[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 53952
	 */
	float tcu_rangeD[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 53976
	 */
	float tcu_rangeM[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54000
	 */
	float tcu_rangeM3[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54024
	 */
	float tcu_rangeM2[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54048
	 */
	float tcu_rangeM1[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54072
	 */
	float tcu_rangePlus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54096
	 */
	float tcu_rangeMinus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 54120
	 */
	float tcu_rangeLow[RANGE_INPUT_COUNT] = {};
	/**
	 * units: lambda
	 * offset 54144
	 */
	scaled_channel<uint8_t, 100, 1> lambdaMaxDeviationTable[LAM_SIZE][LAM_SIZE] = {};
	/**
	 * offset 54160
	 */
	uint16_t lambdaMaxDeviationLoadBins[LAM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 54168
	 */
	uint16_t lambdaMaxDeviationRpmBins[LAM_SIZE] = {};
	/**
	 * units: %
	 * offset 54176
	 */
	uint8_t injectorStagingTable[INJ_STAGING_COUNT][INJ_STAGING_COUNT] = {};
	/**
	 * offset 54212
	 */
	uint16_t injectorStagingLoadBins[INJ_STAGING_COUNT] = {};
	/**
	 * units: RPM
	 * offset 54224
	 */
	uint16_t injectorStagingRpmBins[INJ_STAGING_COUNT] = {};
	/**
	 * units: deg C
	 * offset 54236
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 54244
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 54252
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: SPECIAL_CASE_PRESSURE
	 * offset 54260
	 */
	uint8_t wwMapBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 54268
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 54276
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: %
	 * offset 54284
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: deg
	 * offset 54300
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: volts
	 * offset 54316
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: ms
	 * offset 54324
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: kPa
	 * offset 54340
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE] = {};
	/**
	 * units: load
	 * offset 54540
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 54560
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: %
	 * offset 54570
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: cc/lobe
	 * offset 54670
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: RPM
	 * offset 54690
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: %
	 * offset 54700
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensation[HPFP_FUEL_MASS_COMPENSATION_SIZE][HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 54828
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensationFuelMass[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: bar
	 * offset 54844
	 */
	scaled_channel<uint16_t, 10, 1> hpfpFuelMassCompensationFuelPressure[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: ms
	 * offset 54860
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearization[FLOW_LINEARIZATION_PRESSURE_SIZE][FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 55372
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearizationFuelMassBins[FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: bar
	 * offset 55404
	 */
	scaled_channel<uint16_t, 10, 1> injectorFlowLinearizationPressureBins[FLOW_LINEARIZATION_PRESSURE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55436
	 */
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	 * units: dB
	 * offset 55468
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55484
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: multiplier
	 * offset 55488
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: SPECIAL_CASE_TEMPERATURE
	 * offset 55492
	 */
	scaled_channel<int16_t, 1, 1> cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55500
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: volt
	 * offset 55508
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: %
	 * offset 55524
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: volts
	 * offset 55532
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: multiplier
	 * offset 55540
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 55548
	 */
	scaled_channel<uint8_t, 1, 1> tcu_shiftTpsBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 55556
	 */
	uint8_t tcu_shiftSpeed12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 55564
	 */
	uint8_t tcu_shiftSpeed23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 55572
	 */
	uint8_t tcu_shiftSpeed34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 55580
	 */
	uint8_t tcu_shiftSpeed21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 55588
	 */
	uint8_t tcu_shiftSpeed32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 55596
	 */
	uint8_t tcu_shiftSpeed43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: ms
	 * offset 55604
	 */
	float tcu_shiftTime;
	/**
	 * units: volts
	 * offset 55608
	 */
	scaled_channel<int16_t, 10, 1> alternatorVoltageTargetTable[ALTERNATOR_VOLTAGE_TARGET_SIZE][ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: Load
	 * offset 55640
	 */
	uint16_t alternatorVoltageTargetLoadBins[ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55648
	 */
	uint16_t alternatorVoltageTargetRpmBins[ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: C
	 * offset 55656
	 */
	float cltBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 55676
	 */
	float cltBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 55696
	 */
	float iatBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 55716
	 */
	float iatBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 55736
	 */
	float cltBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 55756
	 */
	float cltBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 55776
	 */
	float iatBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 55796
	 */
	float iatBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 55816
	 */
	scaled_channel<uint8_t, 1, 100> minimumOilPressureBins[8] = {};
	/**
	 * units: kPa
	 * offset 55824
	 */
	scaled_channel<uint8_t, 1, 10> minimumOilPressureValues[8] = {};
	/**
	 * offset 55832
	 */
	blend_table_s targetAfrBlends[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * @@DYNO_RPM_STEP_TOOLTIP@@
	 * units: Rpm
	 * offset 56208
	 */
	scaled_channel<uint8_t, 1, 1> dynoRpmStep;
	/**
	 * @@DYNO_SAE_TEMPERATURE_C_TOOLTIP@@
	 * units: C
	 * offset 56209
	 */
	scaled_channel<int8_t, 1, 1> dynoSaeTemperatureC;
	/**
	 * @@DYNO_SAE_RELATIVE_HUMIDITY_TOOLTIP@@
	 * units: %
	 * offset 56210
	 */
	scaled_channel<uint8_t, 1, 1> dynoSaeRelativeHumidity;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56211
	 */
	uint8_t alignmentFill_at_56211[1] = {};
	/**
	 * @@DYNO_SAE_BARO_TOOLTIP@@
	 * units: KPa
	 * offset 56212
	 */
	scaled_channel<float, 1, 1> dynoSaeBaro;
	/**
	 * @@DYNO_CAR_WHEEL_DIA_INCH_TOOLTIP@@
	 * units: Inch
	 * offset 56216
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelDiaInch;
	/**
	 * @@DYNO_CAR_WHEEL_ASPECT_RATIO_TOOLTIP@@
	 * units: Aspect Ratio (height)
	 * offset 56217
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelAspectRatio;
	/**
	 * @@DYNO_CAR_WHEEL_TIRE_WIDTH_TOOLTIP@@
	 * units: Width mm
	 * offset 56218
	 */
	scaled_channel<int16_t, 1, 1> dynoCarWheelTireWidthMm;
	/**
	 * @@DYNO_CAR_GEAR_PRIMARY_REDUCTION_TOOLTIP@@
	 * units: Units
	 * offset 56220
	 */
	scaled_channel<float, 1, 1> dynoCarGearPrimaryReduction;
	/**
	 * @@DYNO_CAR_GEAR_RATIO_TOOLTIP@@
	 * units: Units
	 * offset 56224
	 */
	scaled_channel<float, 1, 1> dynoCarGearRatio;
	/**
	 * @@DYNO_CAR_GEAR_FINAL_DRIVE_TOOLTIP@@
	 * units: Units
	 * offset 56228
	 */
	scaled_channel<float, 1, 1> dynoCarGearFinalDrive;
	/**
	 * @@DYNO_CAR_CAR_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 56232
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCarMassKg;
	/**
	 * @@DYNO_CAR_CARGO_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 56234
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCargoMassKg;
	/**
	 * @@DYNO_CAR_COEFF_OF_DRAG_TOOLTIP@@
	 * units: Coeff
	 * offset 56236
	 */
	scaled_channel<float, 1, 1> dynoCarCoeffOfDrag;
	/**
	 * @@DYNO_CAR_FRONTAL_AREA_TOOLTIP@@
	 * units: m2
	 * offset 56240
	 */
	scaled_channel<float, 1, 1> dynoCarFrontalAreaM2;
	/**
	 * units: deg
	 * offset 56244
	 */
	scaled_channel<int8_t, 10, 1> trailingSparkTable[TRAILING_SPARK_SIZE][TRAILING_SPARK_SIZE] = {};
	/**
	 * units: rpm
	 * offset 56260
	 */
	scaled_channel<uint8_t, 1, 50> trailingSparkRpmBins[TRAILING_SPARK_SIZE] = {};
	/**
	 * units: Load
	 * offset 56264
	 */
	scaled_channel<uint8_t, 1, 5> trailingSparkLoadBins[TRAILING_SPARK_SIZE] = {};
	/**
	 * units: RPM
	 * offset 56268
	 */
	scaled_channel<uint8_t, 1, 100> maximumOilPressureBins[4] = {};
	/**
	 * units: kPa
	 * offset 56272
	 */
	scaled_channel<uint8_t, 1, 10> maximumOilPressureValues[4] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 56276
	 */
	gppwm_channel_e torqueReductionCutXaxis;
	/**
	 * How many % of ignition events will be cut
	 * units: %
	 * offset 56277
	 */
	int8_t torqueReductionIgnitionCutTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56289
	 */
	uint8_t alignmentFill_at_56289[1] = {};
	/**
	 * offset 56290
	 */
	int16_t torqueReductionCutXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 56302
	 */
	int8_t torqueReductionCutGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 56304
	 */
	gppwm_channel_e torqueReductionTimeXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56305
	 */
	uint8_t alignmentFill_at_56305[3] = {};
	/**
	 * For how long after the pin has been triggered will the cut/reduction stay active. After that, even if the pin is still triggered, torque is re-introduced
	 * units: ms
	 * offset 56308
	 */
	float torqueReductionTimeTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 56356
	 */
	int16_t torqueReductionTimeXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 56368
	 */
	int8_t torqueReductionTimeGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 56370
	 */
	gppwm_channel_e torqueReductionIgnitionRetardXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56371
	 */
	uint8_t alignmentFill_at_56371[1] = {};
	/**
	 * How many degrees of timing advance will be reduced during the Torque Reduction Time
	 * units: deg
	 * offset 56372
	 */
	float torqueReductionIgnitionRetardTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 56420
	 */
	int16_t torqueReductionIgnitionRetardXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 56432
	 */
	int8_t torqueReductionIgnitionRetardGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 56434
	 */
	uint8_t alignmentFill_at_56434[2] = {};
	/**
	offset 56436 bit 0 */
	bool boardUseTempPullUp : 1 {};
	/**
	offset 56436 bit 1 */
	bool boardUse2stepPullDown : 1 {};
	/**
	offset 56436 bit 2 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 56436 bit 3 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 56436 bit 4 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 56436 bit 5 */
	bool boardUseH1PullDown : 1 {};
	/**
	offset 56436 bit 6 */
	bool boardUseH2PullDown : 1 {};
	/**
	offset 56436 bit 7 */
	bool boardUseH3PullDown : 1 {};
	/**
	offset 56436 bit 8 */
	bool boardUseH4PullDown : 1 {};
	/**
	offset 56436 bit 9 */
	bool boardUseH5PullDown : 1 {};
	/**
	offset 56436 bit 10 */
	bool boardUseFlexPullDown : 1 {};
	/**
	offset 56436 bit 11 */
	bool unusedBit_283_11 : 1 {};
	/**
	offset 56436 bit 12 */
	bool unusedBit_283_12 : 1 {};
	/**
	offset 56436 bit 13 */
	bool unusedBit_283_13 : 1 {};
	/**
	offset 56436 bit 14 */
	bool unusedBit_283_14 : 1 {};
	/**
	offset 56436 bit 15 */
	bool unusedBit_283_15 : 1 {};
	/**
	offset 56436 bit 16 */
	bool unusedBit_283_16 : 1 {};
	/**
	offset 56436 bit 17 */
	bool unusedBit_283_17 : 1 {};
	/**
	offset 56436 bit 18 */
	bool unusedBit_283_18 : 1 {};
	/**
	offset 56436 bit 19 */
	bool unusedBit_283_19 : 1 {};
	/**
	offset 56436 bit 20 */
	bool unusedBit_283_20 : 1 {};
	/**
	offset 56436 bit 21 */
	bool unusedBit_283_21 : 1 {};
	/**
	offset 56436 bit 22 */
	bool unusedBit_283_22 : 1 {};
	/**
	offset 56436 bit 23 */
	bool unusedBit_283_23 : 1 {};
	/**
	offset 56436 bit 24 */
	bool unusedBit_283_24 : 1 {};
	/**
	offset 56436 bit 25 */
	bool unusedBit_283_25 : 1 {};
	/**
	offset 56436 bit 26 */
	bool unusedBit_283_26 : 1 {};
	/**
	offset 56436 bit 27 */
	bool unusedBit_283_27 : 1 {};
	/**
	offset 56436 bit 28 */
	bool unusedBit_283_28 : 1 {};
	/**
	offset 56436 bit 29 */
	bool unusedBit_283_29 : 1 {};
	/**
	offset 56436 bit 30 */
	bool unusedBit_283_30 : 1 {};
	/**
	offset 56436 bit 31 */
	bool unusedBit_283_31 : 1 {};
};
static_assert(sizeof(persistent_config_s) == 56440);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt
