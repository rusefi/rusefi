// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on kinetis_gen_config.bat integration/rusefi_config.txt Mon May 02 03:12:42 UTC 2022
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
	%
	 * offset 1
	 */
	int8_t maxRemove;
	/**
	 * Time constant for correction while in this cell: this sets responsiveness of the closed loop correction. A value of 5.0 means it will try to make most of the correction within 5 seconds, and a value of 1.0 will try to correct within 1 second.
	sec
	 * offset 2
	 */
	scaled_channel<uint16_t, 10, 1> timeConstant;
};
static_assert(sizeof(stft_cell_cfg_s) == 4);

// start of stft_s
struct stft_s {
	/**
	 * Below this RPM, the idle region is active
	RPM
	 * offset 0
	 */
	scaled_channel<uint8_t, 1, 50> maxIdleRegionRpm;
	/**
	 * Below this engine load, the overrun region is active
	load
	 * offset 1
	 */
	uint8_t maxOverrunLoad;
	/**
	 * Above this engine load, the power region is active
	load
	 * offset 2
	 */
	uint8_t minPowerLoad;
	/**
	 * When close to correct AFR, pause correction. This can improve stability by not changing the adjustment if the error is extremely small, but is not required.
	%
	 * offset 3
	 */
	scaled_channel<uint8_t, 10, 1> deadband;
	/**
	 * Below this temperature, correction is disabled.
	C
	 * offset 4
	 */
	int8_t minClt;
	/**
	 * Below this AFR, correction is paused
	afr
	 * offset 5
	 */
	scaled_channel<uint8_t, 10, 1> minAfr;
	/**
	 * Above this AFR, correction is paused
	afr
	 * offset 6
	 */
	scaled_channel<uint8_t, 10, 1> maxAfr;
	/**
	 * Delay after starting the engine before beginning closed loop correction.
	seconds
	 * offset 7
	 */
	uint8_t startupDelay;
	/**
	 * offset 8
	 */
	stft_cell_cfg_s cellCfgs[STFT_CELL_COUNT];
};
static_assert(sizeof(stft_s) == 24);

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
	Gpio mosiPin;
	/**
	 * offset 2
	 */
	Gpio misoPin;
	/**
	 * offset 4
	 */
	Gpio sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 6
	 */
	uint8_t alignmentFill_at_6[2];
};
static_assert(sizeof(spi_pins) == 8);

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
	%
	 * offset 2
	 */
	uint8_t dutyIfError;
	/**
	 * need 4 byte alignment
	units
	 * offset 3
	 */
	uint8_t alignmentFill_at_3[1];
	/**
	 * Select a frequency to run PWM at.
	 * Set this to 0hz to enable on-off mode.
	hz
	 * offset 4
	 */
	uint16_t pwmFrequency;
	/**
	 * Hysteresis: in on-off mode, turn the output on when the table value is above this duty.
	%
	 * offset 6
	 */
	uint8_t onAboveDuty;
	/**
	 * Hysteresis: in on-off mode, turn the output off when the table value is below this duty.
	%
	 * offset 7
	 */
	uint8_t offBelowDuty;
	/**
	 * Selects the load axis to use for the table.
	 * offset 8
	 */
	gppwm_channel_e loadAxis;
	/**
	unit
	 * offset 9
	 */
	uint8_t alignmentFill_map;
	/**
	load
	 * offset 10
	 */
	uint8_t loadBins[GPPWM_LOAD_COUNT];
	/**
	RPM
	 * offset 18
	 */
	scaled_channel<uint8_t, 1, 50> rpmBins[GPPWM_RPM_COUNT];
	/**
	duty
	 * offset 26
	 */
	uint8_t table[GPPWM_RPM_COUNT][GPPWM_LOAD_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 90
	 */
	uint8_t alignmentFill_at_90[2];
};
static_assert(sizeof(gppwm_channel) == 92);

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
	 * offset 9
	 */
	adc_channel_e hwChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2];
};
static_assert(sizeof(air_pressure_sensor_config_s) == 12);

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
static_assert(sizeof(MAP_sensor_config_s) == 140);

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
	volts
	 * offset 4
	 */
	float v1;
	/**
	kPa
	 * offset 8
	 */
	float value1;
	/**
	volts
	 * offset 12
	 */
	float v2;
	/**
	kPa
	 * offset 16
	 */
	float value2;
};
static_assert(sizeof(linear_sensor_s) == 20);

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
	/**
	 * need 4 byte alignment
	units
	 * offset 9
	 */
	uint8_t alignmentFill_at_9[3];
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
	volts
	 * offset 4
	 */
	float v1;
	/**
	AFR
	 * offset 8
	 */
	float value1;
	/**
	volts
	 * offset 12
	 */
	float v2;
	/**
	AFR
	 * offset 16
	 */
	float value2;
};
static_assert(sizeof(afr_sensor_s) == 20);

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
	units
	 * offset 11
	 */
	uint8_t alignmentFill_at_11[1];
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
	rpm
	 * offset 0
	 */
	scaled_channel<uint8_t, 1, 50> rpmBins[6];
	/**
	volts
	 * offset 6
	 */
	scaled_channel<uint8_t, 100, 1> values[6];
	/**
	 * offset 12
	 */
	Gpio pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 14
	 */
	uint8_t alignmentFill_at_14[2];
};
static_assert(sizeof(vr_threshold_s) == 16);

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
	 * set rpm_hard_limit X
	rpm
	 * offset 5
	 */
	scaled_channel<uint8_t, 1, 50> rpmHardLimit;
	/**
	 * A secondary Rev limit engaged by the driver to help launch the vehicle faster
	rpm
	 * offset 6
	 */
	scaled_channel<uint8_t, 1, 50> launchRpm;
	/**
	 * Engine sniffer would be disabled above this rpm
	 * set engineSnifferRpmThreshold X
	RPM
	 * offset 7
	 */
	scaled_channel<uint8_t, 1, 50> engineSnifferRpmThreshold;
	/**
	 * Disable multispark above this engine speed.
	rpm
	 * offset 8
	 */
	scaled_channel<uint8_t, 1, 50> multisparkMaxRpm;
	/**
	 * Above this RPM, disable AC. Set to 0 to disable check.
	rpm
	 * offset 9
	 */
	scaled_channel<uint8_t, 1, 50> maxAcRpm;
	/**
	 * Above this TPS, disable AC. Set to 0 to disable check.
	%
	 * offset 10
	 */
	uint8_t maxAcTps;
	/**
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	deg C
	 * offset 11
	 */
	uint8_t maxAcClt;
	/**
	RPM
	 * offset 12
	 */
	scaled_channel<uint8_t, 1, 50> knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	deg
	 * offset 28
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	count
	 * offset 29
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	RPM
	 * offset 30
	 */
	scaled_channel<uint8_t, 1, 50> sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 38
	 */
	uint8_t alignmentFill_at_38[2];
	/**
	ms
	 * offset 40
	 */
	float sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * offset 72
	 */
	injector_s injector;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 140 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On Ford vehicles one of the sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 140 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 140 bit 2 */
	bool isVerboseAuxPid1 : 1 {};
	/**
	offset 140 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 140 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 140 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 140 bit 6 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 140 bit 7 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 140 bit 8 */
	bool enableTrailingSparks : 1 {};
	/**
	 * enable cj125verbose/disable cj125verbose
	offset 140 bit 9 */
	bool isCJ125Verbose : 1 {};
	/**
	 * Is your UA CJ125 output wired to MCU via resistor divider? Ua can go over 3.3v but only at lambda >3, i.e very lean AFR above 44.1
	 * When exposed to free air and 17x gain, Ua will be 4.17 volt
	offset 140 bit 10 */
	bool cj125isUaDivided : 1 {};
	/**
	offset 140 bit 11 */
	bool cj125isLsu49 : 1 {};
	/**
	 * TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 140 bit 12 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 140 bit 13 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 140 bit 14 */
	bool useEeprom : 1 {};
	/**
	 * Is your UR CJ125 output wired to MCU via resistor divider?
	 * Looks like 3v range should be enough, divider generally not needed.
	offset 140 bit 15 */
	bool cj125isUrDivided : 1 {};
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 140 bit 16 */
	bool useCicPidForIdle : 1 {};
	/**
	offset 140 bit 17 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 140 bit 18 */
	bool useInstantRpmForIdle : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 140 bit 19 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 140 bit 20 */
	bool launchControlEnabled : 1 {};
	/**
	offset 140 bit 21 */
	bool rollingLaunchEnabled : 1 {};
	/**
	offset 140 bit 22 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 140 bit 23 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	 * 
	 * enable logic_level_trigger
	offset 140 bit 24 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 140 bit 25 */
	bool useTLE8888_stepper : 1 {};
	/**
	 * If enabled, the MAP estimate table will be used if the MAP sensor fails to estimate manifold pressure based on RPM and TPS.
	offset 140 bit 26 */
	bool enableMapEstimationTableFallback : 1 {};
	/**
	offset 140 bit 27 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 140 bit 28 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 140 bit 29 */
	bool artificialTestMisfire : 1 {};
	/**
	offset 140 bit 30 */
	bool issue_294_31 : 1 {};
	/**
	offset 140 bit 31 */
	bool unusedBit_47_31 : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * set tps_min X
	ADC
	 * offset 144
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * set tps_max X
	ADC
	 * offset 146
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 148
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 150
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 152
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	ms
	 * offset 160
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	rpm
	 * offset 164
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	rpm
	 * offset 166
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 168
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 308
	 */
	ThermistorConf clt;
	/**
	 * offset 340
	 */
	ThermistorConf iat;
	/**
	deg
	 * offset 372
	 */
	int launchTimingRetard;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	integer
	 * offset 376
	 */
	int hip9011PrescalerAndSDO;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	kHz
	 * offset 380
	 */
	float knockBandCustom;
	/**
	 * offset 384
	 */
	specs_s specs;
	/**
	 * Cylinder diameter in mm.
	mm
	 * offset 396
	 */
	float cylinderBore;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS commonly only used for NA engines
	 * Speed Density requires MAP sensor and is the default choice for may installs
	 * MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	 * offset 400
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * offset 401
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 404
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups. If your injectors are individually wired you will also need to enable "Two wire batch emulation". 
	 * set injection_mode X
	 * See also twoWireBatchInjection
	 * offset 405
	 */
	injection_mode_e injectionMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 406
	 */
	uint8_t alignmentFill_at_406[2];
	/**
	 * this is about deciding when the injector starts it's squirt
	 * See also injectionPhase map
	 * todo: do we need even need this since we have the map anyway?
	deg
	 * offset 408
	 */
	angle_t extraInjectionOffset;
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	deg
	 * offset 412
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 416
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	count
	 * offset 417
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	kph
	 * offset 418
	 */
	uint8_t maxIdleVss;
	/**
	 * need 4 byte alignment
	units
	 * offset 419
	 */
	uint8_t alignmentFill_at_419[1];
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	kPa
	 * offset 420
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 422
	 */
	timing_mode_e timingMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 423
	 */
	uint8_t alignmentFill_at_423[1];
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	RPM
	 * offset 424
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchnization point before TDC and negative in case of synchnization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * set global_trigger_offset_angle X
	deg btdc
	 * offset 428
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	coef
	 * offset 432
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	coef
	 * offset 436
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 440
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 444
	 */
	float fanOffTemperature;
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	revs/km
	 * offset 448
	 */
	float driveWheelRevPerKm;
	/**
	 * set can_mode X
	 * offset 452
	 */
	can_nbc_e canNbcType;
	/**
	 * need 4 byte alignment
	units
	 * offset 453
	 */
	uint8_t alignmentFill_at_453[3];
	/**
	 * CANbus thread period in ms
	ms
	 * offset 456
	 */
	int canSleepPeriodMs;
	/**
	 * offset 460
	 */
	display_mode_e displayMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 461
	 */
	uint8_t alignmentFill_at_461[3];
	/**
	index
	 * offset 464
	 */
	int byFirmwareVersion;
	/**
	index
	 * offset 468
	 */
	int HD44780width;
	/**
	index
	 * offset 472
	 */
	int HD44780height;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
	 * offset 476
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 477
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 478
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * set_analog_input_pin tps2 X
	 * offset 479
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	x
	 * offset 480
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	angle
	 * offset 484
	 */
	int trailingSparkAngle;
	/**
	 * offset 488
	 */
	trigger_config_s trigger;
	/**
	 * offset 504
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * Single value to be used in event of a failed MAP sensor 
	 * This value is only used for speed density fueling calculations.
	kPa
	 * offset 505
	 */
	uint8_t failedMapFallback;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost.
	%
	 * offset 506
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 507
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * set global_fuel_correction X
	coef
	 * offset 508
	 */
	float globalFuelCorrection;
	/**
	volts
	 * offset 512
	 */
	float adcVcc;
	/**
	Deg
	 * offset 516
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 520
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 522
	 */
	uint8_t camInputsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * need 4 byte alignment
	units
	 * offset 525
	 */
	uint8_t alignmentFill_at_525[3];
	/**
	 * offset 528
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * set_analog_input_pin pps X
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 548
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 549
	 */
	uint8_t alignmentFill_at_549[1];
	/**
	 * offset 550
	 */
	Gpio tle6240_cs;
	/**
	 * offset 552
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 553
	 */
	uint8_t alignmentFill_at_553[1];
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 554
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * @see hasBaroSensor
	 * offset 556
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 568
	 */
	idle_hardware_s idle;
	/**
	 * Value between 0 and 100 used in Manual mode
	%
	 * offset 580
	 */
	float manIdlePosition;
	/**
	 * Ignition timing to remove when a knock event occurs.
	%
	 * offset 584
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	deg/s
	 * offset 585
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Maximum amount of knock retard.
	deg
	 * offset 586
	 */
	uint8_t knockRetardMaximum;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 587
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	ratio
	 * offset 588
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	count
	 * offset 590
	 */
	uint8_t vssToothCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 591
	 */
	uint8_t alignmentFill_at_591[1];
	/**
	 * offset 592
	 */
	Gpio l9779_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 594
	 */
	uint8_t alignmentFill_at_594[2];
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * TODO: rename to triggerSimulatorRpm
	Rpm
	 * offset 596
	 */
	int triggerSimulatorFrequency;
	/**
	 * offset 600
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 624
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 648
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 649
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 650
	 */
	Gpio HD44780_rs;
	/**
	 * offset 652
	 */
	Gpio HD44780_e;
	/**
	 * offset 654
	 */
	Gpio HD44780_db4;
	/**
	 * offset 656
	 */
	Gpio HD44780_db5;
	/**
	 * offset 658
	 */
	Gpio HD44780_db6;
	/**
	 * offset 660
	 */
	Gpio HD44780_db7;
	/**
	 * offset 662
	 */
	Gpio gps_rx_pin;
	/**
	 * offset 664
	 */
	Gpio gps_tx_pin;
	/**
	 * offset 666
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 668
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 669
	 */
	uint8_t alignmentFill_at_669[1];
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 670
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 672
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 673
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 674
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 676
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 678
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 680
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 681
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 682
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 690
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 691
	 */
	uint8_t alignmentFill_at_691[1];
	/**
	 * set_cj125_heater_pin XXX
	 * offset 692
	 */
	Gpio wboHeaterPin;
	/**
	 * set_cj125_cs_pin XXX
	 * offset 694
	 */
	Gpio cj125CsPin;
	/**
	 * offset 696
	 */
	spi_device_e max31855spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 697
	 */
	uint8_t alignmentFill_at_697[1];
	/**
	 * offset 698
	 */
	Gpio debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 700
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 701
	 */
	uint8_t alignmentFill_at_701[1];
	/**
	 * offset 702
	 */
	Gpio mc33972_cs;
	/**
	 * offset 704
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 705
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 706
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 707
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 708
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	%
	 * offset 709
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 717
	 */
	ego_sensor_e afr_type;
	/**
	 * need 4 byte alignment
	units
	 * offset 718
	 */
	uint8_t alignmentFill_at_718[2];
	/**
	 * 0.1 is a good default value
	x
	 * offset 720
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 724
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 730
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 731
	 */
	uint8_t alignmentFill_at_731[1];
	/**
	 * offset 732
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 734
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 735
	 */
	uint8_t alignmentFill_at_735[1];
	/**
	 * offset 736
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 738
	 */
	Gpio sdCardCsPin;
	/**
	 * set_can_tx_pin X
	 * offset 740
	 */
	Gpio canTxPin;
	/**
	 * set_can_rx_pin X
	 * offset 742
	 */
	Gpio canRxPin;
	/**
	 * offset 744
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * Additional idle % while A/C is active
	%
	 * offset 745
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * Ratio between the wheels and your transmission output.
	ratio
	 * offset 746
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 748
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 750
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	units
	 * offset 751
	 */
	uint8_t alignmentFill_at_751[1];
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	mv
	 * offset 752
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 754
	 */
	uint16_t wastegatePositionMax;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	mv
	 * offset 756
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 758
	 */
	uint16_t idlePositionMax;
	/**
	 * Secondary TTL channel baud rate
	BPs
	 * offset 760
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	CR
	 * offset 764
	 */
	float compressionRatio;
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 768
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 774
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 777
	 */
	uint8_t alignmentFill_at_777[1];
	/**
	 * Narrow band o2 heater, not used for CJ125. 'ON' if engine is running, 'OFF' if stopped or cranking. See wboHeaterPin
	 * offset 778
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 780
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * need 4 byte alignment
	units
	 * offset 781
	 */
	uint8_t alignmentFill_at_781[3];
	/**
	offset 784 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 784 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 784 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 784 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 784 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 784 bit 5 */
	bool isEngineControlEnabled : 1 {};
	/**
	offset 784 bit 6 */
	bool isHip9011Enabled : 1 {};
	/**
	offset 784 bit 7 */
	bool isVerboseAlternator : 1 {};
	/**
	offset 784 bit 8 */
	bool useSerialPort : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 784 bit 9 */
	bool useStepperIdle : 1 {};
	/**
	offset 784 bit 10 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 784 bit 11 */
	bool useTpicAdvancedMode : 1 {};
	/**
	offset 784 bit 12 */
	bool useLcdScreen : 1 {};
	/**
	offset 784 bit 13 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 784 bit 14 */
	bool enableVerboseCanTx : 1 {};
	/**
	 * This will cause the alternator to be operated in a basic on or off mode, this is the simplest alternator control.
	offset 784 bit 15 */
	bool onOffAlternatorLogic : 1 {};
	/**
	 * enable cj125/disable cj125
	offset 784 bit 16 */
	bool isCJ125Enabled : 1 {};
	/**
	 * Use rise or fall signal front
	 * get vvtCamSensorUseRise
	offset 784 bit 17 */
	bool vvtCamSensorUseRise : 1 {};
	/**
	 * Useful for individual intakes
	offset 784 bit 18 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 784 bit 19 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 784 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 784 bit 21 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * This setting allows the ECU to open the IAC during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars. Used in Auto-PID Idle mode.
	offset 784 bit 22 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 784 bit 23 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 784 bit 24 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 784 bit 25 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 784 bit 26 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 784 bit 27 */
	bool pauseEtbControl : 1 {};
	/**
	offset 784 bit 28 */
	bool alignEngineSnifferAtTDC : 1 {};
	/**
	 * This setting allows the ETB to act as the idle air control valve and move to regulate the airflow at idle.
	offset 784 bit 29 */
	bool useETBforIdleControl : 1 {};
	/**
	offset 784 bit 30 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 784 bit 31 */
	bool enableAemXSeries : 1 {};
	/**
	 * offset 788
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 796
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 797
	 */
	uint8_t alignmentFill_at_797[1];
	/**
	 * offset 798
	 */
	Gpio hip9011CsPin;
	/**
	 * offset 800
	 */
	Gpio hip9011IntHoldPin;
	/**
	 * offset 802
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 803
	 */
	uint8_t alignmentFill_at_803[1];
	/**
	 * offset 804
	 */
	uint32_t verboseCanBaseAddress;
	/**
	v
	 * offset 808
	 */
	uint8_t mc33_hvolt;
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	kPa
	 * offset 809
	 */
	uint8_t minimumBoostClosedLoopMap;
	/**
	 * Optional Radiator Fan used with A/C
	 * offset 810
	 */
	output_pin_e acFanPin;
	/**
	 * offset 812
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * offset 813
	 */
	spi_device_e l9779spiDevice;
	/**
	volts
	 * offset 814
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	 * offset 822
	 */
	imu_type_e imuType;
	/**
	multiplier
	 * offset 823
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 831
	 */
	uint8_t alignmentFill_at_831[1];
	/**
	kg
	 * offset 832
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling?
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	RPM
	 * offset 834
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	ms
	 * offset 836
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 838
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 840
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 842
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 844
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 846
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 847
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 848
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 864
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * offset 866
	 */
	Gpio test557pin;
	/**
	 * offset 868
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 869
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	:1
	 * offset 870
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	%
	 * offset 871
	 */
	uint8_t etbMaximumPosition;
	/**
	 * SD card logging period, in milliseconds
	ms
	 * offset 872
	 */
	int16_t sdCardPeriodMs;
	/**
	 * offset 874
	 */
	adc_channel_e idlePositionSensor;
	/**
	 * need 4 byte alignment
	units
	 * offset 875
	 */
	uint8_t alignmentFill_at_875[1];
	/**
	 * offset 876
	 */
	Gpio debugMapAveraging;
	/**
	 * offset 878
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 880
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 881
	 */
	uint8_t alignmentFill_at_881[1];
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 882
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 884
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 886
	 */
	uint8_t alignmentFill_at_886[2];
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	count
	 * offset 888
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	%
	 * offset 892
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	%
	 * offset 894
	 */
	int16_t stepperParkingExtraSteps;
	/**
	ADC
	 * offset 896
	 */
	uint16_t tps1SecondaryMin;
	/**
	ADC
	 * offset 898
	 */
	uint16_t tps1SecondaryMax;
	/**
	rpm
	 * offset 900
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	Seconds
	 * offset 902
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 904
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 906
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 907
	 */
	uint8_t alignmentFill_at_907[1];
	/**
	 * offset 908
	 */
	output_pin_e acRelayPin;
	/**
	 * offset 910
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 911
	 */
	uint8_t alignmentFill_at_911[1];
	/**
	 * offset 912
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 944
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 946
	 */
	Gpio spi1misoPin;
	/**
	 * offset 948
	 */
	Gpio spi1sckPin;
	/**
	 * offset 950
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 952
	 */
	Gpio spi2misoPin;
	/**
	 * offset 954
	 */
	Gpio spi2sckPin;
	/**
	 * offset 956
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 958
	 */
	Gpio spi3misoPin;
	/**
	 * offset 960
	 */
	Gpio spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 962
	 */
	Gpio cdmInputPin;
	/**
	 * offset 964
	 */
	Gpio joystickCenterPin;
	/**
	 * offset 966
	 */
	Gpio joystickAPin;
	/**
	 * offset 968
	 */
	Gpio joystickBPin;
	/**
	 * offset 970
	 */
	Gpio joystickCPin;
	/**
	 * offset 972
	 */
	Gpio joystickDPin;
	/**
	 * offset 974
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEFI console Sensor Sniffer mode
	 * offset 975
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 976
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * need 4 byte alignment
	units
	 * offset 977
	 */
	uint8_t alignmentFill_at_977[3];
	/**
	offset 980 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 980 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 980 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 980 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 980 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 980 bit 5 */
	bool unfinishedenableLaunchBoost : 1 {};
	/**
	offset 980 bit 6 */
	bool unfinishedlaunchDisableBySpeed : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 980 bit 7 */
	bool enableCanVss : 1 {};
	/**
	offset 980 bit 8 */
	bool enableInnovateLC2 : 1 {};
	/**
	offset 980 bit 9 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise. Use of this mode is required if you have a narrowband O2 sensor.
	offset 980 bit 10 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 980 bit 11 */
	bool tempBooleanForVerySpecialCases : 1 {};
	/**
	offset 980 bit 12 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * enable vvt_details
	offset 980 bit 13 */
	bool verboseVVTDecoding : 1 {};
	/**
	 * get invertCamVVTSignal
	offset 980 bit 14 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	 * enable consumeObdSensors
	offset 980 bit 15 */
	bool consumeObdSensors : 1 {};
	/**
	offset 980 bit 16 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 980 bit 17 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 980 bit 18 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 980 bit 19 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 980 bit 20 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 980 bit 21 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 980 bit 22 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 980 bit 23 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 980 bit 24 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 980 bit 25 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 980 bit 26 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 980 bit 27 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 980 bit 28 */
	bool tcuEnabled : 1 {};
	/**
	offset 980 bit 29 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coilss
	offset 980 bit 30 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 980 bit 31 */
	bool verboseCan2 : 1 {};
	/**
	 * offset 984
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 1000
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 1002
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1003
	 */
	uint8_t alignmentFill_at_1003[1];
	/**
	 * offset 1004
	 */
	switch_input_pin_e antiLagActivatePin;
	/**
	 * offset 1006
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 1008
	 */
	pid_s boostPid;
	/**
	 * offset 1028
	 */
	boostType_e boostType;
	/**
	 * need 4 byte alignment
	units
	 * offset 1029
	 */
	uint8_t alignmentFill_at_1029[3];
	/**
	Hz
	 * offset 1032
	 */
	int boostPwmFrequency;
	/**
	 * offset 1036
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1037
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1038
	 */
	uint8_t alignmentFill_at_1038[2];
	/**
	 * Disabled above this speed
	Kph
	 * offset 1040
	 */
	int launchSpeedThreshold;
	/**
	 * Range from Launch Rpm for Timing Retard to activate
	RPM
	 * offset 1044
	 */
	int launchTimingRpmRange;
	/**
	 * Extra Fuel Added
	%
	 * offset 1048
	 */
	int launchFuelAdded;
	/**
	 * Duty Cycle for the Boost Solenoid
	%
	 * offset 1052
	 */
	int launchBoostDuty;
	/**
	 * RPM Range for Hard Cut
	rpm
	 * offset 1056
	 */
	int hardCutRpmRange;
	/**
	rpm
	 * offset 1060
	 */
	int launchAdvanceRpmRange;
	/**
	rpm
	 * offset 1064
	 */
	int launchTpsTreshold;
	/**
	rpm
	 * offset 1068
	 */
	float launchActivateDelay;
	/**
	 * offset 1072
	 */
	stft_s stft;
	/**
	 * offset 1096
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1112
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1144
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1176
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1208
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * offset 1220
	 */
	etb_function_e etbFunctions[ETB_COUNT];
	/**
	 * offset 1222
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1223
	 */
	uint8_t alignmentFill_at_1223[1];
	/**
	 * offset 1224
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1226
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1227
	 */
	uint8_t alignmentFill_at_1227[1];
	/**
	 * offset 1228
	 */
	Gpio drv8860_miso;
	/**
	volt
	 * offset 1230
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * offset 1246
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1262
	 */
	uint8_t alignmentFill_at_1262[2];
	/**
	 * Angle between cam sensor and VVT zero position
	 * set vvt_offset X
	value
	 * offset 1264
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1268
	 */
	float vvtOffsetsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 1280
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1312
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	ADC
	 * offset 1376
	 */
	uint16_t tps2SecondaryMin;
	/**
	ADC
	 * offset 1378
	 */
	uint16_t tps2SecondaryMax;
	/**
	offset 1380 bit 0 */
	bool disablePrimaryUart : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1380 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Print details into rusEFI console
	 * enable verbose_idle
	offset 1380 bit 2 */
	bool isVerboseIAC : 1 {};
	/**
	offset 1380 bit 3 */
	bool boardUseTachPullUp : 1 {};
	/**
	offset 1380 bit 4 */
	bool boardUseTempPullUp : 1 {};
	/**
	 * This options enables data for 'engine sniffer' tab in console, which comes at some CPU price
	offset 1380 bit 5 */
	bool isEngineChartEnabled : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1380 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1380 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1380 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write
	offset 1380 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1380 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1380 bit 11 */
	bool boardUse2stepPullDown : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1380 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1380 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1380 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1380 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1380 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	offset 1380 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1380 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1380 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1380 bit 20 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 1380 bit 21 */
	bool boardUseCamPullDown : 1 {};
	/**
	offset 1380 bit 22 */
	bool boardUseCamVrPullUp : 1 {};
	/**
	offset 1380 bit 23 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 1380 bit 24 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 1380 bit 25 */
	bool boardUseD4PullDown : 1 {};
	/**
	offset 1380 bit 26 */
	bool boardUseD5PullDown : 1 {};
	/**
	 * Sometimes we just have to shut the engine down. Use carefully!
	offset 1380 bit 27 */
	bool useFSIO5ForCriticalIssueEngineStop : 1 {};
	/**
	 * Sometimes we have to miss injection on purpose to attract driver's attention
	offset 1380 bit 28 */
	bool useFSIO4ForSeriousEngineWarning : 1 {};
	/**
	offset 1380 bit 29 */
	bool launchActivateInverted : 1 {};
	/**
	offset 1380 bit 30 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1380 bit 31 */
	bool skippedWheelOnCam : 1 {};
	/**
	 * offset 1384
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 1385
	 */
	uint8_t alignmentFill_at_1385[1];
	/**
	 * A/C button input
	 * offset 1386
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1388
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	%
	 * offset 1389
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1390
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1391
	 */
	uint8_t alignmentFill_at_1391[1];
	/**
	 * Enable fuel injection - This is default off for new projects as a safety feature, set to "true" to enable fuel injection and further injector settings.
	offset 1392 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	 * Enable ignition - This is default off for new projects as a safety feature, set to "true" to enable ignition and further ignition settings.
	offset 1392 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1392 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1392 bit 3 */
	bool complexWallModel : 1 {};
	/**
	offset 1392 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1392 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1392 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1392 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1392 bit 8 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1392 bit 9 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * enable trigger_details
	offset 1392 bit 10 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1392 bit 11 */
	bool isManualSpinningMode : 1 {};
	/**
	 * This is needed if your coils are individually wired and you wish to use batch injection.
	 * enable two_wire_batch_injection
	offset 1392 bit 12 */
	bool twoWireBatchInjection : 1 {};
	/**
	 * VR sensors are only precise on rising front
	 * enable trigger_only_front
	offset 1392 bit 13 */
	bool useOnlyRisingEdgeForTrigger : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1392 bit 14 */
	bool twoWireBatchIgnition : 1 {};
	/**
	offset 1392 bit 15 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1392 bit 16 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1392 bit 17 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1392 bit 18 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1392 bit 19 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1392 bit 20 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1392 bit 21 */
	bool launchSmoothRetard : 1 {};
	/**
	offset 1392 bit 22 */
	bool unused1476b20 : 1 {};
	/**
	offset 1392 bit 23 */
	bool unused1476b8 : 1 {};
	/**
	offset 1392 bit 24 */
	bool unused_1484_bit_24 : 1 {};
	/**
	offset 1392 bit 25 */
	bool unused_1484_bit_25 : 1 {};
	/**
	offset 1392 bit 26 */
	bool unused_1484_bit_26 : 1 {};
	/**
	offset 1392 bit 27 */
	bool unused_1484_bit_27 : 1 {};
	/**
	offset 1392 bit 28 */
	bool unused_1484_bit_28 : 1 {};
	/**
	offset 1392 bit 29 */
	bool unused_1484_bit_29 : 1 {};
	/**
	offset 1392 bit 30 */
	bool unused_1484_bit_30 : 1 {};
	/**
	offset 1392 bit 31 */
	bool unused_1484_bit_31 : 1 {};
	/**
	count
	 * offset 1396
	 */
	uint32_t engineChartSize;
	/**
	mult
	 * offset 1400
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1404
	 */
	Gpio camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * offset 1406
	 */
	uint8_t camInputsDebugPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * need 4 byte alignment
	units
	 * offset 1409
	 */
	uint8_t alignmentFill_at_1409[1];
	/**
	 * Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	RPM
	 * offset 1410
	 */
	int16_t acIdleRpmBump;
	/**
	 * set warningPeriod X
	seconds
	 * offset 1412
	 */
	int16_t warningPeriod;
	/**
	 * need 4 byte alignment
	units
	 * offset 1414
	 */
	uint8_t alignmentFill_at_1414[2];
	/**
	angle
	 * offset 1416
	 */
	float knockDetectionWindowStart;
	/**
	angle
	 * offset 1420
	 */
	float knockDetectionWindowEnd;
	/**
	ms
	 * offset 1424
	 */
	float idleStepperReactionTime;
	/**
	count
	 * offset 1428
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	sec
	 * offset 1432
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	index
	 * offset 1436
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	kPa
	 * offset 1440
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	RPM
	 * offset 1456
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	ratio
	 * offset 1472
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	Ratio
	 * offset 1536
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	%
	 * offset 1568
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1600
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	Seconds
	 * offset 1604
	 */
	float wwaeTau;
	/**
	 * offset 1608
	 */
	pid_s alternatorControl;
	/**
	 * offset 1628
	 */
	pid_s etb;
	/**
	 * offset 1648
	 */
	Gpio triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 1654
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * set tps2_min X
	ADC
	 * offset 1656
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps2_max X
	ADC
	 * offset 1658
	 */
	int16_t tps2Max;
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
	units
	 * offset 1663
	 */
	uint8_t alignmentFill_at_1663[1];
	/**
	 * offset 1664
	 */
	Gpio mc33816_flag0;
	/**
	Pulse
	 * offset 1666
	 */
	uint8_t tachPulsePerRev;
	/**
	 * need 4 byte alignment
	units
	 * offset 1667
	 */
	uint8_t alignmentFill_at_1667[1];
	/**
	 * kPa value which is too low to be true
	kPa
	 * offset 1668
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	kPa
	 * offset 1672
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	ms
	 * offset 1676
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	ms
	 * offset 1678
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1680
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	Fraction
	 * offset 1700
	 */
	float wwaeBeta;
	/**
	 * blue LED on many rusEFI boards.
	 * Blue Communication LED which is expected to blink at 50% duty cycle during normal board operation.
	 * If USB communication cable is connected Blue LED starts to blink faster.
	 * offset 1704
	 */
	Gpio communicationLedPin;
	/**
	 * Green LED on many rusEFI boards.
	 * Off if engine is stopped, blinks if engine is cranking, solid if engine is running.
	 * offset 1706
	 */
	Gpio runningLedPin;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1708
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1710
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1712
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * offset 1716
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1718
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	voltage
	 * offset 1720
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1724
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	seconds
	 * offset 1728
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	RPM
	 * offset 1730
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * CLT-based target RPM for automatic idle controller
	C
	 * offset 1732
	 */
	float cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	RPM
	 * offset 1796
	 */
	scaled_channel<uint8_t, 1, 50> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	Volts
	 * offset 1812
	 */
	float targetVBatt;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	%
	 * offset 1816
	 */
	float alternatorOffAboveTps;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	cycles
	 * offset 1820
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	percent
	 * offset 1822
	 */
	int16_t iacByTpsTaper;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_tx_pin X
	 * offset 1824
	 */
	Gpio auxSerialTxPin;
	/**
	 * offset 1826
	 */
	Gpio warningLedPin;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_rx_pin X
	 * offset 1828
	 */
	Gpio auxSerialRxPin;
	/**
	 * offset 1830
	 */
	Gpio LIS302DLCsPin;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	sec
	 * offset 1832
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 1833
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 1834
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	sec
	 * offset 1835
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	roc
	 * offset 1836
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1840
	 */
	brain_input_pin_e auxSpeedSensorInputPin[2];
	/**
	 * offset 1844
	 */
	uint8_t totalGearsCount;
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 1845
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1846
	 */
	uint8_t alignmentFill_at_1846[2];
	/**
	 * Band rate for primary TTL
	BPs
	 * offset 1848
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	roc
	 * offset 1852
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	coeff
	 * offset 1856
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	coeff
	 * offset 1860
	 */
	float slowAdcAlpha;
	/**
	 * See http://rusefi.com/s/debugmode
	 * 
	 * set debug_mode X
	 * offset 1864
	 */
	debug_mode_e debugMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1865
	 */
	uint8_t alignmentFill_at_1865[3];
	/**
	BPs
	 * offset 1868
	 */
	uint32_t auxSerialSpeed;
	/**
	voltage
	 * offset 1872
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1876
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * set can_baudrate
	 * offset 1880
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1881
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1882
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1883
	 */
	load_override_e afrOverrideMode;
	/**
	A
	 * offset 1884
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	A
	 * offset 1885
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	us
	 * offset 1886
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	ms
	 * offset 1887
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1888 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	offset 1888 bit 1 */
	bool unused1740b0 : 1 {};
	/**
	offset 1888 bit 2 */
	bool unused1740b1 : 1 {};
	/**
	offset 1888 bit 3 */
	bool unused1740b2 : 1 {};
	/**
	offset 1888 bit 4 */
	bool unused1127 : 1 {};
	/**
	offset 1888 bit 5 */
	bool unused1128 : 1 {};
	/**
	offset 1888 bit 6 */
	bool unused1129 : 1 {};
	/**
	offset 1888 bit 7 */
	bool unused1130 : 1 {};
	/**
	offset 1888 bit 8 */
	bool unusedBit_549_8 : 1 {};
	/**
	offset 1888 bit 9 */
	bool unusedBit_549_9 : 1 {};
	/**
	offset 1888 bit 10 */
	bool unusedBit_549_10 : 1 {};
	/**
	offset 1888 bit 11 */
	bool unusedBit_549_11 : 1 {};
	/**
	offset 1888 bit 12 */
	bool unusedBit_549_12 : 1 {};
	/**
	offset 1888 bit 13 */
	bool unusedBit_549_13 : 1 {};
	/**
	offset 1888 bit 14 */
	bool unusedBit_549_14 : 1 {};
	/**
	offset 1888 bit 15 */
	bool unusedBit_549_15 : 1 {};
	/**
	offset 1888 bit 16 */
	bool unusedBit_549_16 : 1 {};
	/**
	offset 1888 bit 17 */
	bool unusedBit_549_17 : 1 {};
	/**
	offset 1888 bit 18 */
	bool unusedBit_549_18 : 1 {};
	/**
	offset 1888 bit 19 */
	bool unusedBit_549_19 : 1 {};
	/**
	offset 1888 bit 20 */
	bool unusedBit_549_20 : 1 {};
	/**
	offset 1888 bit 21 */
	bool unusedBit_549_21 : 1 {};
	/**
	offset 1888 bit 22 */
	bool unusedBit_549_22 : 1 {};
	/**
	offset 1888 bit 23 */
	bool unusedBit_549_23 : 1 {};
	/**
	offset 1888 bit 24 */
	bool unusedBit_549_24 : 1 {};
	/**
	offset 1888 bit 25 */
	bool unusedBit_549_25 : 1 {};
	/**
	offset 1888 bit 26 */
	bool unusedBit_549_26 : 1 {};
	/**
	offset 1888 bit 27 */
	bool unusedBit_549_27 : 1 {};
	/**
	offset 1888 bit 28 */
	bool unusedBit_549_28 : 1 {};
	/**
	offset 1888 bit 29 */
	bool unusedBit_549_29 : 1 {};
	/**
	offset 1888 bit 30 */
	bool unusedBit_549_30 : 1 {};
	/**
	offset 1888 bit 31 */
	bool unusedBit_549_31 : 1 {};
	/**
	ms
	 * offset 1892
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOffTime;
	/**
	count
	 * offset 1893
	 */
	uint8_t benchTestCount;
	/**
	ms
	 * offset 1894
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOnTime;
	/**
	 * offset 1895
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * set_can2_tx_pin X
	 * offset 1896
	 */
	Gpio can2TxPin;
	/**
	 * set_can2_rx_pin X
	 * offset 1898
	 */
	Gpio can2RxPin;
	/**
	 * offset 1900
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1901
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1902
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1903
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1904
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1906
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1907
	 */
	uint8_t alignmentFill_at_1907[1];
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	kPa (absolute)
	 * offset 1908
	 */
	float boostCutPressure;
	/**
	kg/h
	 * offset 1912
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16];
	/**
	ratio
	 * offset 1928
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16];
	/**
	 * Fixed timing, useful for TDC testing
	deg
	 * offset 1944
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	v
	 * offset 1948
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	v
	 * offset 1952
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	value
	 * offset 1956
	 */
	float egoValueShift;
	/**
	 * offset 1960
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * offset 1962
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * offset 1963
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * offset 1964
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1965
	 */
	uint8_t alignmentFill_at_1965[3];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	percent
	 * offset 1968
	 */
	int crankingIACposition;
	/**
	 * offset 1972
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 1976
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 1980
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 1984
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 1988
	 */
	pwm_freq_t vvtOutputFrequency[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #1 is active
	%
	 * offset 1990
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * need 4 byte alignment
	units
	 * offset 1991
	 */
	uint8_t alignmentFill_at_1991[1];
	/**
	Hz
	 * offset 1992
	 */
	int alternatorPwmFrequency;
	/**
	 * Narrow Band WBO Approximation
	V
	 * offset 1996
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	ratio
	 * offset 2028
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * set vvt_mode X
	 * offset 2060
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * offset 2061
	 */
	uint8_t vvtModePadding[CAMS_PER_BANK_padding];
	/**
	 * Additional idle % when fan #2 is active
	%
	 * offset 2062
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	sec
	 * offset 2063
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 2064
	 */
	adc_channel_e auxAnalogInputs[AUX_ANALOG_INPUT_COUNT];
	/**
	 * offset 2072
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * CLT-based timing correction
	C
	 * offset 2096
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	degree
	 * offset 2128
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2160
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 2161
	 */
	pin_output_mode_e LIS302DLCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 2162
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 2163
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	kPa
	 * offset 2164
	 */
	float fuelReferencePressure;
	/**
	 * Fuel multiplier (enrichment) immediately after engine start
	mult
	 * offset 2168
	 */
	float postCrankingFactor;
	/**
	 * Time over which to taper out after start enrichment
	seconds
	 * offset 2172
	 */
	float postCrankingDurationSec;
	/**
	 * offset 2176
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 2208
	 */
	ThermistorConf auxTempSensor2;
	/**
	Deg
	 * offset 2240
	 */
	int16_t knockSamplingDuration;
	/**
	Hz
	 * offset 2242
	 */
	int16_t etbFreq;
	/**
	 * offset 2244
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 2264
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	%
	 * offset 2265
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	%
	 * offset 2266
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 2267
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder timing correction
	deg
	 * offset 2268
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	seconds
	 * offset 2316
	 */
	float idlePidActivationTime;
	/**
	 * offset 2320
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 2321
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 2322
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 2323
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 2324
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 2325
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 2326
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 2327
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 2328
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 2329
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 2330
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 2332
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 2334
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * lambda input
	 * offset 2336
	 */
	adc_channel_e cj125ua;
	/**
	 * heater input
	 * offset 2337
	 */
	adc_channel_e cj125ur;
	/**
	 * offset 2338
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2339
	 */
	uint8_t alignmentFill_at_2339[1];
	/**
	 * offset 2340
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 2360
	 */
	float injectorCorrectionPolynomial[8];
	/**
	C
	 * offset 2392
	 */
	int8_t primeBins[8];
	/**
	 * offset 2400
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2420
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 2421
	 */
	uint8_t alignmentFill_at_2421[1];
	/**
	 * offset 2422
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 2424
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 2425
	 */
	uint8_t fan2OffTemperature;
	/**
	 * need 4 byte alignment
	units
	 * offset 2426
	 */
	uint8_t alignmentFill_at_2426[2];
	/**
	x
	 * offset 2428
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 2492
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	x
	 * offset 2556
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 2620
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	x
	 * offset 2684
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 2716
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	x
	 * offset 2748
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 2780
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	 * offset 2812
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2814
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2816
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2817
	 */
	uint8_t alignmentFill_at_2817[1];
	/**
	 * offset 2818
	 */
	Gpio mc33816_cs;
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	RPM
	 * offset 2820
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	deg
	 * offset 2836
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * todo: more comments
	 * offset 2852
	 */
	Gpio servoOutputPins[SERVO_COUNT];
	/**
	 * This sets the RPM above which fuel cut is active.
	rpm
	 * offset 2868
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	rpm
	 * offset 2870
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	%
	 * offset 2872
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	C
	 * offset 2874
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	%
	 * offset 2876
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	kPa
	 * offset 2878
	 */
	int16_t coastingFuelCutMap;
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	C
	 * offset 2880
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	%
	 * offset 2944
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 3008
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 3028
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * CLT-based target RPM for hard limit depending on CLT like on Lexus LFA
	C
	 * offset 3048
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 3052
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * offset 3060
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	x
	 * offset 3156
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3188
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	x
	 * offset 3220
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3252
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	 * offset 3284
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 3348
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 3476
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 3480
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	kg/h
	 * offset 3484
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	deg/sec
	 * offset 3488
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	deg/sec
	 * offset 3492
	 */
	float tChargeAirDecrLimit;
	/**
	 * offset 3496
	 */
	tChargeMode_e tChargeMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3497
	 */
	uint8_t alignmentFill_at_3497[3];
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	target TPS position
	 * offset 3500
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	ETB duty cycle bias
	 * offset 3532
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	 * offset 3564
	 */
	float hip9011Gain;
	/**
	 * iTerm min value
	 * offset 3568
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 3570
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 3572
	 */
	pid_s idleTimingPid;
	/**
	 * If the RPM closer to target than this value, disable timing correction to prevent oscillation
	RPM
	 * offset 3592
	 */
	int16_t idleTimingPidDeadZone;
	/**
	 * A delay in cycles between fuel-enrich. portions
	cycles
	 * offset 3594
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	coef
	 * offset 3596
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 3600
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 3601
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 3602
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 3604
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	:1
	 * offset 3605
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 3606
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	%
	 * offset 3608
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 3612
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	mg
	 * offset 3624
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[8];
	/**
	 * Trigger comparator center point voltage
	V
	 * offset 3632
	 */
	uint8_t triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	V
	 * offset 3633
	 */
	uint8_t triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	V
	 * offset 3634
	 */
	uint8_t triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	RPM
	 * offset 3635
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * offset 3636
	 */
	pid_s idleRpmPid2;
	/**
	%
	 * offset 3656
	 */
	uint8_t iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	Load
	 * offset 3720
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	RPM
	 * offset 3728
	 */
	scaled_channel<uint8_t, 1, 50> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * set can_vss X
	 * offset 3736
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	units
	 * offset 3737
	 */
	uint8_t alignmentFill_at_3737[3];
	/**
	 * offset 3740
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	mA
	 * offset 4108
	 */
	uint16_t mc33_i_boost;
	/**
	mA
	 * offset 4110
	 */
	uint16_t mc33_i_peak;
	/**
	mA
	 * offset 4112
	 */
	uint16_t mc33_i_hold;
	/**
	us
	 * offset 4114
	 */
	uint16_t mc33_t_max_boost;
	/**
	us
	 * offset 4116
	 */
	uint16_t mc33_t_peak_off;
	/**
	us
	 * offset 4118
	 */
	uint16_t mc33_t_peak_tot;
	/**
	us
	 * offset 4120
	 */
	uint16_t mc33_t_bypass;
	/**
	us
	 * offset 4122
	 */
	uint16_t mc33_t_hold_off;
	/**
	us
	 * offset 4124
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 4126
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 4127
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 4128
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 4129
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	dB
	 * offset 4135
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 4151
	 */
	uint8_t alignmentFill_at_4151[1];
	/**
	from
	 * offset 4152
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	 * offset 4224
	 */
	int8_t unused4080[12];
	/**
	to
	 * offset 4236
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	lobes/cam
	 * offset 4308
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 4309
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Crank angle ATDC of first lobe peak
	deg
	 * offset 4310
	 */
	uint8_t hpfpPeakPos;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	deg
	 * offset 4311
	 */
	uint8_t hpfpMinAngle;
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	cc
	 * offset 4312
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	deg
	 * offset 4314
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 4315
	 */
	uint8_t issFilterReciprocal;
	/**
	%/kPa
	 * offset 4316
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	%/kPa/lobe
	 * offset 4318
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	kPa/s
	 * offset 4320
	 */
	uint16_t hpfpTargetDecay;
	/**
	%
	 * offset 4322
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	deg
	 * offset 4338
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	volts
	 * offset 4354
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	ms
	 * offset 4362
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	kPa
	 * offset 4378
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	load
	 * offset 4578
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	RPM
	 * offset 4598
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	%
	 * offset 4608
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	cc/lobe
	 * offset 4708
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	RPM
	 * offset 4728
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	 * offset 4738
	 */
	output_pin_e stepper_raw_output[4];
	/**
	ratio
	 * offset 4746
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[GEARS_COUNT];
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	ms
	 * offset 4762
	 */
	uint16_t vvtActivationDelayMs;
	/**
	deg C
	 * offset 4764
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	/**
	 * offset 4772
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	/**
	 * offset 4780
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	/**
	kPa
	 * offset 4788
	 */
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	/**
	 * offset 4796
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 4804
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	/**
	Nm
	 * offset 4812
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_SIZE];
	/**
	RPM
	 * offset 4848
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE];
	/**
	Load
	 * offset 4860
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE];
	/**
	 * offset 4872
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 4873
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4874
	 */
	uint8_t alignmentFill_at_4874[2];
	/**
	 * offset 4876
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 4896
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 4916
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 4918
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4919
	 */
	uint8_t alignmentFill_at_4919[1];
	/**
	 * offset 4920
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 4922
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4923
	 */
	uint8_t alignmentFill_at_4923[1];
	/**
	 * offset 4924
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 4926
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 4928
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4929
	 */
	uint8_t alignmentFill_at_4929[1];
	/**
	 * offset 4930
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 4932
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 4934
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4935
	 */
	uint8_t alignmentFill_at_4935[1];
	/**
	 * offset 4936
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	units
	 * offset 4938
	 */
	uint8_t mainUnusedEnd[250];
};
static_assert(sizeof(engine_configuration_s) == 5188);

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
	 * offset 5188
	 */
	error_message_t warning_message;
	/**
	C
	 * offset 5308
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	Seconds
	 * offset 5340
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	%
	 * offset 5372
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	Seconds
	 * offset 5404
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 5436
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	RPM
	 * offset 5500
	 */
	scaled_channel<uint8_t, 1, 50> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 5508
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5572
	 */
	scaled_channel<uint8_t, 1, 1> boostTpsBins[BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5580
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	%
	 * offset 5644
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	RPM
	 * offset 5652
	 */
	scaled_channel<uint8_t, 1, 50> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	C
	 * offset 5660
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	%
	 * offset 5692
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	RPM
	 * offset 5724
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	deg
	 * offset 5732
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	RPM
	 * offset 5764
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	/**
	load
	 * offset 5768
	 */
	scaled_channel<uint8_t, 1, 1> idleVeLoadBins[IDLE_VE_SIZE];
	/**
	%
	 * offset 5772
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	/**
	 * offset 5804
	 */
	lua_script_t luaScript;
	/**
	C
	 * offset 13804
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	ratio
	 * offset 13868
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	C
	 * offset 13932
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 13996
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 14060
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	C
	 * offset 14092
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	ratio
	 * offset 14124
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	counter
	 * offset 14156
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	C
	 * offset 14188
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	ratio
	 * offset 14252
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	kg/hour
	 * offset 14316
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	V
	 * offset 15340
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	deg
	 * offset 16364
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Temperature
	 * offset 16876
	 */
	float ignitionIatCorrLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 16940
	 */
	uint16_t ignitionIatCorrRpmBins[IGN_RPM_COUNT];
	/**
	deg
	 * offset 16972
	 */
	int16_t injectionPhase[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 17484
	 */
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 17516
	 */
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	onoff
	 * offset 17548
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	kPa
	 * offset 17608
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	% TPS
	 * offset 18120
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18152
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 18184
	 */
	uint8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 18248
	 */
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 18264
	 */
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 18280
	 */
	uint8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 18344
	 */
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 18360
	 */
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	/**
	deg
	 * offset 18376
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 18888
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 18920
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	%
	 * offset 18952
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	kPa
	 * offset 19464
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 19496
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	lambda
	 * offset 19528
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	 * offset 19784
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 19816
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 19848
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	from
	 * offset 20104
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	to
	 * offset 20136
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	value
	 * offset 20168
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20424
	 */
	uint16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20440
	 */
	uint16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20456
	 */
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20520
	 */
	uint16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20536
	 */
	uint16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20552
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20616
	 */
	uint16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20632
	 */
	uint16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20648
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20712
	 */
	uint16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20728
	 */
	uint16_t scriptTable4RpmBins[SCRIPT_TABLE_8];
	/**
	 * offset 20744
	 */
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20752
	 */
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20760
	 */
	cyl_trim_s ignTrims[12];
	/**
	 * offset 20952
	 */
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20960
	 */
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20968
	 */
	cyl_trim_s fuelTrims[12];
	/**
	ratio
	 * offset 21160
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	/**
	Airmass
	 * offset 21176
	 */
	uint8_t tcu_pcAirmassBins[8];
	/**
	%
	 * offset 21184
	 */
	uint8_t tcu_pcValsR[8];
	/**
	%
	 * offset 21192
	 */
	uint8_t tcu_pcValsN[8];
	/**
	%
	 * offset 21200
	 */
	uint8_t tcu_pcVals1[8];
	/**
	%
	 * offset 21208
	 */
	uint8_t tcu_pcVals2[8];
	/**
	%
	 * offset 21216
	 */
	uint8_t tcu_pcVals3[8];
	/**
	%
	 * offset 21224
	 */
	uint8_t tcu_pcVals4[8];
	/**
	%
	 * offset 21232
	 */
	uint8_t tcu_pcVals12[8];
	/**
	%
	 * offset 21240
	 */
	uint8_t tcu_pcVals23[8];
	/**
	%
	 * offset 21248
	 */
	uint8_t tcu_pcVals34[8];
	/**
	%
	 * offset 21256
	 */
	uint8_t tcu_pcVals21[8];
	/**
	%
	 * offset 21264
	 */
	uint8_t tcu_pcVals32[8];
	/**
	%
	 * offset 21272
	 */
	uint8_t tcu_pcVals43[8];
	/**
	TPS
	 * offset 21280
	 */
	uint8_t tcu_tccTpsBins[8];
	/**
	MPH
	 * offset 21288
	 */
	uint8_t tcu_tccLockSpeed[8];
	/**
	MPH
	 * offset 21296
	 */
	uint8_t tcu_tccUnlockSpeed[8];
	/**
	KPH
	 * offset 21304
	 */
	uint8_t tcu_32SpeedBins[8];
	/**
	%
	 * offset 21312
	 */
	uint8_t tcu_32Vals[8];
};
static_assert(sizeof(persistent_config_s) == 21320);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on kinetis_gen_config.bat integration/rusefi_config.txt Mon May 02 03:12:42 UTC 2022
