// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on hellen_cypress_gen_config.bat integration/rusefi_config.txt Sat Dec 25 04:39:41 UTC 2021
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
	uint16_t timeConstant;
	/** total size 4*/
};

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
	uint8_t deadband;
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
	uint8_t minAfr;
	/**
	 * Above this AFR, correction is paused
	afr
	 * offset 6
	 */
	uint8_t maxAfr;
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
	/** total size 24*/
};

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
	/** total size 20*/
};

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
	/** total size 8*/
};

// start of spi_pins
struct spi_pins {
	/**
	 * offset 0
	 */
	brain_pin_e mosiPin;
	/**
	 * offset 1
	 */
	brain_pin_e misoPin;
	/**
	 * offset 2
	 */
	brain_pin_e sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 3
	 */
	uint8_t alignmentFill_at_3[1];
	/** total size 4*/
};

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
	 * offset 1
	 */
	uint8_t dutyIfError;
	/**
	 * Select a frequency to run PWM at.
	 * Set this to 0hz to enable on-off mode.
	hz
	 * offset 2
	 */
	uint16_t pwmFrequency;
	/**
	 * Hysteresis: in on-off mode, turn the output on when the table value is above this duty.
	%
	 * offset 4
	 */
	uint8_t onAboveDuty;
	/**
	 * Hysteresis: in on-off mode, turn the output off when the table value is below this duty.
	%
	 * offset 5
	 */
	uint8_t offBelowDuty;
	/**
	 * Selects the load axis to use for the table.
	 * offset 6
	 */
	gppwm_channel_e loadAxis;
	/**
	unit
	 * offset 7
	 */
	uint8_t alignmentFill_map;
	/**
	load
	 * offset 8
	 */
	uint8_t loadBins[GPPWM_LOAD_COUNT];
	/**
	RPM
	 * offset 16
	 */
	scaled_channel<uint8_t, 1, 50> rpmBins[GPPWM_RPM_COUNT];
	/**
	duty
	 * offset 24
	 */
	uint8_t table[GPPWM_RPM_COUNT][GPPWM_LOAD_COUNT];
	/** total size 88*/
};

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
	unit
	 * offset 13
	 */
	uint8_t alignmentFill[3];
	/** total size 16*/
};

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
	/** total size 144*/
};

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
	/** total size 28*/
};

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
	unit
	 * offset 1
	 */
	uint8_t alignmentFill[3];
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
	/** total size 20*/
};

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
	/** total size 32*/
};

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
	/** total size 68*/
};

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
	/** total size 12*/
};

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
	/** total size 16*/
};

// start of afr_sensor_s
struct afr_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	unit
	 * offset 1
	 */
	uint8_t alignmentFill_afr[3];
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
	/** total size 20*/
};

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
	 * offset 5
	 */
	output_pin_e stepperDirectionPin;
	/**
	 * offset 6
	 */
	output_pin_e stepperStepPin;
	/**
	 * offset 7
	 */
	pin_output_mode_e solenoidPinMode;
	/** total size 8*/
};

// start of dc_io
struct dc_io {
	/**
	 * offset 0
	 */
	brain_pin_e directionPin1;
	/**
	 * offset 1
	 */
	brain_pin_e directionPin2;
	/**
	 * Acts as EN pin in two-wire mode
	 * offset 2
	 */
	brain_pin_e controlPin;
	/**
	 * offset 3
	 */
	brain_pin_e disablePin;
	/** total size 4*/
};

