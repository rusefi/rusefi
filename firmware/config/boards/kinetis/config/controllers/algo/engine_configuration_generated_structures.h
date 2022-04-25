// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on kinetis_gen_config.bat integration/rusefi_config.txt Mon Apr 25 03:44:07 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of stft_cell_cfg_s
struct stft_cell_cfg_s {
	/**
	%
	 * offset 0
	 */
	int8_t maxAdd;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	%
	 * offset 4
	 */
	int8_t maxRemove;
	/**
	 * need 4 byte alignment
	units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[1];
	/**
	 * Time constant for correction while in this cell: this sets responsiveness of the closed loop correction. A value of 5.0 means it will try to make most of the correction within 5 seconds, and a value of 1.0 will try to correct within 1 second.
	sec
	 * offset 6
	 */
	uint16_t timeConstant;
};
static_assert(sizeof(stft_cell_cfg_s) == 8);

// start of stft_s
struct stft_s {
	/**
	 * Below this RPM, the idle region is active
	RPM
	 * offset 0
	 */
	scaled_channel<uint8_t, 1, 50> maxIdleRegionRpm;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	 * Below this engine load, the overrun region is active
	load
	 * offset 4
	 */
	uint8_t maxOverrunLoad;
	/**
	 * need 4 byte alignment
	units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3];
	/**
	 * Above this engine load, the power region is active
	load
	 * offset 8
	 */
	uint8_t minPowerLoad;
	/**
	 * need 4 byte alignment
	units
	 * offset 9
	 */
	uint8_t alignmentFill_at_9[3];
	/**
	 * When close to correct AFR, pause correction. This can improve stability by not changing the adjustment if the error is extremely small, but is not required.
	%
	 * offset 12
	 */
	uint8_t deadband;
	/**
	 * need 4 byte alignment
	units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3];
	/**
	 * Below this temperature, correction is disabled.
	C
	 * offset 16
	 */
	int8_t minClt;
	/**
	 * need 4 byte alignment
	units
	 * offset 17
	 */
	uint8_t alignmentFill_at_17[3];
	/**
	 * Below this AFR, correction is paused
	afr
	 * offset 20
	 */
	uint8_t minAfr;
	/**
	 * need 4 byte alignment
	units
	 * offset 21
	 */
	uint8_t alignmentFill_at_21[3];
	/**
	 * Above this AFR, correction is paused
	afr
	 * offset 24
	 */
	uint8_t maxAfr;
	/**
	 * need 4 byte alignment
	units
	 * offset 25
	 */
	uint8_t alignmentFill_at_25[3];
	/**
	 * Delay after starting the engine before beginning closed loop correction.
	seconds
	 * offset 28
	 */
	uint8_t startupDelay;
	/**
	 * need 4 byte alignment
	units
	 * offset 29
	 */
	uint8_t alignmentFill_at_29[3];
	/**
	 * offset 32
	 */
	stft_cell_cfg_s cellCfgs[STFT_CELL_COUNT];
};
static_assert(sizeof(stft_s) == 64);

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
	ms
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

// start of cranking_parameters_s
struct cranking_parameters_s {
	/**
	 * Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for CLT, IAT, TPS ect, to give the final cranking pulse width.
	 * A reasonable starting point is 60mg per liter per cylinder.
	 * ex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.
	mg
	 * offset 0
	 */
	float baseFuel;
	/**
	 * This sets the RPM limit below which the ECU will use cranking fuel and ignition logic, typically this is around 350-450rpm. 
	 * set cranking_rpm X
	RPM
	 * offset 4
	 */
	int16_t rpm;
	/**
	 * need 4 byte alignment
	units
	 * offset 6
	 */
	uint8_t alignmentFill_at_6[2];
};
static_assert(sizeof(cranking_parameters_s) == 8);

// start of spi_pins
struct spi_pins {
	/**
	 * offset 0
	 */
	brain_pin_e mosiPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	 * offset 4
	 */
	brain_pin_e misoPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3];
	/**
	 * offset 8
	 */
	brain_pin_e sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 9
	 */
	uint8_t alignmentFill_at_9[3];
};
static_assert(sizeof(spi_pins) == 12);

// start of gppwm_channel
struct gppwm_channel {
	/**
	 * Select a pin to use for PWM or on-off output.
	 * offset 0
	 */
	output_pin_e pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	 * If an error (with a sensor, etc) is detected, this value is used instead of reading from the table.
	 * This should be a safe value for whatever hardware is connected to prevent damage.
	%
	 * offset 4
	 */
	uint8_t dutyIfError;
	/**
	 * need 4 byte alignment
	units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[1];
	/**
	 * Select a frequency to run PWM at.
	 * Set this to 0hz to enable on-off mode.
	hz
	 * offset 6
	 */
	uint16_t pwmFrequency;
	/**
	 * Hysteresis: in on-off mode, turn the output on when the table value is above this duty.
	%
	 * offset 8
	 */
	uint8_t onAboveDuty;
	/**
	 * need 4 byte alignment
	units
	 * offset 9
	 */
	uint8_t alignmentFill_at_9[3];
	/**
	 * Hysteresis: in on-off mode, turn the output off when the table value is below this duty.
	%
	 * offset 12
	 */
	uint8_t offBelowDuty;
	/**
	 * need 4 byte alignment
	units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3];
	/**
	 * Selects the load axis to use for the table.
	 * offset 16
	 */
	gppwm_channel_e loadAxis;
	/**
	 * need 4 byte alignment
	units
	 * offset 17
	 */
	uint8_t alignmentFill_at_17[3];
	/**
	unit
	 * offset 20
	 */
	uint8_t alignmentFill_map;
	/**
	 * need 4 byte alignment
	units
	 * offset 21
	 */
	uint8_t alignmentFill_at_21[3];
	/**
	load
	 * offset 24
	 */
	uint8_t loadBins[GPPWM_LOAD_COUNT];
	/**
	RPM
	 * offset 32
	 */
	scaled_channel<uint8_t, 1, 50> rpmBins[GPPWM_RPM_COUNT];
	/**
	duty
	 * offset 40
	 */
	uint8_t table[GPPWM_RPM_COUNT][GPPWM_LOAD_COUNT];
};
static_assert(sizeof(gppwm_channel) == 104);

// start of air_pressure_sensor_config_s
struct air_pressure_sensor_config_s {
	/**
	 * kPa value at low volts
	kpa
	 * offset 0
	 */
	float lowValue;
	/**
	 * kPa value at high volts
	kpa
	 * offset 4
	 */
	float highValue;
	/**
	 * offset 8
	 */
	air_pressure_sensor_type_e type;
	/**
	 * offset 12
	 */
	adc_channel_e hwChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3];
	/**
	unit
	 * offset 16
	 */
	uint8_t alignmentFill[3];
	/**
	 * need 4 byte alignment
	units
	 * offset 19
	 */
	uint8_t alignmentFill_at_19[1];
};
static_assert(sizeof(air_pressure_sensor_config_s) == 20);

/**
 * @brief MAP averaging configuration

*/
// start of MAP_sensor_config_s
struct MAP_sensor_config_s {
	/**
	 * offset 0
	 */
	float samplingAngleBins[MAP_ANGLE_SIZE];
	/**
	 * MAP averaging sampling start crank degree angle
	deg
	 * offset 32
	 */
	float samplingAngle[MAP_ANGLE_SIZE];
	/**
	 * offset 64
	 */
	float samplingWindowBins[MAP_WINDOW_SIZE];
	/**
	 * MAP averaging angle crank degree duration
	deg
	 * offset 96
	 */
	float samplingWindow[MAP_WINDOW_SIZE];
	/**
	 * offset 128
	 */
	air_pressure_sensor_config_s sensor;
};
static_assert(sizeof(MAP_sensor_config_s) == 148);

/**
 * @brief Thermistor known values

*/
// start of thermistor_conf_s
struct thermistor_conf_s {
	/**
	 * these values are in Celcius
	*C
	 * offset 0
	 */
	float tempC_1;
	/**
	*C
	 * offset 4
	 */
	float tempC_2;
	/**
	*C
	 * offset 8
	 */
	float tempC_3;
	/**
	Ohm
	 * offset 12
	 */
	float resistance_1;
	/**
	Ohm
	 * offset 16
	 */
	float resistance_2;
	/**
	Ohm
	 * offset 20
	 */
	float resistance_3;
	/**
	 * Pull-up resistor value on your board
	Ohm
	 * offset 24
	 */
	float bias_resistor;
};
static_assert(sizeof(thermistor_conf_s) == 28);

/**
 * @brief Linear sensor interpolation

*/
// start of linear_sensor_s
struct linear_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	unit
	 * offset 4
	 */
	uint8_t alignmentFill[3];
	/**
	 * need 4 byte alignment
	units
	 * offset 7
	 */
	uint8_t alignmentFill_at_7[1];
	/**
	volts
	 * offset 8
	 */
	float v1;
	/**
	kPa
	 * offset 12
	 */
	float value1;
	/**
	volts
	 * offset 16
	 */
	float v2;
	/**
	kPa
	 * offset 20
	 */
	float value2;
};
static_assert(sizeof(linear_sensor_s) == 24);

/**
 * @brief Thermistor curve parameters

*/
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
	units
	 * offset 29
	 */
	uint8_t alignmentFill_at_29[3];
};
static_assert(sizeof(ThermistorConf) == 32);

// start of injector_s
struct injector_s {
	/**
	 * This is your injector flow at the fuel pressure used in the vehicle. cc/min, cubic centimetre per minute
	 * By the way, g/s = 0.125997881 * (lb/hr)
	 * g/s = 0.125997881 * (cc/min)/10.5
	 * g/s = 0.0119997981 * cc/min
	cm3/min
	 * offset 0
	 */
	float flow;
	/**
	 * set_flat_injector_lag LAG
	 * set_injector_lag VOLTAGE LAG
	volts
	 * offset 4
	 */
	float battLagCorrBins[VBAT_INJECTOR_CURVE_SIZE];
	/**
	 * ms delay between injector open and close dead times
	ms
	 * offset 36
	 */
	float battLagCorr[VBAT_INJECTOR_CURVE_SIZE];
};
static_assert(sizeof(injector_s) == 68);

// start of specs_s
struct specs_s {
	/**
	 * Engine volume/capacity, in litres
	 * see also cylindersCount
	L
	 * offset 0
	 */
	float displacement;
	/**
	 * Number of cylinder the engine has.
	 * offset 4
	 */
	uint32_t cylindersCount;
	/**
	 * offset 8
	 */
	firing_order_e firingOrder;
};
static_assert(sizeof(specs_s) == 12);

