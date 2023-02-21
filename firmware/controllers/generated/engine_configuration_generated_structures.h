// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on gen_config.sh integration/rusefi_config.txt Tue Feb 21 11:00:58 PST 2023
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
	load
	 * offset 10
	 */
	scaled_channel<int16_t, 10, 1> loadBins[GPPWM_LOAD_COUNT];
	/**
	RPM
	 * offset 26
	 */
	scaled_channel<int16_t, 1, 1> rpmBins[GPPWM_RPM_COUNT];
	/**
	duty
	 * offset 42
	 */
	scaled_channel<uint8_t, 2, 1> table[GPPWM_RPM_COUNT][GPPWM_LOAD_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 106
	 */
	uint8_t alignmentFill_at_106[2];
};
static_assert(sizeof(gppwm_channel) == 108);

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
	 * offset 0
	 */
	trigger_type_e type;
	/**
	number
	 * offset 4
	 */
	int customTotalToothCount;
	/**
	number
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
	units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2];
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
	 * offset 0
	 */
	engine_type_e engineType;
	/**
	 * Disable sensor sniffer above this rpm
	RPM
	 * offset 4
	 */
	uint16_t sensorSnifferRpmThreshold;
	/**
	 * A secondary Rev limit engaged by the driver to help launch the vehicle faster
	rpm
	 * offset 6
	 */
	uint16_t launchRpm;
	/**
	rpm
	 * offset 8
	 */
	uint16_t rpmHardLimit;
	/**
	 * Engine sniffer would be disabled above this rpm
	RPM
	 * offset 10
	 */
	uint16_t engineSnifferRpmThreshold;
	/**
	 * Disable multispark above this engine speed.
	rpm
	 * offset 12
	 */
	scaled_channel<uint8_t, 1, 50> multisparkMaxRpm;
	/**
	 * Above this RPM, disable AC. Set to 0 to disable check.
	rpm
	 * offset 13
	 */
	scaled_channel<uint8_t, 1, 50> maxAcRpm;
	/**
	 * Above this TPS, disable AC. Set to 0 to disable check.
	%
	 * offset 14
	 */
	uint8_t maxAcTps;
	/**
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	deg C
	 * offset 15
	 */
	uint8_t maxAcClt;
	/**
	RPM
	 * offset 16
	 */
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	deg
	 * offset 48
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	count
	 * offset 49
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 50
	 */
	uint8_t alignmentFill_at_50[2];
	/**
	 * offset 52
	 */
	injector_s injector;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 120 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 120 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 120 bit 2 */
	bool isVerboseAuxPid1 : 1 {};
	/**
	offset 120 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 120 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 120 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 120 bit 6 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 120 bit 7 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 120 bit 8 */
	bool enableTrailingSparks : 1 {};
	/**
	 * TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 120 bit 9 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 120 bit 10 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 120 bit 11 */
	bool useEeprom : 1 {};
	/**
	offset 120 bit 12 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 120 bit 13 */
	bool kickStartCranking : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 120 bit 14 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 120 bit 15 */
	bool launchControlEnabled : 1 {};
	/**
	 * "Detect double trigger edges"
	offset 120 bit 16 */
	bool doNotFilterTriggerEdgeNoise : 1 {};
	/**
	offset 120 bit 17 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 120 bit 18 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	 * 
	 * enable logic_level_trigger
	offset 120 bit 19 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 120 bit 20 */
	bool useTLE8888_stepper : 1 {};
	/**
	 * If enabled, the MAP estimate table will be used if the MAP sensor fails to estimate manifold pressure based on RPM and TPS.
	offset 120 bit 21 */
	bool enableMapEstimationTableFallback : 1 {};
	/**
	offset 120 bit 22 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 120 bit 23 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 120 bit 24 */
	bool artificialTestMisfire : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 120 bit 25 */
	bool useFordRedundantPps : 1 {};
	/**
	offset 120 bit 26 */
	bool unusedBit_40_26 : 1 {};
	/**
	offset 120 bit 27 */
	bool unusedBit_40_27 : 1 {};
	/**
	offset 120 bit 28 */
	bool unusedBit_40_28 : 1 {};
	/**
	offset 120 bit 29 */
	bool unusedBit_40_29 : 1 {};
	/**
	offset 120 bit 30 */
	bool unusedBit_40_30 : 1 {};
	/**
	offset 120 bit 31 */
	bool unusedBit_40_31 : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	ADC
	 * offset 124
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	ADC
	 * offset 126
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 128
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 130
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 132
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	ms
	 * offset 140
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	rpm
	 * offset 144
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	rpm
	 * offset 146
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 148
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 288
	 */
	ThermistorConf clt;
	/**
	 * offset 320
	 */
	ThermistorConf iat;
	/**
	deg
	 * offset 352
	 */
	int launchTimingRetard;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	kHz
	 * offset 356
	 */
	float knockBandCustom;
	/**
	 * offset 360
	 */
	specs_s specs;
	/**
	 * Cylinder diameter in mm.
	mm
	 * offset 372
	 */
	float cylinderBore;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS commonly only used for NA engines
	 * Speed Density requires MAP sensor and is the default choice for may installs
	 * MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	 * offset 376
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	%
	 * offset 377
	 */
	uint8_t ALSMaxTPS;
	/**
	 * offset 378
	 */
	Gpio criticalLedPin;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 380
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups. If your injectors are individually wired you will also need to enable "Two wire batch emulation".
	 * offset 381
	 */
	injection_mode_e injectionMode;
	/**
	 * Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 382
	 */
	uint16_t boostControlMinRpm;
	/**
	 * Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 384
	 */
	uint8_t boostControlMinTps;
	/**
	 * Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 385
	 */
	uint8_t boostControlMinMap;
	/**
	 * need 4 byte alignment
	units
	 * offset 386
	 */
	uint8_t alignmentFill_at_386[2];
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	deg
	 * offset 388
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * offset 392
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	count
	 * offset 393
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	kph
	 * offset 394
	 */
	uint8_t maxIdleVss;
	/**
	 * need 4 byte alignment
	units
	 * offset 395
	 */
	uint8_t alignmentFill_at_395[1];
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	kPa
	 * offset 396
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 398
	 */
	timing_mode_e timingMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 399
	 */
	uint8_t alignmentFill_at_399[1];
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	RPM
	 * offset 400
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchnization point before TDC and negative in case of synchnization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	deg btdc
	 * offset 404
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	coef
	 * offset 408
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	coef
	 * offset 412
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 416
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 420
	 */
	float fanOffTemperature;
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	revs/km
	 * offset 424
	 */
	float driveWheelRevPerKm;
	/**
	 * offset 428
	 */
	can_nbc_e canNbcType;
	/**
	 * need 4 byte alignment
	units
	 * offset 429
	 */
	uint8_t alignmentFill_at_429[3];
	/**
	 * CANbus thread period in ms
	ms
	 * offset 432
	 */
	int canSleepPeriodMs;
	/**
	 * offset 436
	 */
	uint8_t unused440;
	/**
	 * need 4 byte alignment
	units
	 * offset 437
	 */
	uint8_t alignmentFill_at_437[3];
	/**
	index
	 * offset 440
	 */
	int byFirmwareVersion;
	/**
	 * First throttle body, first sensor
	 * offset 444
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 445
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 446
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body, first sensor
	 * offset 447
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	x
	 * offset 448
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	angle
	 * offset 452
	 */
	int trailingSparkAngle;
	/**
	 * offset 456
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	%
	 * offset 468
	 */
	float airByRpmTaper;
	/**
	 * Single value to be used in event of a failed MAP sensor 
	 * This value is only used for speed density fueling calculations.
	kPa
	 * offset 472
	 */
	uint8_t failedMapFallback;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	%
	 * offset 473
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 474
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 475
	 */
	uint8_t alignmentFill_at_475[1];
	/**
	coef
	 * offset 476
	 */
	float globalFuelCorrection;
	/**
	volts
	 * offset 480
	 */
	float adcVcc;
	/**
	Deg
	 * offset 484
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 488
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 496
	 */
	afr_sensor_s afr;
	/**
	 * Throttle pedal position first channel
	 * offset 516
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 517
	 */
	uint8_t alignmentFill_at_517[1];
	/**
	 * offset 518
	 */
	Gpio tle6240_cs;
	/**
	 * offset 520
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 521
	 */
	uint8_t alignmentFill_at_521[1];
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 522
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * @see hasBaroSensor
	 * offset 524
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 536
	 */
	idle_hardware_s idle;
	/**
	 * Value between 0 and 100 used in Manual mode
	%
	 * offset 548
	 */
	float manIdlePosition;
	/**
	 * Ignition timing to remove when a knock event occurs.
	%
	 * offset 552
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	deg/s
	 * offset 553
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 554
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 555
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	ratio
	 * offset 556
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	count
	 * offset 558
	 */
	uint8_t vssToothCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 559
	 */
	uint8_t alignmentFill_at_559[1];
	/**
	 * offset 560
	 */
	Gpio l9779_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 562
	 */
	uint8_t alignmentFill_at_562[2];
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * TODO: rename to triggerSimulatorRpm
	Rpm
	 * offset 564
	 */
	int triggerSimulatorFrequency;
	/**
	 * offset 568
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 592
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 616
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 617
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 618
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 620
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 621
	 */
	uint8_t alignmentFill_at_621[1];
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 622
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 624
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 625
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 626
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 628
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 630
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 632
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 633
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 634
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 635
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 636
	 */
	Gpio debugTriggerSync;
	/**
	 * offset 638
	 */
	Gpio mc33972_cs;
	/**
	 * offset 640
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 641
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 642
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 643
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 644
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	%
	 * offset 645
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 653
	 */
	ego_sensor_e afr_type;
	/**
	 * need 4 byte alignment
	units
	 * offset 654
	 */
	uint8_t alignmentFill_at_654[2];
	/**
	 * 0.1 is a good default value
	x
	 * offset 656
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 660
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	us
	 * offset 664
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * offset 666
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 668
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 669
	 */
	uint8_t alignmentFill_at_669[1];
	/**
	 * offset 670
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 672
	 */
	Gpio sdCardCsPin;
	/**
	 * set_can_tx_pin X
	 * offset 674
	 */
	Gpio canTxPin;
	/**
	 * set_can_rx_pin X
	 * offset 676
	 */
	Gpio canRxPin;
	/**
	 * offset 678
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * Additional idle % while A/C is active
	%
	 * offset 679
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * Ratio between the wheels and your transmission output.
	ratio
	 * offset 680
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 682
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 684
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	units
	 * offset 685
	 */
	uint8_t alignmentFill_at_685[1];
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	mv
	 * offset 686
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 688
	 */
	uint16_t wastegatePositionMax;
	/**
	 * need 4 byte alignment
	units
	 * offset 690
	 */
	uint8_t alignmentFill_at_690[2];
	/**
	 * Secondary TTL channel baud rate
	BPs
	 * offset 692
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	CR
	 * offset 696
	 */
	float compressionRatio;
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 700
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	g/s
	 * offset 704
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 706
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 708
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 709
	 */
	uint8_t alignmentFill_at_709[1];
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 710
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 712
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * need 4 byte alignment
	units
	 * offset 713
	 */
	uint8_t alignmentFill_at_713[3];
	/**
	offset 716 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 716 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 716 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 716 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 716 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 716 bit 5 */
	bool isEngineControlEnabled : 1 {};
	/**
	offset 716 bit 6 */
	bool isVerboseAlternator : 1 {};
	/**
	offset 716 bit 7 */
	bool verboseQuad : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 716 bit 8 */
	bool useStepperIdle : 1 {};
	/**
	offset 716 bit 9 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 716 bit 10 */
	bool unused760b12 : 1 {};
	/**
	offset 716 bit 11 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 716 bit 12 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 716 bit 13 */
	bool etb1configured : 1 {};
	/**
	offset 716 bit 14 */
	bool etb2configured : 1 {};
	/**
	 * Useful for individual intakes
	offset 716 bit 15 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 716 bit 16 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 716 bit 17 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 716 bit 18 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 716 bit 19 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 716 bit 20 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 716 bit 21 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 716 bit 22 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 716 bit 23 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 716 bit 24 */
	bool pauseEtbControl : 1 {};
	/**
	offset 716 bit 25 */
	bool alignEngineSnifferAtTDC : 1 {};
	/**
	offset 716 bit 26 */
	bool unused764b29 : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 716 bit 27 */
	bool enableAemXSeries : 1 {};
	/**
	offset 716 bit 28 */
	bool unusedBit_211_28 : 1 {};
	/**
	offset 716 bit 29 */
	bool unusedBit_211_29 : 1 {};
	/**
	offset 716 bit 30 */
	bool unusedBit_211_30 : 1 {};
	/**
	offset 716 bit 31 */
	bool unusedBit_211_31 : 1 {};
	/**
	 * offset 720
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 728
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 729
	 */
	uint8_t alignmentFill_at_729[3];
	/**
	 * offset 732
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	v
	 * offset 736
	 */
	uint8_t mc33_hvolt;
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	kPa
	 * offset 737
	 */
	uint8_t minimumBoostClosedLoopMap;
	/**
	 * Optional Radiator Fan used with A/C
	 * offset 738
	 */
	output_pin_e acFanPin;
	/**
	 * offset 740
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * offset 741
	 */
	spi_device_e l9779spiDevice;
	/**
	volts
	 * offset 742
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	 * offset 750
	 */
	imu_type_e imuType;
	/**
	multiplier
	 * offset 751
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 759
	 */
	uint8_t alignmentFill_at_759[1];
	/**
	kg
	 * offset 760
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling?
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	RPM
	 * offset 762
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	ms
	 * offset 764
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 766
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 768
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 770
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 772
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 774
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 775
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 776
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 792
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * offset 794
	 */
	Gpio test557pin;
	/**
	 * offset 796
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 797
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	:1
	 * offset 798
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	%
	 * offset 799
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	hz
	 * offset 800
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 802
	 */
	Gpio debugMapAveraging;
	/**
	 * offset 804
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 806
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 807
	 */
	uint8_t alignmentFill_at_807[1];
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 808
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 810
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	count
	 * offset 812
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	%
	 * offset 816
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	%
	 * offset 818
	 */
	int16_t stepperParkingExtraSteps;
	/**
	ADC
	 * offset 820
	 */
	uint16_t tps1SecondaryMin;
	/**
	ADC
	 * offset 822
	 */
	uint16_t tps1SecondaryMax;
	/**
	rpm
	 * offset 824
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	Seconds
	 * offset 826
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 828
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 830
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 831
	 */
	uint8_t alignmentFill_at_831[1];
	/**
	 * offset 832
	 */
	output_pin_e acRelayPin;
	/**
	 * offset 834
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 835
	 */
	uint8_t alignmentFill_at_835[1];
	/**
	 * offset 836
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 868
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 870
	 */
	Gpio spi1misoPin;
	/**
	 * offset 872
	 */
	Gpio spi1sckPin;
	/**
	 * offset 874
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 876
	 */
	Gpio spi2misoPin;
	/**
	 * offset 878
	 */
	Gpio spi2sckPin;
	/**
	 * offset 880
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 882
	 */
	Gpio spi3misoPin;
	/**
	 * offset 884
	 */
	Gpio spi3sckPin;
	/**
	 * offset 886
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEFI console Sensor Sniffer mode
	 * offset 887
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 888
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * need 4 byte alignment
	units
	 * offset 889
	 */
	uint8_t alignmentFill_at_889[3];
	/**
	offset 892 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 892 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 892 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 892 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 892 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 892 bit 5 */
	bool enableCanVss : 1 {};
	/**
	offset 892 bit 6 */
	bool enableInnovateLC2 : 1 {};
	/**
	offset 892 bit 7 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 892 bit 8 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 892 bit 9 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 892 bit 10 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	 * enable vvt_details
	offset 892 bit 11 */
	bool verboseVVTDecoding : 1 {};
	/**
	 * get invertCamVVTSignal
	offset 892 bit 12 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	 * enable consumeObdSensors
	offset 892 bit 13 */
	bool consumeObdSensors : 1 {};
	/**
	offset 892 bit 14 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 892 bit 15 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 892 bit 16 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 892 bit 17 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 892 bit 18 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 892 bit 19 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 892 bit 20 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 892 bit 21 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 892 bit 22 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 892 bit 23 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 892 bit 24 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 892 bit 25 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 892 bit 26 */
	bool tcuEnabled : 1 {};
	/**
	offset 892 bit 27 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coilss
	offset 892 bit 28 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 892 bit 29 */
	bool verboseCan2 : 1 {};
	/**
	offset 892 bit 30 */
	bool unusedBit_308_30 : 1 {};
	/**
	offset 892 bit 31 */
	bool unusedBit_308_31 : 1 {};
	/**
	 * offset 896
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 912
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 914
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 915
	 */
	uint8_t alignmentFill_at_915[1];
	/**
	 * offset 916
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 918
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 920
	 */
	pid_s boostPid;
	/**
	 * offset 940
	 */
	boostType_e boostType;
	/**
	 * need 4 byte alignment
	units
	 * offset 941
	 */
	uint8_t alignmentFill_at_941[3];
	/**
	Hz
	 * offset 944
	 */
	int boostPwmFrequency;
	/**
	 * offset 948
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 949
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 950
	 */
	uint8_t alignmentFill_at_950[2];
	/**
	 * Disabled above this speed
	Kph
	 * offset 952
	 */
	int launchSpeedThreshold;
	/**
	 * Range from Launch RPM for Timing Retard to activate
	RPM
	 * offset 956
	 */
	int launchTimingRpmRange;
	/**
	 * Extra Fuel Added
	%
	 * offset 960
	 */
	int launchFuelAdded;
	/**
	 * Duty Cycle for the Boost Solenoid
	%
	 * offset 964
	 */
	int launchBoostDuty;
	/**
	 * Range from Launch RPM to activate Hard Cut
	RPM
	 * offset 968
	 */
	int hardCutRpmRange;
	/**
	 * offset 972
	 */
	float turbochargerFilter;
	/**
	 * offset 976
	 */
	int launchTpsThreshold;
	/**
	 * offset 980
	 */
	float launchActivateDelay;
	/**
	 * offset 984
	 */
	stft_s stft;
	/**
	 * offset 1008
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1024
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1056
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1088
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1120
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * offset 1132
	 */
	dc_function_e etbFunctions[ETB_COUNT];
	/**
	 * offset 1134
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1135
	 */
	uint8_t alignmentFill_at_1135[1];
	/**
	 * offset 1136
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1138
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1139
	 */
	uint8_t alignmentFill_at_1139[1];
	/**
	 * offset 1140
	 */
	Gpio drv8860_miso;
	/**
	volt
	 * offset 1142
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * offset 1158
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1174
	 */
	uint8_t alignmentFill_at_1174[2];
	/**
	 * Angle between cam sensor and VVT zero position
	value
	 * offset 1176
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1192
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1224
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	ADC
	 * offset 1288
	 */
	uint16_t tps2SecondaryMin;
	/**
	ADC
	 * offset 1290
	 */
	uint16_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1292 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1292 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Print details into rusEFI console
	 * enable verbose_idle
	offset 1292 bit 2 */
	bool isVerboseIAC : 1 {};
	/**
	offset 1292 bit 3 */
	bool boardUseTachPullUp : 1 {};
	/**
	offset 1292 bit 4 */
	bool boardUseTempPullUp : 1 {};
	/**
	offset 1292 bit 5 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1292 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1292 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1292 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write
	offset 1292 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1292 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1292 bit 11 */
	bool boardUse2stepPullDown : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1292 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1292 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1292 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1292 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1292 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1292 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1292 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1292 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1292 bit 20 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 1292 bit 21 */
	bool boardUseCamPullDown : 1 {};
	/**
	offset 1292 bit 22 */
	bool boardUseCamVrPullUp : 1 {};
	/**
	offset 1292 bit 23 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 1292 bit 24 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 1292 bit 25 */
	bool boardUseD4PullDown : 1 {};
	/**
	offset 1292 bit 26 */
	bool boardUseD5PullDown : 1 {};
	/**
	offset 1292 bit 27 */
	bool verboseIsoTp : 1 {};
	/**
	offset 1292 bit 28 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1292 bit 29 */
	bool launchActivateInverted : 1 {};
	/**
	offset 1292 bit 30 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1292 bit 31 */
	bool skippedWheelOnCam : 1 {};
	/**
	 * offset 1296
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 1297
	 */
	uint8_t alignmentFill_at_1297[1];
	/**
	 * A/C button input
	 * offset 1298
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1300
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	%
	 * offset 1301
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1302
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1303
	 */
	uint8_t alignmentFill_at_1303[1];
	/**
	offset 1304 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1304 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1304 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1304 bit 3 */
	bool complexWallModel : 1 {};
	/**
	offset 1304 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1304 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1304 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1304 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1304 bit 8 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1304 bit 9 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	 * enable trigger_details
	offset 1304 bit 10 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1304 bit 11 */
	bool isManualSpinningMode : 1 {};
	/**
	 * This is needed if your coils are individually wired and you wish to use batch injection.
	 * enable two_wire_batch_injection
	offset 1304 bit 12 */
	bool twoWireBatchInjection : 1 {};
	/**
	offset 1304 bit 13 */
	bool neverInstantRpm : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1304 bit 14 */
	bool twoWireBatchIgnition : 1 {};
	/**
	offset 1304 bit 15 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1304 bit 16 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1304 bit 17 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1304 bit 18 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1304 bit 19 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1304 bit 20 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1304 bit 21 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1304 bit 22 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1304 bit 23 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors. WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1304 bit 24 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1304 bit 25 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1304 bit 26 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1304 bit 27 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	offset 1304 bit 28 */
	bool sdTriggerLog : 1 {};
	/**
	offset 1304 bit 29 */
	bool ALSActivateInverted : 1 {};
	/**
	offset 1304 bit 30 */
	bool unused_1484_bit_30 : 1 {};
	/**
	offset 1304 bit 31 */
	bool tempBooleanForVerySpecialLogic : 1 {};
	/**
	count
	 * offset 1308
	 */
	uint32_t engineChartSize;
	/**
	mult
	 * offset 1312
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1316
	 */
	Gpio camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	RPM
	 * offset 1324
	 */
	int16_t acIdleRpmBump;
	/**
	seconds
	 * offset 1326
	 */
	int16_t warningPeriod;
	/**
	angle
	 * offset 1328
	 */
	float knockDetectionWindowStart;
	/**
	angle
	 * offset 1332
	 */
	float knockDetectionWindowEnd;
	/**
	ms
	 * offset 1336
	 */
	float idleStepperReactionTime;
	/**
	count
	 * offset 1340
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	sec
	 * offset 1344
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	index
	 * offset 1348
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1352
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	Seconds
	 * offset 1356
	 */
	float wwaeTau;
	/**
	 * offset 1360
	 */
	pid_s alternatorControl;
	/**
	 * offset 1380
	 */
	pid_s etb;
	/**
	 * offset 1400
	 */
	Gpio triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * RPM range above upper limit for extra air taper,"RPM", 1, 0, 0, 1500, 0
	 * offset 1404
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1406
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	ADC
	 * offset 1408
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	ADC
	 * offset 1410
	 */
	int16_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1412
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1414
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1415
	 */
	uint8_t alignmentFill_at_1415[1];
	/**
	 * offset 1416
	 */
	Gpio mc33816_flag0;
	/**
	Pulse
	 * offset 1418
	 */
	uint8_t tachPulsePerRev;
	/**
	 * need 4 byte alignment
	units
	 * offset 1419
	 */
	uint8_t alignmentFill_at_1419[1];
	/**
	 * kPa value which is too low to be true
	kPa
	 * offset 1420
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	kPa
	 * offset 1424
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	ms
	 * offset 1428
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	ms
	 * offset 1430
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1432
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	Fraction
	 * offset 1452
	 */
	float wwaeBeta;
	/**
	 * blue LED on many rusEFI boards.
	 * Blue Communication LED which is expected to blink at 50% duty cycle during normal board operation.
	 * If USB communication cable is connected Blue LED starts to blink faster.
	 * offset 1456
	 */
	Gpio communicationLedPin;
	/**
	 * Green LED on many rusEFI boards.
	 * Off if engine is stopped, blinks if engine is cranking, solid if engine is running.
	 * offset 1458
	 */
	Gpio runningLedPin;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1460
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1462
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1464
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * offset 1468
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1470
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	voltage
	 * offset 1472
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1476
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	seconds
	 * offset 1480
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	RPM
	 * offset 1482
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	Volts
	 * offset 1484
	 */
	float targetVBatt;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	%
	 * offset 1488
	 */
	float alternatorOffAboveTps;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	cycles
	 * offset 1492
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	percent
	 * offset 1494
	 */
	int16_t iacByTpsTaper;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * offset 1496
	 */
	Gpio auxSerialTxPin;
	/**
	 * offset 1498
	 */
	Gpio warningLedPin;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * offset 1500
	 */
	Gpio auxSerialRxPin;
	/**
	 * offset 1502
	 */
	Gpio LIS302DLCsPin;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	sec
	 * offset 1504
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 1505
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 1506
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	sec
	 * offset 1507
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	roc
	 * offset 1508
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1512
	 */
	brain_input_pin_e auxSpeedSensorInputPin[2];
	/**
	 * offset 1516
	 */
	uint8_t totalGearsCount;
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 1517
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1518
	 */
	uint8_t alignmentFill_at_1518[2];
	/**
	 * Band rate for primary TTL
	BPs
	 * offset 1520
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	roc
	 * offset 1524
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	coeff
	 * offset 1528
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	coeff
	 * offset 1532
	 */
	float slowAdcAlpha;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1536
	 */
	debug_mode_e debugMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1537
	 */
	uint8_t alignmentFill_at_1537[3];
	/**
	BPs
	 * offset 1540
	 */
	uint32_t auxSerialSpeed;
	/**
	voltage
	 * offset 1544
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1548
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * set can_baudrate
	 * offset 1552
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1553
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1554
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1555
	 */
	load_override_e afrOverrideMode;
	/**
	A
	 * offset 1556
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	A
	 * offset 1557
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	us
	 * offset 1558
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	ms
	 * offset 1559
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1560 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Allow OpenBLT on Primary CAN
	offset 1560 bit 1 */
	bool canOpenBLT : 1 {};
	/**
	 * Allow OpenBLT on Secondary CAN
	offset 1560 bit 2 */
	bool can2OpenBLT : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (defualt, cc/min) or mass flow (g/s).
	offset 1560 bit 3 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1560 bit 4 */
	bool unused1127 : 1 {};
	/**
	offset 1560 bit 5 */
	bool unused1128 : 1 {};
	/**
	offset 1560 bit 6 */
	bool unused1129 : 1 {};
	/**
	offset 1560 bit 7 */
	bool unused1130 : 1 {};
	/**
	offset 1560 bit 8 */
	bool unusedBit_508_8 : 1 {};
	/**
	offset 1560 bit 9 */
	bool unusedBit_508_9 : 1 {};
	/**
	offset 1560 bit 10 */
	bool unusedBit_508_10 : 1 {};
	/**
	offset 1560 bit 11 */
	bool unusedBit_508_11 : 1 {};
	/**
	offset 1560 bit 12 */
	bool unusedBit_508_12 : 1 {};
	/**
	offset 1560 bit 13 */
	bool unusedBit_508_13 : 1 {};
	/**
	offset 1560 bit 14 */
	bool unusedBit_508_14 : 1 {};
	/**
	offset 1560 bit 15 */
	bool unusedBit_508_15 : 1 {};
	/**
	offset 1560 bit 16 */
	bool unusedBit_508_16 : 1 {};
	/**
	offset 1560 bit 17 */
	bool unusedBit_508_17 : 1 {};
	/**
	offset 1560 bit 18 */
	bool unusedBit_508_18 : 1 {};
	/**
	offset 1560 bit 19 */
	bool unusedBit_508_19 : 1 {};
	/**
	offset 1560 bit 20 */
	bool unusedBit_508_20 : 1 {};
	/**
	offset 1560 bit 21 */
	bool unusedBit_508_21 : 1 {};
	/**
	offset 1560 bit 22 */
	bool unusedBit_508_22 : 1 {};
	/**
	offset 1560 bit 23 */
	bool unusedBit_508_23 : 1 {};
	/**
	offset 1560 bit 24 */
	bool unusedBit_508_24 : 1 {};
	/**
	offset 1560 bit 25 */
	bool unusedBit_508_25 : 1 {};
	/**
	offset 1560 bit 26 */
	bool unusedBit_508_26 : 1 {};
	/**
	offset 1560 bit 27 */
	bool unusedBit_508_27 : 1 {};
	/**
	offset 1560 bit 28 */
	bool unusedBit_508_28 : 1 {};
	/**
	offset 1560 bit 29 */
	bool unusedBit_508_29 : 1 {};
	/**
	offset 1560 bit 30 */
	bool unusedBit_508_30 : 1 {};
	/**
	offset 1560 bit 31 */
	bool unusedBit_508_31 : 1 {};
	/**
	 * Time between bench test pulses
	ms
	 * offset 1564
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOffTime;
	/**
	 * How many test bench pulses do you want
	count
	 * offset 1565
	 */
	uint8_t benchTestCount;
	/**
	 * Length of each of the test pulses
	ms
	 * offset 1566
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOnTime;
	/**
	 * offset 1567
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * set_can2_tx_pin X
	 * offset 1568
	 */
	Gpio can2TxPin;
	/**
	 * set_can2_rx_pin X
	 * offset 1570
	 */
	Gpio can2RxPin;
	/**
	 * offset 1572
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1573
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1574
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1575
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1576
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1578
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1579
	 */
	uint8_t alignmentFill_at_1579[1];
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	kPa (absolute)
	 * offset 1580
	 */
	float boostCutPressure;
	/**
	kg/h
	 * offset 1584
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16];
	/**
	ratio
	 * offset 1600
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16];
	/**
	 * Fixed timing, useful for TDC testing
	deg
	 * offset 1616
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	v
	 * offset 1620
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	v
	 * offset 1624
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	value
	 * offset 1628
	 */
	float egoValueShift;
	/**
	 * VVT output
	 * TODO: rename to vvtOutputs
	 * offset 1632
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * offset 1640
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1641
	 */
	uint8_t alignmentFill_at_1641[3];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	percent
	 * offset 1644
	 */
	int crankingIACposition;
	/**
	 * offset 1648
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 1652
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 1656
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 1660
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 1664
	 */
	pwm_freq_t vvtOutputFrequency[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #1 is active
	%
	 * offset 1668
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * need 4 byte alignment
	units
	 * offset 1669
	 */
	uint8_t alignmentFill_at_1669[3];
	/**
	Hz
	 * offset 1672
	 */
	int alternatorPwmFrequency;
	/**
	 * offset 1676
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #2 is active
	%
	 * offset 1678
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	sec
	 * offset 1679
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1680
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT];
	/**
	 * offset 1688
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 1712
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1713
	 */
	pin_output_mode_e LIS302DLCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 1714
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 1715
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	kPa
	 * offset 1716
	 */
	float fuelReferencePressure;
	/**
	 * Fuel multiplier (enrichment) immediately after engine start
	mult
	 * offset 1720
	 */
	float postCrankingFactor;
	/**
	 * Time over which to taper out after start enrichment
	seconds
	 * offset 1724
	 */
	float postCrankingDurationSec;
	/**
	 * offset 1728
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1760
	 */
	ThermistorConf auxTempSensor2;
	/**
	Deg
	 * offset 1792
	 */
	int16_t knockSamplingDuration;
	/**
	Hz
	 * offset 1794
	 */
	int16_t etbFreq;
	/**
	 * offset 1796
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 1816
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	%
	 * offset 1817
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	%
	 * offset 1818
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 1819
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder timing correction
	deg
	 * offset 1820
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	seconds
	 * offset 1868
	 */
	float idlePidActivationTime;
	/**
	 * offset 1872
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 1873
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 1874
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 1875
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 1876
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 1877
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 1878
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 1879
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 1880
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 1881
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 1882
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 1884
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 1886
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * offset 1888
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1889
	 */
	uint8_t alignmentFill_at_1889[3];
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 1892
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 1932
	 */
	float injectorCorrectionPolynomial[8];
	/**
	C
	 * offset 1964
	 */
	int8_t primeBins[8];
	/**
	 * offset 1972
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 1992
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1993
	 */
	uint8_t alignmentFill_at_1993[1];
	/**
	 * offset 1994
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 1996
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 1997
	 */
	uint8_t fan2OffTemperature;
	/**
	 * offset 1998
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2000
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2002
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2003
	 */
	uint8_t alignmentFill_at_2003[1];
	/**
	 * offset 2004
	 */
	Gpio mc33816_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 2006
	 */
	uint8_t alignmentFill_at_2006[2];
	/**
	 * offset 2008
	 */
	float auxFrequencyFilter;
	/**
	RPM
	 * offset 2012
	 */
	int16_t vvtControlMinRpm;
	/**
	 * offset 2014
	 */
	brain_input_pin_e sentInputPins[SENT_INPUT_COUNT];
	/**
	 * offset 2016
	 */
	int8_t launchFuelAdderPercent;
	/**
	 * Time required to detect a stuck throttle.
	sec
	 * offset 2017
	 */
	scaled_channel<uint8_t, 50, 1> etbJamTimeout;
	/**
	 * By the way ETB PID runs at 500hz, length in 1/500 of second here.
	 * offset 2018
	 */
	uint16_t etbExpAverageLength;
	/**
	 * offset 2020
	 */
	float etbDutyThreshold;
	/**
	 * This sets the RPM above which fuel cut is active.
	rpm
	 * offset 2024
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	rpm
	 * offset 2026
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	%
	 * offset 2028
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	C
	 * offset 2030
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	%
	 * offset 2032
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	kPa
	 * offset 2034
	 */
	int16_t coastingFuelCutMap;
	/**
	 * offset 2036
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2056
	 */
	linear_sensor_s lowPressureFuel;
	/**
	C
	 * offset 2076
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	RPM
	 * offset 2080
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * offset 2088
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	 * offset 2184
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 2248
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2376
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2380
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	kg/h
	 * offset 2384
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	deg/sec
	 * offset 2388
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	deg/sec
	 * offset 2392
	 */
	float tChargeAirDecrLimit;
	/**
	 * offset 2396
	 */
	tChargeMode_e tChargeMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2397
	 */
	uint8_t alignmentFill_at_2397[1];
	/**
	 * iTerm min value
	 * offset 2398
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2400
	 */
	int16_t etb_iTermMax;
	/**
	 * need 4 byte alignment
	units
	 * offset 2402
	 */
	uint8_t alignmentFill_at_2402[2];
	/**
	 * See useIdleTimingPidControl
	 * offset 2404
	 */
	pid_s idleTimingPid;
	/**
	 * By the way ETB PID runs at 500hz, length in 1/500 of second here.
	 * offset 2424
	 */
	int16_t etbRocExpAverageLength;
	/**
	 * A delay in cycles between fuel-enrich. portions
	cycles
	 * offset 2426
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	coef
	 * offset 2428
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2432
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2433
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2434
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2436
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	:1
	 * offset 2437
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2438
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	%
	 * offset 2440
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2444
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	mg
	 * offset 2456
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[8];
	/**
	 * Trigger comparator center point voltage
	V
	 * offset 2464
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	V
	 * offset 2465
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	V
	 * offset 2466
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	RPM
	 * offset 2467
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * offset 2468
	 */
	pid_s idleRpmPid2;
	/**
	 * offset 2488
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	units
	 * offset 2489
	 */
	uint8_t alignmentFill_at_2489[3];
	/**
	 * offset 2492
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	 * Boost Current
	mA
	 * offset 2924
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	mA
	 * offset 2926
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	mA
	 * offset 2928
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	us
	 * offset 2930
	 */
	uint16_t mc33_t_max_boost;
	/**
	us
	 * offset 2932
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	us
	 * offset 2934
	 */
	uint16_t mc33_t_peak_tot;
	/**
	us
	 * offset 2936
	 */
	uint16_t mc33_t_bypass;
	/**
	us
	 * offset 2938
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	us
	 * offset 2940
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 2942
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 2943
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 2944
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 2945
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	dB
	 * offset 2951
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 2967
	 */
	uint8_t alignmentFill_at_2967[1];
	/**
	ratio
	 * offset 2968
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	ratio
	 * offset 3040
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	rpm
	 * offset 3112
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	sec
	 * offset 3113
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	sec
	 * offset 3114
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * need 4 byte alignment
	units
	 * offset 3115
	 */
	uint8_t alignmentFill_at_3115[1];
	/**
	mg
	 * offset 3116
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	multiplier
	 * offset 3118
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	/**
	%
	 * offset 3122
	 */
	uint8_t etbJamIntegratorLimit;
	/**
	lobes/cam
	 * offset 3123
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 3124
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Crank angle ATDC of first lobe peak
	deg
	 * offset 3125
	 */
	uint8_t hpfpPeakPos;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	deg
	 * offset 3126
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	units
	 * offset 3127
	 */
	uint8_t alignmentFill_at_3127[1];
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	cc
	 * offset 3128
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	deg
	 * offset 3130
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 3131
	 */
	uint8_t issFilterReciprocal;
	/**
	%/kPa
	 * offset 3132
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	%/kPa/lobe
	 * offset 3134
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	kPa/s
	 * offset 3136
	 */
	uint16_t hpfpTargetDecay;
	/**
	%
	 * offset 3138
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	deg
	 * offset 3154
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	volts
	 * offset 3170
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	ms
	 * offset 3178
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	kPa
	 * offset 3194
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	load
	 * offset 3394
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	RPM
	 * offset 3414
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	%
	 * offset 3424
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	cc/lobe
	 * offset 3524
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	RPM
	 * offset 3544
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	 * offset 3554
	 */
	output_pin_e stepper_raw_output[4];
	/**
	ratio
	 * offset 3562
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[GEARS_COUNT];
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	ms
	 * offset 3578
	 */
	uint16_t vvtActivationDelayMs;
	/**
	deg C
	 * offset 3580
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	/**
	 * offset 3588
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3596
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	/**
	kPa
	 * offset 3604
	 */
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	/**
	 * offset 3612
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3620
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	/**
	Nm
	 * offset 3628
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_SIZE];
	/**
	RPM
	 * offset 3664
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE];
	/**
	Load
	 * offset 3676
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE];
	/**
	 * offset 3688
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3689
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3690
	 */
	uint8_t alignmentFill_at_3690[2];
	/**
	 * offset 3692
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3712
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 3732
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 3734
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3735
	 */
	uint8_t alignmentFill_at_3735[1];
	/**
	 * offset 3736
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 3738
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3739
	 */
	uint8_t alignmentFill_at_3739[1];
	/**
	 * offset 3740
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 3742
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 3744
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3745
	 */
	uint8_t alignmentFill_at_3745[1];
	/**
	 * offset 3746
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 3748
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 3750
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3751
	 */
	uint8_t alignmentFill_at_3751[1];
	/**
	 * offset 3752
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * need 4 byte alignment
	units
	 * offset 3754
	 */
	uint8_t alignmentFill_at_3754[2];
	/**
	%
	 * offset 3756
	 */
	float etbMinimumPosition;
	/**
	 * offset 3760
	 */
	uint16_t tuneHidingKey;
	/**
	 * offset 3762
	 */
	vin_number_t vinNumber;
	/**
	 * need 4 byte alignment
	units
	 * offset 3779
	 */
	uint8_t alignmentFill_at_3779[1];
	/**
	 * offset 3780
	 */
	uint16_t highSpeedOffsets[HIGH_SPEED_COUNT];
	/**
	 * offset 3844
	 */
	float etbDutyShutdownThreshold;
	/**
	 * offset 3848
	 */
	Gpio luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT];
	/**
	RPM
	 * offset 3864
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	/**
	rpm
	 * offset 3868
	 */
	int16_t ALSMinRPM;
	/**
	rpm
	 * offset 3870
	 */
	int16_t ALSMaxRPM;
	/**
	sec
	 * offset 3872
	 */
	int16_t ALSMaxDuration;
	/**
	C
	 * offset 3874
	 */
	int8_t ALSMinCLT;
	/**
	C
	 * offset 3875
	 */
	int8_t ALSMaxCLT;
	/**
	 * offset 3876
	 */
	uint8_t alsMinTimeBetween;
	/**
	 * offset 3877
	 */
	uint8_t alsEtbPosition;
	/**
	 * offset 3878
	 */
	uint8_t acRelayAlternatorDutyAdder;
	/**
	deg
	 * offset 3879
	 */
	uint8_t instantRpmRange;
	/**
	 * offset 3880
	 */
	uint8_t unusedA11lignmentFill_at_33;
	/**
	 * offset 3881
	 */
	SentEtbType sentEtbType;
	/**
	 * need 4 byte alignment
	units
	 * offset 3882
	 */
	uint8_t alignmentFill_at_3882[2];
	/**
	%
	 * offset 3884
	 */
	int ALSIdleAdd;
	/**
	%
	 * offset 3888
	 */
	int ALSEtbAdd;
	/**
	 * offset 3892
	 */
	int ALSSkipRatio;
	/**
	%
	 * offset 3896
	 */
	uint8_t ALSMaxDriverThrottleIntent;
	/**
	 * offset 3897
	 */
	pin_input_mode_e ALSActivatePinMode;
	/**
	 * For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	%
	 * offset 3898
	 */
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	/**
	 * For Toyota ETCS-i, use ~69%
	%
	 * offset 3899
	 */
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	/**
	 * offset 3900
	 */
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT];
	/**
	units
	 * offset 3908
	 */
	uint8_t mainUnusedEnd[96];
};
static_assert(sizeof(engine_configuration_s) == 4004);

