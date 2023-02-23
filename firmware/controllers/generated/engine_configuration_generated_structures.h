// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on gen_config.sh integration/rusefi_config.txt Thu Feb 23 15:07:49 PST 2023
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
	scaled_channel<int, 100, 1> tempC_1;
	/**
	*C
	 * offset 4
	 */
	scaled_channel<int, 100, 1> tempC_2;
	/**
	*C
	 * offset 8
	 */
	scaled_channel<int, 100, 1> tempC_3;
	/**
	Ohm
	 * offset 12
	 */
	scaled_channel<uint32_t, 100, 1> resistance_1;
	/**
	Ohm
	 * offset 16
	 */
	scaled_channel<uint32_t, 100, 1> resistance_2;
	/**
	Ohm
	 * offset 20
	 */
	scaled_channel<uint32_t, 100, 1> resistance_3;
	/**
	 * Pull-up resistor value on your board
	Ohm
	 * offset 24
	 */
	scaled_channel<uint32_t, 10, 1> bias_resistor;
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
	scaled_channel<uint16_t, 100, 1> battLagCorrBins[VBAT_INJECTOR_CURVE_SIZE];
	/**
	 * ms delay between injector open and close dead times
	ms
	 * offset 20
	 */
	scaled_channel<uint16_t, 100, 1> battLagCorr[VBAT_INJECTOR_CURVE_SIZE];
};
static_assert(sizeof(injector_s) == 36);