/**
 * @brief Trigger wheel(s) configuration

*/
// start of trigger_config_s
struct trigger_config_s {
	/**
	 * https://github.com/rusefi/rusefi/wiki/All-Supported-Triggers
	 * set trigger_type X
	 * offset 0
	 */
	trigger_type_e type;
	/**
	offset 4 bit 0 */
	bool todoRemoveMeOneDay0 : 1 {};
	/**
	offset 4 bit 1 */
	bool todoRemoveMeOneDay1 : 1 {};
	/**
	 * This option could be used if your second trigger channel is broken
	offset 4 bit 2 */
	bool useOnlyFirstChannel : 1 {};
	/**
	offset 4 bit 3 */
	bool unusedBit_4_3 : 1 {};
	/**
	offset 4 bit 4 */
	bool unusedBit_4_4 : 1 {};
	/**
	offset 4 bit 5 */
	bool unusedBit_4_5 : 1 {};
	/**
	offset 4 bit 6 */
	bool unusedBit_4_6 : 1 {};
	/**
	offset 4 bit 7 */
	bool unusedBit_4_7 : 1 {};
	/**
	offset 4 bit 8 */
	bool unusedBit_4_8 : 1 {};
	/**
	offset 4 bit 9 */
	bool unusedBit_4_9 : 1 {};
	/**
	offset 4 bit 10 */
	bool unusedBit_4_10 : 1 {};
	/**
	offset 4 bit 11 */
	bool unusedBit_4_11 : 1 {};
	/**
	offset 4 bit 12 */
	bool unusedBit_4_12 : 1 {};
	/**
	offset 4 bit 13 */
	bool unusedBit_4_13 : 1 {};
	/**
	offset 4 bit 14 */
	bool unusedBit_4_14 : 1 {};
	/**
	offset 4 bit 15 */
	bool unusedBit_4_15 : 1 {};
	/**
	offset 4 bit 16 */
	bool unusedBit_4_16 : 1 {};
	/**
	offset 4 bit 17 */
	bool unusedBit_4_17 : 1 {};
	/**
	offset 4 bit 18 */
	bool unusedBit_4_18 : 1 {};
	/**
	offset 4 bit 19 */
	bool unusedBit_4_19 : 1 {};
	/**
	offset 4 bit 20 */
	bool unusedBit_4_20 : 1 {};
	/**
	offset 4 bit 21 */
	bool unusedBit_4_21 : 1 {};
	/**
	offset 4 bit 22 */
	bool unusedBit_4_22 : 1 {};
	/**
	offset 4 bit 23 */
	bool unusedBit_4_23 : 1 {};
	/**
	offset 4 bit 24 */
	bool unusedBit_4_24 : 1 {};
	/**
	offset 4 bit 25 */
	bool unusedBit_4_25 : 1 {};
	/**
	offset 4 bit 26 */
	bool unusedBit_4_26 : 1 {};
	/**
	offset 4 bit 27 */
	bool unusedBit_4_27 : 1 {};
	/**
	offset 4 bit 28 */
	bool unusedBit_4_28 : 1 {};
	/**
	offset 4 bit 29 */
	bool unusedBit_4_29 : 1 {};
	/**
	offset 4 bit 30 */
	bool unusedBit_4_30 : 1 {};
	/**
	offset 4 bit 31 */
	bool unusedBit_4_31 : 1 {};
	/**
	number
	 * offset 8
	 */
	int customTotalToothCount;
	/**
	number
	 * offset 12
	 */
	int customSkippedToothCount;
};
static_assert(sizeof(trigger_config_s) == 16);

// start of afr_sensor_s
struct afr_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	unit
	 * offset 4
	 */
	uint8_t alignmentFill_afr[3];
	/**
	 * need 4 byte alignment
	units
	 * offset 7
	 */
	uint8_t alignmentFill_at_7[1];
	/**
	volts
	 * offset 8
	 */
	float v1;
	/**
	AFR
	 * offset 12
	 */
	float value1;
	/**
	volts
	 * offset 16
	 */
	float v2;
	/**
	AFR
	 * offset 20
	 */
	float value2;
};
static_assert(sizeof(afr_sensor_s) == 24);

// start of idle_hardware_s
struct idle_hardware_s {
	/**
	Hz
	 * offset 0
	 */
	int solenoidFrequency;
	/**
	 * offset 4
	 */
	output_pin_e solenoidPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3];
	/**
	 * offset 8
	 */
	brain_pin_e stepperDirectionPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 9
	 */
	uint8_t alignmentFill_at_9[3];
	/**
	 * offset 12
	 */
	brain_pin_e stepperStepPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3];
	/**
	 * offset 16
	 */
	pin_output_mode_e solenoidPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 17
	 */
	uint8_t alignmentFill_at_17[3];
};
static_assert(sizeof(idle_hardware_s) == 20);

// start of dc_io
struct dc_io {
	/**
	 * offset 0
	 */
	brain_pin_e directionPin1;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	 * offset 4
	 */
	brain_pin_e directionPin2;
	/**
	 * need 4 byte alignment
	units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3];
	/**
	 * Acts as EN pin in two-wire mode
	 * offset 8
	 */
	brain_pin_e controlPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 9
	 */
	uint8_t alignmentFill_at_9[3];
	/**
	 * offset 12
	 */
	brain_pin_e disablePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3];
};
static_assert(sizeof(dc_io) == 16);

// start of vr_threshold_s
struct vr_threshold_s {
	/**
	 * offset 0
	 */
	brain_pin_e pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	 * offset 4
	 */
	uint8_t pad[3];
	/**
	 * need 4 byte alignment
	units
	 * offset 7
	 */
	uint8_t alignmentFill_at_7[1];
	/**
	rpm
	 * offset 8
	 */
	scaled_channel<uint8_t, 1, 50> rpmBins[6];
	/**
	 * need 4 byte alignment
	units
	 * offset 14
	 */
	uint8_t alignmentFill_at_14[2];
	/**
	volts
	 * offset 16
	 */
	scaled_channel<uint8_t, 100, 1> values[6];
	/**
	 * need 4 byte alignment
	units
	 * offset 22
	 */
	uint8_t alignmentFill_at_22[2];
};
static_assert(sizeof(vr_threshold_s) == 24);