// start of cyl_trim_s
struct cyl_trim_s {
	/**
	 * offset 0
	 */
	scaled_channel<int8_t, 5, 1> table[TRIM_SIZE][TRIM_SIZE];
};
static_assert(sizeof(cyl_trim_s) == 16);

// start of blend_table_s
struct blend_table_s {
	/**
	 * offset 0
	 */
	scaled_channel<int16_t, 10, 1> table[8][8];
	/**
	Load
	 * offset 128
	 */
	uint16_t loadBins[8];
	/**
	RPM
	 * offset 144
	 */
	uint16_t rpmBins[8];
	/**
	 * offset 160
	 */
	gppwm_channel_e blendParameter;
	/**
	 * need 4 byte alignment
	units
	 * offset 161
	 */
	uint8_t alignmentFill_at_161[1];
	/**
	 * offset 162
	 */
	scaled_channel<int16_t, 10, 1> blendBins[8];
	/**
	%
	 * offset 178
	 */
	scaled_channel<uint8_t, 2, 1> blendValues[8];
	/**
	 * need 4 byte alignment
	units
	 * offset 186
	 */
	uint8_t alignmentFill_at_186[2];
};
static_assert(sizeof(blend_table_s) == 188);

// start of persistent_config_s
struct persistent_config_s {
	/**
	 * offset 0
	 */
	engine_configuration_s engineConfiguration;
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	target TPS position
	 * offset 4004
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	ETB duty cycle bias
	 * offset 4036
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	%
	 * offset 4068
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	Load
	 * offset 4132
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	RPM
	 * offset 4140
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	RPM
	 * offset 4148
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	ms
	 * offset 4164
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * CLT-based target RPM for automatic idle controller
	C
	 * offset 4180
	 */
	scaled_channel<int8_t, 1, 2> cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	RPM
	 * offset 4196
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * CLT-based timing correction
	C
	 * offset 4212
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	degree
	 * offset 4244
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	x
	 * offset 4276
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 4340
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	x
	 * offset 4404
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 4468
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	x
	 * offset 4532
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4564
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4596
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4628
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4660
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4692
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4724
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4756
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	kPa
	 * offset 4788
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	RPM
	 * offset 4804
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	ratio
	 * offset 4820
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	Ratio
	 * offset 4884
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	%
	 * offset 4916
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	RPM
	 * offset 4948
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	deg
	 * offset 4956
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * RPM-based idle position for coasting
	RPM
	 * offset 4964
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE];
	/**
	 * RPM-based idle position for coasting
	%
	 * offset 4980
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 4996
	 */
	error_message_t warning_message;
	/**
	C
	 * offset 5116
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	Seconds
	 * offset 5148
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	%
	 * offset 5180
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	Seconds
	 * offset 5212
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 5244
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	RPM
	 * offset 5308
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 5316
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5380
	 */
	uint8_t boostTpsBins[BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5388
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	%
	 * offset 5452
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	RPM
	 * offset 5460
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	C
	 * offset 5468
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	%
	 * offset 5500
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	RPM
	 * offset 5532
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	deg
	 * offset 5540
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	RPM
	 * offset 5572
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	/**
	load
	 * offset 5576
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE];
	/**
	%
	 * offset 5580
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	/**
	 * offset 5612
	 */
	lua_script_t luaScript;
	/**
	C
	 * offset 13612
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	ratio
	 * offset 13676
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	C
	 * offset 13740
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 13804
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 13868
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	C
	 * offset 13900
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	ratio
	 * offset 13932
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	counter
	 * offset 13964
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	C
	 * offset 13996
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	ratio
	 * offset 14060
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	kg/hour
	 * offset 14124
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	V
	 * offset 15148
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	deg
	 * offset 16172
	 */
	scaled_channel<int8_t, 10, 1> ignitionIatCorrTable[8][8];
	/**
	C
	 * offset 16236
	 */
	int8_t ignitionIatCorrTempBins[8];
	/**
	Load
	 * offset 16244
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[8];
	/**
	deg
	 * offset 16252
	 */
	int16_t injectionPhase[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 16764
	 */
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 16796
	 */
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	onoff
	 * offset 16828
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	kPa
	 * offset 16888
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	% TPS
	 * offset 17400
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 17432
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 17464
	 */
	int8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 17528
	 */
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 17544
	 */
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 17560
	 */
	int8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 17624
	 */
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 17640
	 */
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	/**
	deg
	 * offset 17656
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 18168
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 18200
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	%
	 * offset 18232
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	kPa
	 * offset 18744
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18776
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	lambda
	 * offset 18808
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	 * offset 19064
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 19096
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 19128
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	from
	 * offset 19384
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	to
	 * offset 19416
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	value
	 * offset 19448
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19704
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19720
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 19736
	 */
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19800
	 */
	int16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19816
	 */
	int16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 19832
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19896
	 */
	int16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19912
	 */
	int16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 19928
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19992
	 */
	int16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20008
	 */
	int16_t scriptTable4RpmBins[SCRIPT_TABLE_8];
	/**
	 * offset 20024
	 */
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20032
	 */
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20040
	 */
	cyl_trim_s ignTrims[12];
	/**
	 * offset 20232
	 */
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20240
	 */
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20248
	 */
	cyl_trim_s fuelTrims[12];
	/**
	ratio
	 * offset 20440
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	/**
	Airmass
	 * offset 20456
	 */
	scaled_channel<uint8_t, 50, 1> tcu_pcAirmassBins[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20464
	 */
	uint8_t tcu_pcValsR[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20472
	 */
	uint8_t tcu_pcValsN[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20480
	 */
	uint8_t tcu_pcVals1[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20488
	 */
	uint8_t tcu_pcVals2[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20496
	 */
	uint8_t tcu_pcVals3[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20504
	 */
	uint8_t tcu_pcVals4[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20512
	 */
	uint8_t tcu_pcVals12[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20520
	 */
	uint8_t tcu_pcVals23[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20528
	 */
	uint8_t tcu_pcVals34[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20536
	 */
	uint8_t tcu_pcVals21[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20544
	 */
	uint8_t tcu_pcVals32[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20552
	 */
	uint8_t tcu_pcVals43[TCU_MAGIC_SIZE];
	/**
	TPS
	 * offset 20560
	 */
	uint8_t tcu_tccTpsBins[8];
	/**
	MPH
	 * offset 20568
	 */
	uint8_t tcu_tccLockSpeed[8];
	/**
	MPH
	 * offset 20576
	 */
	uint8_t tcu_tccUnlockSpeed[8];
	/**
	KPH
	 * offset 20584
	 */
	uint8_t tcu_32SpeedBins[8];
	/**
	%
	 * offset 20592
	 */
	uint8_t tcu_32Vals[8];
	/**
	%
	 * offset 20600
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[6][6];
	/**
	%
	 * offset 20636
	 */
	uint8_t throttle2TrimTpsBins[6];
	/**
	RPM
	 * offset 20642
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[6];
	/**
	deg
	 * offset 20648
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[6][6];
	/**
	%
	 * offset 20684
	 */
	uint8_t maxKnockRetardLoadBins[6];
	/**
	RPM
	 * offset 20690
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[6];
	/**
	deg
	 * offset 20696
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[4][4];
	/**
	TPS
	 * offset 20728
	 */
	uint16_t alsIgnRetardLoadBins[4];
	/**
	RPM
	 * offset 20736
	 */
	uint16_t alsIgnRetardrpmBins[4];
	/**
	percent
	 * offset 20744
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[4][4];
	/**
	TPS
	 * offset 20776
	 */
	uint16_t alsFuelAdjustmentLoadBins[4];
	/**
	RPM
	 * offset 20784
	 */
	uint16_t alsFuelAdjustmentrpmBins[4];
	/**
	 * offset 20792
	 */
	blend_table_s ignBlends[IGN_BLEND_COUNT];
	/**
	 * offset 21544
	 */
	blend_table_s veBlends[VE_BLEND_COUNT];
};
static_assert(sizeof(persistent_config_s) == 22296);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on gen_config.sh integration/rusefi_config.txt Tue Feb 21 11:00:58 PST 2023