// start of specs_s
struct specs_s {
	/**
	 * Engine volume/capacity, in litres
	 * see also cylindersCount
	L
	 * offset 0
	 */
	scaled_channel<uint16_t, 1000, 1> displacement;
	/**
	 * need 4 byte alignment
	units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2];
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
	volts
	 * offset 2
	 */
	scaled_channel<uint16_t, 1000, 1> v1;
	/**
	AFR
	 * offset 4
	 */
	scaled_channel<uint16_t, 1000, 1> value1;
	/**
	volts
	 * offset 6
	 */
	scaled_channel<uint16_t, 1000, 1> v2;
	/**
	AFR
	 * offset 8
	 */
	scaled_channel<uint16_t, 1000, 1> value2;
	/**
	 * need 4 byte alignment
	units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2];
};
static_assert(sizeof(afr_sensor_s) == 12);

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
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * offset 50
	 */
	ignition_mode_e ignitionMode;
	/**
	 * offset 51
	 */
	can_nbc_e canNbcType;
	/**
	 * offset 52
	 */
	injector_s injector;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 88 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 88 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 88 bit 2 */
	bool isVerboseAuxPid1 : 1 {};
	/**
	offset 88 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 88 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 88 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 88 bit 6 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 88 bit 7 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 88 bit 8 */
	bool enableTrailingSparks : 1 {};
	/**
	 * TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 88 bit 9 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 88 bit 10 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 88 bit 11 */
	bool useEeprom : 1 {};
	/**
	offset 88 bit 12 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 88 bit 13 */
	bool kickStartCranking : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 88 bit 14 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 88 bit 15 */
	bool launchControlEnabled : 1 {};
	/**
	 * "Detect double trigger edges"
	offset 88 bit 16 */
	bool doNotFilterTriggerEdgeNoise : 1 {};
	/**
	offset 88 bit 17 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 88 bit 18 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	 * 
	 * enable logic_level_trigger
	offset 88 bit 19 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 88 bit 20 */
	bool useTLE8888_stepper : 1 {};
	/**
	offset 88 bit 21 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 88 bit 22 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 88 bit 23 */
	bool artificialTestMisfire : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 88 bit 24 */
	bool useFordRedundantPps : 1 {};
	/**
	offset 88 bit 25 */
	bool unusedBit_40_25 : 1 {};
	/**
	offset 88 bit 26 */
	bool unusedBit_40_26 : 1 {};
	/**
	offset 88 bit 27 */
	bool unusedBit_40_27 : 1 {};
	/**
	offset 88 bit 28 */
	bool unusedBit_40_28 : 1 {};
	/**
	offset 88 bit 29 */
	bool unusedBit_40_29 : 1 {};
	/**
	offset 88 bit 30 */
	bool unusedBit_40_30 : 1 {};
	/**
	offset 88 bit 31 */
	bool unusedBit_40_31 : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	ADC
	 * offset 92
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	ADC
	 * offset 94
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 96
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equiped.
	%
	 * offset 98
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 100
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	ms
	 * offset 108
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	rpm
	 * offset 112
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	rpm
	 * offset 114
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 116
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 256
	 */
	ThermistorConf clt;
	/**
	 * offset 288
	 */
	ThermistorConf iat;
	/**
	deg
	 * offset 320
	 */
	int launchTimingRetard;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	kHz
	 * offset 324
	 */
	float knockBandCustom;
	/**
	 * offset 328
	 */
	specs_s specs;
	/**
	 * Cylinder diameter in mm.
	mm
	 * offset 340
	 */
	float cylinderBore;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS commonly only used for NA engines
	 * Speed Density requires MAP sensor and is the default choice for may installs
	 * MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	 * offset 344
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	%
	 * offset 345
	 */
	uint8_t ALSMaxTPS;
	/**
	 * offset 346
	 */
	Gpio criticalLedPin;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 348
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups. If your injectors are individually wired you will also need to enable "Two wire batch emulation".
	 * offset 349
	 */
	injection_mode_e injectionMode;
	/**
	 * Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 350
	 */
	uint16_t boostControlMinRpm;
	/**
	 * Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 352
	 */
	uint8_t boostControlMinTps;
	/**
	 * Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 353
	 */
	uint8_t boostControlMinMap;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 354
	 */
	timing_mode_e timingMode;
	/**
	 * offset 355
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	deg
	 * offset 356
	 */
	angle_t crankingTimingAngle;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	count
	 * offset 360
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	kph
	 * offset 361
	 */
	uint8_t maxIdleVss;
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	kPa
	 * offset 362
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	RPM
	 * offset 364
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchnization point before TDC and negative in case of synchnization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	deg btdc
	 * offset 368
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	coef
	 * offset 372
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	coef
	 * offset 376
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 380
	 */
	uint8_t fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 381
	 */
	uint8_t fanOffTemperature;
	/**
	 * need 4 byte alignment
	units
	 * offset 382
	 */
	uint8_t alignmentFill_at_382[2];
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	revs/km
	 * offset 384
	 */
	float driveWheelRevPerKm;
	/**
	 * CANbus thread period in ms
	ms
	 * offset 388
	 */
	int canSleepPeriodMs;
	/**
	index
	 * offset 392
	 */
	int byFirmwareVersion;
	/**
	 * First throttle body, first sensor
	 * offset 396
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 397
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 398
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body, first sensor
	 * offset 399
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	x
	 * offset 400
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	angle
	 * offset 404
	 */
	int trailingSparkAngle;
	/**
	 * offset 408
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	%
	 * offset 420
	 */
	float airByRpmTaper;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	%
	 * offset 424
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * Throttle pedal position first channel
	 * offset 425
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * offset 426
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 427
	 */
	uint8_t alignmentFill_at_427[1];
	/**
	coef
	 * offset 428
	 */
	float globalFuelCorrection;
	/**
	volts
	 * offset 432
	 */
	float adcVcc;
	/**
	Deg
	 * offset 436
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 440
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 448
	 */
	afr_sensor_s afr;
	/**
	 * offset 460
	 */
	Gpio tle6240_cs;
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 462
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * @see hasBaroSensor
	 * offset 464
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 476
	 */
	idle_hardware_s idle;
	/**
	 * Value between 0 and 100 used in Manual mode
	%
	 * offset 488
	 */
	float manIdlePosition;
	/**
	 * Ignition timing to remove when a knock event occurs.
	%
	 * offset 492
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	deg/s
	 * offset 493
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 494
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 495
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	ratio
	 * offset 496
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	count
	 * offset 498
	 */
	uint8_t vssToothCount;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 499
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 500
	 */
	Gpio l9779_cs;
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 502
	 */
	output_pin_e o2heaterPin;
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * TODO: rename to triggerSimulatorRpm
	Rpm
	 * offset 504
	 */
	int triggerSimulatorFrequency;
	/**
	 * offset 508
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 532
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 556
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 557
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 558
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 560
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 561
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 562
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 564
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 565
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 566
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 568
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 570
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 572
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 573
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 574
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 575
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 576
	 */
	Gpio debugTriggerSync;
	/**
	 * offset 578
	 */
	Gpio mc33972_cs;
	/**
	 * offset 580
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 581
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 582
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 583
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	%
	 * offset 584
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * 0.1 is a good default value
	x
	 * offset 592
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 596
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	us
	 * offset 600
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * offset 602
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 604
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * offset 605
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * offset 606
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 608
	 */
	Gpio sdCardCsPin;
	/**
	 * set_can_tx_pin X
	 * offset 610
	 */
	Gpio canTxPin;
	/**
	 * set_can_rx_pin X
	 * offset 612
	 */
	Gpio canRxPin;
	/**
	 * offset 614
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * Additional idle % while A/C is active
	%
	 * offset 615
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * Ratio between the wheels and your transmission output.
	ratio
	 * offset 616
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 618
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 620
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * offset 621
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	mv
	 * offset 622
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 624
	 */
	uint16_t wastegatePositionMax;
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 626
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * Secondary TTL channel baud rate
	BPs
	 * offset 628
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	CR
	 * offset 632
	 */
	float compressionRatio;
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 636
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	g/s
	 * offset 640
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 642
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	offset 644 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 644 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 644 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 644 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 644 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 644 bit 5 */
	bool isVerboseAlternator : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 644 bit 6 */
	bool useStepperIdle : 1 {};
	/**
	offset 644 bit 7 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 644 bit 8 */
	bool unused760b12 : 1 {};
	/**
	offset 644 bit 9 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 644 bit 10 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 644 bit 11 */
	bool etb1configured : 1 {};
	/**
	offset 644 bit 12 */
	bool etb2configured : 1 {};
	/**
	 * Useful for individual intakes
	offset 644 bit 13 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 644 bit 14 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 644 bit 15 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 644 bit 16 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 644 bit 17 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 644 bit 18 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 644 bit 19 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 644 bit 20 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 644 bit 21 */
	bool pauseEtbControl : 1 {};
	/**
	offset 644 bit 22 */
	bool alignEngineSnifferAtTDC : 1 {};
	/**
	offset 644 bit 23 */
	bool unused764b29 : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 644 bit 24 */
	bool enableAemXSeries : 1 {};
	/**
	offset 644 bit 25 */
	bool unusedBit_191_25 : 1 {};
	/**
	offset 644 bit 26 */
	bool unusedBit_191_26 : 1 {};
	/**
	offset 644 bit 27 */
	bool unusedBit_191_27 : 1 {};
	/**
	offset 644 bit 28 */
	bool unusedBit_191_28 : 1 {};
	/**
	offset 644 bit 29 */
	bool unusedBit_191_29 : 1 {};
	/**
	offset 644 bit 30 */
	bool unusedBit_191_30 : 1 {};
	/**
	offset 644 bit 31 */
	bool unusedBit_191_31 : 1 {};
	/**
	 * offset 648
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 656
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	v
	 * offset 660
	 */
	uint8_t mc33_hvolt;
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	kPa
	 * offset 661
	 */
	uint8_t minimumBoostClosedLoopMap;
	/**
	 * Optional Radiator Fan used with A/C
	 * offset 662
	 */
	output_pin_e acFanPin;
	/**
	 * offset 664
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * offset 665
	 */
	spi_device_e l9779spiDevice;
	/**
	volts
	 * offset 666
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	multiplier
	 * offset 674
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	kg
	 * offset 682
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling?
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	RPM
	 * offset 684
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	ms
	 * offset 686
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 688
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 690
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 692
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 694
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 696
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 697
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 698
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 714
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * offset 716
	 */
	Gpio test557pin;
	/**
	 * offset 718
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 719
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	:1
	 * offset 720
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	%
	 * offset 721
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	hz
	 * offset 722
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 724
	 */
	Gpio debugMapAveraging;
	/**
	 * offset 726
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 728
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * offset 729
	 */
	imu_type_e imuType;
	/**
	 * See also starterControlPin
	 * offset 730
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	count
	 * offset 732
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	%
	 * offset 736
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	%
	 * offset 738
	 */
	int16_t stepperParkingExtraSteps;
	/**
	ADC
	 * offset 740
	 */
	uint16_t tps1SecondaryMin;
	/**
	ADC
	 * offset 742
	 */
	uint16_t tps1SecondaryMax;
	/**
	rpm
	 * offset 744
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	Seconds
	 * offset 746
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 748
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 750
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * offset 751
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * offset 752
	 */
	output_pin_e acRelayPin;
	/**
	 * offset 754
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * offset 755
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * offset 756
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 788
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 790
	 */
	Gpio spi1misoPin;
	/**
	 * offset 792
	 */
	Gpio spi1sckPin;
	/**
	 * offset 794
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 796
	 */
	Gpio spi2misoPin;
	/**
	 * offset 798
	 */
	Gpio spi2sckPin;
	/**
	 * offset 800
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 802
	 */
	Gpio spi3misoPin;
	/**
	 * offset 804
	 */
	Gpio spi3sckPin;
	/**
	 * offset 806
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEFI console Sensor Sniffer mode
	 * offset 807
	 */
	sensor_chart_e sensorChartMode;
	/**
	offset 808 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 808 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 808 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 808 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 808 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 808 bit 5 */
	bool enableCanVss : 1 {};
	/**
	offset 808 bit 6 */
	bool enableInnovateLC2 : 1 {};
	/**
	offset 808 bit 7 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 808 bit 8 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 808 bit 9 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 808 bit 10 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	 * enable vvt_details
	offset 808 bit 11 */
	bool verboseVVTDecoding : 1 {};
	/**
	 * get invertCamVVTSignal
	offset 808 bit 12 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	 * enable consumeObdSensors
	offset 808 bit 13 */
	bool consumeObdSensors : 1 {};
	/**
	offset 808 bit 14 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 808 bit 15 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 808 bit 16 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 808 bit 17 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 808 bit 18 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 808 bit 19 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 808 bit 20 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 808 bit 21 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 808 bit 22 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 808 bit 23 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 808 bit 24 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 808 bit 25 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 808 bit 26 */
	bool tcuEnabled : 1 {};
	/**
	offset 808 bit 27 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coilss
	offset 808 bit 28 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 808 bit 29 */
	bool verboseCan2 : 1 {};
	/**
	offset 808 bit 30 */
	bool unusedBit_284_30 : 1 {};
	/**
	offset 808 bit 31 */
	bool unusedBit_284_31 : 1 {};
	/**
	 * offset 812
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 828
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 830
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * offset 831
	 */
	boostType_e boostType;
	/**
	 * offset 832
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 834
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 836
	 */
	pid_s boostPid;
	/**
	Hz
	 * offset 856
	 */
	int boostPwmFrequency;
	/**
	 * offset 860
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 861
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * offset 862
	 */
	Gpio mc33816_flag0;
	/**
	 * Disabled above this speed
	Kph
	 * offset 864
	 */
	int launchSpeedThreshold;
	/**
	 * Range from Launch RPM for Timing Retard to activate
	RPM
	 * offset 868
	 */
	int launchTimingRpmRange;
	/**
	 * Extra Fuel Added
	%
	 * offset 872
	 */
	int launchFuelAdded;
	/**
	 * Duty Cycle for the Boost Solenoid
	%
	 * offset 876
	 */
	int launchBoostDuty;
	/**
	 * Range from Launch RPM to activate Hard Cut
	RPM
	 * offset 880
	 */
	int hardCutRpmRange;
	/**
	 * offset 884
	 */
	float turbochargerFilter;
	/**
	 * offset 888
	 */
	int launchTpsThreshold;
	/**
	 * offset 892
	 */
	float launchActivateDelay;
	/**
	 * offset 896
	 */
	stft_s stft;
	/**
	 * offset 920
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 936
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 968
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1000
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1032
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * offset 1044
	 */
	dc_function_e etbFunctions[ETB_COUNT];
	/**
	 * offset 1046
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1048
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * See also idleRpmPid
	 * offset 1049
	 */
	idle_mode_e idleMode;
	/**
	 * offset 1050
	 */
	Gpio drv8860_miso;
	/**
	volt
	 * offset 1052
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * offset 1068
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * Angle between cam sensor and VVT zero position
	value
	 * offset 1084
	 */
	scaled_channel<int16_t, 10, 1> vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1092
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1124
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	ADC
	 * offset 1188
	 */
	uint16_t tps2SecondaryMin;
	/**
	ADC
	 * offset 1190
	 */
	uint16_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1192 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1192 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Print details into rusEFI console
	 * enable verbose_idle
	offset 1192 bit 2 */
	bool isVerboseIAC : 1 {};
	/**
	offset 1192 bit 3 */
	bool boardUseTachPullUp : 1 {};
	/**
	offset 1192 bit 4 */
	bool boardUseTempPullUp : 1 {};
	/**
	offset 1192 bit 5 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1192 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1192 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1192 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write
	offset 1192 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1192 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1192 bit 11 */
	bool boardUse2stepPullDown : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1192 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1192 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1192 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1192 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1192 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1192 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1192 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1192 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1192 bit 20 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 1192 bit 21 */
	bool boardUseCamPullDown : 1 {};
	/**
	offset 1192 bit 22 */
	bool boardUseCamVrPullUp : 1 {};
	/**
	offset 1192 bit 23 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 1192 bit 24 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 1192 bit 25 */
	bool boardUseD4PullDown : 1 {};
	/**
	offset 1192 bit 26 */
	bool boardUseD5PullDown : 1 {};
	/**
	offset 1192 bit 27 */
	bool verboseIsoTp : 1 {};
	/**
	offset 1192 bit 28 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1192 bit 29 */
	bool launchActivateInverted : 1 {};
	/**
	offset 1192 bit 30 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1192 bit 31 */
	bool skippedWheelOnCam : 1 {};
	/**
	 * A/C button input
	 * offset 1196
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1198
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	%
	 * offset 1199
	 */
	uint8_t etbNeutralPosition;
	/**
	offset 1200 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1200 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1200 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1200 bit 3 */
	bool complexWallModel : 1 {};
	/**
	offset 1200 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1200 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1200 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1200 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1200 bit 8 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1200 bit 9 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	 * enable trigger_details
	offset 1200 bit 10 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1200 bit 11 */
	bool isManualSpinningMode : 1 {};
	/**
	 * This is needed if your coils are individually wired and you wish to use batch injection.
	 * enable two_wire_batch_injection
	offset 1200 bit 12 */
	bool twoWireBatchInjection : 1 {};
	/**
	offset 1200 bit 13 */
	bool neverInstantRpm : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1200 bit 14 */
	bool twoWireBatchIgnition : 1 {};
	/**
	offset 1200 bit 15 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1200 bit 16 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1200 bit 17 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1200 bit 18 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1200 bit 19 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1200 bit 20 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1200 bit 21 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1200 bit 22 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1200 bit 23 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors. WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1200 bit 24 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1200 bit 25 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1200 bit 26 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1200 bit 27 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	offset 1200 bit 28 */
	bool sdTriggerLog : 1 {};
	/**
	offset 1200 bit 29 */
	bool ALSActivateInverted : 1 {};
	/**
	offset 1200 bit 30 */
	bool unused_1484_bit_30 : 1 {};
	/**
	offset 1200 bit 31 */
	bool tempBooleanForVerySpecialLogic : 1 {};
	/**
	count
	 * offset 1204
	 */
	uint32_t engineChartSize;
	/**
	mult
	 * offset 1208
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1212
	 */
	Gpio camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	RPM
	 * offset 1220
	 */
	int16_t acIdleRpmBump;
	/**
	seconds
	 * offset 1222
	 */
	int16_t warningPeriod;
	/**
	angle
	 * offset 1224
	 */
	float knockDetectionWindowStart;
	/**
	angle
	 * offset 1228
	 */
	float knockDetectionWindowEnd;
	/**
	ms
	 * offset 1232
	 */
	float idleStepperReactionTime;
	/**
	count
	 * offset 1236
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	sec
	 * offset 1240
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	index
	 * offset 1244
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1248
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	Seconds
	 * offset 1252
	 */
	float wwaeTau;
	/**
	 * offset 1256
	 */
	pid_s alternatorControl;
	/**
	 * offset 1276
	 */
	pid_s etb;
	/**
	 * offset 1296
	 */
	Gpio triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * RPM range above upper limit for extra air taper,"RPM", 1, 0, 0, 1500, 0
	 * offset 1300
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1302
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	ADC
	 * offset 1304
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	ADC
	 * offset 1306
	 */
	int16_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1308
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1310
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	Pulse
	 * offset 1311
	 */
	uint8_t tachPulsePerRev;
	/**
	 * kPa value which is too low to be true
	kPa
	 * offset 1312
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	kPa
	 * offset 1316
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	ms
	 * offset 1320
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	ms
	 * offset 1322
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1324
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	Fraction
	 * offset 1344
	 */
	float wwaeBeta;
	/**
	 * blue LED on many rusEFI boards.
	 * Blue Communication LED which is expected to blink at 50% duty cycle during normal board operation.
	 * If USB communication cable is connected Blue LED starts to blink faster.
	 * offset 1348
	 */
	Gpio communicationLedPin;
	/**
	 * Green LED on many rusEFI boards.
	 * Off if engine is stopped, blinks if engine is cranking, solid if engine is running.
	 * offset 1350
	 */
	Gpio runningLedPin;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1352
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1354
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1356
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * offset 1360
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1362
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	voltage
	 * offset 1364
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1368
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	seconds
	 * offset 1372
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	RPM
	 * offset 1374
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	Volts
	 * offset 1376
	 */
	float targetVBatt;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	%
	 * offset 1380
	 */
	float alternatorOffAboveTps;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	cycles
	 * offset 1384
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	percent
	 * offset 1386
	 */
	int16_t iacByTpsTaper;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * offset 1388
	 */
	Gpio auxSerialTxPin;
	/**
	 * offset 1390
	 */
	Gpio warningLedPin;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * offset 1392
	 */
	Gpio auxSerialRxPin;
	/**
	 * offset 1394
	 */
	Gpio LIS302DLCsPin;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	sec
	 * offset 1396
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 1397
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 1398
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	sec
	 * offset 1399
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	roc
	 * offset 1400
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1404
	 */
	brain_input_pin_e auxSpeedSensorInputPin[2];
	/**
	 * offset 1408
	 */
	uint8_t totalGearsCount;
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 1409
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1410
	 */
	debug_mode_e debugMode;
	/**
	 * Additional idle % when fan #1 is active
	%
	 * offset 1411
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * Band rate for primary TTL
	BPs
	 * offset 1412
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	roc
	 * offset 1416
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	coeff
	 * offset 1420
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	coeff
	 * offset 1424
	 */
	float slowAdcAlpha;
	/**
	BPs
	 * offset 1428
	 */
	uint32_t auxSerialSpeed;
	/**
	voltage
	 * offset 1432
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 1436
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * set can_baudrate
	 * offset 1440
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1441
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1442
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1443
	 */
	load_override_e afrOverrideMode;
	/**
	A
	 * offset 1444
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	A
	 * offset 1445
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	us
	 * offset 1446
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	ms
	 * offset 1447
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1448 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Allow OpenBLT on Primary CAN
	offset 1448 bit 1 */
	bool canOpenBLT : 1 {};
	/**
	 * Allow OpenBLT on Secondary CAN
	offset 1448 bit 2 */
	bool can2OpenBLT : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (defualt, cc/min) or mass flow (g/s).
	offset 1448 bit 3 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1448 bit 4 */
	bool unused1127 : 1 {};
	/**
	offset 1448 bit 5 */
	bool unused1128 : 1 {};
	/**
	offset 1448 bit 6 */
	bool unused1129 : 1 {};
	/**
	offset 1448 bit 7 */
	bool unused1130 : 1 {};
	/**
	offset 1448 bit 8 */
	bool unusedBit_471_8 : 1 {};
	/**
	offset 1448 bit 9 */
	bool unusedBit_471_9 : 1 {};
	/**
	offset 1448 bit 10 */
	bool unusedBit_471_10 : 1 {};
	/**
	offset 1448 bit 11 */
	bool unusedBit_471_11 : 1 {};
	/**
	offset 1448 bit 12 */
	bool unusedBit_471_12 : 1 {};
	/**
	offset 1448 bit 13 */
	bool unusedBit_471_13 : 1 {};
	/**
	offset 1448 bit 14 */
	bool unusedBit_471_14 : 1 {};
	/**
	offset 1448 bit 15 */
	bool unusedBit_471_15 : 1 {};
	/**
	offset 1448 bit 16 */
	bool unusedBit_471_16 : 1 {};
	/**
	offset 1448 bit 17 */
	bool unusedBit_471_17 : 1 {};
	/**
	offset 1448 bit 18 */
	bool unusedBit_471_18 : 1 {};
	/**
	offset 1448 bit 19 */
	bool unusedBit_471_19 : 1 {};
	/**
	offset 1448 bit 20 */
	bool unusedBit_471_20 : 1 {};
	/**
	offset 1448 bit 21 */
	bool unusedBit_471_21 : 1 {};
	/**
	offset 1448 bit 22 */
	bool unusedBit_471_22 : 1 {};
	/**
	offset 1448 bit 23 */
	bool unusedBit_471_23 : 1 {};
	/**
	offset 1448 bit 24 */
	bool unusedBit_471_24 : 1 {};
	/**
	offset 1448 bit 25 */
	bool unusedBit_471_25 : 1 {};
	/**
	offset 1448 bit 26 */
	bool unusedBit_471_26 : 1 {};
	/**
	offset 1448 bit 27 */
	bool unusedBit_471_27 : 1 {};
	/**
	offset 1448 bit 28 */
	bool unusedBit_471_28 : 1 {};
	/**
	offset 1448 bit 29 */
	bool unusedBit_471_29 : 1 {};
	/**
	offset 1448 bit 30 */
	bool unusedBit_471_30 : 1 {};
	/**
	offset 1448 bit 31 */
	bool unusedBit_471_31 : 1 {};
	/**
	 * Time between bench test pulses
	ms
	 * offset 1452
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOffTime;
	/**
	 * How many test bench pulses do you want
	count
	 * offset 1453
	 */
	uint8_t benchTestCount;
	/**
	 * Length of each of the test pulses
	ms
	 * offset 1454
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOnTime;
	/**
	 * offset 1455
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * set_can2_tx_pin X
	 * offset 1456
	 */
	Gpio can2TxPin;
	/**
	 * set_can2_rx_pin X
	 * offset 1458
	 */
	Gpio can2RxPin;
	/**
	 * offset 1460
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1461
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1462
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1463
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1464
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1466
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * offset 1467
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	kPa (absolute)
	 * offset 1468
	 */
	float boostCutPressure;
	/**
	kg/h
	 * offset 1472
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16];
	/**
	ratio
	 * offset 1488
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16];
	/**
	 * Fixed timing, useful for TDC testing
	deg
	 * offset 1504
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	v
	 * offset 1508
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	v
	 * offset 1512
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	value
	 * offset 1516
	 */
	float egoValueShift;
	/**
	 * VVT output
	 * TODO: rename to vvtOutputs
	 * offset 1520
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	percent
	 * offset 1528
	 */
	int crankingIACposition;
	/**
	 * offset 1532
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 1536
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 1540
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 1544
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 1548
	 */
	pwm_freq_t vvtOutputFrequency[CAMS_PER_BANK];
	/**
	Hz
	 * offset 1552
	 */
	int alternatorPwmFrequency;
	/**
	 * offset 1556
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #2 is active
	%
	 * offset 1558
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	sec
	 * offset 1559
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1560
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT];
	/**
	 * offset 1568
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 1592
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1593
	 */
	pin_output_mode_e LIS302DLCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 1594
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 1595
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	kPa
	 * offset 1596
	 */
	float fuelReferencePressure;
	/**
	 * Fuel multiplier (enrichment) immediately after engine start
	mult
	 * offset 1600
	 */
	float postCrankingFactor;
	/**
	 * Time over which to taper out after start enrichment
	seconds
	 * offset 1604
	 */
	float postCrankingDurationSec;
	/**
	 * offset 1608
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1640
	 */
	ThermistorConf auxTempSensor2;
	/**
	Deg
	 * offset 1672
	 */
	int16_t knockSamplingDuration;
	/**
	Hz
	 * offset 1674
	 */
	int16_t etbFreq;
	/**
	 * offset 1676
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 1696
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	%
	 * offset 1697
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	%
	 * offset 1698
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 1699
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder timing correction
	deg
	 * offset 1700
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	seconds
	 * offset 1748
	 */
	float idlePidActivationTime;
	/**
	 * offset 1752
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 1753
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 1754
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 1755
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 1756
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 1757
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 1758
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 1759
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 1760
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 1761
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 1762
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 1764
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 1766
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * offset 1768
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 1769
	 */
	tChargeMode_e tChargeMode;
	/**
	 * offset 1770
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 1771
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 1772
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 1812
	 */
	float injectorCorrectionPolynomial[8];
	/**
	C
	 * offset 1844
	 */
	int8_t primeBins[8];
	/**
	 * offset 1852
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 1872
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 1874
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 1875
	 */
	uint8_t fan2OffTemperature;
	/**
	 * offset 1876
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 1878
	 */
	Gpio tle8888_cs;
	/**
	 * offset 1880
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * offset 1881
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * offset 1882
	 */
	Gpio mc33816_cs;
	/**
	 * offset 1884
	 */
	float auxFrequencyFilter;
	/**
	RPM
	 * offset 1888
	 */
	int16_t vvtControlMinRpm;
	/**
	 * offset 1890
	 */
	brain_input_pin_e sentInputPins[SENT_INPUT_COUNT];
	/**
	 * offset 1892
	 */
	int8_t launchFuelAdderPercent;
	/**
	 * Time required to detect a stuck throttle.
	sec
	 * offset 1893
	 */
	scaled_channel<uint8_t, 50, 1> etbJamTimeout;
	/**
	 * By the way ETB PID runs at 500hz, length in 1/500 of second here.
	 * offset 1894
	 */
	uint16_t etbExpAverageLength;
	/**
	 * This sets the RPM above which fuel cut is active.
	rpm
	 * offset 1896
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	rpm
	 * offset 1898
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	%
	 * offset 1900
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	C
	 * offset 1902
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	%
	 * offset 1904
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	kPa
	 * offset 1906
	 */
	int16_t coastingFuelCutMap;
	/**
	 * offset 1908
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 1928
	 */
	linear_sensor_s lowPressureFuel;
	/**
	C
	 * offset 1948
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	RPM
	 * offset 1952
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * offset 1960
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	 * offset 2056
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 2120
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2248
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2252
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	kg/h
	 * offset 2256
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	deg/sec
	 * offset 2260
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	deg/sec
	 * offset 2264
	 */
	float tChargeAirDecrLimit;
	/**
	 * iTerm min value
	 * offset 2268
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2270
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2272
	 */
	pid_s idleTimingPid;
	/**
	 * By the way ETB PID runs at 500hz, length in 1/500 of second here.
	 * offset 2292
	 */
	int16_t etbRocExpAverageLength;
	/**
	 * A delay in cycles between fuel-enrich. portions
	cycles
	 * offset 2294
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	coef
	 * offset 2296
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2300
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2301
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2302
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2304
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	:1
	 * offset 2305
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2306
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	%
	 * offset 2308
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2312
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	mg
	 * offset 2324
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[8];
	/**
	 * Trigger comparator center point voltage
	V
	 * offset 2332
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	V
	 * offset 2333
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	V
	 * offset 2334
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	RPM
	 * offset 2335
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * offset 2336
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	 * Boost Current
	mA
	 * offset 2768
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	mA
	 * offset 2770
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	mA
	 * offset 2772
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	us
	 * offset 2774
	 */
	uint16_t mc33_t_max_boost;
	/**
	us
	 * offset 2776
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	us
	 * offset 2778
	 */
	uint16_t mc33_t_peak_tot;
	/**
	us
	 * offset 2780
	 */
	uint16_t mc33_t_bypass;
	/**
	us
	 * offset 2782
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	us
	 * offset 2784
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 2786
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	dB
	 * offset 2792
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	ratio
	 * offset 2808
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	ratio
	 * offset 2880
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	rpm
	 * offset 2952
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	sec
	 * offset 2953
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	sec
	 * offset 2954
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * offset 2955
	 */
	pin_input_mode_e acSwitchMode;
	/**
	mg
	 * offset 2956
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	multiplier
	 * offset 2958
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	/**
	%
	 * offset 2962
	 */
	uint8_t etbJamIntegratorLimit;
	/**
	lobes/cam
	 * offset 2963
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 2964
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Crank angle ATDC of first lobe peak
	deg
	 * offset 2965
	 */
	uint8_t hpfpPeakPos;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	deg
	 * offset 2966
	 */
	uint8_t hpfpMinAngle;
	/**
	 * offset 2967
	 */
	vin_number_t vinNumber;
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	cc
	 * offset 2984
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	deg
	 * offset 2986
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 2987
	 */
	uint8_t issFilterReciprocal;
	/**
	%/kPa
	 * offset 2988
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	%/kPa/lobe
	 * offset 2990
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	kPa/s
	 * offset 2992
	 */
	uint16_t hpfpTargetDecay;
	/**
	%
	 * offset 2994
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	deg
	 * offset 3010
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	volts
	 * offset 3026
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	ms
	 * offset 3034
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	kPa
	 * offset 3050
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	load
	 * offset 3250
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	RPM
	 * offset 3270
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	%
	 * offset 3280
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	cc/lobe
	 * offset 3380
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	RPM
	 * offset 3400
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	 * offset 3410
	 */
	output_pin_e stepper_raw_output[4];
	/**
	ratio
	 * offset 3418
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[GEARS_COUNT];
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	ms
	 * offset 3434
	 */
	uint16_t vvtActivationDelayMs;
	/**
	deg C
	 * offset 3436
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	/**
	 * offset 3444
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3452
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	/**
	kPa
	 * offset 3460
	 */
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	/**
	 * offset 3468
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3476
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3484
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3485
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3486
	 */
	uint8_t alignmentFill_at_3486[2];
	/**
	 * offset 3488
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3508
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 3528
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 3530
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * offset 3531
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * offset 3532
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 3534
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 3536
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 3538
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * offset 3539
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * offset 3540
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 3542
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 3544
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * need 4 byte alignment
	units
	 * offset 3546
	 */
	uint8_t alignmentFill_at_3546[2];
	/**
	%
	 * offset 3548
	 */
	float etbMinimumPosition;
	/**
	 * offset 3552
	 */
	uint16_t tuneHidingKey;
	/**
	 * offset 3554
	 */
	uint16_t highSpeedOffsets[HIGH_SPEED_COUNT];
	/**
	 * offset 3618
	 */
	SentEtbType sentEtbType;
	/**
	 * need 4 byte alignment
	units
	 * offset 3619
	 */
	uint8_t alignmentFill_at_3619[1];
	/**
	 * offset 3620
	 */
	Gpio luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT];
	/**
	RPM
	 * offset 3636
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	/**
	rpm
	 * offset 3640
	 */
	int16_t ALSMinRPM;
	/**
	rpm
	 * offset 3642
	 */
	int16_t ALSMaxRPM;
	/**
	sec
	 * offset 3644
	 */
	int16_t ALSMaxDuration;
	/**
	C
	 * offset 3646
	 */
	int8_t ALSMinCLT;
	/**
	C
	 * offset 3647
	 */
	int8_t ALSMaxCLT;
	/**
	 * offset 3648
	 */
	uint8_t alsMinTimeBetween;
	/**
	 * offset 3649
	 */
	uint8_t alsEtbPosition;
	/**
	 * offset 3650
	 */
	uint8_t acRelayAlternatorDutyAdder;
	/**
	deg
	 * offset 3651
	 */
	uint8_t instantRpmRange;
	/**
	%
	 * offset 3652
	 */
	int ALSIdleAdd;
	/**
	%
	 * offset 3656
	 */
	int ALSEtbAdd;
	/**
	 * offset 3660
	 */
	int ALSSkipRatio;
	/**
	%
	 * offset 3664
	 */
	uint8_t ALSMaxDriverThrottleIntent;
	/**
	 * offset 3665
	 */
	pin_input_mode_e ALSActivatePinMode;
	/**
	 * For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	%
	 * offset 3666
	 */
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	/**
	 * For Toyota ETCS-i, use ~69%
	%
	 * offset 3667
	 */
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	/**
	 * offset 3668
	 */
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT];
	/**
	units
	 * offset 3676
	 */
	uint8_t mainUnusedEnd[96];
};
static_assert(sizeof(engine_configuration_s) == 3772);

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
	 * offset 3772
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	ETB duty cycle bias
	 * offset 3804
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	%
	 * offset 3836
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	Load
	 * offset 3900
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	RPM
	 * offset 3908
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	RPM
	 * offset 3916
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	ms
	 * offset 3932
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * CLT-based target RPM for automatic idle controller
	C
	 * offset 3948
	 */
	scaled_channel<int8_t, 1, 2> cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	RPM
	 * offset 3964
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * CLT-based timing correction
	C
	 * offset 3980
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	degree
	 * offset 4012
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	x
	 * offset 4044
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 4108
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	x
	 * offset 4172
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 4236
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	x
	 * offset 4300
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4332
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4364
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4396
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4428
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4460
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	x
	 * offset 4492
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 4524
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	kPa
	 * offset 4556
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	RPM
	 * offset 4572
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	ratio
	 * offset 4588
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	Ratio
	 * offset 4652
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	%
	 * offset 4684
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	RPM
	 * offset 4716
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	deg
	 * offset 4724
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * RPM-based idle position for coasting
	RPM
	 * offset 4732
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE];
	/**
	 * RPM-based idle position for coasting
	%
	 * offset 4748
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 4764
	 */
	error_message_t warning_message;
	/**
	C
	 * offset 4884
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	Seconds
	 * offset 4916
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	%
	 * offset 4948
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	Seconds
	 * offset 4980
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 5012
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	RPM
	 * offset 5076
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 5084
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5148
	 */
	uint8_t boostTpsBins[BOOST_LOAD_COUNT];
	/**
	%
	 * offset 5156
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	%
	 * offset 5220
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	RPM
	 * offset 5228
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	C
	 * offset 5236
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	%
	 * offset 5268
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	RPM
	 * offset 5300
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	deg
	 * offset 5308
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	RPM
	 * offset 5340
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	/**
	load
	 * offset 5344
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE];
	/**
	%
	 * offset 5348
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	/**
	 * offset 5380
	 */
	lua_script_t luaScript;
	/**
	C
	 * offset 13380
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	ratio
	 * offset 13444
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	C
	 * offset 13508
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 13572
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 13636
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	C
	 * offset 13668
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	ratio
	 * offset 13700
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	counter
	 * offset 13732
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	C
	 * offset 13764
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	ratio
	 * offset 13828
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	kg/hour
	 * offset 13892
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	V
	 * offset 14916
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	deg
	 * offset 15940
	 */
	scaled_channel<int8_t, 10, 1> ignitionIatCorrTable[8][8];
	/**
	C
	 * offset 16004
	 */
	int8_t ignitionIatCorrTempBins[8];
	/**
	Load
	 * offset 16012
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[8];
	/**
	deg
	 * offset 16020
	 */
	int16_t injectionPhase[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 16532
	 */
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 16564
	 */
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	onoff
	 * offset 16596
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	kPa
	 * offset 16656
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	% TPS
	 * offset 17168
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 17200
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 17232
	 */
	int8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 17296
	 */
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 17312
	 */
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 17328
	 */
	int8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 17392
	 */
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 17408
	 */
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	/**
	deg
	 * offset 17424
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 17936
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 17968
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	%
	 * offset 18000
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	kPa
	 * offset 18512
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18544
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	lambda
	 * offset 18576
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	 * offset 18832
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18864
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 18896
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	from
	 * offset 19152
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	to
	 * offset 19184
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	value
	 * offset 19216
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19472
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19488
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 19504
	 */
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19568
	 */
	int16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19584
	 */
	int16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 19600
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19664
	 */
	int16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19680
	 */
	int16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 19696
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 19760
	 */
	int16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 19776
	 */
	int16_t scriptTable4RpmBins[SCRIPT_TABLE_8];
	/**
	 * offset 19792
	 */
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 19800
	 */
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 19808
	 */
	cyl_trim_s ignTrims[12];
	/**
	 * offset 20000
	 */
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 20008
	 */
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 20016
	 */
	cyl_trim_s fuelTrims[12];
	/**
	ratio
	 * offset 20208
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	/**
	Airmass
	 * offset 20224
	 */
	scaled_channel<uint8_t, 50, 1> tcu_pcAirmassBins[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20232
	 */
	uint8_t tcu_pcValsR[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20240
	 */
	uint8_t tcu_pcValsN[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20248
	 */
	uint8_t tcu_pcVals1[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20256
	 */
	uint8_t tcu_pcVals2[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20264
	 */
	uint8_t tcu_pcVals3[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20272
	 */
	uint8_t tcu_pcVals4[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20280
	 */
	uint8_t tcu_pcVals12[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20288
	 */
	uint8_t tcu_pcVals23[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20296
	 */
	uint8_t tcu_pcVals34[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20304
	 */
	uint8_t tcu_pcVals21[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20312
	 */
	uint8_t tcu_pcVals32[TCU_MAGIC_SIZE];
	/**
	%
	 * offset 20320
	 */
	uint8_t tcu_pcVals43[TCU_MAGIC_SIZE];
	/**
	TPS
	 * offset 20328
	 */
	uint8_t tcu_tccTpsBins[8];
	/**
	MPH
	 * offset 20336
	 */
	uint8_t tcu_tccLockSpeed[8];
	/**
	MPH
	 * offset 20344
	 */
	uint8_t tcu_tccUnlockSpeed[8];
	/**
	KPH
	 * offset 20352
	 */
	uint8_t tcu_32SpeedBins[8];
	/**
	%
	 * offset 20360
	 */
	uint8_t tcu_32Vals[8];
	/**
	%
	 * offset 20368
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[6][6];
	/**
	%
	 * offset 20404
	 */
	uint8_t throttle2TrimTpsBins[6];
	/**
	RPM
	 * offset 20410
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[6];
	/**
	deg
	 * offset 20416
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[6][6];
	/**
	%
	 * offset 20452
	 */
	uint8_t maxKnockRetardLoadBins[6];
	/**
	RPM
	 * offset 20458
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[6];
	/**
	deg
	 * offset 20464
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[4][4];
	/**
	TPS
	 * offset 20496
	 */
	uint16_t alsIgnRetardLoadBins[4];
	/**
	RPM
	 * offset 20504
	 */
	uint16_t alsIgnRetardrpmBins[4];
	/**
	percent
	 * offset 20512
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[4][4];
	/**
	TPS
	 * offset 20544
	 */
	uint16_t alsFuelAdjustmentLoadBins[4];
	/**
	RPM
	 * offset 20552
	 */
	uint16_t alsFuelAdjustmentrpmBins[4];
	/**
	 * offset 20560
	 */
	blend_table_s ignBlends[IGN_BLEND_COUNT];
	/**
	 * offset 21312
	 */
	blend_table_s veBlends[VE_BLEND_COUNT];
};
static_assert(sizeof(persistent_config_s) == 22064);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on gen_config.sh integration/rusefi_config.txt Thu Feb 23 15:07:49 PST 2023