// start of engine_configuration_s
struct engine_configuration_s {
	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
	 * set engine_type X
	 * offset 0
	 */
	engine_type_e engineType;
	/**
	 * Disable sensor sniffer above this rpm
	RPM
	 * offset 4
	 */
	scaled_channel<uint8_t, 1, 50> sensorSnifferRpmThreshold;
	/**
	 * need 4 byte alignment
	units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3];
	/**
	 * set rpm_hard_limit X
	rpm
	 * offset 8
	 */
	scaled_channel<uint8_t, 1, 50> rpmHardLimit;
	/**
	 * need 4 byte alignment
	units
	 * offset 9
	 */
	uint8_t alignmentFill_at_9[3];
	/**
	 * A secondary Rev limit engaged by the driver to help launch the vehicle faster
	rpm
	 * offset 12
	 */
	scaled_channel<uint8_t, 1, 50> launchRpm;
	/**
	 * need 4 byte alignment
	units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3];
	/**
	 * Engine sniffer would be disabled above this rpm
	 * set engineSnifferRpmThreshold X
	RPM
	 * offset 16
	 */
	scaled_channel<uint8_t, 1, 50> engineSnifferRpmThreshold;
	/**
	 * need 4 byte alignment
	units
	 * offset 17
	 */
	uint8_t alignmentFill_at_17[3];
	/**
	 * Disable multispark above this engine speed.
	rpm
	 * offset 20
	 */
	scaled_channel<uint8_t, 1, 50> multisparkMaxRpm;
	/**
	 * need 4 byte alignment
	units
	 * offset 21
	 */
	uint8_t alignmentFill_at_21[3];
	/**
	 * Above this RPM, disable AC. Set to 0 to disable check.
	rpm
	 * offset 24
	 */
	scaled_channel<uint8_t, 1, 50> maxAcRpm;
	/**
	 * need 4 byte alignment
	units
	 * offset 25
	 */
	uint8_t alignmentFill_at_25[3];
	/**
	 * Above this TPS, disable AC. Set to 0 to disable check.
	%
	 * offset 28
	 */
	uint8_t maxAcTps;
	/**
	 * need 4 byte alignment
	units
	 * offset 29
	 */
	uint8_t alignmentFill_at_29[3];
	/**
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	deg C
	 * offset 32
	 */
	uint8_t maxAcClt;
	/**
	 * need 4 byte alignment
	units
	 * offset 33
	 */
	uint8_t alignmentFill_at_33[3];
	/**
	RPM
	 * offset 36
	 */
	scaled_channel<uint8_t, 1, 50> knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * offset 52
	 */
	uint8_t unused28[24];
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	deg
	 * offset 76
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * need 4 byte alignment
	units
	 * offset 77
	 */
	uint8_t alignmentFill_at_77[3];
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	count
	 * offset 80
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 81
	 */
	uint8_t alignmentFill_at_81[3];
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	RPM
	 * offset 84
	 */
	scaled_channel<uint8_t, 1, 50> sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	ms
	 * offset 92
	 */
	float sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * offset 124
	 */
	injector_s injector;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 192 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On Ford vehicles one of the sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 192 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 192 bit 2 */
	bool isVerboseAuxPid1 : 1 {};
	/**
	offset 192 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 192 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 192 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 192 bit 6 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 192 bit 7 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 192 bit 8 */
	bool enableTrailingSparks : 1 {};
	/**
	 * enable cj125verbose/disable cj125verbose
	offset 192 bit 9 */
	bool isCJ125Verbose : 1 {};
	/**
	 * Is your UA CJ125 output wired to MCU via resistor divider? Ua can go over 3.3v but only at lambda >3, i.e very lean AFR above 44.1
	 * When exposed to free air and 17x gain, Ua will be 4.17 volt
	offset 192 bit 10 */
	bool cj125isUaDivided : 1 {};
	/**
	offset 192 bit 11 */
	bool cj125isLsu49 : 1 {};
	/**
	 * TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 192 bit 12 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 192 bit 13 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 192 bit 14 */
	bool useEeprom : 1 {};
	/**
	 * Is your UR CJ125 output wired to MCU via resistor divider?
	 * Looks like 3v range should be enough, divider generally not needed.
	offset 192 bit 15 */
	bool cj125isUrDivided : 1 {};
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 192 bit 16 */
	bool useCicPidForIdle : 1 {};
	/**
	offset 192 bit 17 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 192 bit 18 */
	bool useInstantRpmForIdle : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 192 bit 19 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 192 bit 20 */
	bool launchControlEnabled : 1 {};
	/**
	offset 192 bit 21 */
	bool rollingLaunchEnabled : 1 {};
	/**
	offset 192 bit 22 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 192 bit 23 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	 * 
	 * enable logic_level_trigger
	offset 192 bit 24 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 192 bit 25 */
	bool useTLE8888_stepper : 1 {};
	/**
	 * If enabled, the MAP estimate table will be used if the MAP sensor fails to estimate manifold pressure based on RPM and TPS.
	offset 192 bit 26 */
	bool enableMapEstimationTableFallback : 1 {};
	/**
	offset 192 bit 27 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 192 bit 28 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 192 bit 29 */
	bool artificialTestMisfire : 1 {};
	/**
	offset 192 bit 30 */
	bool issue_294_31 : 1 {};
	/**
	offset 192 bit 31 */
	bool unusedBit_57_31 : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * set tps_min X
	ADC
	 * offset 196
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * set tps_max X
	ADC
	 * offset 198
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 200
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 202
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 204
	 */
	cranking_parameters_s cranking;
	/**
	 * offset 212
	 */
	float unused184;
	/**
	 * Dwell duration while cranking
	ms
	 * offset 216
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	rpm
	 * offset 220
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	rpm
	 * offset 222
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 224
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 372
	 */
	ThermistorConf clt;
	/**
	 * offset 404
	 */
	ThermistorConf iat;
	/**
	deg
	 * offset 436
	 */
	int launchTimingRetard;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	integer
	 * offset 440
	 */
	int hip9011PrescalerAndSDO;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	kHz
	 * offset 444
	 */
	float knockBandCustom;
	/**
	 * offset 448
	 */
	specs_s specs;
	/**
	 * Cylinder diameter in mm.
	mm
	 * offset 460
	 */
	float cylinderBore;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS commonly only used for NA engines
	 * Speed Density requires MAP sensor and is the default choice for may installs
	 * MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	 * offset 464
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 468
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups. If your injectors are individually wired you will also need to enable "Two wire batch emulation". 
	 * set injection_mode X
	 * See also twoWireBatchInjection
	 * offset 472
	 */
	injection_mode_e injectionMode;
	/**
	 * this is about deciding when the injector starts it's squirt
	 * See also injectionPhase map
	 * todo: do we need even need this since we have the map anyway?
	deg
	 * offset 476
	 */
	angle_t extraInjectionOffset;
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	deg
	 * offset 480
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 484
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	count
	 * offset 488
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * need 4 byte alignment
	units
	 * offset 489
	 */
	uint8_t alignmentFill_at_489[3];
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	kph
	 * offset 492
	 */
	uint8_t maxIdleVss;
	/**
	 * need 4 byte alignment
	units
	 * offset 493
	 */
	uint8_t alignmentFill_at_493[1];
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	kPa
	 * offset 494
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 496
	 */
	timing_mode_e timingMode;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	RPM
	 * offset 500
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchnization point before TDC and negative in case of synchnization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * set global_trigger_offset_angle X
	deg btdc
	 * offset 504
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	coef
	 * offset 508
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	coef
	 * offset 512
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 516
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 520
	 */
	float fanOffTemperature;
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	revs/km
	 * offset 524
	 */
	float driveWheelRevPerKm;
	/**
	 * set can_mode X
	 * offset 528
	 */
	can_nbc_e canNbcType;
	/**
	 * CANbus thread period in ms
	ms
	 * offset 532
	 */
	int canSleepPeriodMs;
	/**
	index
	 * offset 536
	 */
	int weHaveUnusedHere;
	/**
	 * offset 540
	 */
	display_mode_e displayMode;
	/**
	index
	 * offset 544
	 */
	int byFirmwareVersion;
	/**
	index
	 * offset 548
	 */
	int HD44780width;
	/**
	index
	 * offset 552
	 */
	int HD44780height;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
	 * offset 556
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 557
	 */
	uint8_t alignmentFill_at_557[3];
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 560
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 561
	 */
	uint8_t alignmentFill_at_561[3];
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 564
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * need 4 byte alignment
	units
	 * offset 565
	 */
	uint8_t alignmentFill_at_565[3];
	/**
	 * Second throttle body position sensor, single channel so far
	 * set_analog_input_pin tps2 X
	 * offset 568
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 569
	 */
	uint8_t alignmentFill_at_569[3];
	/**
	 * 0.1 is a good default value
	x
	 * offset 572
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	angle
	 * offset 576
	 */
	int trailingSparkAngle;
	/**
	 * offset 580
	 */
	trigger_config_s trigger;
	/**
	 * offset 596
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 597
	 */
	uint8_t alignmentFill_at_597[3];
	/**
	 * Single value to be used in event of a failed MAP sensor 
	 * This value is only used for speed density fueling calculations.
	kPa
	 * offset 600
	 */
	uint8_t failedMapFallback;
	/**
	 * need 4 byte alignment
	units
	 * offset 601
	 */
	uint8_t alignmentFill_at_601[3];
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost.
	%
	 * offset 604
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * need 4 byte alignment
	units
	 * offset 605
	 */
	uint8_t alignmentFill_at_605[3];
	/**
	 * offset 608
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 609
	 */
	uint8_t alignmentFill_at_609[3];
	/**
	 * set global_fuel_correction X
	coef
	 * offset 612
	 */
	float globalFuelCorrection;
	/**
	volts
	 * offset 616
	 */
	float adcVcc;
	/**
	Deg
	 * offset 620
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 624
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 625
	 */
	uint8_t alignmentFill_at_625[3];
	/**
	 * offset 628
	 */
	uint8_t camInputsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * need 4 byte alignment
	units
	 * offset 631
	 */
	uint8_t alignmentFill_at_631[1];
	/**
	 * offset 632
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * set_analog_input_pin pps X
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 656
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 657
	 */
	uint8_t alignmentFill_at_657[3];
	/**
	 * offset 660
	 */
	brain_pin_e tle6240_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 661
	 */
	uint8_t alignmentFill_at_661[3];
	/**
	 * offset 664
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 665
	 */
	uint8_t alignmentFill_at_665[3];
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 668
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 669
	 */
	uint8_t alignmentFill_at_669[3];
	/**
	 * @see hasBaroSensor
	 * offset 672
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 692
	 */
	idle_hardware_s idle;
	/**
	 * Value between 0 and 100 used in Manual mode
	%
	 * offset 712
	 */
	float manIdlePosition;
	/**
	 * Ignition timing to remove when a knock event occurs.
	%
	 * offset 716
	 */
	uint8_t knockRetardAggression;
	/**
	 * need 4 byte alignment
	units
	 * offset 717
	 */
	uint8_t alignmentFill_at_717[3];
	/**
	 * After a knock event, reapply timing at this rate.
	deg/s
	 * offset 720
	 */
	uint8_t knockRetardReapplyRate;
	/**
	 * need 4 byte alignment
	units
	 * offset 721
	 */
	uint8_t alignmentFill_at_721[3];
	/**
	 * Maximum amount of knock retard.
	deg
	 * offset 724
	 */
	uint8_t knockRetardMaximum;
	/**
	 * need 4 byte alignment
	units
	 * offset 725
	 */
	uint8_t alignmentFill_at_725[3];
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 728
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * need 4 byte alignment
	units
	 * offset 729
	 */
	uint8_t alignmentFill_at_729[1];
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	ratio
	 * offset 730
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	count
	 * offset 732
	 */
	uint8_t vssToothCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 733
	 */
	uint8_t alignmentFill_at_733[3];
	/**
	 * offset 736
	 */
	brain_pin_e l9779_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 737
	 */
	uint8_t alignmentFill_at_737[3];
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * TODO: rename to triggerSimulatorRpm
	Rpm
	 * offset 740
	 */
	int triggerSimulatorFrequency;
	/**
	 * offset 744
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 756
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 768
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 769
	 */
	uint8_t alignmentFill_at_769[3];
	/**
	 * offset 772
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 773
	 */
	uint8_t alignmentFill_at_773[3];
	/**
	 * offset 776
	 */
	brain_pin_e HD44780_rs;
	/**
	 * need 4 byte alignment
	units
	 * offset 777
	 */
	uint8_t alignmentFill_at_777[3];
	/**
	 * offset 780
	 */
	brain_pin_e HD44780_e;
	/**
	 * need 4 byte alignment
	units
	 * offset 781
	 */
	uint8_t alignmentFill_at_781[3];
	/**
	 * offset 784
	 */
	brain_pin_e HD44780_db4;
	/**
	 * need 4 byte alignment
	units
	 * offset 785
	 */
	uint8_t alignmentFill_at_785[3];
	/**
	 * offset 788
	 */
	brain_pin_e HD44780_db5;
	/**
	 * need 4 byte alignment
	units
	 * offset 789
	 */
	uint8_t alignmentFill_at_789[3];
	/**
	 * offset 792
	 */
	brain_pin_e HD44780_db6;
	/**
	 * need 4 byte alignment
	units
	 * offset 793
	 */
	uint8_t alignmentFill_at_793[3];
	/**
	 * offset 796
	 */
	brain_pin_e HD44780_db7;
	/**
	 * need 4 byte alignment
	units
	 * offset 797
	 */
	uint8_t alignmentFill_at_797[3];
	/**
	 * offset 800
	 */
	brain_pin_e gps_rx_pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 801
	 */
	uint8_t alignmentFill_at_801[3];
	/**
	 * offset 804
	 */
	brain_pin_e gps_tx_pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 805
	 */
	uint8_t alignmentFill_at_805[3];
	/**
	 * offset 808
	 */
	output_pin_e fuelPumpPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 809
	 */
	uint8_t alignmentFill_at_809[3];
	/**
	 * offset 812
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 813
	 */
	uint8_t alignmentFill_at_813[3];
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 816
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 817
	 */
	uint8_t alignmentFill_at_817[3];
	/**
	 * offset 820
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 821
	 */
	uint8_t alignmentFill_at_821[3];
	/**
	 * offset 824
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 825
	 */
	uint8_t alignmentFill_at_825[3];
	/**
	 * offset 828
	 */
	output_pin_e fanPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 829
	 */
	uint8_t alignmentFill_at_829[3];
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 832
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 833
	 */
	uint8_t alignmentFill_at_833[3];
	/**
	 * offset 836
	 */
	output_pin_e alternatorControlPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 837
	 */
	uint8_t alignmentFill_at_837[3];
	/**
	 * offset 840
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 841
	 */
	uint8_t alignmentFill_at_841[3];
	/**
	 * offset 844
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 845
	 */
	uint8_t alignmentFill_at_845[3];
	/**
	 * offset 848
	 */
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 852
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 853
	 */
	uint8_t alignmentFill_at_853[3];
	/**
	 * set_cj125_heater_pin XXX
	 * offset 856
	 */
	brain_pin_e wboHeaterPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 857
	 */
	uint8_t alignmentFill_at_857[3];
	/**
	 * set_cj125_cs_pin XXX
	 * offset 860
	 */
	brain_pin_e cj125CsPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 861
	 */
	uint8_t alignmentFill_at_861[3];
	/**
	 * offset 864
	 */
	spi_device_e max31855spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 865
	 */
	uint8_t alignmentFill_at_865[3];
	/**
	 * offset 868
	 */
	brain_pin_e debugTriggerSync;
	/**
	 * need 4 byte alignment
	units
	 * offset 869
	 */
	uint8_t alignmentFill_at_869[3];
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 872
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 873
	 */
	uint8_t alignmentFill_at_873[3];
	/**
	 * offset 876
	 */
	brain_pin_e mc33972_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 877
	 */
	uint8_t alignmentFill_at_877[3];
	/**
	 * offset 880
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 881
	 */
	uint8_t alignmentFill_at_881[3];
	/**
	 * Useful in Research&Development phase
	 * offset 884
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 885
	 */
	uint8_t alignmentFill_at_885[3];
	/**
	 * First throttle body, second sensor.
	 * offset 888
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 889
	 */
	uint8_t alignmentFill_at_889[3];
	/**
	 * Second throttle body, second sensor.
	 * offset 892
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 893
	 */
	uint8_t alignmentFill_at_893[3];
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 896
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 897
	 */
	uint8_t alignmentFill_at_897[3];
	/**
	%
	 * offset 900
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 908
	 */
	ego_sensor_e afr_type;
	/**
	 * 0.1 is a good default value
	x
	 * offset 912
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 916
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 919
	 */
	uint8_t alignmentFill_at_919[1];
	/**
	 * offset 920
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 921
	 */
	uint8_t alignmentFill_at_921[3];
	/**
	 * offset 924
	 */
	output_pin_e tachOutputPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 925
	 */
	uint8_t alignmentFill_at_925[3];
	/**
	 * offset 928
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 929
	 */
	uint8_t alignmentFill_at_929[3];
	/**
	 * offset 932
	 */
	output_pin_e mainRelayPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 933
	 */
	uint8_t alignmentFill_at_933[3];
	/**
	 * offset 936
	 */
	brain_pin_e sdCardCsPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 937
	 */
	uint8_t alignmentFill_at_937[3];
	/**
	 * set_can_tx_pin X
	 * offset 940
	 */
	brain_pin_e canTxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 941
	 */
	uint8_t alignmentFill_at_941[3];
	/**
	 * set_can_rx_pin X
	 * offset 944
	 */
	brain_pin_e canRxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 945
	 */
	uint8_t alignmentFill_at_945[3];
	/**
	 * offset 948
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 949
	 */
	uint8_t alignmentFill_at_949[3];
	/**
	 * Additional idle % while A/C is active
	%
	 * offset 952
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * need 4 byte alignment
	units
	 * offset 953
	 */
	uint8_t alignmentFill_at_953[1];
	/**
	 * Ratio between the wheels and your transmission output.
	ratio
	 * offset 954
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 956
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 957
	 */
	uint8_t alignmentFill_at_957[3];
	/**
	 * offset 960
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	units
	 * offset 961
	 */
	uint8_t alignmentFill_at_961[1];
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	mv
	 * offset 962
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 964
	 */
	uint16_t wastegatePositionMax;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	mv
	 * offset 966
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 968
	 */
	uint16_t idlePositionMax;
	/**
	 * need 4 byte alignment
	units
	 * offset 970
	 */
	uint8_t alignmentFill_at_970[2];
	/**
	 * Secondary TTL channel baud rate
	BPs
	 * offset 972
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	CR
	 * offset 976
	 */
	float compressionRatio;
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 980
	 */
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 983
	 */
	uint8_t alignmentFill_at_983[1];
	/**
	 * offset 984
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 987
	 */
	uint8_t alignmentFill_at_987[1];
	/**
	 * Narrow band o2 heater, not used for CJ125. 'ON' if engine is running, 'OFF' if stopped or cranking. See wboHeaterPin
	 * offset 988
	 */
	output_pin_e o2heaterPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 989
	 */
	uint8_t alignmentFill_at_989[3];
	/**
	 * offset 992
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * need 4 byte alignment
	units
	 * offset 993
	 */
	uint8_t alignmentFill_at_993[3];
	/**
	offset 996 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 996 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 996 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 996 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 996 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 996 bit 5 */
	bool isEngineControlEnabled : 1 {};
	/**
	offset 996 bit 6 */
	bool isHip9011Enabled : 1 {};
	/**
	offset 996 bit 7 */
	bool isVerboseAlternator : 1 {};
	/**
	offset 996 bit 8 */
	bool useSerialPort : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 996 bit 9 */
	bool useStepperIdle : 1 {};
	/**
	offset 996 bit 10 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 996 bit 11 */
	bool useTpicAdvancedMode : 1 {};
	/**
	offset 996 bit 12 */
	bool useLcdScreen : 1 {};
	/**
	offset 996 bit 13 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 996 bit 14 */
	bool enableVerboseCanTx : 1 {};
	/**
	 * This will cause the alternator to be operated in a basic on or off mode, this is the simplest alternator control.
	offset 996 bit 15 */
	bool onOffAlternatorLogic : 1 {};
	/**
	 * enable cj125/disable cj125
	offset 996 bit 16 */
	bool isCJ125Enabled : 1 {};
	/**
	 * Use rise or fall signal front
	 * get vvtCamSensorUseRise
	offset 996 bit 17 */
	bool vvtCamSensorUseRise : 1 {};
	/**
	 * Useful for individual intakes
	offset 996 bit 18 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 996 bit 19 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 996 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 996 bit 21 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * This setting allows the ECU to open the IAC during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars. Used in Auto-PID Idle mode.
	offset 996 bit 22 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 996 bit 23 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 996 bit 24 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 996 bit 25 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 996 bit 26 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 996 bit 27 */
	bool pauseEtbControl : 1 {};
	/**
	offset 996 bit 28 */
	bool alignEngineSnifferAtTDC : 1 {};
	/**
	 * This setting allows the ETB to act as the idle air control valve and move to regulate the airflow at idle.
	offset 996 bit 29 */
	bool useETBforIdleControl : 1 {};
	/**
	offset 996 bit 30 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 996 bit 31 */
	bool enableAemXSeries : 1 {};
	/**
	 * offset 1000
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 1004
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1005
	 */
	uint8_t alignmentFill_at_1005[3];
	/**
	 * offset 1008
	 */
	brain_pin_e hip9011CsPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1009
	 */
	uint8_t alignmentFill_at_1009[3];
	/**
	 * offset 1012
	 */
	brain_pin_e hip9011IntHoldPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1013
	 */
	uint8_t alignmentFill_at_1013[3];
	/**
	 * offset 1016
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1017
	 */
	uint8_t alignmentFill_at_1017[3];
	/**
	 * offset 1020
	 */
	uint32_t verboseCanBaseAddress;
	/**
	v
	 * offset 1024
	 */
	uint8_t mc33_hvolt;
	/**
	 * need 4 byte alignment
	units
	 * offset 1025
	 */
	uint8_t alignmentFill_at_1025[3];
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	kPa
	 * offset 1028
	 */
	uint8_t minimumBoostClosedLoopMap;
	/**
	 * need 4 byte alignment
	units
	 * offset 1029
	 */
	uint8_t alignmentFill_at_1029[3];
	/**
	 * Optional Radiator Fan used with A/C
	 * offset 1032
	 */
	output_pin_e acFanPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1033
	 */
	uint8_t alignmentFill_at_1033[3];
	/**
	 * offset 1036
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1037
	 */
	uint8_t alignmentFill_at_1037[3];
	/**
	 * offset 1040
	 */
	spi_device_e l9779spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1041
	 */
	uint8_t alignmentFill_at_1041[3];
	/**
	volts
	 * offset 1044
	 */
	uint8_t dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	 * offset 1052
	 */
	imu_type_e imuType;
	/**
	 * need 4 byte alignment
	units
	 * offset 1053
	 */
	uint8_t alignmentFill_at_1053[3];
	/**
	multiplier
	 * offset 1056
	 */
	uint8_t dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	kg
	 * offset 1064
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling?
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	RPM
	 * offset 1066
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	ms
	 * offset 1068
	 */
	uint16_t applyNonlinearBelowPulse;
	/**
	 * need 4 byte alignment
	units
	 * offset 1070
	 */
	uint8_t alignmentFill_at_1070[2];
	/**
	 * offset 1072
	 */
	brain_pin_e lps25BaroSensorScl;
	/**
	 * need 4 byte alignment
	units
	 * offset 1073
	 */
	uint8_t alignmentFill_at_1073[3];
	/**
	 * offset 1076
	 */
	brain_pin_e lps25BaroSensorSda;
	/**
	 * need 4 byte alignment
	units
	 * offset 1077
	 */
	uint8_t alignmentFill_at_1077[3];
	/**
	 * offset 1080
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1081
	 */
	uint8_t alignmentFill_at_1081[3];
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 1084
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1085
	 */
	uint8_t alignmentFill_at_1085[3];
	/**
	 * offset 1088
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1089
	 */
	uint8_t alignmentFill_at_1089[3];
	/**
	 * offset 1092
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1093
	 */
	uint8_t alignmentFill_at_1093[3];
	/**
	 * offset 1096
	 */
	brain_pin_e max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 1104
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1105
	 */
	uint8_t alignmentFill_at_1105[3];
	/**
	 * offset 1108
	 */
	brain_pin_e test557pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1109
	 */
	uint8_t alignmentFill_at_1109[3];
	/**
	 * offset 1112
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1113
	 */
	uint8_t alignmentFill_at_1113[3];
	/**
	 * offset 1116
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1117
	 */
	uint8_t alignmentFill_at_1117[3];
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	:1
	 * offset 1120
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * need 4 byte alignment
	units
	 * offset 1121
	 */
	uint8_t alignmentFill_at_1121[3];
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	%
	 * offset 1124
	 */
	uint8_t etbMaximumPosition;
	/**
	 * need 4 byte alignment
	units
	 * offset 1125
	 */
	uint8_t alignmentFill_at_1125[1];
	/**
	 * SD card logging period, in milliseconds
	ms
	 * offset 1126
	 */
	int16_t sdCardPeriodMs;
	/**
	 * offset 1128
	 */
	adc_channel_e idlePositionSensor;
	/**
	 * need 4 byte alignment
	units
	 * offset 1129
	 */
	uint8_t alignmentFill_at_1129[3];
	/**
	 * offset 1132
	 */
	brain_pin_e debugMapAveraging;
	/**
	 * need 4 byte alignment
	units
	 * offset 1133
	 */
	uint8_t alignmentFill_at_1133[3];
	/**
	 * offset 1136
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1137
	 */
	uint8_t alignmentFill_at_1137[3];
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 1140
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1141
	 */
	uint8_t alignmentFill_at_1141[3];
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 1144
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1145
	 */
	uint8_t alignmentFill_at_1145[3];
	/**
	 * See also starterControlPin
	 * offset 1148
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1149
	 */
	uint8_t alignmentFill_at_1149[3];
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	count
	 * offset 1152
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	%
	 * offset 1156
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	%
	 * offset 1158
	 */
	int16_t stepperParkingExtraSteps;
	/**
	ADC
	 * offset 1160
	 */
	uint16_t tps1SecondaryMin;
	/**
	ADC
	 * offset 1162
	 */
	uint16_t tps1SecondaryMax;
	/**
	rpm
	 * offset 1164
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	Seconds
	 * offset 1166
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 1168
	 */
	brain_pin_e triggerErrorPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1169
	 */
	uint8_t alignmentFill_at_1169[3];
	/**
	 * offset 1172
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1173
	 */
	uint8_t alignmentFill_at_1173[3];
	/**
	 * offset 1176
	 */
	output_pin_e acRelayPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1177
	 */
	uint8_t alignmentFill_at_1177[3];
	/**
	 * offset 1180
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1181
	 */
	uint8_t alignmentFill_at_1181[3];
	/**
	 * offset 1184
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 1216
	 */
	brain_pin_e spi1mosiPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1217
	 */
	uint8_t alignmentFill_at_1217[3];
	/**
	 * offset 1220
	 */
	brain_pin_e spi1misoPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1221
	 */
	uint8_t alignmentFill_at_1221[3];
	/**
	 * offset 1224
	 */
	brain_pin_e spi1sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1225
	 */
	uint8_t alignmentFill_at_1225[3];
	/**
	 * offset 1228
	 */
	brain_pin_e spi2mosiPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1229
	 */
	uint8_t alignmentFill_at_1229[3];
	/**
	 * offset 1232
	 */
	brain_pin_e spi2misoPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1233
	 */
	uint8_t alignmentFill_at_1233[3];
	/**
	 * offset 1236
	 */
	brain_pin_e spi2sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1237
	 */
	uint8_t alignmentFill_at_1237[3];
	/**
	 * offset 1240
	 */
	brain_pin_e spi3mosiPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1241
	 */
	uint8_t alignmentFill_at_1241[3];
	/**
	 * offset 1244
	 */
	brain_pin_e spi3misoPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1245
	 */
	uint8_t alignmentFill_at_1245[3];
	/**
	 * offset 1248
	 */
	brain_pin_e spi3sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1249
	 */
	uint8_t alignmentFill_at_1249[3];
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 1252
	 */
	brain_pin_e cdmInputPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1253
	 */
	uint8_t alignmentFill_at_1253[3];
	/**
	 * offset 1256
	 */
	brain_pin_e joystickCenterPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1257
	 */
	uint8_t alignmentFill_at_1257[3];
	/**
	 * offset 1260
	 */
	brain_pin_e joystickAPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1261
	 */
	uint8_t alignmentFill_at_1261[3];
	/**
	 * offset 1264
	 */
	brain_pin_e joystickBPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1265
	 */
	uint8_t alignmentFill_at_1265[3];
	/**
	 * offset 1268
	 */
	brain_pin_e joystickCPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1269
	 */
	uint8_t alignmentFill_at_1269[3];
	/**
	 * offset 1272
	 */
	brain_pin_e joystickDPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1273
	 */
	uint8_t alignmentFill_at_1273[3];
	/**
	 * offset 1276
	 */
	uart_device_e consoleUartDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1277
	 */
	uint8_t alignmentFill_at_1277[3];
	/**
	 * rusEFI console Sensor Sniffer mode
	 * offset 1280
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 1284
	 */
	maf_sensor_type_e mafSensorType;
	/**
	offset 1288 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 1288 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 1288 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 1288 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 1288 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 1288 bit 5 */
	bool unfinishedenableLaunchBoost : 1 {};
	/**
	offset 1288 bit 6 */
	bool unfinishedlaunchDisableBySpeed : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 1288 bit 7 */
	bool enableCanVss : 1 {};
	/**
	offset 1288 bit 8 */
	bool enableInnovateLC2 : 1 {};
	/**
	offset 1288 bit 9 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise. Use of this mode is required if you have a narrowband O2 sensor.
	offset 1288 bit 10 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 1288 bit 11 */
	bool tempBooleanForVerySpecialCases : 1 {};
	/**
	offset 1288 bit 12 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * enable vvt_details
	offset 1288 bit 13 */
	bool verboseVVTDecoding : 1 {};
	/**
	 * get invertCamVVTSignal
	offset 1288 bit 14 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	 * enable consumeObdSensors
	offset 1288 bit 15 */
	bool consumeObdSensors : 1 {};
	/**
	offset 1288 bit 16 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 1288 bit 17 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 1288 bit 18 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 1288 bit 19 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 1288 bit 20 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 1288 bit 21 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 1288 bit 22 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 1288 bit 23 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 1288 bit 24 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 1288 bit 25 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 1288 bit 26 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 1288 bit 27 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 1288 bit 28 */
	bool tcuEnabled : 1 {};
	/**
	offset 1288 bit 29 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coilss
	offset 1288 bit 30 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 1288 bit 31 */
	bool verboseCan2 : 1 {};
	/**
	 * offset 1292
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 1324
	 */
	output_pin_e boostControlPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1325
	 */
	uint8_t alignmentFill_at_1325[3];
	/**
	 * offset 1328
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1329
	 */
	uint8_t alignmentFill_at_1329[3];
	/**
	 * offset 1332
	 */
	switch_input_pin_e antiLagActivatePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1333
	 */
	uint8_t alignmentFill_at_1333[3];
	/**
	 * offset 1336
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1337
	 */
	uint8_t alignmentFill_at_1337[3];
	/**
	 * offset 1340
	 */
	pid_s boostPid;
	/**
	 * offset 1360
	 */
	boostType_e boostType;
	/**
	Hz
	 * offset 1364
	 */
	int boostPwmFrequency;
	/**
	 * offset 1368
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1372
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * Disabled above this speed
	Kph
	 * offset 1376
	 */
	int launchSpeedThreshold;
	/**
	 * Range from Launch Rpm for Timing Retard to activate
	RPM
	 * offset 1380
	 */
	int launchTimingRpmRange;
	/**
	 * Extra Fuel Added
	%
	 * offset 1384
	 */
	int launchFuelAdded;
	/**
	 * Duty Cycle for the Boost Solenoid
	%
	 * offset 1388
	 */
	int launchBoostDuty;
	/**
	 * RPM Range for Hard Cut
	rpm
	 * offset 1392
	 */
	int hardCutRpmRange;
	/**
	rpm
	 * offset 1396
	 */
	int launchAdvanceRpmRange;
	/**
	rpm
	 * offset 1400
	 */
	int launchTpsTreshold;
	/**
	rpm
	 * offset 1404
	 */
	float launchActivateDelay;
	/**
	 * offset 1408
	 */
	stft_s stft;
	/**
	 * offset 1472
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1504
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1536
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1568
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1600
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1606
	 */
	uint8_t alignmentFill_at_1606[2];
	/**
	 * offset 1608
	 */
	etb_function_e etbFunctions[ETB_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1610
	 */
	uint8_t alignmentFill_at_1610[2];
	/**
	 * offset 1612
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1613
	 */
	uint8_t alignmentFill_at_1613[3];
	/**
	 * offset 1616
	 */
	brain_pin_e drv8860_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 1617
	 */
	uint8_t alignmentFill_at_1617[3];
	/**
	 * offset 1620
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1621
	 */
	uint8_t alignmentFill_at_1621[3];
	/**
	 * offset 1624
	 */
	brain_pin_e drv8860_miso;
	/**
	 * need 4 byte alignment
	units
	 * offset 1625
	 */
	uint8_t alignmentFill_at_1625[1];
	/**
	volt
	 * offset 1626
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1642
	 */
	uint8_t alignmentFill_at_1642[2];
	/**
	 * offset 1644
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * Angle between cam sensor and VVT zero position
	 * set vvt_offset X
	value
	 * offset 1652
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1656
	 */
	float vvtOffsetsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 1668
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1716
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	ADC
	 * offset 1780
	 */
	uint16_t tps2SecondaryMin;
	/**
	ADC
	 * offset 1782
	 */
	uint16_t tps2SecondaryMax;
	/**
	offset 1784 bit 0 */
	bool disablePrimaryUart : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1784 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Print details into rusEFI console
	 * enable verbose_idle
	offset 1784 bit 2 */
	bool isVerboseIAC : 1 {};
	/**
	offset 1784 bit 3 */
	bool boardUseTachPullUp : 1 {};
	/**
	offset 1784 bit 4 */
	bool boardUseTempPullUp : 1 {};
	/**
	 * This options enables data for 'engine sniffer' tab in console, which comes at some CPU price
	offset 1784 bit 5 */
	bool isEngineChartEnabled : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1784 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1784 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1784 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write
	offset 1784 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1784 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1784 bit 11 */
	bool boardUse2stepPullDown : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1784 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1784 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1784 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1784 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1784 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	offset 1784 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1784 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1784 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1784 bit 20 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 1784 bit 21 */
	bool boardUseCamPullDown : 1 {};
	/**
	offset 1784 bit 22 */
	bool boardUseCamVrPullUp : 1 {};
	/**
	offset 1784 bit 23 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 1784 bit 24 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 1784 bit 25 */
	bool boardUseD4PullDown : 1 {};
	/**
	offset 1784 bit 26 */
	bool boardUseD5PullDown : 1 {};
	/**
	 * Sometimes we just have to shut the engine down. Use carefully!
	offset 1784 bit 27 */
	bool useFSIO5ForCriticalIssueEngineStop : 1 {};
	/**
	 * Sometimes we have to miss injection on purpose to attract driver's attention
	offset 1784 bit 28 */
	bool useFSIO4ForSeriousEngineWarning : 1 {};
	/**
	offset 1784 bit 29 */
	bool launchActivateInverted : 1 {};
	/**
	offset 1784 bit 30 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1784 bit 31 */
	bool skippedWheelOnCam : 1 {};
	/**
	 * offset 1788
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 1789
	 */
	uint8_t alignmentFill_at_1789[3];
	/**
	 * A/C button input
	 * offset 1792
	 */
	switch_input_pin_e acSwitch;
	/**
	 * need 4 byte alignment
	units
	 * offset 1793
	 */
	uint8_t alignmentFill_at_1793[3];
	/**
	 * offset 1796
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 1797
	 */
	uint8_t alignmentFill_at_1797[3];
	/**
	 * Expected neutral position
	%
	 * offset 1800
	 */
	uint8_t etbNeutralPosition;
	/**
	 * need 4 byte alignment
	units
	 * offset 1801
	 */
	uint8_t alignmentFill_at_1801[3];
	/**
	 * See also idleRpmPid
	 * offset 1804
	 */
	idle_mode_e idleMode;
	/**
	 * Enable fuel injection - This is default off for new projects as a safety feature, set to "true" to enable fuel injection and further injector settings.
	offset 1808 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	 * Enable ignition - This is default off for new projects as a safety feature, set to "true" to enable ignition and further ignition settings.
	offset 1808 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1808 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1808 bit 3 */
	bool complexWallModel : 1 {};
	/**
	offset 1808 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1808 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1808 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1808 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1808 bit 8 */
	bool unused1476b8 : 1 {};
	/**
	offset 1808 bit 9 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1808 bit 10 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * enable trigger_details
	offset 1808 bit 11 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1808 bit 12 */
	bool isManualSpinningMode : 1 {};
	/**
	 * This is needed if your coils are individually wired and you wish to use batch injection.
	 * enable two_wire_batch_injection
	offset 1808 bit 13 */
	bool twoWireBatchInjection : 1 {};
	/**
	 * VR sensors are only precise on rising front
	 * enable trigger_only_front
	offset 1808 bit 14 */
	bool useOnlyRisingEdgeForTrigger : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1808 bit 15 */
	bool twoWireBatchIgnition : 1 {};
	/**
	offset 1808 bit 16 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1808 bit 17 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1808 bit 18 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1808 bit 19 */
	bool flexCranking : 1 {};
	/**
	offset 1808 bit 20 */
	bool unused1476b20 : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1808 bit 21 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1808 bit 22 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1808 bit 23 */
	bool launchSmoothRetard : 1 {};
	/**
	offset 1808 bit 24 */
	bool unused_1484_bit_24 : 1 {};
	/**
	offset 1808 bit 25 */
	bool unused_1484_bit_25 : 1 {};
	/**
	offset 1808 bit 26 */
	bool unused_1484_bit_26 : 1 {};
	/**
	offset 1808 bit 27 */
	bool unused_1484_bit_27 : 1 {};
	/**
	offset 1808 bit 28 */
	bool unused_1484_bit_28 : 1 {};
	/**
	offset 1808 bit 29 */
	bool unused_1484_bit_29 : 1 {};
	/**
	offset 1808 bit 30 */
	bool unused_1484_bit_30 : 1 {};
	/**
	offset 1808 bit 31 */
	bool unused_1484_bit_31 : 1 {};
	/**
	count
	 * offset 1812
	 */
	uint32_t engineChartSize;
	/**
	mult
	 * offset 1816
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1820
	 */
	brain_pin_e camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1821
	 */
	uint8_t alignmentFill_at_1821[3];
	/**
	 * offset 1824
	 */
	uint8_t camInputsDebugPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * need 4 byte alignment
	units
	 * offset 1827
	 */
	uint8_t alignmentFill_at_1827[1];
	/**
	 * Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	RPM
	 * offset 1828
	 */
	int16_t acIdleRpmBump;
	/**
	 * set warningPeriod X
	seconds
	 * offset 1830
	 */
	int16_t warningPeriod;
	/**
	angle
	 * offset 1832
	 */
	float knockDetectionWindowStart;
	/**
	angle
	 * offset 1836
	 */
	float knockDetectionWindowEnd;
	/**
	ms
	 * offset 1840
	 */
	float idleStepperReactionTime;
	/**
	count
	 * offset 1844
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	sec
	 * offset 1848
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	index
	 * offset 1852
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	kPa
	 * offset 1856
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	RPM
	 * offset 1872
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	ratio
	 * offset 1888
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	Ratio
	 * offset 1952
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	%
	 * offset 1984
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 2016
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	Seconds
	 * offset 2020
	 */
	float wwaeTau;
	/**
	 * offset 2024
	 */
	pid_s alternatorControl;
	/**
	 * offset 2044
	 */
	pid_s etb;
	/**
	 * offset 2064
	 */
	brain_pin_e triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 2067
	 */
	uint8_t alignmentFill_at_2067[1];
	/**
	 * offset 2068
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2069
	 */
	uint8_t alignmentFill_at_2069[1];
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * set tps2_min X
	ADC
	 * offset 2070
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps2_max X
	ADC
	 * offset 2072
	 */
	int16_t tps2Max;
	/**
	 * need 4 byte alignment
	units
	 * offset 2074
	 */
	uint8_t alignmentFill_at_2074[2];
	/**
	 * See also startStopButtonPin
	 * offset 2076
	 */
	output_pin_e starterControlPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2077
	 */
	uint8_t alignmentFill_at_2077[3];
	/**
	 * offset 2080
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2081
	 */
	uint8_t alignmentFill_at_2081[3];
	/**
	 * offset 2084
	 */
	brain_pin_e mc33816_flag0;
	/**
	 * need 4 byte alignment
	units
	 * offset 2085
	 */
	uint8_t alignmentFill_at_2085[3];
	/**
	Pulse
	 * offset 2088
	 */
	uint8_t tachPulsePerRev;
	/**
	 * need 4 byte alignment
	units
	 * offset 2089
	 */
	uint8_t alignmentFill_at_2089[3];
	/**
	 * kPa value which is too low to be true
	kPa
	 * offset 2092
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	kPa
	 * offset 2096
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	ms
	 * offset 2100
	 */
	uint16_t multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	ms
	 * offset 2102
	 */
	uint16_t multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 2104
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	Fraction
	 * offset 2124
	 */
	float wwaeBeta;
	/**
	 * blue LED on many rusEFI boards.
	 * Blue Communication LED which is expected to blink at 50% duty cycle during normal board operation.
	 * If USB communication cable is connected Blue LED starts to blink faster.
	 * offset 2128
	 */
	brain_pin_e communicationLedPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2129
	 */
	uint8_t alignmentFill_at_2129[3];
	/**
	 * Green LED on many rusEFI boards.
	 * Off if engine is stopped, blinks if engine is cranking, solid if engine is running.
	 * offset 2132
	 */
	brain_pin_e runningLedPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2133
	 */
	uint8_t alignmentFill_at_2133[3];
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 2136
	 */
	brain_pin_e binarySerialTxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2137
	 */
	uint8_t alignmentFill_at_2137[3];
	/**
	 * offset 2140
	 */
	brain_pin_e binarySerialRxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2141
	 */
	uint8_t alignmentFill_at_2141[3];
	/**
	 * offset 2144
	 */
	brain_pin_e auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 2146
	 */
	uint8_t alignmentFill_at_2146[2];
	/**
	 * offset 2148
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2149
	 */
	uint8_t alignmentFill_at_2149[3];
	/**
	 * offset 2152
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2153
	 */
	uint8_t alignmentFill_at_2153[3];
	/**
	voltage
	 * offset 2156
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 2160
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	seconds
	 * offset 2164
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	RPM
	 * offset 2166
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * CLT-based target RPM for automatic idle controller
	C
	 * offset 2168
	 */
	float cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	RPM
	 * offset 2232
	 */
	scaled_channel<uint8_t, 1, 50> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	Volts
	 * offset 2248
	 */
	float targetVBatt;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	%
	 * offset 2252
	 */
	float alternatorOffAboveTps;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	cycles
	 * offset 2256
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	percent
	 * offset 2258
	 */
	int16_t iacByTpsTaper;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_tx_pin X
	 * offset 2260
	 */
	brain_pin_e auxSerialTxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2261
	 */
	uint8_t alignmentFill_at_2261[3];
	/**
	 * offset 2264
	 */
	brain_pin_e warningLedPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2265
	 */
	uint8_t alignmentFill_at_2265[3];
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_rx_pin X
	 * offset 2268
	 */
	brain_pin_e auxSerialRxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2269
	 */
	uint8_t alignmentFill_at_2269[3];
	/**
	 * offset 2272
	 */
	brain_pin_e LIS302DLCsPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2273
	 */
	uint8_t alignmentFill_at_2273[3];
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	sec
	 * offset 2276
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * need 4 byte alignment
	units
	 * offset 2277
	 */
	uint8_t alignmentFill_at_2277[3];
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 2280
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * need 4 byte alignment
	units
	 * offset 2281
	 */
	uint8_t alignmentFill_at_2281[3];
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 2284
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * need 4 byte alignment
	units
	 * offset 2285
	 */
	uint8_t alignmentFill_at_2285[3];
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	sec
	 * offset 2288
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * need 4 byte alignment
	units
	 * offset 2289
	 */
	uint8_t alignmentFill_at_2289[3];
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	roc
	 * offset 2292
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 2296
	 */
	brain_input_pin_e auxSpeedSensorInputPin[2];
	/**
	 * need 4 byte alignment
	units
	 * offset 2298
	 */
	uint8_t alignmentFill_at_2298[2];
	/**
	 * offset 2300
	 */
	uint8_t totalGearsCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 2301
	 */
	uint8_t alignmentFill_at_2301[3];
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 2304
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2305
	 */
	uint8_t alignmentFill_at_2305[3];
	/**
	 * Band rate for primary TTL
	BPs
	 * offset 2308
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	roc
	 * offset 2312
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	coeff
	 * offset 2316
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	coeff
	 * offset 2320
	 */
	float slowAdcAlpha;
	/**
	 * See http://rusefi.com/s/debugmode
	 * 
	 * set debug_mode X
	 * offset 2324
	 */
	debug_mode_e debugMode;
	/**
	BPs
	 * offset 2328
	 */
	uint32_t auxSerialSpeed;
	/**
	voltage
	 * offset 2332
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 2336
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * set can_baudrate
	 * offset 2340
	 */
	can_baudrate_e canBaudRate;
	/**
	 * need 4 byte alignment
	units
	 * offset 2341
	 */
	uint8_t alignmentFill_at_2341[3];
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2344
	 */
	ve_override_e veOverrideMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2345
	 */
	uint8_t alignmentFill_at_2345[3];
	/**
	 * offset 2348
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * need 4 byte alignment
	units
	 * offset 2349
	 */
	uint8_t alignmentFill_at_2349[3];
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2352
	 */
	load_override_e afrOverrideMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2353
	 */
	uint8_t alignmentFill_at_2353[3];
	/**
	A
	 * offset 2356
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	 * need 4 byte alignment
	units
	 * offset 2357
	 */
	uint8_t alignmentFill_at_2357[3];
	/**
	A
	 * offset 2360
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * need 4 byte alignment
	units
	 * offset 2361
	 */
	uint8_t alignmentFill_at_2361[3];
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	us
	 * offset 2364
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * need 4 byte alignment
	units
	 * offset 2365
	 */
	uint8_t alignmentFill_at_2365[3];
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	ms
	 * offset 2368
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * need 4 byte alignment
	units
	 * offset 2369
	 */
	uint8_t alignmentFill_at_2369[3];
	/**
	offset 2372 bit 0 */
	bool unused1740b0 : 1 {};
	/**
	offset 2372 bit 1 */
	bool unused1740b1 : 1 {};
	/**
	offset 2372 bit 2 */
	bool unused1740b2 : 1 {};
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 2372 bit 3 */
	bool stepperDcInvertedPins : 1 {};
	/**
	offset 2372 bit 4 */
	bool unused1127 : 1 {};
	/**
	offset 2372 bit 5 */
	bool unused1128 : 1 {};
	/**
	offset 2372 bit 6 */
	bool unused1129 : 1 {};
	/**
	offset 2372 bit 7 */
	bool unused1130 : 1 {};
	/**
	offset 2372 bit 8 */
	bool unusedBit_687_8 : 1 {};
	/**
	offset 2372 bit 9 */
	bool unusedBit_687_9 : 1 {};
	/**
	offset 2372 bit 10 */
	bool unusedBit_687_10 : 1 {};
	/**
	offset 2372 bit 11 */
	bool unusedBit_687_11 : 1 {};
	/**
	offset 2372 bit 12 */
	bool unusedBit_687_12 : 1 {};
	/**
	offset 2372 bit 13 */
	bool unusedBit_687_13 : 1 {};
	/**
	offset 2372 bit 14 */
	bool unusedBit_687_14 : 1 {};
	/**
	offset 2372 bit 15 */
	bool unusedBit_687_15 : 1 {};
	/**
	offset 2372 bit 16 */
	bool unusedBit_687_16 : 1 {};
	/**
	offset 2372 bit 17 */
	bool unusedBit_687_17 : 1 {};
	/**
	offset 2372 bit 18 */
	bool unusedBit_687_18 : 1 {};
	/**
	offset 2372 bit 19 */
	bool unusedBit_687_19 : 1 {};
	/**
	offset 2372 bit 20 */
	bool unusedBit_687_20 : 1 {};
	/**
	offset 2372 bit 21 */
	bool unusedBit_687_21 : 1 {};
	/**
	offset 2372 bit 22 */
	bool unusedBit_687_22 : 1 {};
	/**
	offset 2372 bit 23 */
	bool unusedBit_687_23 : 1 {};
	/**
	offset 2372 bit 24 */
	bool unusedBit_687_24 : 1 {};
	/**
	offset 2372 bit 25 */
	bool unusedBit_687_25 : 1 {};
	/**
	offset 2372 bit 26 */
	bool unusedBit_687_26 : 1 {};
	/**
	offset 2372 bit 27 */
	bool unusedBit_687_27 : 1 {};
	/**
	offset 2372 bit 28 */
	bool unusedBit_687_28 : 1 {};
	/**
	offset 2372 bit 29 */
	bool unusedBit_687_29 : 1 {};
	/**
	offset 2372 bit 30 */
	bool unusedBit_687_30 : 1 {};
	/**
	offset 2372 bit 31 */
	bool unusedBit_687_31 : 1 {};
	/**
	ms
	 * offset 2376
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOffTime;
	/**
	 * need 4 byte alignment
	units
	 * offset 2377
	 */
	uint8_t alignmentFill_at_2377[3];
	/**
	count
	 * offset 2380
	 */
	uint8_t benchTestCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 2381
	 */
	uint8_t alignmentFill_at_2381[3];
	/**
	ms
	 * offset 2384
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOnTime;
	/**
	 * need 4 byte alignment
	units
	 * offset 2385
	 */
	uint8_t alignmentFill_at_2385[3];
	/**
	 * offset 2388
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2389
	 */
	uint8_t alignmentFill_at_2389[3];
	/**
	 * set_can2_tx_pin X
	 * offset 2392
	 */
	brain_pin_e can2TxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2393
	 */
	uint8_t alignmentFill_at_2393[3];
	/**
	 * set_can2_rx_pin X
	 * offset 2396
	 */
	brain_pin_e can2RxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2397
	 */
	uint8_t alignmentFill_at_2397[3];
	/**
	 * offset 2400
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2401
	 */
	uint8_t alignmentFill_at_2401[3];
	/**
	 * offset 2404
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * need 4 byte alignment
	units
	 * offset 2405
	 */
	uint8_t alignmentFill_at_2405[3];
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2408
	 */
	load_override_e ignOverrideMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2409
	 */
	uint8_t alignmentFill_at_2409[3];
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 2412
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * need 4 byte alignment
	units
	 * offset 2413
	 */
	uint8_t alignmentFill_at_2413[3];
	/**
	 * offset 2416
	 */
	output_pin_e hpfpValvePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2417
	 */
	uint8_t alignmentFill_at_2417[3];
	/**
	 * offset 2420
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2421
	 */
	uint8_t alignmentFill_at_2421[3];
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	kPa (absolute)
	 * offset 2424
	 */
	float boostCutPressure;
	/**
	kg/h
	 * offset 2428
	 */
	uint8_t tchargeBins[16];
	/**
	ratio
	 * offset 2444
	 */
	uint8_t tchargeValues[16];
	/**
	counter
	 * offset 2460
	 */
	float unusedMapAccelTaperBins[8];
	/**
	 * Fixed timing, useful for TDC testing
	deg
	 * offset 2492
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	v
	 * offset 2496
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	v
	 * offset 2500
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	value
	 * offset 2504
	 */
	float egoValueShift;
	/**
	 * offset 2508
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 2509
	 */
	uint8_t alignmentFill_at_2509[3];
	/**
	 * offset 2512
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 2513
	 */
	uint8_t alignmentFill_at_2513[3];
	/**
	 * offset 2516
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2517
	 */
	uint8_t alignmentFill_at_2517[3];
	/**
	 * offset 2520
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2521
	 */
	uint8_t alignmentFill_at_2521[3];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	percent
	 * offset 2524
	 */
	int crankingIACposition;
	/**
	 * offset 2528
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 2532
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 2536
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 2540
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 2544
	 */
	pwm_freq_t vvtOutputFrequency[CAMS_PER_BANK];
	/**
	 * need 4 byte alignment
	units
	 * offset 2546
	 */
	uint8_t alignmentFill_at_2546[2];
	/**
	 * Additional idle % when fan #1 is active
	%
	 * offset 2548
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * need 4 byte alignment
	units
	 * offset 2549
	 */
	uint8_t alignmentFill_at_2549[3];
	/**
	Hz
	 * offset 2552
	 */
	int alternatorPwmFrequency;
	/**
	 * Narrow Band WBO Approximation
	V
	 * offset 2556
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	ratio
	 * offset 2588
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * set vvt_mode X
	 * offset 2620
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * need 4 byte alignment
	units
	 * offset 2621
	 */
	uint8_t alignmentFill_at_2621[3];
	/**
	 * offset 2624
	 */
	uint8_t vvtModePadding[CAMS_PER_BANK_padding];
	/**
	 * need 4 byte alignment
	units
	 * offset 2625
	 */
	uint8_t alignmentFill_at_2625[3];
	/**
	 * Additional idle % when fan #2 is active
	%
	 * offset 2628
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * need 4 byte alignment
	units
	 * offset 2629
	 */
	uint8_t alignmentFill_at_2629[3];
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	sec
	 * offset 2632
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * need 4 byte alignment
	units
	 * offset 2633
	 */
	uint8_t alignmentFill_at_2633[3];
	/**
	 * offset 2636
	 */
	adc_channel_e auxAnalogInputs[AUX_ANALOG_INPUT_COUNT];
	/**
	 * offset 2644
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * CLT-based timing correction
	C
	 * offset 2656
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	degree
	 * offset 2688
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2720
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2721
	 */
	uint8_t alignmentFill_at_2721[3];
	/**
	 * offset 2724
	 */
	pin_output_mode_e LIS302DLCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2725
	 */
	uint8_t alignmentFill_at_2725[3];
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 2728
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2729
	 */
	uint8_t alignmentFill_at_2729[3];
	/**
	 * offset 2732
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2733
	 */
	uint8_t alignmentFill_at_2733[3];
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	kPa
	 * offset 2736
	 */
	float fuelReferencePressure;
	/**
	 * Fuel multiplier (enrichment) immediately after engine start
	mult
	 * offset 2740
	 */
	float postCrankingFactor;
	/**
	 * Time over which to taper out after start enrichment
	seconds
	 * offset 2744
	 */
	float postCrankingDurationSec;
	/**
	 * offset 2748
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 2780
	 */
	ThermistorConf auxTempSensor2;
	/**
	Deg
	 * offset 2812
	 */
	int16_t knockSamplingDuration;
	/**
	Hz
	 * offset 2814
	 */
	int16_t etbFreq;
	/**
	 * offset 2816
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 2836
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * need 4 byte alignment
	units
	 * offset 2837
	 */
	uint8_t alignmentFill_at_2837[3];
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	%
	 * offset 2840
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * need 4 byte alignment
	units
	 * offset 2841
	 */
	uint8_t alignmentFill_at_2841[3];
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	%
	 * offset 2844
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * need 4 byte alignment
	units
	 * offset 2845
	 */
	uint8_t alignmentFill_at_2845[3];
	/**
	 * offset 2848
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 2849
	 */
	uint8_t alignmentFill_at_2849[3];
	/**
	 * per-cylinder timing correction
	deg
	 * offset 2852
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	seconds
	 * offset 2900
	 */
	float idlePidActivationTime;
	/**
	 * offset 2904
	 */
	pin_mode_e spi1SckMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2905
	 */
	uint8_t alignmentFill_at_2905[3];
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 2908
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2909
	 */
	uint8_t alignmentFill_at_2909[3];
	/**
	 * offset 2912
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2913
	 */
	uint8_t alignmentFill_at_2913[3];
	/**
	 * offset 2916
	 */
	pin_mode_e spi2SckMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2917
	 */
	uint8_t alignmentFill_at_2917[3];
	/**
	 * offset 2920
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2921
	 */
	uint8_t alignmentFill_at_2921[3];
	/**
	 * offset 2924
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2925
	 */
	uint8_t alignmentFill_at_2925[3];
	/**
	 * offset 2928
	 */
	pin_mode_e spi3SckMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2929
	 */
	uint8_t alignmentFill_at_2929[3];
	/**
	 * offset 2932
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2933
	 */
	uint8_t alignmentFill_at_2933[3];
	/**
	 * offset 2936
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2937
	 */
	uint8_t alignmentFill_at_2937[3];
	/**
	 * offset 2940
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2941
	 */
	uint8_t alignmentFill_at_2941[3];
	/**
	 * ResetB
	 * offset 2944
	 */
	brain_pin_e mc33816_rstb;
	/**
	 * need 4 byte alignment
	units
	 * offset 2945
	 */
	uint8_t alignmentFill_at_2945[3];
	/**
	 * offset 2948
	 */
	brain_pin_e mc33816_driven;
	/**
	 * need 4 byte alignment
	units
	 * offset 2949
	 */
	uint8_t alignmentFill_at_2949[3];
	/**
	 * Brake pedal switch
	 * offset 2952
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2953
	 */
	uint8_t alignmentFill_at_2953[3];
	/**
	 * lambda input
	 * offset 2956
	 */
	adc_channel_e cj125ua;
	/**
	 * need 4 byte alignment
	units
	 * offset 2957
	 */
	uint8_t alignmentFill_at_2957[3];
	/**
	 * heater input
	 * offset 2960
	 */
	adc_channel_e cj125ur;
	/**
	 * need 4 byte alignment
	units
	 * offset 2961
	 */
	uint8_t alignmentFill_at_2961[3];
	/**
	 * offset 2964
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2965
	 */
	uint8_t alignmentFill_at_2965[3];
	/**
	 * offset 2968
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 2988
	 */
	float injectorCorrectionPolynomial[8];
	/**
	C
	 * offset 3020
	 */
	int8_t primeBins[8];
	/**
	 * offset 3028
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 3052
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 3053
	 */
	uint8_t alignmentFill_at_3053[3];
	/**
	 * offset 3056
	 */
	output_pin_e fan2Pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 3057
	 */
	uint8_t alignmentFill_at_3057[3];
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 3060
	 */
	uint8_t fan2OnTemperature;
	/**
	 * need 4 byte alignment
	units
	 * offset 3061
	 */
	uint8_t alignmentFill_at_3061[3];
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 3064
	 */
	uint8_t fan2OffTemperature;
	/**
	 * need 4 byte alignment
	units
	 * offset 3065
	 */
	uint8_t alignmentFill_at_3065[3];
	/**
	x
	 * offset 3068
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 3132
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	x
	 * offset 3196
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 3260
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	x
	 * offset 3324
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3356
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	x
	 * offset 3388
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3420
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	 * offset 3452
	 */
	brain_pin_e stepperEnablePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 3453
	 */
	uint8_t alignmentFill_at_3453[3];
	/**
	 * offset 3456
	 */
	brain_pin_e tle8888_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 3457
	 */
	uint8_t alignmentFill_at_3457[3];
	/**
	 * offset 3460
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3461
	 */
	uint8_t alignmentFill_at_3461[3];
	/**
	 * offset 3464
	 */
	brain_pin_e mc33816_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 3465
	 */
	uint8_t alignmentFill_at_3465[3];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	RPM
	 * offset 3468
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	deg
	 * offset 3484
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * todo: more comments
	 * offset 3500
	 */
	brain_pin_e servoOutputPins[SERVO_COUNT];
	/**
	 * This sets the RPM above which fuel cut is active.
	rpm
	 * offset 3508
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	rpm
	 * offset 3510
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	%
	 * offset 3512
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	C
	 * offset 3514
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	%
	 * offset 3516
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	kPa
	 * offset 3518
	 */
	int16_t coastingFuelCutMap;
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	C
	 * offset 3520
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	%
	 * offset 3584
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 3648
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 3672
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * CLT-based target RPM for hard limit depending on CLT like on Lexus LFA
	C
	 * offset 3696
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 3700
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * offset 3708
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	x
	 * offset 3804
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3836
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	x
	 * offset 3868
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3900
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	 * offset 3932
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 3996
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 4124
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 4128
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	kg/h
	 * offset 4132
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	deg/sec
	 * offset 4136
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	deg/sec
	 * offset 4140
	 */
	float tChargeAirDecrLimit;
	/**
	 * offset 4144
	 */
	tChargeMode_e tChargeMode;
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	target TPS position
	 * offset 4148
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	ETB duty cycle bias
	 * offset 4180
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	 * offset 4212
	 */
	float hip9011Gain;
	/**
	 * iTerm min value
	 * offset 4216
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 4218
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 4220
	 */
	pid_s idleTimingPid;
	/**
	 * If the RPM closer to target than this value, disable timing correction to prevent oscillation
	RPM
	 * offset 4240
	 */
	int16_t idleTimingPidDeadZone;
	/**
	 * A delay in cycles between fuel-enrich. portions
	cycles
	 * offset 4242
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	coef
	 * offset 4244
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 4248
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 4249
	 */
	uint8_t alignmentFill_at_4249[3];
	/**
	 * offset 4252
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 4253
	 */
	uint8_t alignmentFill_at_4253[1];
	/**
	 * iTerm min value
	 * offset 4254
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 4256
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 4257
	 */
	uint8_t alignmentFill_at_4257[3];
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	:1
	 * offset 4260
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * need 4 byte alignment
	units
	 * offset 4261
	 */
	uint8_t alignmentFill_at_4261[1];
	/**
	 * iTerm max value
	 * offset 4262
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	%
	 * offset 4264
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 4268
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	mg
	 * offset 4280
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[8];
	/**
	 * Trigger comparator center point voltage
	V
	 * offset 4288
	 */
	uint8_t triggerCompCenterVolt;
	/**
	 * need 4 byte alignment
	units
	 * offset 4289
	 */
	uint8_t alignmentFill_at_4289[3];
	/**
	 * Trigger comparator hysteresis voltage (Min)
	V
	 * offset 4292
	 */
	uint8_t triggerCompHystMin;
	/**
	 * need 4 byte alignment
	units
	 * offset 4293
	 */
	uint8_t alignmentFill_at_4293[3];
	/**
	 * Trigger comparator hysteresis voltage (Max)
	V
	 * offset 4296
	 */
	uint8_t triggerCompHystMax;
	/**
	 * need 4 byte alignment
	units
	 * offset 4297
	 */
	uint8_t alignmentFill_at_4297[3];
	/**
	 * VR-sensor saturation RPM
	RPM
	 * offset 4300
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * need 4 byte alignment
	units
	 * offset 4301
	 */
	uint8_t alignmentFill_at_4301[3];
	/**
	 * offset 4304
	 */
	pid_s idleRpmPid2;
	/**
	%
	 * offset 4324
	 */
	uint8_t iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	Load
	 * offset 4388
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	RPM
	 * offset 4396
	 */
	scaled_channel<uint8_t, 1, 50> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * set can_vss X
	 * offset 4404
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * offset 4408
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	mA
	 * offset 4824
	 */
	uint16_t mc33_i_boost;
	/**
	mA
	 * offset 4826
	 */
	uint16_t mc33_i_peak;
	/**
	mA
	 * offset 4828
	 */
	uint16_t mc33_i_hold;
	/**
	us
	 * offset 4830
	 */
	uint16_t mc33_t_max_boost;
	/**
	us
	 * offset 4832
	 */
	uint16_t mc33_t_peak_off;
	/**
	us
	 * offset 4834
	 */
	uint16_t mc33_t_peak_tot;
	/**
	us
	 * offset 4836
	 */
	uint16_t mc33_t_bypass;
	/**
	us
	 * offset 4838
	 */
	uint16_t mc33_t_hold_off;
	/**
	us
	 * offset 4840
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * need 4 byte alignment
	units
	 * offset 4842
	 */
	uint8_t alignmentFill_at_4842[2];
	/**
	 * offset 4844
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4845
	 */
	uint8_t alignmentFill_at_4845[3];
	/**
	 * offset 4848
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4849
	 */
	uint8_t alignmentFill_at_4849[3];
	/**
	 * offset 4852
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4853
	 */
	uint8_t alignmentFill_at_4853[3];
	/**
	 * offset 4856
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 4862
	 */
	uint8_t alignmentFill_at_4862[2];
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	dB
	 * offset 4864
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	from
	 * offset 4880
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	Percent
	 * offset 4952
	 */
	int8_t fuelTrim[MAX_CYLINDER_COUNT];
	/**
	to
	 * offset 4964
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	lobes/cam
	 * offset 5036
	 */
	uint8_t hpfpCamLobes;
	/**
	 * need 4 byte alignment
	units
	 * offset 5037
	 */
	uint8_t alignmentFill_at_5037[3];
	/**
	 * offset 5040
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * need 4 byte alignment
	units
	 * offset 5041
	 */
	uint8_t alignmentFill_at_5041[3];
	/**
	 * Crank angle ATDC of first lobe peak
	deg
	 * offset 5044
	 */
	uint8_t hpfpPeakPos;
	/**
	 * need 4 byte alignment
	units
	 * offset 5045
	 */
	uint8_t alignmentFill_at_5045[3];
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	deg
	 * offset 5048
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	units
	 * offset 5049
	 */
	uint8_t alignmentFill_at_5049[1];
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	cc
	 * offset 5050
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	deg
	 * offset 5052
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * need 4 byte alignment
	units
	 * offset 5053
	 */
	uint8_t alignmentFill_at_5053[3];
	/**
	 * offset 5056
	 */
	uint8_t issFilterReciprocal;
	/**
	 * need 4 byte alignment
	units
	 * offset 5057
	 */
	uint8_t alignmentFill_at_5057[1];
	/**
	%/kPa
	 * offset 5058
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	%/kPa/lobe
	 * offset 5060
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	kPa/s
	 * offset 5062
	 */
	uint16_t hpfpTargetDecay;
	/**
	%
	 * offset 5064
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	deg
	 * offset 5080
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	volts
	 * offset 5096
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	ms
	 * offset 5104
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	kPa
	 * offset 5120
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	load
	 * offset 5320
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	RPM
	 * offset 5340
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 5350
	 */
	uint8_t alignmentFill_at_5350[2];
	/**
	%
	 * offset 5352
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	cc/lobe
	 * offset 5452
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	RPM
	 * offset 5472
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 5482
	 */
	uint8_t alignmentFill_at_5482[2];
	/**
	TPS
	 * offset 5484
	 */
	uint8_t unusedTpsBins[24];
	/**
	 * offset 5508
	 */
	output_pin_e stepper_raw_output[4];
	/**
	ratio
	 * offset 5512
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[GEARS_COUNT];
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	ms
	 * offset 5528
	 */
	uint16_t vvtActivationDelayMs;
	/**
	RPM
	 * offset 5530
	 */
	uint16_t unusedShort;
	/**
	deg C
	 * offset 5532
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	/**
	 * offset 5540
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	/**
	 * offset 5548
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	/**
	kPa
	 * offset 5556
	 */
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	/**
	 * offset 5564
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 5572
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	/**
	Nm
	 * offset 5580
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_SIZE];
	/**
	RPM
	 * offset 5616
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE];
	/**
	Load
	 * offset 5628
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE];
	/**
	 * offset 5640
	 */
	GearControllerMode gearControllerMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5641
	 */
	uint8_t alignmentFill_at_5641[3];
	/**
	 * offset 5644
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5645
	 */
	uint8_t alignmentFill_at_5645[3];
	/**
	 * offset 5648
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 5672
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 5696
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * need 4 byte alignment
	units
	 * offset 5697
	 */
	uint8_t alignmentFill_at_5697[3];
	/**
	 * offset 5700
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5701
	 */
	uint8_t alignmentFill_at_5701[3];
	/**
	 * offset 5704
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * need 4 byte alignment
	units
	 * offset 5705
	 */
	uint8_t alignmentFill_at_5705[3];
	/**
	 * offset 5708
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5709
	 */
	uint8_t alignmentFill_at_5709[3];
	/**
	 * offset 5712
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * need 4 byte alignment
	units
	 * offset 5714
	 */
	uint8_t alignmentFill_at_5714[2];
	/**
	 * offset 5716
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 5717
	 */
	uint8_t alignmentFill_at_5717[3];
	/**
	 * offset 5720
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5721
	 */
	uint8_t alignmentFill_at_5721[3];
	/**
	 * offset 5724
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * need 4 byte alignment
	units
	 * offset 5726
	 */
	uint8_t alignmentFill_at_5726[2];
	/**
	 * offset 5728
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 5729
	 */
	uint8_t alignmentFill_at_5729[3];
	/**
	 * offset 5732
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5733
	 */
	uint8_t alignmentFill_at_5733[3];
	/**
	 * offset 5736
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * need 4 byte alignment
	units
	 * offset 5738
	 */
	uint8_t alignmentFill_at_5738[2];
	/**
	units
	 * offset 5740
	 */
	uint8_t mainUnusedEnd[222];
	/**
	 * need 4 byte alignment
	units
	 * offset 5962
	 */
	uint8_t alignmentFill_at_5962[2];
};
static_assert(sizeof(engine_configuration_s) == 5964);

// start of cyl_trim_s
struct cyl_trim_s {
	/**
	 * offset 0
	 */
	scaled_channel<int8_t, 5, 1> table[TRIM_SIZE][TRIM_SIZE];
};
static_assert(sizeof(cyl_trim_s) == 16);

// start of persistent_config_s
struct persistent_config_s {
	/**
	 * offset 0
	 */
	engine_configuration_s engineConfiguration;
	/**
	 * offset 5964
	 */
	error_message_t warning_message;
	/**
	C
	 * offset 6084
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	Seconds
	 * offset 6116
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	%
	 * offset 6148
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	Seconds
	 * offset 6180
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 6212
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	RPM
	 * offset 6276
	 */
	scaled_channel<uint8_t, 1, 50> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 6284
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	%
	 * offset 6348
	 */
	scaled_channel<uint8_t, 1, 1> boostTpsBins[BOOST_LOAD_COUNT];
	/**
	%
	 * offset 6356
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	%
	 * offset 6420
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	RPM
	 * offset 6428
	 */
	scaled_channel<uint8_t, 1, 50> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	C
	 * offset 6436
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	%
	 * offset 6468
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	RPM
	 * offset 6500
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	deg
	 * offset 6508
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	RPM
	 * offset 6540
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	/**
	load
	 * offset 6544
	 */
	scaled_channel<uint8_t, 1, 1> idleVeLoadBins[IDLE_VE_SIZE];
	/**
	%
	 * offset 6548
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	/**
	 * offset 6580
	 */
	lua_script_t luaScript;
	/**
	C
	 * offset 14580
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	ratio
	 * offset 14644
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	C
	 * offset 14708
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 14772
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 14836
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	C
	 * offset 14868
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	ratio
	 * offset 14900
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	counter
	 * offset 14932
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	C
	 * offset 14964
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	ratio
	 * offset 15028
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	kg/hour
	 * offset 15092
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	V
	 * offset 16116
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	deg
	 * offset 17140
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Temperature
	 * offset 17652
	 */
	float ignitionIatCorrLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 17716
	 */
	uint16_t ignitionIatCorrRpmBins[IGN_RPM_COUNT];
	/**
	deg
	 * offset 17748
	 */
	int16_t injectionPhase[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 18260
	 */
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18292
	 */
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	onoff
	 * offset 18324
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	 * offset 18384
	 */
	float unused17440;
	/**
	kPa
	 * offset 18388
	 */
	uint16_t mapEstimateTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	% TPS
	 * offset 18900
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18932
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 18964
	 */
	uint8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19028
	 */
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19044
	 */
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 19060
	 */
	uint8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19124
	 */
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19140
	 */
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	/**
	deg
	 * offset 19156
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 19668
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 19700
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	%
	 * offset 19732
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	kPa
	 * offset 20244
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 20276
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	lambda
	 * offset 20308
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	 * offset 20564
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 20596
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 20628
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	from
	 * offset 20884
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	to
	 * offset 20916
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	value
	 * offset 20948
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 21204
	 */
	uint16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 21220
	 */
	uint16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 21236
	 */
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 21300
	 */
	uint16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 21316
	 */
	uint16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 21332
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 21396
	 */
	uint16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 21412
	 */
	uint16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 21428
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 21492
	 */
	uint16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 21508
	 */
	uint16_t scriptTable4RpmBins[SCRIPT_TABLE_8];
	/**
	 * offset 21524
	 */
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 21532
	 */
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 21540
	 */
	cyl_trim_s ignTrims[12];
	/**
	 * offset 21732
	 */
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 21740
	 */
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 21748
	 */
	cyl_trim_s fuelTrims[12];
	/**
	ratio
	 * offset 21940
	 */
	uint16_t crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	/**
	Airmass
	 * offset 21956
	 */
	uint8_t tcu_pcAirmassBins[8];
	/**
	%
	 * offset 21964
	 */
	uint8_t tcu_pcValsR[8];
	/**
	%
	 * offset 21972
	 */
	uint8_t tcu_pcValsN[8];
	/**
	%
	 * offset 21980
	 */
	uint8_t tcu_pcVals1[8];
	/**
	%
	 * offset 21988
	 */
	uint8_t tcu_pcVals2[8];
	/**
	%
	 * offset 21996
	 */
	uint8_t tcu_pcVals3[8];
	/**
	%
	 * offset 22004
	 */
	uint8_t tcu_pcVals4[8];
	/**
	%
	 * offset 22012
	 */
	uint8_t tcu_pcVals12[8];
	/**
	%
	 * offset 22020
	 */
	uint8_t tcu_pcVals23[8];
	/**
	%
	 * offset 22028
	 */
	uint8_t tcu_pcVals34[8];
	/**
	%
	 * offset 22036
	 */
	uint8_t tcu_pcVals21[8];
	/**
	%
	 * offset 22044
	 */
	uint8_t tcu_pcVals32[8];
	/**
	%
	 * offset 22052
	 */
	uint8_t tcu_pcVals43[8];
	/**
	TPS
	 * offset 22060
	 */
	uint8_t tcu_tccTpsBins[8];
	/**
	MPH
	 * offset 22068
	 */
	uint8_t tcu_tccLockSpeed[8];
	/**
	MPH
	 * offset 22076
	 */
	uint8_t tcu_tccUnlockSpeed[8];
	/**
	KPH
	 * offset 22084
	 */
	uint8_t tcu_32SpeedBins[8];
	/**
	%
	 * offset 22092
	 */
	uint8_t tcu_32Vals[8];
};
static_assert(sizeof(persistent_config_s) == 22100);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on kinetis_gen_config.bat integration/rusefi_config.txt Mon Apr 25 03:44:07 UTC 2022