// start of vr_threshold_s
struct vr_threshold_s {
	/**
	 * offset 0
	 */
	brain_pin_e pin;
	/**
	 * offset 1
	 */
	uint8_t pad[3];
	/**
	rpm
	 * offset 4
	 */
	scaled_channel<uint8_t, 1, 50> rpmBins[6];
	/**
	volts
	 * offset 10
	 */
	scaled_channel<uint8_t, 100, 1> values[6];
	/** total size 16*/
};

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
	 * Knock sensor output knock detection threshold depending on current RPM
	v
	 * offset 12
	 */
	float knockNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	RPM
	 * offset 44
	 */
	scaled_channel<uint8_t, 1, 50> knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	deg
	 * offset 52
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	count
	 * offset 53
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	RPM
	 * offset 54
	 */
	scaled_channel<uint8_t, 1, 50> sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 62
	 */
	uint8_t alignmentFill_at_62[2];
	/**
	ms
	 * offset 64
	 */
	float sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * offset 96
	 */
	injector_s injector;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 164 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On Ford vehicles one of the sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 164 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 164 bit 2 */
	bool isVerboseAuxPid1 : 1 {};
	/**
	offset 164 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 164 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 164 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 164 bit 6 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 164 bit 7 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 164 bit 8 */
	bool enableTrailingSparks : 1 {};
	/**
	 * enable cj125verbose/disable cj125verbose
	offset 164 bit 9 */
	bool isCJ125Verbose : 1 {};
	/**
	 * Is your UA CJ125 output wired to MCU via resistor divider? Ua can go over 3.3v but only at lambda >3, i.e very lean AFR above 44.1
	 * When exposed to free air and 17x gain, Ua will be 4.17 volt
	offset 164 bit 10 */
	bool cj125isUaDivided : 1 {};
	/**
	offset 164 bit 11 */
	bool cj125isLsu49 : 1 {};
	/**
	 * TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 164 bit 12 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 164 bit 13 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 164 bit 14 */
	bool showSdCardWarning : 1 {};
	/**
	 * Is your UR CJ125 output wired to MCU via resistor divider?
	 * Looks like 3v range should be enough, divider generally not needed.
	offset 164 bit 15 */
	bool cj125isUrDivided : 1 {};
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 164 bit 16 */
	bool useCicPidForIdle : 1 {};
	/**
	offset 164 bit 17 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 164 bit 18 */
	bool useInstantRpmForIdle : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 164 bit 19 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 164 bit 20 */
	bool launchControlEnabled : 1 {};
	/**
	offset 164 bit 21 */
	bool rollingLaunchEnabled : 1 {};
	/**
	offset 164 bit 22 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 164 bit 23 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	 * 
	 * enable logic_level_trigger
	offset 164 bit 24 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 164 bit 25 */
	bool useTLE8888_stepper : 1 {};
	/**
	 * If enabled, the MAP estimate table will be used if the MAP sensor fails to estimate manifold pressure based on RPM and TPS.
	offset 164 bit 26 */
	bool enableMapEstimationTableFallback : 1 {};
	/**
	offset 164 bit 27 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing CAN messages in rusEFI console
	offset 164 bit 28 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 164 bit 29 */
	bool artificialTestMisfire : 1 {};
	/**
	offset 164 bit 30 */
	bool issue_294_31 : 1 {};
	/**
	offset 164 bit 31 */
	bool unusedBit_48_31 : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * set tps_min X
	ADC
	 * offset 168
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * set tps_max X
	ADC
	 * offset 170
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 172
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 174
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 176
	 */
	cranking_parameters_s cranking;
	/**
	*C
	 * offset 184
	 */
	float primingSquirtDurationMs;
	/**
	 * Dwell duration while cranking
	ms
	 * offset 188
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	rpm
	 * offset 192
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	rpm
	 * offset 194
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 196
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 340
	 */
	ThermistorConf clt;
	/**
	 * offset 372
	 */
	ThermistorConf iat;
	/**
	deg
	 * offset 404
	 */
	int launchTimingRetard;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	integer
	 * offset 408
	 */
	int hip9011PrescalerAndSDO;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	kHz
	 * offset 412
	 */
	float knockBandCustom;
	/**
	 * offset 416
	 */
	specs_s specs;
	/**
	 * Cylinder diameter in mm.
	mm
	 * offset 428
	 */
	float cylinderBore;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS commonly only used for NA engines
	 * Speed Density requires MAP sensor and is the default choice for may installs
	 * MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	 * offset 432
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 436
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups. If your injectors are individually wired you will also need to enable "Two wire batch emulation". 
	 * set injection_mode X
	 * See also twoWireBatchInjection
	 * offset 440
	 */
	injection_mode_e injectionMode;
	/**
	 * this is about deciding when the injector starts it's squirt
	 * See also injectionPhase map
	 * todo: do we need even need this since we have the map anyway?
	deg
	 * offset 444
	 */
	angle_t extraInjectionOffset;
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	deg
	 * offset 448
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 452
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	count
	 * offset 456
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	kph
	 * offset 457
	 */
	uint8_t maxIdleVss;
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	kPa
	 * offset 458
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 460
	 */
	timing_mode_e timingMode;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	RPM
	 * offset 464
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchnization point before TDC and negative in case of synchnization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * set global_trigger_offset_angle X
	deg btdc
	 * offset 468
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	coef
	 * offset 472
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	coef
	 * offset 476
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 480
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 484
	 */
	float fanOffTemperature;
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	revs/km
	 * offset 488
	 */
	float driveWheelRevPerKm;
	/**
	 * set can_mode X
	 * offset 492
	 */
	can_nbc_e canNbcType;
	/**
	 * CANbus thread period in ms
	ms
	 * offset 496
	 */
	int canSleepPeriodMs;
	/**
	 * 'Some triggers could be mounted differently. Most well-known triggers imply specific sensor setup. 4 stroke with symmetrical crank' is a pretty special case for example on Miata NB2
	 * See engineCycle
	 * set operation_mode X
	 * offset 500
	 */
	operation_mode_e ambiguousOperationMode;
	/**
	 * offset 504
	 */
	display_mode_e displayMode;
	/**
	index
	 * offset 508
	 */
	int byFirmwareVersion;
	/**
	index
	 * offset 512
	 */
	int HD44780width;
	/**
	index
	 * offset 516
	 */
	int HD44780height;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
	 * offset 520
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 521
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 522
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * set_analog_input_pin tps2 X
	 * offset 523
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	x
	 * offset 524
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	angle
	 * offset 528
	 */
	int trailingSparkAngle;
	/**
	 * offset 532
	 */
	trigger_config_s trigger;
	/**
	 * offset 548
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * Single value to be used in event of a failed MAP sensor 
	 * This value is only used for speed density fueling calculations.
	kPa
	 * offset 549
	 */
	uint8_t failedMapFallback;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost.
	%
	 * offset 550
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 551
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * set global_fuel_correction X
	coef
	 * offset 552
	 */
	float globalFuelCorrection;
	/**
	volts
	 * offset 556
	 */
	float adcVcc;
	/**
	Deg
	 * offset 560
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 564
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 568
	 */
	uint8_t camInputsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 568
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * set_analog_input_pin pps X
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 588
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * offset 589
	 */
	brain_pin_e tle6240_cs;
	/**
	 * offset 590
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 591
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * @see hasBaroSensor
	 * offset 592
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 608
	 */
	idle_hardware_s idle;
	/**
	 * Value between 0 and 100 used in Manual mode
	%
	 * offset 616
	 */
	float manIdlePosition;
	/**
	 * Ignition timing to remove when a knock event occurs.
	%
	 * offset 620
	 */
	uint8_t knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	deg/s
	 * offset 621
	 */
	uint8_t knockRetardReapplyRate;
	/**
	 * Maximum amount of knock retard.
	deg
	 * offset 622
	 */
	uint8_t knockRetardMaximum;
	/**
	 * offset 623
	 */
	uint8_t mapCamDetectionThreshold;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	ratio
	 * offset 624
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	count
	 * offset 626
	 */
	uint8_t vssToothCount;
	/**
	 * offset 627
	 */
	uint8_t mapCamAveragingLength;
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * TODO: rename to triggerSimulatorRpm
	Rpm
	 * offset 628
	 */
	int triggerSimulatorFrequency;
	/**
	 * offset 632
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 644
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 656
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 657
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 658
	 */
	brain_pin_e HD44780_rs;
	/**
	 * offset 659
	 */
	brain_pin_e HD44780_e;
	/**
	 * offset 660
	 */
	brain_pin_e HD44780_db4;
	/**
	 * offset 661
	 */
	brain_pin_e HD44780_db5;
	/**
	 * offset 662
	 */
	brain_pin_e HD44780_db6;
	/**
	 * offset 663
	 */
	brain_pin_e HD44780_db7;
	/**
	 * offset 664
	 */
	brain_pin_e gps_rx_pin;
	/**
	 * offset 665
	 */
	brain_pin_e gps_tx_pin;
	/**
	 * offset 666
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 667
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 668
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 669
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 670
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 671
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 672
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 673
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 674
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 675
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 676
	 */
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 680
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * set_cj125_heater_pin XXX
	 * offset 681
	 */
	brain_pin_e wboHeaterPin;
	/**
	 * set_cj125_cs_pin XXX
	 * offset 682
	 */
	brain_pin_e cj125CsPin;
	/**
	 * offset 683
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 684
	 */
	brain_pin_e debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 685
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 686
	 */
	brain_pin_e mc33972_cs;
	/**
	 * offset 687
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 688
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 689
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 690
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 691
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	%
	 * offset 692
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 700
	 */
	ego_sensor_e afr_type;
	/**
	 * 0.1 is a good default value
	x
	 * offset 704
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 708
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 711
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * offset 712
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 713
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * offset 714
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 715
	 */
	brain_pin_e sdCardCsPin;
	/**
	 * set_can_tx_pin X
	 * offset 716
	 */
	brain_pin_e canTxPin;
	/**
	 * set_can_rx_pin X
	 * offset 717
	 */
	brain_pin_e canRxPin;
	/**
	 * offset 718
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * Additional idle % while A/C is active
	%
	 * offset 719
	 */
	uint8_t acIdleExtraOffset;
	/**
	ms
	 * offset 720
	 */
	int unused720;
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	mv
	 * offset 724
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 726
	 */
	uint16_t wastegatePositionMax;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	mv
	 * offset 728
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 730
	 */
	uint16_t idlePositionMax;
	/**
	 * Secondary TTL channel baud rate
	BPs
	 * offset 732
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	CR
	 * offset 736
	 */
	float compressionRatio;
	/**
	 * Each rusEfi piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEfi board.
	 * See also directSelfStimulation which is different.
	 * offset 740
	 */
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 743
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * Narrow band o2 heater, not used for CJ125. 'ON' if engine is running, 'OFF' if stopped or cranking. See wboHeaterPin
	 * offset 746
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 747
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	offset 748 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 748 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 748 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 748 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 748 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 748 bit 5 */
	bool isEngineControlEnabled : 1 {};
	/**
	offset 748 bit 6 */
	bool isHip9011Enabled : 1 {};
	/**
	offset 748 bit 7 */
	bool isVerboseAlternator : 1 {};
	/**
	offset 748 bit 8 */
	bool useSerialPort : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 748 bit 9 */
	bool useStepperIdle : 1 {};
	/**
	offset 748 bit 10 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 748 bit 11 */
	bool useTpicAdvancedMode : 1 {};
	/**
	offset 748 bit 12 */
	bool useLcdScreen : 1 {};
	/**
	offset 748 bit 13 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 748 bit 14 */
	bool enableVerboseCanTx : 1 {};
	/**
	 * This will cause the alternator to be operated in a basic on or off mode, this is the simplest alternator control.
	offset 748 bit 15 */
	bool onOffAlternatorLogic : 1 {};
	/**
	 * enable cj125/disable cj125
	offset 748 bit 16 */
	bool isCJ125Enabled : 1 {};
	/**
	 * Use rise or fall signal front
	 * get vvtCamSensorUseRise
	offset 748 bit 17 */
	bool vvtCamSensorUseRise : 1 {};
	/**
	 * Useful for individual intakes
	offset 748 bit 18 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 748 bit 19 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 748 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 748 bit 21 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * This setting allows the ECU to open the IAC during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars. Used in Auto-PID Idle mode.
	offset 748 bit 22 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 748 bit 23 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 748 bit 24 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 748 bit 25 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 748 bit 26 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 748 bit 27 */
	bool pauseEtbControl : 1 {};
	/**
	offset 748 bit 28 */
	bool alignEngineSnifferAtTDC : 1 {};
	/**
	 * This setting allows the ETB to act as the idle air control valve and move to regulate the airflow at idle.
	offset 748 bit 29 */
	bool useETBforIdleControl : 1 {};
	/**
	offset 748 bit 30 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 748 bit 31 */
	bool enableAemXSeries : 1 {};
	/**
	 * offset 752
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 756
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * offset 757
	 */
	brain_pin_e hip9011CsPin;
	/**
	 * offset 758
	 */
	brain_pin_e hip9011IntHoldPin;
	/**
	 * offset 759
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
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
	 * offset 767
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * offset 768
	 */
	uint8_t mapCamSkipFactor;
	/**
	volts
	 * offset 769
	 */
	uint8_t dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	 * offset 777
	 */
	imu_type_e imuType;
	/**
	multiplier
	 * offset 778
	 */
	uint8_t dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	kg
	 * offset 786
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling?
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	RPM
	 * offset 788
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	ms
	 * offset 790
	 */
	uint16_t applyNonlinearBelowPulse;
	/**
	 * offset 792
	 */
	brain_pin_e lps25BaroSensorScl;
	/**
	 * offset 793
	 */
	brain_pin_e lps25BaroSensorSda;
	/**
	 * offset 794
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 795
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 796
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 797
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 798
	 */
	brain_pin_e max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 806
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * offset 807
	 */
	brain_pin_e test557pin;
	/**
	 * offset 808
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 809
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	:1
	 * offset 810
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	%
	 * offset 811
	 */
	uint8_t etbMaximumPosition;
	/**
	 * SD card logging period, in milliseconds
	ms
	 * offset 812
	 */
	int16_t sdCardPeriodMs;
	/**
	 * offset 814
	 */
	adc_channel_e idlePositionSensor;
	/**
	 * offset 815
	 */
	brain_pin_e debugMapAveraging;
	/**
	 * offset 816
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 817
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 818
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 819
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	count
	 * offset 820
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	%
	 * offset 824
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	%
	 * offset 826
	 */
	int16_t stepperParkingExtraSteps;
	/**
	ADC
	 * offset 828
	 */
	uint16_t tps1SecondaryMin;
	/**
	ADC
	 * offset 830
	 */
	uint16_t tps1SecondaryMax;
	/**
	rpm
	 * offset 832
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	Seconds
	 * offset 834
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 836
	 */
	brain_pin_e triggerErrorPin;
	/**
	 * offset 837
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * offset 838
	 */
	output_pin_e acRelayPin;
	/**
	 * offset 839
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * offset 840
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 872
	 */
	brain_pin_e spi1mosiPin;
	/**
	 * offset 873
	 */
	brain_pin_e spi1misoPin;
	/**
	 * offset 874
	 */
	brain_pin_e spi1sckPin;
	/**
	 * offset 875
	 */
	brain_pin_e spi2mosiPin;
	/**
	 * offset 876
	 */
	brain_pin_e spi2misoPin;
	/**
	 * offset 877
	 */
	brain_pin_e spi2sckPin;
	/**
	 * offset 878
	 */
	brain_pin_e spi3mosiPin;
	/**
	 * offset 879
	 */
	brain_pin_e spi3misoPin;
	/**
	 * offset 880
	 */
	brain_pin_e spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 881
	 */
	brain_pin_e cdmInputPin;
	/**
	 * offset 882
	 */
	brain_pin_e joystickCenterPin;
	/**
	 * offset 883
	 */
	brain_pin_e joystickAPin;
	/**
	 * offset 884
	 */
	brain_pin_e joystickBPin;
	/**
	 * offset 885
	 */
	brain_pin_e joystickCPin;
	/**
	 * offset 886
	 */
	brain_pin_e joystickDPin;
	/**
	 * offset 887
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEfi console Sensor Sniffer mode
	 * offset 888
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 892
	 */
	maf_sensor_type_e mafSensorType;
	/**
	offset 896 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 896 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 896 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 896 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 896 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 896 bit 5 */
	bool unfinishedenableLaunchBoost : 1 {};
	/**
	offset 896 bit 6 */
	bool unfinishedlaunchDisableBySpeed : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 896 bit 7 */
	bool enableCanVss : 1 {};
	/**
	offset 896 bit 8 */
	bool enableInnovateLC2 : 1 {};
	/**
	offset 896 bit 9 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise. Use of this mode is required if you have a narrowband O2 sensor.
	offset 896 bit 10 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 896 bit 11 */
	bool mapCamLookForLowPeaks : 1 {};
	/**
	offset 896 bit 12 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * enable vvt_details
	offset 896 bit 13 */
	bool verboseVVTDecoding : 1 {};
	/**
	 * get invertCamVVTSignal
	offset 896 bit 14 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	 * enable consumeObdSensors
	offset 896 bit 15 */
	bool consumeObdSensors : 1 {};
	/**
	offset 896 bit 16 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 896 bit 17 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 896 bit 18 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 896 bit 19 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 896 bit 20 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 896 bit 21 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 896 bit 22 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 896 bit 23 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 896 bit 24 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 896 bit 25 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 896 bit 26 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 896 bit 27 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 896 bit 28 */
	bool tcuEnabled : 1 {};
	/**
	offset 896 bit 29 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	offset 896 bit 30 */
	bool unusedBit_310_30 : 1 {};
	/**
	offset 896 bit 31 */
	bool unusedBit_310_31 : 1 {};
	/**
	 * offset 900
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 908
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 909
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * offset 910
	 */
	switch_input_pin_e antiLagActivatePin;
	/**
	 * offset 911
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 912
	 */
	pid_s boostPid;
	/**
	 * offset 932
	 */
	boostType_e boostType;
	/**
	Hz
	 * offset 936
	 */
	int boostPwmFrequency;
	/**
	 * offset 940
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 944
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * Disabled above this speed
	Kph
	 * offset 948
	 */
	int launchSpeedThreshold;
	/**
	 * Range from Launch Rpm for Timing Retard to activate
	RPM
	 * offset 952
	 */
	int launchTimingRpmRange;
	/**
	 * Extra Fuel Added
	%
	 * offset 956
	 */
	int launchFuelAdded;
	/**
	 * Duty Cycle for the Boost Solenoid
	%
	 * offset 960
	 */
	int launchBoostDuty;
	/**
	 * RPM Range for Hard Cut
	rpm
	 * offset 964
	 */
	int hardCutRpmRange;
	/**
	rpm
	 * offset 968
	 */
	int launchAdvanceRpmRange;
	/**
	rpm
	 * offset 972
	 */
	int launchTpsTreshold;
	/**
	rpm
	 * offset 976
	 */
	float launchActivateDelay;
	/**
	 * offset 980
	 */
	stft_s stft;
	/**
	 * offset 1004
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1012
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1044
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1076
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1108
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * offset 1114
	 */
	etb_function_e etbFunctions[ETB_COUNT];
	/**
	 * offset 1116
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * offset 1117
	 */
	brain_pin_e drv8860_cs;
	/**
	 * offset 1118
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * offset 1119
	 */
	brain_pin_e drv8860_miso;
	/**
	volt
	 * offset 1120
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * offset 1136
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * Angle between cam sensor and VVT zero position
	 * set vvt_offset X
	value
	 * offset 1144
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1160
	 */
	float vvtOffsetsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 1160
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1192
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	ADC
	 * offset 1256
	 */
	uint16_t tps2SecondaryMin;
	/**
	ADC
	 * offset 1258
	 */
	uint16_t tps2SecondaryMax;
	/**
	offset 1260 bit 0 */
	bool unused1464b0 : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1260 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Print details into rusEfi console
	 * enable verbose_idle
	offset 1260 bit 2 */
	bool isVerboseIAC : 1 {};
	/**
	offset 1260 bit 3 */
	bool unused1464b3 : 1 {};
	/**
	offset 1260 bit 4 */
	bool unused1464b4 : 1 {};
	/**
	 * This options enables data for 'engine sniffer' tab in console, which comes at some CPU price
	offset 1260 bit 5 */
	bool isEngineChartEnabled : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1260 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1260 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1260 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write
	offset 1260 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1260 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1260 bit 11 */
	bool unusedBitWasHere16 : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1260 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1260 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1260 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1260 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1260 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	offset 1260 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1260 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1260 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1260 bit 20 */
	bool unused1464b20 : 1 {};
	/**
	offset 1260 bit 21 */
	bool unusedBitWasHere1 : 1 {};
	/**
	offset 1260 bit 22 */
	bool unusedBitWasHere2 : 1 {};
	/**
	offset 1260 bit 23 */
	bool unusedBitWasHere3 : 1 {};
	/**
	offset 1260 bit 24 */
	bool unusedBitWasHere4 : 1 {};
	/**
	offset 1260 bit 25 */
	bool unusedBitWasHere5 : 1 {};
	/**
	offset 1260 bit 26 */
	bool unusedBitHere1 : 1 {};
	/**
	 * Sometimes we just have to shut the engine down. Use carefully!
	offset 1260 bit 27 */
	bool useFSIO5ForCriticalIssueEngineStop : 1 {};
	/**
	 * Sometimes we have to miss injection on purpose to attract driver's attention
	offset 1260 bit 28 */
	bool useFSIO4ForSeriousEngineWarning : 1 {};
	/**
	offset 1260 bit 29 */
	bool unusedBitHere2 : 1 {};
	/**
	offset 1260 bit 30 */
	bool unusedBitHere3 : 1 {};
	/**
	offset 1260 bit 31 */
	bool unusedBitHere4 : 1 {};
	/**
	 * offset 1264
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * A/C button input
	 * offset 1265
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1266
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	%
	 * offset 1267
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1268
	 */
	idle_mode_e idleMode;
	/**
	 * Enable fuel injection - This is default off for new projects as a safety feature, set to "true" to enable fuel injection and further injector settings.
	offset 1272 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	 * Enable ignition - This is default off for new projects as a safety feature, set to "true" to enable ignition and further ignition settings.
	offset 1272 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1272 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	offset 1272 bit 3 */
	bool unused1476b3 : 1 {};
	/**
	offset 1272 bit 4 */
	bool unusedBit4_1476 : 1 {};
	/**
	offset 1272 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1272 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1272 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1272 bit 8 */
	bool unused1476b8 : 1 {};
	/**
	offset 1272 bit 9 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1272 bit 10 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * enable trigger_details
	offset 1272 bit 11 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1272 bit 12 */
	bool isManualSpinningMode : 1 {};
	/**
	 * This is needed if your coils are individually wired and you wish to use batch injection.
	 * enable two_wire_batch_injection
	offset 1272 bit 13 */
	bool twoWireBatchInjection : 1 {};
	/**
	 * VR sensors are only precise on rising front
	 * enable trigger_only_front
	offset 1272 bit 14 */
	bool useOnlyRisingEdgeForTrigger : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1272 bit 15 */
	bool twoWireBatchIgnition : 1 {};
	/**
	offset 1272 bit 16 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1272 bit 17 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1272 bit 18 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	offset 1272 bit 19 */
	bool unused1476b19 : 1 {};
	/**
	offset 1272 bit 20 */
	bool unused1476b20 : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1272 bit 21 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1272 bit 22 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1272 bit 23 */
	bool launchSmoothRetard : 1 {};
	/**
	offset 1272 bit 24 */
	bool unused_1484_bit_24 : 1 {};
	/**
	offset 1272 bit 25 */
	bool unused_1484_bit_25 : 1 {};
	/**
	offset 1272 bit 26 */
	bool unused_1484_bit_26 : 1 {};
	/**
	offset 1272 bit 27 */
	bool unused_1484_bit_27 : 1 {};
	/**
	offset 1272 bit 28 */
	bool unused_1484_bit_28 : 1 {};
	/**
	offset 1272 bit 29 */
	bool unused_1484_bit_29 : 1 {};
	/**
	offset 1272 bit 30 */
	bool unused_1484_bit_30 : 1 {};
	/**
	offset 1272 bit 31 */
	bool unused_1484_bit_31 : 1 {};
	/**
	count
	 * offset 1276
	 */
	uint32_t engineChartSize;
	/**
	mult
	 * offset 1280
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1284
	 */
	brain_pin_e camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * offset 1288
	 */
	uint8_t camInputsDebugPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	RPM
	 * offset 1288
	 */
	int16_t acIdleRpmBump;
	/**
	 * set warningPeriod X
	seconds
	 * offset 1290
	 */
	int16_t warningPeriod;
	/**
	angle
	 * offset 1292
	 */
	float knockDetectionWindowStart;
	/**
	angle
	 * offset 1296
	 */
	float knockDetectionWindowEnd;
	/**
	ms
	 * offset 1300
	 */
	float idleStepperReactionTime;
	/**
	count
	 * offset 1304
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	sec
	 * offset 1308
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	index
	 * offset 1312
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	kPa
	 * offset 1316
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	RPM
	 * offset 1332
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	ratio
	 * offset 1348
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	Ratio
	 * offset 1412
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	%
	 * offset 1444
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1476
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	Seconds
	 * offset 1480
	 */
	float wwaeTau;
	/**
	 * offset 1484
	 */
	pid_s alternatorControl;
	/**
	 * offset 1504
	 */
	pid_s etb;
	/**
	 * offset 1524
	 */
	brain_pin_e triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 1527
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * set tps2_min X
	ADC
	 * offset 1528
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps2_max X
	ADC
	 * offset 1530
	 */
	int16_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1532
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1533
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * offset 1534
	 */
	brain_pin_e mc33816_flag0;
	/**
	Pulse
	 * offset 1535
	 */
	uint8_t tachPulsePerRev;
	/**
	 * kPa value which is too low to be true
	kPa
	 * offset 1536
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	kPa
	 * offset 1540
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	ms
	 * offset 1544
	 */
	uint16_t multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	ms
	 * offset 1546
	 */
	uint16_t multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1548
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	Fraction
	 * offset 1568
	 */
	float wwaeBeta;
	/**
	 * blue LED on many rusEFI boards.
	 * Blue Communication LED which is expected to blink at 50% duty cycle during normal board operation.
	 * If USB communication cable is connected Blue LED starts to blink faster.
	 * offset 1572
	 */
	brain_pin_e communicationLedPin;
	/**
	 * Green LED on many rusEFI boards.
	 * Off if engine is stopped, blinks if engine is cranking, solid if engine is running.
	 * offset 1573
	 */
	brain_pin_e runningLedPin;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1574
	 */
	brain_pin_e binarySerialTxPin;
	/**
	 * offset 1575
	 */
	brain_pin_e binarySerialRxPin;
	/**
	 * offset 1576
	 */
	brain_pin_e auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * offset 1578
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1579
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	voltage
	 * offset 1580
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1584
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	seconds
	 * offset 1588
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	RPM
	 * offset 1590
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * CLT-based target RPM for automatic idle controller
	C
	 * offset 1592
	 */
	float cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	RPM
	 * offset 1656
	 */
	scaled_channel<uint8_t, 1, 50> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	Volts
	 * offset 1672
	 */
	float targetVBatt;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	%
	 * offset 1676
	 */
	float alternatorOffAboveTps;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	cycles
	 * offset 1680
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	percent
	 * offset 1682
	 */
	int16_t iacByTpsTaper;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_tx_pin X
	 * offset 1684
	 */
	brain_pin_e auxSerialTxPin;
	/**
	 * offset 1685
	 */
	brain_pin_e warningLedPin;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_rx_pin X
	 * offset 1686
	 */
	brain_pin_e auxSerialRxPin;
	/**
	 * offset 1687
	 */
	brain_pin_e LIS302DLCsPin;
	/**
	 * This is the number of engine cycles that the TPS position change can occur over, a longer duration will make the enrichment more active but too long may affect steady state driving, a good default is 30-60 cycles.
	cycles
	 * offset 1688
	 */
	int tpsAccelLength;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	roc
	 * offset 1692
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	cycles
	 * offset 1696
	 */
	int engineLoadAccelLength;
	/**
	 * Band rate for primary TTL
	BPs
	 * offset 1700
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	roc
	 * offset 1704
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	coeff
	 * offset 1708
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	coeff
	 * offset 1712
	 */
	float slowAdcAlpha;
	/**
	 * See http://rusefi.com/s/debugmode
	 * 
	 * set debug_mode X
	 * offset 1716
	 */
	debug_mode_e debugMode;
	/**
	BPs
	 * offset 1720
	 */
	uint32_t auxSerialSpeed;
	/**
	voltage
	 * offset 1724
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1728
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * set can_baudrate
	 * offset 1732
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1733
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1734
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1735
	 */
	load_override_e afrOverrideMode;
	/**
	 * offset 1736
	 */
	uint32_t unused1736;
	/**
	offset 1740 bit 0 */
	bool unused1740b0 : 1 {};
	/**
	offset 1740 bit 1 */
	bool unused1740b1 : 1 {};
	/**
	offset 1740 bit 2 */
	bool unused1740b2 : 1 {};
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1740 bit 3 */
	bool stepperDcInvertedPins : 1 {};
	/**
	offset 1740 bit 4 */
	bool unused1127 : 1 {};
	/**
	offset 1740 bit 5 */
	bool unused1128 : 1 {};
	/**
	offset 1740 bit 6 */
	bool unused1129 : 1 {};
	/**
	offset 1740 bit 7 */
	bool unused1130 : 1 {};
	/**
	offset 1740 bit 8 */
	bool unusedBit_498_8 : 1 {};
	/**
	offset 1740 bit 9 */
	bool unusedBit_498_9 : 1 {};
	/**
	offset 1740 bit 10 */
	bool unusedBit_498_10 : 1 {};
	/**
	offset 1740 bit 11 */
	bool unusedBit_498_11 : 1 {};
	/**
	offset 1740 bit 12 */
	bool unusedBit_498_12 : 1 {};
	/**
	offset 1740 bit 13 */
	bool unusedBit_498_13 : 1 {};
	/**
	offset 1740 bit 14 */
	bool unusedBit_498_14 : 1 {};
	/**
	offset 1740 bit 15 */
	bool unusedBit_498_15 : 1 {};
	/**
	offset 1740 bit 16 */
	bool unusedBit_498_16 : 1 {};
	/**
	offset 1740 bit 17 */
	bool unusedBit_498_17 : 1 {};
	/**
	offset 1740 bit 18 */
	bool unusedBit_498_18 : 1 {};
	/**
	offset 1740 bit 19 */
	bool unusedBit_498_19 : 1 {};
	/**
	offset 1740 bit 20 */
	bool unusedBit_498_20 : 1 {};
	/**
	offset 1740 bit 21 */
	bool unusedBit_498_21 : 1 {};
	/**
	offset 1740 bit 22 */
	bool unusedBit_498_22 : 1 {};
	/**
	offset 1740 bit 23 */
	bool unusedBit_498_23 : 1 {};
	/**
	offset 1740 bit 24 */
	bool unusedBit_498_24 : 1 {};
	/**
	offset 1740 bit 25 */
	bool unusedBit_498_25 : 1 {};
	/**
	offset 1740 bit 26 */
	bool unusedBit_498_26 : 1 {};
	/**
	offset 1740 bit 27 */
	bool unusedBit_498_27 : 1 {};
	/**
	offset 1740 bit 28 */
	bool unusedBit_498_28 : 1 {};
	/**
	offset 1740 bit 29 */
	bool unusedBit_498_29 : 1 {};
	/**
	offset 1740 bit 30 */
	bool unusedBit_498_30 : 1 {};
	/**
	offset 1740 bit 31 */
	bool unusedBit_498_31 : 1 {};
	/**
	 * offset 1744
	 */
	uint32_t unused1744;
	/**
	 * set_can2_tx_pin X
	 * offset 1748
	 */
	brain_pin_e can2TxPin;
	/**
	 * set_can2_rx_pin X
	 * offset 1749
	 */
	brain_pin_e can2RxPin;
	/**
	 * offset 1750
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1751
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1752
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1753
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1754
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1755
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	kPa (absolute)
	 * offset 1756
	 */
	float boostCutPressure;
	/**
	counter
	 * offset 1760
	 */
	float mapAccelTaperBins[MAP_ACCEL_TAPER];
	/**
	mult
	 * offset 1792
	 */
	float mapAccelTaperMult[MAP_ACCEL_TAPER];
	/**
	 * Fixed timing, useful for TDC testing
	deg
	 * offset 1824
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	v
	 * offset 1828
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	v
	 * offset 1832
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	value
	 * offset 1836
	 */
	float egoValueShift;
	/**
	 * offset 1840
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * offset 1844
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * offset 1845
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * offset 1846
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1847
	 */
	uint8_t alignmentFill_at_1847[1];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	percent
	 * offset 1848
	 */
	int crankingIACposition;
	/**
	 * offset 1852
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 1856
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 1860
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 1864
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 1868
	 */
	pwm_freq_t vvtOutputFrequency[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #1 is active
	%
	 * offset 1872
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * need 4 byte alignment
	units
	 * offset 1873
	 */
	uint8_t alignmentFill_at_1873[3];
	/**
	Hz
	 * offset 1876
	 */
	int alternatorPwmFrequency;
	/**
	 * Narrow Band WBO Approximation
	V
	 * offset 1880
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	ratio
	 * offset 1912
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * set vvt_mode X
	 * offset 1944
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * offset 1946
	 */
	uint8_t vvtModePadding[CAMS_PER_BANK_padding];
	/**
	 * Additional idle % when fan #2 is active
	%
	 * offset 1946
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	sec
	 * offset 1947
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1948
	 */
	adc_channel_e auxAnalogInputs[AUX_ANALOG_INPUT_COUNT];
	/**
	 * offset 1956
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * CLT-based timing correction
	C
	 * offset 1968
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	degree
	 * offset 2000
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2032
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 2033
	 */
	pin_output_mode_e LIS302DLCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 2034
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 2035
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	kPa
	 * offset 2036
	 */
	float fuelReferencePressure;
	/**
	 * Fuel multiplier (enrichment) immediately after engine start
	mult
	 * offset 2040
	 */
	float postCrankingFactor;
	/**
	 * Time over which to taper out after start enrichment
	seconds
	 * offset 2044
	 */
	float postCrankingDurationSec;
	/**
	 * offset 2048
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 2080
	 */
	ThermistorConf auxTempSensor2;
	/**
	Deg
	 * offset 2112
	 */
	int16_t knockSamplingDuration;
	/**
	Hz
	 * offset 2114
	 */
	int16_t etbFreq;
	/**
	 * offset 2116
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 2136
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	%
	 * offset 2137
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	%
	 * offset 2138
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 2139
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder timing correction
	deg
	 * offset 2140
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	seconds
	 * offset 2188
	 */
	float idlePidActivationTime;
	/**
	 * offset 2192
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 2193
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 2194
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 2195
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 2196
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 2197
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 2198
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 2199
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 2200
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 2201
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 2202
	 */
	brain_pin_e mc33816_rstb;
	/**
	 * offset 2203
	 */
	brain_pin_e mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 2204
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * lambda input
	 * offset 2205
	 */
	adc_channel_e cj125ua;
	/**
	 * heater input
	 * offset 2206
	 */
	adc_channel_e cj125ur;
	/**
	 * offset 2207
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 2208
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 2248
	 */
	float injectorCorrectionPolynomial[8];
	/**
	C
	 * offset 2280
	 */
	int8_t primeBins[8];
	/**
	 * offset 2288
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2308
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * offset 2309
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 2310
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 2311
	 */
	uint8_t fan2OffTemperature;
	/**
	x
	 * offset 2312
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 2376
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	x
	 * offset 2440
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 2504
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	x
	 * offset 2568
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 2600
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	x
	 * offset 2632
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 2664
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	 * offset 2696
	 */
	output_pin_e stepperEnablePin;
	/**
	 * offset 2697
	 */
	brain_pin_e tle8888_cs;
	/**
	 * offset 2698
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * offset 2699
	 */
	brain_pin_e mc33816_cs;
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	RPM
	 * offset 2700
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	deg
	 * offset 2716
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * todo: more comments
	 * offset 2732
	 */
	brain_pin_e servoOutputPins[SERVO_COUNT];
	/**
	 * This sets the RPM above which fuel cut is active.
	rpm
	 * offset 2740
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	rpm
	 * offset 2742
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active.
	%
	 * offset 2744
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	C
	 * offset 2746
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	%
	 * offset 2748
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	kPa
	 * offset 2750
	 */
	int16_t coastingFuelCutMap;
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	C
	 * offset 2752
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	%
	 * offset 2816
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 2880
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2900
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * CLT-based target RPM for hard limit depending on CLT like on Lexus LFA
	C
	 * offset 2920
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 2924
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * offset 2932
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	x
	 * offset 3028
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3060
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	x
	 * offset 3092
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3124
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	 * offset 3156
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 3220
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 3348
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 3352
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	kg/h
	 * offset 3356
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	deg/sec
	 * offset 3360
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	deg/sec
	 * offset 3364
	 */
	float tChargeAirDecrLimit;
	/**
	 * offset 3368
	 */
	tChargeMode_e tChargeMode;
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	target TPS position
	 * offset 3372
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	ETB duty cycle bias
	 * offset 3404
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	 * offset 3436
	 */
	float hip9011Gain;
	/**
	 * iTerm min value
	 * offset 3440
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 3442
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 3444
	 */
	pid_s idleTimingPid;
	/**
	 * If the RPM closer to target than this value, disable timing correction to prevent oscillation
	RPM
	 * offset 3464
	 */
	int16_t idleTimingPidDeadZone;
	/**
	 * A delay in cycles between fuel-enrich. portions
	cycles
	 * offset 3466
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	coef
	 * offset 3468
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 3472
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 3473
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 3474
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 3476
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	:1
	 * offset 3477
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 3478
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	%
	 * offset 3480
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 3484
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	mg
	 * offset 3496
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[8];
	/**
	 * Trigger comparator center point voltage
	V
	 * offset 3504
	 */
	uint8_t triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	V
	 * offset 3505
	 */
	uint8_t triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	V
	 * offset 3506
	 */
	uint8_t triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	RPM
	 * offset 3507
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * offset 3508
	 */
	pid_s idleRpmPid2;
	/**
	%
	 * offset 3528
	 */
	uint8_t iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	Load
	 * offset 3592
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	RPM
	 * offset 3600
	 */
	scaled_channel<uint8_t, 1, 50> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * set can_vss X
	 * offset 3608
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * offset 3612
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	mA
	 * offset 3964
	 */
	uint16_t mc33_i_boost;
	/**
	mA
	 * offset 3966
	 */
	uint16_t mc33_i_peak;
	/**
	mA
	 * offset 3968
	 */
	uint16_t mc33_i_hold;
	/**
	us
	 * offset 3970
	 */
	uint16_t mc33_t_max_boost;
	/**
	us
	 * offset 3972
	 */
	uint16_t mc33_t_peak_off;
	/**
	us
	 * offset 3974
	 */
	uint16_t mc33_t_peak_tot;
	/**
	us
	 * offset 3976
	 */
	uint16_t mc33_t_bypass;
	/**
	us
	 * offset 3978
	 */
	uint16_t mc33_t_hold_off;
	/**
	us
	 * offset 3980
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 3982
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 3983
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 3984
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 3985
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	dB
	 * offset 3991
	 */
	int8_t knockBaseNoise[IGN_RPM_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 4007
	 */
	uint8_t alignmentFill_at_4007[1];
	/**
	from
	 * offset 4008
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	Percent
	 * offset 4080
	 */
	int8_t fuelTrim[MAX_CYLINDER_COUNT];
	/**
	to
	 * offset 4092
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	lobes/cam
	 * offset 4164
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 4165
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Crank angle ATDC of first lobe peak
	deg
	 * offset 4166
	 */
	uint8_t hpfpPeakPos;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	deg
	 * offset 4167
	 */
	uint8_t hpfpMinAngle;
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	cc
	 * offset 4168
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	deg
	 * offset 4170
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 4171
	 */
	uint8_t unusedFiller_4703;
	/**
	%/kPa
	 * offset 4172
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	%/kPa/lobe
	 * offset 4174
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	kPa/s
	 * offset 4176
	 */
	uint16_t hpfpTargetDecay;
	/**
	%
	 * offset 4178
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	deg
	 * offset 4194
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	volts
	 * offset 4210
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	ms
	 * offset 4218
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	kPa
	 * offset 4234
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	load
	 * offset 4434
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	RPM
	 * offset 4454
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	%
	 * offset 4464
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	cc/lobe
	 * offset 4564
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	RPM
	 * offset 4584
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	Nm
	 * offset 4594
	 */
	scaled_channel<uint8_t, 1, 10> torqueValues[TORQUE_CURVE_SIZE];
	/**
	RPM
	 * offset 4602
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 4618
	 */
	uint8_t alignmentFill_at_4618[2];
	/**
	units
	 * offset 4620
	 */
	int mainUnusedEnd[100];
	/** total size 5020*/
};

// start of cyl_trim_s
struct cyl_trim_s {
	/**
	 * offset 0
	 */
	scaled_channel<int8_t, 5, 1> table[TRIM_SIZE][TRIM_SIZE];
	/** total size 16*/
};

// start of persistent_config_s
struct persistent_config_s {
	/**
	 * offset 0
	 */
	engine_configuration_s engineConfiguration;
	/**
	 * offset 5020
	 */
	error_message_t warning_message;
	/**
	C
	 * offset 5140
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	Seconds
	 * offset 5172
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	%
	 * offset 5204
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	Seconds
	 * offset 5236
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 5268
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	RPM
	 * offset 5332
	 */
	scaled_channel<uint8_t, 1, 50> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 5340
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5404
	 */
	scaled_channel<uint8_t, 1, 1> boostTpsBins[BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5412
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	%
	 * offset 5476
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	RPM
	 * offset 5484
	 */
	scaled_channel<uint8_t, 1, 50> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	C
	 * offset 5492
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	%
	 * offset 5524
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	RPM
	 * offset 5556
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	deg
	 * offset 5564
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional VE table for Idle (see useSeparateVEForIdle)
	RPM
	 * offset 5596
	 */
	scaled_channel<uint8_t, 1, 50> idleVeBins[IDLE_VE_CURVE_SIZE];
	/**
	 * Optional VE table for Idle (see useSeparateVEForIdle)
	%
	 * offset 5604
	 */
	float idleVe[IDLE_VE_CURVE_SIZE];
	/**
	 * offset 5636
	 */
	lua_script_t luaScript;
	/**
	C
	 * offset 13636
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	ratio
	 * offset 13700
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	C
	 * offset 13764
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 13828
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 13892
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	C
	 * offset 13924
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	ratio
	 * offset 13956
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	counter
	 * offset 13988
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	C
	 * offset 14020
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	ratio
	 * offset 14084
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	kg/hour
	 * offset 14148
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	V
	 * offset 15172
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	deg
	 * offset 16196
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Temperature
	 * offset 16708
	 */
	float ignitionIatCorrLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 16772
	 */
	uint16_t ignitionIatCorrRpmBins[IGN_RPM_COUNT];
	/**
	deg
	 * offset 16804
	 */
	int16_t injectionPhase[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 17316
	 */
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 17348
	 */
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	onoff
	 * offset 17380
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	 * Good example: number of tooth on wheel, For Can 10 is a good number.
	Hz
	 * offset 17440
	 */
	float vssFilterReciprocal;
	/**
	kPa
	 * offset 17444
	 */
	uint16_t mapEstimateTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	% TPS
	 * offset 17956
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 17988
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 18020
	 */
	uint8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 18084
	 */
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 18100
	 */
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 18116
	 */
	uint8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 18180
	 */
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 18196
	 */
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	/**
	deg
	 * offset 18212
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 18724
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 18756
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	%
	 * offset 18788
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	kPa
	 * offset 19300
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 19332
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	lambda
	 * offset 19364
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	 * offset 19620
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 19652
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 19684
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	from
	 * offset 19940
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	to
	 * offset 19972
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	value
	 * offset 20004
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20260
	 */
	uint16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20276
	 */
	uint16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20292
	 */
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20356
	 */
	uint16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20372
	 */
	uint16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20388
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20452
	 */
	uint16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20468
	 */
	uint16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20484
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20548
	 */
	uint16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20564
	 */
	uint16_t scriptTable4RpmBins[SCRIPT_TABLE_8];
	/**
	 * offset 20580
	 */
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20588
	 */
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20596
	 */
	cyl_trim_s ignTrims[12];
	/**
	 * offset 20788
	 */
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20796
	 */
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20804
	 */
	cyl_trim_s fuelTrims[12];
	/** total size 20996*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on hellen_cypress_gen_config.bat integration/rusefi_config.txt Sat Dec 25 04:39:41 UTC 2021
