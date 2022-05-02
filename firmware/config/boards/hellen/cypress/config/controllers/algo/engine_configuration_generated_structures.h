// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on hellen_cypress_gen_config.bat integration/rusefi_config.txt Mon May 02 08:19:32 UTC 2022
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
	 * need 4 byte alignment
	units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2];
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
	 * offset 2
	 */
	scaled_channel<uint8_t, 1, 50> sensorSnifferRpmThreshold;
	/**
	 * set rpm_hard_limit X
	rpm
	 * offset 3
	 */
	scaled_channel<uint8_t, 1, 50> rpmHardLimit;
	/**
	 * A secondary Rev limit engaged by the driver to help launch the vehicle faster
	rpm
	 * offset 4
	 */
	scaled_channel<uint8_t, 1, 50> launchRpm;
	/**
	 * Engine sniffer would be disabled above this rpm
	 * set engineSnifferRpmThreshold X
	RPM
	 * offset 5
	 */
	scaled_channel<uint8_t, 1, 50> engineSnifferRpmThreshold;
	/**
	 * Disable multispark above this engine speed.
	rpm
	 * offset 6
	 */
	scaled_channel<uint8_t, 1, 50> multisparkMaxRpm;
	/**
	 * Above this RPM, disable AC. Set to 0 to disable check.
	rpm
	 * offset 7
	 */
	scaled_channel<uint8_t, 1, 50> maxAcRpm;
	/**
	 * Above this TPS, disable AC. Set to 0 to disable check.
	%
	 * offset 8
	 */
	uint8_t maxAcTps;
	/**
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	deg C
	 * offset 9
	 */
	uint8_t maxAcClt;
	/**
	RPM
	 * offset 10
	 */
	scaled_channel<uint8_t, 1, 50> knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	deg
	 * offset 26
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	count
	 * offset 27
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * offset 28
	 */
	injector_s injector;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 96 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On Ford vehicles one of the sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 96 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 96 bit 2 */
	bool isVerboseAuxPid1 : 1 {};
	/**
	offset 96 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 96 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 96 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 96 bit 6 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 96 bit 7 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 96 bit 8 */
	bool enableTrailingSparks : 1 {};
	/**
	 * enable cj125verbose/disable cj125verbose
	offset 96 bit 9 */
	bool isCJ125Verbose : 1 {};
	/**
	 * Is your UA CJ125 output wired to MCU via resistor divider? Ua can go over 3.3v but only at lambda >3, i.e very lean AFR above 44.1
	 * When exposed to free air and 17x gain, Ua will be 4.17 volt
	offset 96 bit 10 */
	bool cj125isUaDivided : 1 {};
	/**
	offset 96 bit 11 */
	bool cj125isLsu49 : 1 {};
	/**
	 * TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 96 bit 12 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 96 bit 13 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 96 bit 14 */
	bool useEeprom : 1 {};
	/**
	 * Is your UR CJ125 output wired to MCU via resistor divider?
	 * Looks like 3v range should be enough, divider generally not needed.
	offset 96 bit 15 */
	bool cj125isUrDivided : 1 {};
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 96 bit 16 */
	bool useCicPidForIdle : 1 {};
	/**
	offset 96 bit 17 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 96 bit 18 */
	bool useInstantRpmForIdle : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 96 bit 19 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 96 bit 20 */
	bool launchControlEnabled : 1 {};
	/**
	offset 96 bit 21 */
	bool rollingLaunchEnabled : 1 {};
	/**
	offset 96 bit 22 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 96 bit 23 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	 * 
	 * enable logic_level_trigger
	offset 96 bit 24 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 96 bit 25 */
	bool useTLE8888_stepper : 1 {};
	/**
	 * If enabled, the MAP estimate table will be used if the MAP sensor fails to estimate manifold pressure based on RPM and TPS.
	offset 96 bit 26 */
	bool enableMapEstimationTableFallback : 1 {};
	/**
	offset 96 bit 27 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 96 bit 28 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 96 bit 29 */
	bool artificialTestMisfire : 1 {};
	/**
	offset 96 bit 30 */
	bool issue_294_31 : 1 {};
	/**
	offset 96 bit 31 */
	bool unusedBit_44_31 : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * set tps_min X
	ADC
	 * offset 100
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * set tps_max X
	ADC
	 * offset 102
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 104
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 106
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 108
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	ms
	 * offset 116
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	rpm
	 * offset 120
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	rpm
	 * offset 122
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 124
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 264
	 */
	ThermistorConf clt;
	/**
	 * offset 296
	 */
	ThermistorConf iat;
	/**
	deg
	 * offset 328
	 */
	int launchTimingRetard;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	integer
	 * offset 332
	 */
	int hip9011PrescalerAndSDO;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	kHz
	 * offset 336
	 */
	float knockBandCustom;
	/**
	 * offset 340
	 */
	specs_s specs;
	/**
	 * Cylinder diameter in mm.
	mm
	 * offset 352
	 */
	float cylinderBore;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS commonly only used for NA engines
	 * Speed Density requires MAP sensor and is the default choice for may installs
	 * MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	 * offset 356
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * offset 357
	 */
	uint8_t alignmentFill_at_1[3];
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 360
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups. If your injectors are individually wired you will also need to enable "Two wire batch emulation". 
	 * set injection_mode X
	 * See also twoWireBatchInjection
	 * offset 361
	 */
	injection_mode_e injectionMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 362
	 */
	uint8_t alignmentFill_at_362[2];
	/**
	 * this is about deciding when the injector starts it's squirt
	 * See also injectionPhase map
	 * todo: do we need even need this since we have the map anyway?
	deg
	 * offset 364
	 */
	angle_t extraInjectionOffset;
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	deg
	 * offset 368
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 372
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	count
	 * offset 373
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	kph
	 * offset 374
	 */
	uint8_t maxIdleVss;
	/**
	 * need 4 byte alignment
	units
	 * offset 375
	 */
	uint8_t alignmentFill_at_375[1];
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	kPa
	 * offset 376
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 378
	 */
	timing_mode_e timingMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 379
	 */
	uint8_t alignmentFill_at_379[1];
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	RPM
	 * offset 380
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchnization point before TDC and negative in case of synchnization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * set global_trigger_offset_angle X
	deg btdc
	 * offset 384
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	coef
	 * offset 388
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	coef
	 * offset 392
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 396
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 400
	 */
	float fanOffTemperature;
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	revs/km
	 * offset 404
	 */
	float driveWheelRevPerKm;
	/**
	 * set can_mode X
	 * offset 408
	 */
	can_nbc_e canNbcType;
	/**
	 * need 4 byte alignment
	units
	 * offset 409
	 */
	uint8_t alignmentFill_at_409[3];
	/**
	 * CANbus thread period in ms
	ms
	 * offset 412
	 */
	int canSleepPeriodMs;
	/**
	 * offset 416
	 */
	display_mode_e displayMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 417
	 */
	uint8_t alignmentFill_at_417[3];
	/**
	index
	 * offset 420
	 */
	int byFirmwareVersion;
	/**
	index
	 * offset 424
	 */
	int HD44780width;
	/**
	index
	 * offset 428
	 */
	int HD44780height;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
	 * offset 432
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 433
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 434
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * set_analog_input_pin tps2 X
	 * offset 435
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	x
	 * offset 436
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	angle
	 * offset 440
	 */
	int trailingSparkAngle;
	/**
	 * offset 444
	 */
	trigger_config_s trigger;
	/**
	 * offset 460
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * Single value to be used in event of a failed MAP sensor 
	 * This value is only used for speed density fueling calculations.
	kPa
	 * offset 461
	 */
	uint8_t failedMapFallback;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost.
	%
	 * offset 462
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 463
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * set global_fuel_correction X
	coef
	 * offset 464
	 */
	float globalFuelCorrection;
	/**
	volts
	 * offset 468
	 */
	float adcVcc;
	/**
	Deg
	 * offset 472
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 476
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 484
	 */
	uint8_t camInputsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 484
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * set_analog_input_pin pps X
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 504
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 505
	 */
	uint8_t alignmentFill_at_505[1];
	/**
	 * offset 506
	 */
	Gpio tle6240_cs;
	/**
	 * offset 508
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 509
	 */
	uint8_t alignmentFill_at_509[1];
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 510
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * @see hasBaroSensor
	 * offset 512
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 524
	 */
	idle_hardware_s idle;
	/**
	 * Value between 0 and 100 used in Manual mode
	%
	 * offset 536
	 */
	float manIdlePosition;
	/**
	 * Ignition timing to remove when a knock event occurs.
	%
	 * offset 540
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	deg/s
	 * offset 541
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Maximum amount of knock retard.
	deg
	 * offset 542
	 */
	uint8_t knockRetardMaximum;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 543
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	ratio
	 * offset 544
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	count
	 * offset 546
	 */
	uint8_t vssToothCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 547
	 */
	uint8_t alignmentFill_at_547[1];
	/**
	 * offset 548
	 */
	Gpio l9779_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 550
	 */
	uint8_t alignmentFill_at_550[2];
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * TODO: rename to triggerSimulatorRpm
	Rpm
	 * offset 552
	 */
	int triggerSimulatorFrequency;
	/**
	 * offset 556
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 580
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 604
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 605
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 606
	 */
	Gpio HD44780_rs;
	/**
	 * offset 608
	 */
	Gpio HD44780_e;
	/**
	 * offset 610
	 */
	Gpio HD44780_db4;
	/**
	 * offset 612
	 */
	Gpio HD44780_db5;
	/**
	 * offset 614
	 */
	Gpio HD44780_db6;
	/**
	 * offset 616
	 */
	Gpio HD44780_db7;
	/**
	 * offset 618
	 */
	Gpio gps_rx_pin;
	/**
	 * offset 620
	 */
	Gpio gps_tx_pin;
	/**
	 * offset 622
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 624
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 625
	 */
	uint8_t alignmentFill_at_625[1];
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 626
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 628
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 629
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 630
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 632
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 634
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 636
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 637
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 638
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 646
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 647
	 */
	uint8_t alignmentFill_at_647[1];
	/**
	 * set_cj125_heater_pin XXX
	 * offset 648
	 */
	Gpio wboHeaterPin;
	/**
	 * set_cj125_cs_pin XXX
	 * offset 650
	 */
	Gpio cj125CsPin;
	/**
	 * offset 652
	 */
	spi_device_e max31855spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 653
	 */
	uint8_t alignmentFill_at_653[1];
	/**
	 * offset 654
	 */
	Gpio debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 656
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 657
	 */
	uint8_t alignmentFill_at_657[1];
	/**
	 * offset 658
	 */
	Gpio mc33972_cs;
	/**
	 * offset 660
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 661
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 662
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 663
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 664
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	%
	 * offset 665
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 673
	 */
	ego_sensor_e afr_type;
	/**
	 * need 4 byte alignment
	units
	 * offset 674
	 */
	uint8_t alignmentFill_at_674[2];
	/**
	 * 0.1 is a good default value
	x
	 * offset 676
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 680
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 686
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 687
	 */
	uint8_t alignmentFill_at_687[1];
	/**
	 * offset 688
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 690
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 691
	 */
	uint8_t alignmentFill_at_691[1];
	/**
	 * offset 692
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 694
	 */
	Gpio sdCardCsPin;
	/**
	 * set_can_tx_pin X
	 * offset 696
	 */
	Gpio canTxPin;
	/**
	 * set_can_rx_pin X
	 * offset 698
	 */
	Gpio canRxPin;
	/**
	 * offset 700
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * Additional idle % while A/C is active
	%
	 * offset 701
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * Ratio between the wheels and your transmission output.
	ratio
	 * offset 702
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 704
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 706
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	units
	 * offset 707
	 */
	uint8_t alignmentFill_at_707[1];
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	mv
	 * offset 708
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 710
	 */
	uint16_t wastegatePositionMax;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	mv
	 * offset 712
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 714
	 */
	uint16_t idlePositionMax;
	/**
	 * Secondary TTL channel baud rate
	BPs
	 * offset 716
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	CR
	 * offset 720
	 */
	float compressionRatio;
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 724
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 730
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 733
	 */
	uint8_t alignmentFill_at_733[1];
	/**
	 * Narrow band o2 heater, not used for CJ125. 'ON' if engine is running, 'OFF' if stopped or cranking. See wboHeaterPin
	 * offset 734
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 736
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * need 4 byte alignment
	units
	 * offset 737
	 */
	uint8_t alignmentFill_at_737[3];
	/**
	offset 740 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 740 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 740 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 740 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 740 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 740 bit 5 */
	bool isEngineControlEnabled : 1 {};
	/**
	offset 740 bit 6 */
	bool isHip9011Enabled : 1 {};
	/**
	offset 740 bit 7 */
	bool isVerboseAlternator : 1 {};
	/**
	offset 740 bit 8 */
	bool useSerialPort : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 740 bit 9 */
	bool useStepperIdle : 1 {};
	/**
	offset 740 bit 10 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 740 bit 11 */
	bool useTpicAdvancedMode : 1 {};
	/**
	offset 740 bit 12 */
	bool useLcdScreen : 1 {};
	/**
	offset 740 bit 13 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 740 bit 14 */
	bool enableVerboseCanTx : 1 {};
	/**
	 * This will cause the alternator to be operated in a basic on or off mode, this is the simplest alternator control.
	offset 740 bit 15 */
	bool onOffAlternatorLogic : 1 {};
	/**
	 * enable cj125/disable cj125
	offset 740 bit 16 */
	bool isCJ125Enabled : 1 {};
	/**
	 * Use rise or fall signal front
	 * get vvtCamSensorUseRise
	offset 740 bit 17 */
	bool vvtCamSensorUseRise : 1 {};
	/**
	 * Useful for individual intakes
	offset 740 bit 18 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 740 bit 19 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 740 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 740 bit 21 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * This setting allows the ECU to open the IAC during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars. Used in Auto-PID Idle mode.
	offset 740 bit 22 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 740 bit 23 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 740 bit 24 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 740 bit 25 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 740 bit 26 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 740 bit 27 */
	bool pauseEtbControl : 1 {};
	/**
	offset 740 bit 28 */
	bool alignEngineSnifferAtTDC : 1 {};
	/**
	 * This setting allows the ETB to act as the idle air control valve and move to regulate the airflow at idle.
	offset 740 bit 29 */
	bool useETBforIdleControl : 1 {};
	/**
	offset 740 bit 30 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 740 bit 31 */
	bool enableAemXSeries : 1 {};
	/**
	 * offset 744
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 752
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 753
	 */
	uint8_t alignmentFill_at_753[1];
	/**
	 * offset 754
	 */
	Gpio hip9011CsPin;
	/**
	 * offset 756
	 */
	Gpio hip9011IntHoldPin;
	/**
	 * offset 758
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 759
	 */
	uint8_t alignmentFill_at_759[1];
	/**
	 * offset 760
	 */
	uint32_t verboseCanBaseAddress;
	/**
	v
	 * offset 764
	 */
	uint8_t mc33_hvolt;
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	kPa
	 * offset 765
	 */
	uint8_t minimumBoostClosedLoopMap;
	/**
	 * Optional Radiator Fan used with A/C
	 * offset 766
	 */
	output_pin_e acFanPin;
	/**
	 * offset 768
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * offset 769
	 */
	spi_device_e l9779spiDevice;
	/**
	volts
	 * offset 770
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	 * offset 778
	 */
	imu_type_e imuType;
	/**
	multiplier
	 * offset 779
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 787
	 */
	uint8_t alignmentFill_at_787[1];
	/**
	kg
	 * offset 788
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling?
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	RPM
	 * offset 790
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	ms
	 * offset 792
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 794
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 796
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 798
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 800
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 802
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 803
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 804
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 820
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * offset 822
	 */
	Gpio test557pin;
	/**
	 * offset 824
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 825
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	:1
	 * offset 826
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	%
	 * offset 827
	 */
	uint8_t etbMaximumPosition;
	/**
	 * SD card logging period, in milliseconds
	ms
	 * offset 828
	 */
	int16_t sdCardPeriodMs;
	/**
	 * offset 830
	 */
	adc_channel_e idlePositionSensor;
	/**
	 * need 4 byte alignment
	units
	 * offset 831
	 */
	uint8_t alignmentFill_at_831[1];
	/**
	 * offset 832
	 */
	Gpio debugMapAveraging;
	/**
	 * offset 834
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 836
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 837
	 */
	uint8_t alignmentFill_at_837[1];
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 838
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 840
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 842
	 */
	uint8_t alignmentFill_at_842[2];
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	count
	 * offset 844
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	%
	 * offset 848
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	%
	 * offset 850
	 */
	int16_t stepperParkingExtraSteps;
	/**
	ADC
	 * offset 852
	 */
	uint16_t tps1SecondaryMin;
	/**
	ADC
	 * offset 854
	 */
	uint16_t tps1SecondaryMax;
	/**
	rpm
	 * offset 856
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	Seconds
	 * offset 858
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 860
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 862
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 863
	 */
	uint8_t alignmentFill_at_863[1];
	/**
	 * offset 864
	 */
	output_pin_e acRelayPin;
	/**
	 * offset 866
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 867
	 */
	uint8_t alignmentFill_at_867[1];
	/**
	 * offset 868
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 900
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 902
	 */
	Gpio spi1misoPin;
	/**
	 * offset 904
	 */
	Gpio spi1sckPin;
	/**
	 * offset 906
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 908
	 */
	Gpio spi2misoPin;
	/**
	 * offset 910
	 */
	Gpio spi2sckPin;
	/**
	 * offset 912
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 914
	 */
	Gpio spi3misoPin;
	/**
	 * offset 916
	 */
	Gpio spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 918
	 */
	Gpio cdmInputPin;
	/**
	 * offset 920
	 */
	Gpio joystickCenterPin;
	/**
	 * offset 922
	 */
	Gpio joystickAPin;
	/**
	 * offset 924
	 */
	Gpio joystickBPin;
	/**
	 * offset 926
	 */
	Gpio joystickCPin;
	/**
	 * offset 928
	 */
	Gpio joystickDPin;
	/**
	 * offset 930
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEFI console Sensor Sniffer mode
	 * offset 931
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 932
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * need 4 byte alignment
	units
	 * offset 933
	 */
	uint8_t alignmentFill_at_933[3];
	/**
	offset 936 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 936 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 936 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 936 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 936 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 936 bit 5 */
	bool unfinishedenableLaunchBoost : 1 {};
	/**
	offset 936 bit 6 */
	bool unfinishedlaunchDisableBySpeed : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 936 bit 7 */
	bool enableCanVss : 1 {};
	/**
	offset 936 bit 8 */
	bool enableInnovateLC2 : 1 {};
	/**
	offset 936 bit 9 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise. Use of this mode is required if you have a narrowband O2 sensor.
	offset 936 bit 10 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 936 bit 11 */
	bool tempBooleanForVerySpecialCases : 1 {};
	/**
	offset 936 bit 12 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * enable vvt_details
	offset 936 bit 13 */
	bool verboseVVTDecoding : 1 {};
	/**
	 * get invertCamVVTSignal
	offset 936 bit 14 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	 * enable consumeObdSensors
	offset 936 bit 15 */
	bool consumeObdSensors : 1 {};
	/**
	offset 936 bit 16 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 936 bit 17 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 936 bit 18 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 936 bit 19 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 936 bit 20 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 936 bit 21 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 936 bit 22 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 936 bit 23 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 936 bit 24 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 936 bit 25 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 936 bit 26 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 936 bit 27 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 936 bit 28 */
	bool tcuEnabled : 1 {};
	/**
	offset 936 bit 29 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coilss
	offset 936 bit 30 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 936 bit 31 */
	bool verboseCan2 : 1 {};
	/**
	 * offset 940
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 956
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 958
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 959
	 */
	uint8_t alignmentFill_at_959[1];
	/**
	 * offset 960
	 */
	switch_input_pin_e antiLagActivatePin;
	/**
	 * offset 962
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 964
	 */
	pid_s boostPid;
	/**
	 * offset 984
	 */
	boostType_e boostType;
	/**
	 * need 4 byte alignment
	units
	 * offset 985
	 */
	uint8_t alignmentFill_at_985[3];
	/**
	Hz
	 * offset 988
	 */
	int boostPwmFrequency;
	/**
	 * offset 992
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 993
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 994
	 */
	uint8_t alignmentFill_at_994[2];
	/**
	 * Disabled above this speed
	Kph
	 * offset 996
	 */
	int launchSpeedThreshold;
	/**
	 * Range from Launch Rpm for Timing Retard to activate
	RPM
	 * offset 1000
	 */
	int launchTimingRpmRange;
	/**
	 * Extra Fuel Added
	%
	 * offset 1004
	 */
	int launchFuelAdded;
	/**
	 * Duty Cycle for the Boost Solenoid
	%
	 * offset 1008
	 */
	int launchBoostDuty;
	/**
	 * RPM Range for Hard Cut
	rpm
	 * offset 1012
	 */
	int hardCutRpmRange;
	/**
	rpm
	 * offset 1016
	 */
	int launchAdvanceRpmRange;
	/**
	rpm
	 * offset 1020
	 */
	int launchTpsTreshold;
	/**
	rpm
	 * offset 1024
	 */
	float launchActivateDelay;
	/**
	 * offset 1028
	 */
	stft_s stft;
	/**
	 * offset 1052
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1068
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1100
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1132
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1164
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * offset 1176
	 */
	etb_function_e etbFunctions[ETB_COUNT];
	/**
	 * offset 1178
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1179
	 */
	uint8_t alignmentFill_at_1179[1];
	/**
	 * offset 1180
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1182
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1183
	 */
	uint8_t alignmentFill_at_1183[1];
	/**
	 * offset 1184
	 */
	Gpio drv8860_miso;
	/**
	volt
	 * offset 1186
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * offset 1202
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1218
	 */
	uint8_t alignmentFill_at_1218[2];
	/**
	 * Angle between cam sensor and VVT zero position
	 * set vvt_offset X
	value
	 * offset 1220
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1236
	 */
	float vvtOffsetsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 1236
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1268
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	ADC
	 * offset 1332
	 */
	uint16_t tps2SecondaryMin;
	/**
	ADC
	 * offset 1334
	 */
	uint16_t tps2SecondaryMax;
	/**
	offset 1336 bit 0 */
	bool disablePrimaryUart : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1336 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Print details into rusEFI console
	 * enable verbose_idle
	offset 1336 bit 2 */
	bool isVerboseIAC : 1 {};
	/**
	offset 1336 bit 3 */
	bool boardUseTachPullUp : 1 {};
	/**
	offset 1336 bit 4 */
	bool boardUseTempPullUp : 1 {};
	/**
	 * This options enables data for 'engine sniffer' tab in console, which comes at some CPU price
	offset 1336 bit 5 */
	bool isEngineChartEnabled : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1336 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1336 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1336 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write
	offset 1336 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1336 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1336 bit 11 */
	bool boardUse2stepPullDown : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1336 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1336 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1336 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1336 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1336 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	offset 1336 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1336 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1336 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1336 bit 20 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 1336 bit 21 */
	bool boardUseCamPullDown : 1 {};
	/**
	offset 1336 bit 22 */
	bool boardUseCamVrPullUp : 1 {};
	/**
	offset 1336 bit 23 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 1336 bit 24 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 1336 bit 25 */
	bool boardUseD4PullDown : 1 {};
	/**
	offset 1336 bit 26 */
	bool boardUseD5PullDown : 1 {};
	/**
	 * Sometimes we just have to shut the engine down. Use carefully!
	offset 1336 bit 27 */
	bool useFSIO5ForCriticalIssueEngineStop : 1 {};
	/**
	 * Sometimes we have to miss injection on purpose to attract driver's attention
	offset 1336 bit 28 */
	bool useFSIO4ForSeriousEngineWarning : 1 {};
	/**
	offset 1336 bit 29 */
	bool launchActivateInverted : 1 {};
	/**
	offset 1336 bit 30 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1336 bit 31 */
	bool skippedWheelOnCam : 1 {};
	/**
	 * offset 1340
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 1341
	 */
	uint8_t alignmentFill_at_1341[1];
	/**
	 * A/C button input
	 * offset 1342
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1344
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	%
	 * offset 1345
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1346
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1347
	 */
	uint8_t alignmentFill_at_1347[1];
	/**
	 * Enable fuel injection - This is default off for new projects as a safety feature, set to "true" to enable fuel injection and further injector settings.
	offset 1348 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	 * Enable ignition - This is default off for new projects as a safety feature, set to "true" to enable ignition and further ignition settings.
	offset 1348 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1348 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1348 bit 3 */
	bool complexWallModel : 1 {};
	/**
	offset 1348 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1348 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1348 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1348 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1348 bit 8 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1348 bit 9 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * enable trigger_details
	offset 1348 bit 10 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1348 bit 11 */
	bool isManualSpinningMode : 1 {};
	/**
	 * This is needed if your coils are individually wired and you wish to use batch injection.
	 * enable two_wire_batch_injection
	offset 1348 bit 12 */
	bool twoWireBatchInjection : 1 {};
	/**
	 * VR sensors are only precise on rising front
	 * enable trigger_only_front
	offset 1348 bit 13 */
	bool useOnlyRisingEdgeForTrigger : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1348 bit 14 */
	bool twoWireBatchIgnition : 1 {};
	/**
	offset 1348 bit 15 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1348 bit 16 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1348 bit 17 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1348 bit 18 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1348 bit 19 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1348 bit 20 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1348 bit 21 */
	bool launchSmoothRetard : 1 {};
	/**
	offset 1348 bit 22 */
	bool unused1476b20 : 1 {};
	/**
	offset 1348 bit 23 */
	bool unused1476b8 : 1 {};
	/**
	offset 1348 bit 24 */
	bool unused_1484_bit_24 : 1 {};
	/**
	offset 1348 bit 25 */
	bool unused_1484_bit_25 : 1 {};
	/**
	offset 1348 bit 26 */
	bool unused_1484_bit_26 : 1 {};
	/**
	offset 1348 bit 27 */
	bool unused_1484_bit_27 : 1 {};
	/**
	offset 1348 bit 28 */
	bool unused_1484_bit_28 : 1 {};
	/**
	offset 1348 bit 29 */
	bool unused_1484_bit_29 : 1 {};
	/**
	offset 1348 bit 30 */
	bool unused_1484_bit_30 : 1 {};
	/**
	offset 1348 bit 31 */
	bool unused_1484_bit_31 : 1 {};
	/**
	count
	 * offset 1352
	 */
	uint32_t engineChartSize;
	/**
	mult
	 * offset 1356
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1360
	 */
	Gpio camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * offset 1368
	 */
	uint8_t camInputsDebugPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	RPM
	 * offset 1368
	 */
	int16_t acIdleRpmBump;
	/**
	 * set warningPeriod X
	seconds
	 * offset 1370
	 */
	int16_t warningPeriod;
	/**
	angle
	 * offset 1372
	 */
	float knockDetectionWindowStart;
	/**
	angle
	 * offset 1376
	 */
	float knockDetectionWindowEnd;
	/**
	ms
	 * offset 1380
	 */
	float idleStepperReactionTime;
	/**
	count
	 * offset 1384
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	sec
	 * offset 1388
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	index
	 * offset 1392
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1396
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	Seconds
	 * offset 1400
	 */
	float wwaeTau;
	/**
	 * offset 1404
	 */
	pid_s alternatorControl;
	/**
	 * offset 1424
	 */
	pid_s etb;
	/**
	 * offset 1444
	 */
	Gpio triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 1450
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * set tps2_min X
	ADC
	 * offset 1452
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps2_max X
	ADC
	 * offset 1454
	 */
	int16_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1456
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1458
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1459
	 */
	uint8_t alignmentFill_at_1459[1];
	/**
	 * offset 1460
	 */
	Gpio mc33816_flag0;
	/**
	Pulse
	 * offset 1462
	 */
	uint8_t tachPulsePerRev;
	/**
	 * need 4 byte alignment
	units
	 * offset 1463
	 */
	uint8_t alignmentFill_at_1463[1];
	/**
	 * kPa value which is too low to be true
	kPa
	 * offset 1464
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	kPa
	 * offset 1468
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	ms
	 * offset 1472
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	ms
	 * offset 1474
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1476
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	Fraction
	 * offset 1496
	 */
	float wwaeBeta;
	/**
	 * blue LED on many rusEFI boards.
	 * Blue Communication LED which is expected to blink at 50% duty cycle during normal board operation.
	 * If USB communication cable is connected Blue LED starts to blink faster.
	 * offset 1500
	 */
	Gpio communicationLedPin;
	/**
	 * Green LED on many rusEFI boards.
	 * Off if engine is stopped, blinks if engine is cranking, solid if engine is running.
	 * offset 1502
	 */
	Gpio runningLedPin;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1504
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1506
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1508
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * offset 1512
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1514
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	voltage
	 * offset 1516
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1520
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	seconds
	 * offset 1524
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	RPM
	 * offset 1526
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	Volts
	 * offset 1528
	 */
	float targetVBatt;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	%
	 * offset 1532
	 */
	float alternatorOffAboveTps;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	cycles
	 * offset 1536
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	percent
	 * offset 1538
	 */
	int16_t iacByTpsTaper;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_tx_pin X
	 * offset 1540
	 */
	Gpio auxSerialTxPin;
	/**
	 * offset 1542
	 */
	Gpio warningLedPin;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_rx_pin X
	 * offset 1544
	 */
	Gpio auxSerialRxPin;
	/**
	 * offset 1546
	 */
	Gpio LIS302DLCsPin;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	sec
	 * offset 1548
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 1549
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 1550
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	sec
	 * offset 1551
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	roc
	 * offset 1552
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1556
	 */
	brain_input_pin_e auxSpeedSensorInputPin[2];
	/**
	 * offset 1560
	 */
	uint8_t totalGearsCount;
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 1561
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1562
	 */
	uint8_t alignmentFill_at_1562[2];
	/**
	 * Band rate for primary TTL
	BPs
	 * offset 1564
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	roc
	 * offset 1568
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	coeff
	 * offset 1572
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	coeff
	 * offset 1576
	 */
	float slowAdcAlpha;
	/**
	 * See http://rusefi.com/s/debugmode
	 * 
	 * set debug_mode X
	 * offset 1580
	 */
	debug_mode_e debugMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1581
	 */
	uint8_t alignmentFill_at_1581[3];
	/**
	BPs
	 * offset 1584
	 */
	uint32_t auxSerialSpeed;
	/**
	voltage
	 * offset 1588
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1592
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * set can_baudrate
	 * offset 1596
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1597
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1598
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1599
	 */
	load_override_e afrOverrideMode;
	/**
	A
	 * offset 1600
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	A
	 * offset 1601
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	us
	 * offset 1602
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	ms
	 * offset 1603
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1604 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	offset 1604 bit 1 */
	bool unused1740b0 : 1 {};
	/**
	offset 1604 bit 2 */
	bool unused1740b1 : 1 {};
	/**
	offset 1604 bit 3 */
	bool unused1740b2 : 1 {};
	/**
	offset 1604 bit 4 */
	bool unused1127 : 1 {};
	/**
	offset 1604 bit 5 */
	bool unused1128 : 1 {};
	/**
	offset 1604 bit 6 */
	bool unused1129 : 1 {};
	/**
	offset 1604 bit 7 */
	bool unused1130 : 1 {};
	/**
	offset 1604 bit 8 */
	bool unusedBit_536_8 : 1 {};
	/**
	offset 1604 bit 9 */
	bool unusedBit_536_9 : 1 {};
	/**
	offset 1604 bit 10 */
	bool unusedBit_536_10 : 1 {};
	/**
	offset 1604 bit 11 */
	bool unusedBit_536_11 : 1 {};
	/**
	offset 1604 bit 12 */
	bool unusedBit_536_12 : 1 {};
	/**
	offset 1604 bit 13 */
	bool unusedBit_536_13 : 1 {};
	/**
	offset 1604 bit 14 */
	bool unusedBit_536_14 : 1 {};
	/**
	offset 1604 bit 15 */
	bool unusedBit_536_15 : 1 {};
	/**
	offset 1604 bit 16 */
	bool unusedBit_536_16 : 1 {};
	/**
	offset 1604 bit 17 */
	bool unusedBit_536_17 : 1 {};
	/**
	offset 1604 bit 18 */
	bool unusedBit_536_18 : 1 {};
	/**
	offset 1604 bit 19 */
	bool unusedBit_536_19 : 1 {};
	/**
	offset 1604 bit 20 */
	bool unusedBit_536_20 : 1 {};
	/**
	offset 1604 bit 21 */
	bool unusedBit_536_21 : 1 {};
	/**
	offset 1604 bit 22 */
	bool unusedBit_536_22 : 1 {};
	/**
	offset 1604 bit 23 */
	bool unusedBit_536_23 : 1 {};
	/**
	offset 1604 bit 24 */
	bool unusedBit_536_24 : 1 {};
	/**
	offset 1604 bit 25 */
	bool unusedBit_536_25 : 1 {};
	/**
	offset 1604 bit 26 */
	bool unusedBit_536_26 : 1 {};
	/**
	offset 1604 bit 27 */
	bool unusedBit_536_27 : 1 {};
	/**
	offset 1604 bit 28 */
	bool unusedBit_536_28 : 1 {};
	/**
	offset 1604 bit 29 */
	bool unusedBit_536_29 : 1 {};
	/**
	offset 1604 bit 30 */
	bool unusedBit_536_30 : 1 {};
	/**
	offset 1604 bit 31 */
	bool unusedBit_536_31 : 1 {};
	/**
	ms
	 * offset 1608
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOffTime;
	/**
	count
	 * offset 1609
	 */
	uint8_t benchTestCount;
	/**
	ms
	 * offset 1610
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOnTime;
	/**
	 * offset 1611
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * set_can2_tx_pin X
	 * offset 1612
	 */
	Gpio can2TxPin;
	/**
	 * set_can2_rx_pin X
	 * offset 1614
	 */
	Gpio can2RxPin;
	/**
	 * offset 1616
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1617
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1618
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1619
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1620
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1622
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1623
	 */
	uint8_t alignmentFill_at_1623[1];
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	kPa (absolute)
	 * offset 1624
	 */
	float boostCutPressure;
	/**
	kg/h
	 * offset 1628
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16];
	/**
	ratio
	 * offset 1644
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16];
	/**
	 * Fixed timing, useful for TDC testing
	deg
	 * offset 1660
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	v
	 * offset 1664
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	v
	 * offset 1668
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	value
	 * offset 1672
	 */
	float egoValueShift;
	/**
	 * offset 1676
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * offset 1684
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * offset 1685
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * offset 1686
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1687
	 */
	uint8_t alignmentFill_at_1687[1];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	percent
	 * offset 1688
	 */
	int crankingIACposition;
	/**
	 * offset 1692
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 1696
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 1700
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 1704
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 1708
	 */
	pwm_freq_t vvtOutputFrequency[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #1 is active
	%
	 * offset 1712
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * need 4 byte alignment
	units
	 * offset 1713
	 */
	uint8_t alignmentFill_at_1713[3];
	/**
	Hz
	 * offset 1716
	 */
	int alternatorPwmFrequency;
	/**
	 * set vvt_mode X
	 * offset 1720
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * offset 1722
	 */
	uint8_t vvtModePadding[CAMS_PER_BANK_padding];
	/**
	 * Additional idle % when fan #2 is active
	%
	 * offset 1722
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	sec
	 * offset 1723
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1724
	 */
	adc_channel_e auxAnalogInputs[AUX_ANALOG_INPUT_COUNT];
	/**
	 * offset 1732
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 1756
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1757
	 */
	pin_output_mode_e LIS302DLCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 1758
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 1759
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	kPa
	 * offset 1760
	 */
	float fuelReferencePressure;
	/**
	 * Fuel multiplier (enrichment) immediately after engine start
	mult
	 * offset 1764
	 */
	float postCrankingFactor;
	/**
	 * Time over which to taper out after start enrichment
	seconds
	 * offset 1768
	 */
	float postCrankingDurationSec;
	/**
	 * offset 1772
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1804
	 */
	ThermistorConf auxTempSensor2;
	/**
	Deg
	 * offset 1836
	 */
	int16_t knockSamplingDuration;
	/**
	Hz
	 * offset 1838
	 */
	int16_t etbFreq;
	/**
	 * offset 1840
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 1860
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	%
	 * offset 1861
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	%
	 * offset 1862
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 1863
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder timing correction
	deg
	 * offset 1864
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	seconds
	 * offset 1912
	 */
	float idlePidActivationTime;
	/**
	 * offset 1916
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 1917
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 1918
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 1919
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 1920
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 1921
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 1922
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 1923
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 1924
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 1925
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 1926
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 1928
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 1930
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * lambda input
	 * offset 1932
	 */
	adc_channel_e cj125ua;
	/**
	 * heater input
	 * offset 1933
	 */
	adc_channel_e cj125ur;
	/**
	 * offset 1934
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1935
	 */
	uint8_t alignmentFill_at_1935[1];
	/**
	 * offset 1936
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 1976
	 */
	float injectorCorrectionPolynomial[8];
	/**
	C
	 * offset 2008
	 */
	int8_t primeBins[8];
	/**
	 * offset 2016
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2036
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 2037
	 */
	uint8_t alignmentFill_at_2037[1];
	/**
	 * offset 2038
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 2040
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 2041
	 */
	uint8_t fan2OffTemperature;
	/**
	 * offset 2042
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2044
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2046
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2047
	 */
	uint8_t alignmentFill_at_2047[1];
	/**
	 * offset 2048
	 */
	Gpio mc33816_cs;
	/**
	 * todo: more comments
	 * offset 2050
	 */
	Gpio servoOutputPins[SERVO_COUNT];
	/**
	 * This sets the RPM above which fuel cut is active.
	rpm
	 * offset 2066
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	rpm
	 * offset 2068
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	%
	 * offset 2070
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	C
	 * offset 2072
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	%
	 * offset 2074
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	kPa
	 * offset 2076
	 */
	int16_t coastingFuelCutMap;
	/**
	 * need 4 byte alignment
	units
	 * offset 2078
	 */
	uint8_t alignmentFill_at_2078[2];
	/**
	 * offset 2080
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2100
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * CLT-based target RPM for hard limit depending on CLT like on Lexus LFA
	C
	 * offset 2120
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 2124
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * offset 2132
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	 * offset 2228
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 2292
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2420
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2424
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	kg/h
	 * offset 2428
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	deg/sec
	 * offset 2432
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	deg/sec
	 * offset 2436
	 */
	float tChargeAirDecrLimit;
	/**
	 * offset 2440
	 */
	tChargeMode_e tChargeMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2441
	 */
	uint8_t alignmentFill_at_2441[3];
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	target TPS position
	 * offset 2444
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	ETB duty cycle bias
	 * offset 2476
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	 * offset 2508
	 */
	float hip9011Gain;
	/**
	 * iTerm min value
	 * offset 2512
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2514
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2516
	 */
	pid_s idleTimingPid;
	/**
	 * If the RPM closer to target than this value, disable timing correction to prevent oscillation
	RPM
	 * offset 2536
	 */
	int16_t idleTimingPidDeadZone;
	/**
	 * A delay in cycles between fuel-enrich. portions
	cycles
	 * offset 2538
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	coef
	 * offset 2540
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2544
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2545
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2546
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2548
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	:1
	 * offset 2549
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2550
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	%
	 * offset 2552
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2556
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	mg
	 * offset 2568
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[8];
	/**
	 * Trigger comparator center point voltage
	V
	 * offset 2576
	 */
	uint8_t triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	V
	 * offset 2577
	 */
	uint8_t triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	V
	 * offset 2578
	 */
	uint8_t triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	RPM
	 * offset 2579
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * offset 2580
	 */
	pid_s idleRpmPid2;
	/**
	 * set can_vss X
	 * offset 2600
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	units
	 * offset 2601
	 */
	uint8_t alignmentFill_at_2601[3];
	/**
	 * offset 2604
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	mA
	 * offset 2972
	 */
	uint16_t mc33_i_boost;
	/**
	mA
	 * offset 2974
	 */
	uint16_t mc33_i_peak;
	/**
	mA
	 * offset 2976
	 */
	uint16_t mc33_i_hold;
	/**
	us
	 * offset 2978
	 */
	uint16_t mc33_t_max_boost;
	/**
	us
	 * offset 2980
	 */
	uint16_t mc33_t_peak_off;
	/**
	us
	 * offset 2982
	 */
	uint16_t mc33_t_peak_tot;
	/**
	us
	 * offset 2984
	 */
	uint16_t mc33_t_bypass;
	/**
	us
	 * offset 2986
	 */
	uint16_t mc33_t_hold_off;
	/**
	us
	 * offset 2988
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 2990
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 2991
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 2992
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 2993
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	dB
	 * offset 2999
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 3015
	 */
	uint8_t alignmentFill_at_3015[1];
	/**
	from
	 * offset 3016
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	 * offset 3088
	 */
	int8_t unused4080[12];
	/**
	to
	 * offset 3100
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	lobes/cam
	 * offset 3172
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 3173
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Crank angle ATDC of first lobe peak
	deg
	 * offset 3174
	 */
	uint8_t hpfpPeakPos;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	deg
	 * offset 3175
	 */
	uint8_t hpfpMinAngle;
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	cc
	 * offset 3176
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	deg
	 * offset 3178
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 3179
	 */
	uint8_t issFilterReciprocal;
	/**
	%/kPa
	 * offset 3180
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	%/kPa/lobe
	 * offset 3182
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	kPa/s
	 * offset 3184
	 */
	uint16_t hpfpTargetDecay;
	/**
	%
	 * offset 3186
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	deg
	 * offset 3202
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	volts
	 * offset 3218
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	ms
	 * offset 3226
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	kPa
	 * offset 3242
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	load
	 * offset 3442
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	RPM
	 * offset 3462
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	%
	 * offset 3472
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	cc/lobe
	 * offset 3572
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	RPM
	 * offset 3592
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	 * offset 3602
	 */
	output_pin_e stepper_raw_output[4];
	/**
	ratio
	 * offset 3610
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[GEARS_COUNT];
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	ms
	 * offset 3626
	 */
	uint16_t vvtActivationDelayMs;
	/**
	deg C
	 * offset 3628
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	/**
	 * offset 3636
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3644
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	/**
	kPa
	 * offset 3652
	 */
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	/**
	 * offset 3660
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3668
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	/**
	Nm
	 * offset 3676
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_SIZE];
	/**
	RPM
	 * offset 3712
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE];
	/**
	Load
	 * offset 3724
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE];
	/**
	 * offset 3736
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3737
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3738
	 */
	uint8_t alignmentFill_at_3738[2];
	/**
	 * offset 3740
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3760
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 3780
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 3782
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3783
	 */
	uint8_t alignmentFill_at_3783[1];
	/**
	 * offset 3784
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 3786
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3787
	 */
	uint8_t alignmentFill_at_3787[1];
	/**
	 * offset 3788
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 3790
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 3792
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3793
	 */
	uint8_t alignmentFill_at_3793[1];
	/**
	 * offset 3794
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 3796
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 3798
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3799
	 */
	uint8_t alignmentFill_at_3799[1];
	/**
	 * offset 3800
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * need 4 byte alignment
	units
	 * offset 3802
	 */
	uint8_t alignmentFill_at_3802[2];
	/**
	 * offset 3804
	 */
	float etbMinimumPosition;
	/**
	units
	 * offset 3808
	 */
	uint8_t mainUnusedEnd[246];
	/**
	 * need 4 byte alignment
	units
	 * offset 4054
	 */
	uint8_t alignmentFill_at_4054[2];
};
static_assert(sizeof(engine_configuration_s) == 4056);

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
	%
	 * offset 4056
	 */
	uint8_t iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	Load
	 * offset 4120
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	RPM
	 * offset 4128
	 */
	scaled_channel<uint8_t, 1, 50> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	RPM
	 * offset 4136
	 */
	scaled_channel<uint8_t, 1, 50> sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	ms
	 * offset 4144
	 */
	float sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * CLT-based target RPM for automatic idle controller
	C
	 * offset 4176
	 */
	float cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	RPM
	 * offset 4240
	 */
	scaled_channel<uint8_t, 1, 50> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * CLT-based timing correction
	C
	 * offset 4256
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	degree
	 * offset 4288
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	x
	 * offset 4320
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 4384
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	x
	 * offset 4448
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 4512
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	x
	 * offset 4576
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4608
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4640
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4672
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4704
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4736
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4768
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4800
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	kPa
	 * offset 4832
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	RPM
	 * offset 4848
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	ratio
	 * offset 4864
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	Ratio
	 * offset 4928
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	%
	 * offset 4960
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Narrow Band WBO Approximation
	V
	 * offset 4992
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	ratio
	 * offset 5024
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	RPM
	 * offset 5056
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	deg
	 * offset 5072
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	C
	 * offset 5088
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	%
	 * offset 5152
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 5216
	 */
	error_message_t warning_message;
	/**
	C
	 * offset 5336
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	Seconds
	 * offset 5368
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	%
	 * offset 5400
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	Seconds
	 * offset 5432
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 5464
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	RPM
	 * offset 5528
	 */
	scaled_channel<uint8_t, 1, 50> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 5536
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5600
	 */
	scaled_channel<uint8_t, 1, 1> boostTpsBins[BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5608
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	%
	 * offset 5672
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	RPM
	 * offset 5680
	 */
	scaled_channel<uint8_t, 1, 50> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	C
	 * offset 5688
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	%
	 * offset 5720
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	RPM
	 * offset 5752
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	deg
	 * offset 5760
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	RPM
	 * offset 5792
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	/**
	load
	 * offset 5796
	 */
	scaled_channel<uint8_t, 1, 1> idleVeLoadBins[IDLE_VE_SIZE];
	/**
	%
	 * offset 5800
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	/**
	 * offset 5832
	 */
	lua_script_t luaScript;
	/**
	C
	 * offset 13832
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	ratio
	 * offset 13896
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	C
	 * offset 13960
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 14024
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 14088
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	C
	 * offset 14120
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	ratio
	 * offset 14152
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	counter
	 * offset 14184
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	C
	 * offset 14216
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	ratio
	 * offset 14280
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	kg/hour
	 * offset 14344
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	V
	 * offset 15368
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	deg
	 * offset 16392
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Temperature
	 * offset 16904
	 */
	float ignitionIatCorrLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 16968
	 */
	uint16_t ignitionIatCorrRpmBins[IGN_RPM_COUNT];
	/**
	deg
	 * offset 17000
	 */
	int16_t injectionPhase[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 17512
	 */
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 17544
	 */
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	onoff
	 * offset 17576
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	kPa
	 * offset 17636
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	% TPS
	 * offset 18148
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18180
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 18212
	 */
	uint8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 18276
	 */
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 18292
	 */
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 18308
	 */
	uint8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 18372
	 */
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 18388
	 */
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	/**
	deg
	 * offset 18404
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 18916
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 18948
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	%
	 * offset 18980
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	kPa
	 * offset 19492
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 19524
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	lambda
	 * offset 19556
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	 * offset 19812
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 19844
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 19876
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	from
	 * offset 20132
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	to
	 * offset 20164
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	value
	 * offset 20196
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20452
	 */
	uint16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20468
	 */
	uint16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20484
	 */
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20548
	 */
	uint16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20564
	 */
	uint16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20580
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20644
	 */
	uint16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20660
	 */
	uint16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20676
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20740
	 */
	uint16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20756
	 */
	uint16_t scriptTable4RpmBins[SCRIPT_TABLE_8];
	/**
	 * offset 20772
	 */
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20780
	 */
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20788
	 */
	cyl_trim_s ignTrims[12];
	/**
	 * offset 20980
	 */
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20988
	 */
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20996
	 */
	cyl_trim_s fuelTrims[12];
	/**
	ratio
	 * offset 21188
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	/**
	Airmass
	 * offset 21204
	 */
	uint8_t tcu_pcAirmassBins[8];
	/**
	%
	 * offset 21212
	 */
	uint8_t tcu_pcValsR[8];
	/**
	%
	 * offset 21220
	 */
	uint8_t tcu_pcValsN[8];
	/**
	%
	 * offset 21228
	 */
	uint8_t tcu_pcVals1[8];
	/**
	%
	 * offset 21236
	 */
	uint8_t tcu_pcVals2[8];
	/**
	%
	 * offset 21244
	 */
	uint8_t tcu_pcVals3[8];
	/**
	%
	 * offset 21252
	 */
	uint8_t tcu_pcVals4[8];
	/**
	%
	 * offset 21260
	 */
	uint8_t tcu_pcVals12[8];
	/**
	%
	 * offset 21268
	 */
	uint8_t tcu_pcVals23[8];
	/**
	%
	 * offset 21276
	 */
	uint8_t tcu_pcVals34[8];
	/**
	%
	 * offset 21284
	 */
	uint8_t tcu_pcVals21[8];
	/**
	%
	 * offset 21292
	 */
	uint8_t tcu_pcVals32[8];
	/**
	%
	 * offset 21300
	 */
	uint8_t tcu_pcVals43[8];
	/**
	TPS
	 * offset 21308
	 */
	uint8_t tcu_tccTpsBins[8];
	/**
	MPH
	 * offset 21316
	 */
	uint8_t tcu_tccLockSpeed[8];
	/**
	MPH
	 * offset 21324
	 */
	uint8_t tcu_tccUnlockSpeed[8];
	/**
	KPH
	 * offset 21332
	 */
	uint8_t tcu_32SpeedBins[8];
	/**
	%
	 * offset 21340
	 */
	uint8_t tcu_32Vals[8];
};
static_assert(sizeof(persistent_config_s) == 21348);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on hellen_cypress_gen_config.bat integration/rusefi_config.txt Mon May 02 08:19:32 UTC 2022
