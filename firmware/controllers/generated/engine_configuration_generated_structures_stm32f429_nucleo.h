// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt Tue Feb 27 01:11:06 UTC 2024
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
	int8_t maxAdd;
	/**
	 * Maximum % that the short term fuel trim can remove
	 * units: %
	 * offset 1
	 */
	int8_t maxRemove;
	/**
	 * Time constant for correction while in this cell: this sets responsiveness of the closed loop correction. A value of 5.0 means it will try to make most of the correction within 5 seconds, and a value of 1.0 will try to correct within 1 second.
	 * units: sec
	 * offset 2
	 */
	scaled_channel<uint16_t, 10, 1> timeConstant;
};
static_assert(sizeof(stft_cell_cfg_s) == 4);

// start of stft_s
struct stft_s {
	/**
	 * Below this RPM, the idle region is active
	 * units: RPM
	 * offset 0
	 */
	scaled_channel<uint8_t, 1, 50> maxIdleRegionRpm;
	/**
	 * Below this engine load, the overrun region is active
	 * units: load
	 * offset 1
	 */
	uint8_t maxOverrunLoad;
	/**
	 * Above this engine load, the power region is active
	 * units: load
	 * offset 2
	 */
	uint8_t minPowerLoad;
	/**
	 * When close to correct AFR, pause correction. This can improve stability by not changing the adjustment if the error is extremely small, but is not required.
	 * units: %
	 * offset 3
	 */
	scaled_channel<uint8_t, 10, 1> deadband;
	/**
	 * Below this temperature, correction is disabled.
	 * units: C
	 * offset 4
	 */
	int8_t minClt;
	/**
	 * Below this AFR, correction is paused
	 * units: afr
	 * offset 5
	 */
	scaled_channel<uint8_t, 10, 1> minAfr;
	/**
	 * Above this AFR, correction is paused
	 * units: afr
	 * offset 6
	 */
	scaled_channel<uint8_t, 10, 1> maxAfr;
	/**
	 * Delay after starting the engine before beginning closed loop correction.
	 * units: seconds
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

// start of cranking_parameters_s
struct cranking_parameters_s {
	/**
	 * Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for CLT, IAT, TPS ect, to give the final cranking pulse width.
	 * A reasonable starting point is 60mg per liter per cylinder.
	 * ex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.
	 * units: mg
	 * offset 0
	 */
	float baseFuel;
	/**
	 * This sets the RPM limit below which the ECU will use cranking fuel and ignition logic, typically this is around 350-450rpm. 
	 * set cranking_rpm X
	 * units: RPM
	 * offset 4
	 */
	int16_t rpm;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 6
	 */
	uint8_t alignmentFill_at_6[2];
};
static_assert(sizeof(cranking_parameters_s) == 8);

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
	uint8_t alignmentFill_at_3[1];
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
	 * units: load
	 * offset 10
	 */
	scaled_channel<int16_t, 2, 1> loadBins[GPPWM_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 26
	 */
	int16_t rpmBins[GPPWM_RPM_COUNT];
	/**
	 * units: duty
	 * offset 42
	 */
	scaled_channel<uint8_t, 2, 1> table[GPPWM_LOAD_COUNT][GPPWM_RPM_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 106
	 */
	uint8_t alignmentFill_at_106[2];
};
static_assert(sizeof(gppwm_channel) == 108);

// start of air_pressure_sensor_config_s
struct air_pressure_sensor_config_s {
	/**
	 * kPa value at low volts
	 * units: kpa
	 * offset 0
	 */
	float lowValue;
	/**
	 * kPa value at high volts
	 * units: kpa
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
	uint8_t alignmentFill_at_10[2];
};
static_assert(sizeof(air_pressure_sensor_config_s) == 12);

// start of MAP_sensor_config_s
struct MAP_sensor_config_s {
	/**
	 * offset 0
	 */
	float samplingAngleBins[MAP_ANGLE_SIZE];
	/**
	 * MAP averaging sampling start crank degree angle
	 * units: deg
	 * offset 32
	 */
	float samplingAngle[MAP_ANGLE_SIZE];
	/**
	 * offset 64
	 */
	float samplingWindowBins[MAP_WINDOW_SIZE];
	/**
	 * MAP averaging angle crank degree duration
	 * units: deg
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
	uint8_t alignmentFill_at_1[3];
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
	 * units: cm3/min
	 * offset 0
	 */
	float flow;
	/**
	 * units: volts
	 * offset 4
	 */
	float battLagCorrBins[VBAT_INJECTOR_CURVE_SIZE];
	/**
	 * ms delay between injector open and close dead times
	 * units: ms
	 * offset 36
	 */
	float battLagCorr[VBAT_INJECTOR_CURVE_SIZE];
};
static_assert(sizeof(injector_s) == 68);

// start of trigger_config_s
struct trigger_config_s {
	/**
	 * https://github.com/rusefi/rusefi/wiki/All-Supported-Triggers
	 * set trigger_type X
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
	uint8_t alignmentFill_at_2[2];
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
	 * units: rpm
	 * offset 0
	 */
	scaled_channel<uint8_t, 1, 50> rpmBins[6];
	/**
	 * units: volts
	 * offset 6
	 */
	scaled_channel<uint8_t, 100, 1> values[6];
	/**
	 * offset 12
	 */
	Gpio pin;
	/**
	 * need 4 byte alignment
	 * units: units
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
	 * offset 2
	 */
	uint16_t startButtonSuppressOnStartUpMs;
	/**
	 * Disable sensor sniffer above this rpm
	 * units: RPM
	 * offset 4
	 */
	uint16_t sensorSnifferRpmThreshold;
	/**
	 * A secondary Rev limit engaged by the driver to help launch the vehicle faster
	 * units: rpm
	 * offset 6
	 */
	uint16_t launchRpm;
	/**
	 * set rpm_hard_limit X
	 * units: rpm
	 * offset 8
	 */
	uint16_t rpmHardLimit;
	/**
	 * Engine sniffer would be disabled above this rpm
	 * set engineSnifferRpmThreshold X
	 * units: RPM
	 * offset 10
	 */
	uint16_t engineSnifferRpmThreshold;
	/**
	 * Disable multispark above this engine speed.
	 * units: rpm
	 * offset 12
	 */
	scaled_channel<uint8_t, 1, 50> multisparkMaxRpm;
	/**
	 * Above this RPM, disable AC. Set to 0 to disable check.
	 * units: rpm
	 * offset 13
	 */
	scaled_channel<uint8_t, 1, 50> maxAcRpm;
	/**
	 * Above this TPS, disable AC. Set to 0 to disable check.
	 * units: %
	 * offset 14
	 */
	uint8_t maxAcTps;
	/**
	 * Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	 * units: deg C
	 * offset 15
	 */
	uint8_t maxAcClt;
	/**
	 * units: RPM
	 * offset 16
	 */
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	 * units: deg
	 * offset 48
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	 * units: count
	 * offset 49
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 50
	 */
	uint8_t alignmentFill_at_50[2];
	/**
	 * offset 52
	 */
	injector_s injector;
	/**
	 * offset 120
	 */
	injector_s injectorSecondary;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 188 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 188 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 188 bit 2 */
	bool enableKline : 1 {};
	/**
	offset 188 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 188 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 188 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 188 bit 6 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 188 bit 7 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 188 bit 8 */
	bool enableTrailingSparks : 1 {};
	/**
	 * TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 188 bit 9 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 188 bit 10 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 188 bit 11 */
	bool useEeprom : 1 {};
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 188 bit 12 */
	bool useCicPidForIdle : 1 {};
	/**
	offset 188 bit 13 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 188 bit 14 */
	bool kickStartCranking : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 188 bit 15 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 188 bit 16 */
	bool launchControlEnabled : 1 {};
	/**
	 * "Detect double trigger edges"
	offset 188 bit 17 */
	bool doNotFilterTriggerEdgeNoise : 1 {};
	/**
	offset 188 bit 18 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 188 bit 19 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	 * 
	 * enable logic_level_trigger
	offset 188 bit 20 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 188 bit 21 */
	bool useTLE8888_stepper : 1 {};
	/**
	offset 188 bit 22 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 188 bit 23 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 188 bit 24 */
	bool artificialTestMisfire : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 188 bit 25 */
	bool useFordRedundantPps : 1 {};
	/**
	offset 188 bit 26 */
	bool cltSensorPulldown : 1 {};
	/**
	offset 188 bit 27 */
	bool iatSensorPulldown : 1 {};
	/**
	offset 188 bit 28 */
	bool allowIdenticalPps : 1 {};
	/**
	offset 188 bit 29 */
	bool overrideVvtTriggerGaps : 1 {};
	/**
	 * If enabled - use onboard SPI Accelerometer, otherwise listen for CAN messages
	offset 188 bit 30 */
	bool useSpiImu : 1 {};
	/**
	offset 188 bit 31 */
	bool enableStagedInjection : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * set tps_min X
	 * units: ADC
	 * offset 192
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * set tps_max X
	 * units: ADC
	 * offset 194
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 196
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 198
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 200
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	 * units: ms
	 * offset 208
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	 * units: rpm
	 * offset 212
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	 * units: rpm
	 * offset 214
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 216
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 356
	 */
	ThermistorConf clt;
	/**
	 * offset 388
	 */
	ThermistorConf iat;
	/**
	 * units: deg
	 * offset 420
	 */
	int launchTimingRetard;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	 * units: integer
	 * offset 424
	 */
	int hip9011PrescalerAndSDO;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	 * Requires power cycling to effect
	 * units: kHz
	 * offset 428
	 */
	float knockBandCustom;
	/**
	 * Engine displacement in litres
	 * units: L
	 * offset 432
	 */
	scaled_channel<uint16_t, 1000, 1> displacement;
	/**
	 * units: RPM
	 * offset 434
	 */
	uint16_t triggerSimulatorRpm;
	/**
	 * Number of cylinder the engine has.
	 * offset 436
	 */
	uint32_t cylindersCount;
	/**
	 * offset 440
	 */
	firing_order_e firingOrder;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 441
	 */
	uint8_t alignmentFill_at_441[3];
	/**
	 * Cylinder diameter in mm.
	 * units: mm
	 * offset 444
	 */
	float cylinderBore;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS commonly only used for NA engines
	 * Speed Density requires MAP sensor and is the default choice for may installs
	 * MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	 * offset 448
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * units: %
	 * offset 449
	 */
	uint8_t ALSMaxTPS;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 450
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups.
	 * offset 451
	 */
	injection_mode_e injectionMode;
	/**
	 * Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 452
	 */
	uint16_t boostControlMinRpm;
	/**
	 * Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 454
	 */
	uint8_t boostControlMinTps;
	/**
	 * Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 455
	 */
	uint8_t boostControlMinMap;
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	 * units: deg
	 * offset 456
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 460
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 461
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	 * units: kph
	 * offset 462
	 */
	uint8_t maxIdleVss;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 463
	 */
	uint8_t alignmentFill_at_463[1];
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	 * units: kPa
	 * offset 464
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 466
	 */
	timing_mode_e timingMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 467
	 */
	uint8_t alignmentFill_at_467[1];
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	 * units: RPM
	 * offset 468
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Positive value in case of synchronization point before TDC and negative in case of synchronization point after TDC
	 * .Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * set global_trigger_offset_angle X
	 * units: deg btdc
	 * offset 472
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	 * units: coef
	 * offset 476
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	 * units: coef
	 * offset 480
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: deg C
	 * offset 484
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: deg C
	 * offset 488
	 */
	float fanOffTemperature;
	/**
	 * offset 492
	 */
	output_pin_e acrPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 494
	 */
	uint8_t alignmentFill_at_494[2];
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	 * units: revs/km
	 * offset 496
	 */
	float driveWheelRevPerKm;
	/**
	 * set can_mode X
	 * offset 500
	 */
	can_nbc_e canNbcType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 501
	 */
	uint8_t alignmentFill_at_501[3];
	/**
	 * CANbus thread period in ms
	 * units: ms
	 * offset 504
	 */
	int canSleepPeriodMs;
	/**
	 * offset 508
	 */
	uint8_t camDecoder2jzPrecision;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 509
	 */
	uint8_t alignmentFill_at_509[3];
	/**
	 * units: index
	 * offset 512
	 */
	int byFirmwareVersion;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
	 * offset 516
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 517
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 518
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * set_analog_input_pin tps2 X
	 * offset 519
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 520
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	 * units: angle
	 * offset 524
	 */
	int trailingSparkAngle;
	/**
	 * offset 528
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	 * units: %
	 * offset 540
	 */
	float airByRpmTaper;
	/**
	 * offset 544
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	 * units: %
	 * offset 545
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 546
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 547
	 */
	uint8_t acrRevolutions;
	/**
	 * units: coef
	 * offset 548
	 */
	float globalFuelCorrection;
	/**
	 * units: volts
	 * offset 552
	 */
	float adcVcc;
	/**
	 * units: Deg
	 * offset 556
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 560
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
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
	 * TPS/PPS error threshold
	 * units: %
	 * offset 589
	 */
	scaled_channel<uint8_t, 10, 1> etbSplit;
	/**
	 * offset 590
	 */
	Gpio tle6240_cs;
	/**
	 * offset 592
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * offset 593
	 */
	pin_output_mode_e mc33810_csPinMode;
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 594
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * @see hasBaroSensor
	 * offset 596
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 608
	 */
	idle_hardware_s idle;
	/**
	 * Value between 0 and 100 used in Manual mode
	 * units: %
	 * offset 620
	 */
	float manIdlePosition;
	/**
	 * Ignition timing to remove when a knock event occurs.
	 * units: %
	 * offset 624
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	 * units: deg/s
	 * offset 625
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 626
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 627
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	 * units: ratio
	 * offset 628
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	 * units: count
	 * offset 630
	 */
	uint8_t vssToothCount;
	/**
	 * Override the Y axis (load) value used for only the Idle VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 631
	 */
	ve_override_e idleVeOverrideMode;
	/**
	 * offset 632
	 */
	Gpio l9779_cs;
	/**
	 * offset 634
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 658
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 682
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 683
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 684
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 686
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * How many consecutive VVT gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 687
	 */
	int8_t gapVvtTrackingLengthOverride;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 688
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 690
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 691
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 692
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 694
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 696
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 698
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 699
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 700
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 708
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 709
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 710
	 */
	Gpio debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 712
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 713
	 */
	uint8_t alignmentFill_at_713[1];
	/**
	 * offset 714
	 */
	Gpio mc33972_cs;
	/**
	 * offset 716
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 717
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 718
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 719
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 720
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * units: %
	 * offset 721
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 729
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 730
	 */
	Gpio mc33810_cs[C_MC33810_COUNT];
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 732
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 736
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	 * units: us
	 * offset 740
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * offset 742
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 743
	 */
	uint8_t alignmentFill_at_743[1];
	/**
	 * offset 744
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 746
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 747
	 */
	uint8_t alignmentFill_at_747[1];
	/**
	 * offset 748
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 750
	 */
	Gpio sdCardCsPin;
	/**
	 * set_can_tx_pin X
	 * offset 752
	 */
	Gpio canTxPin;
	/**
	 * set_can_rx_pin X
	 * offset 754
	 */
	Gpio canRxPin;
	/**
	 * offset 756
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * Additional idle % while A/C is active
	 * units: %
	 * offset 757
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * Ratio between the wheels and your transmission output.
	 * units: ratio
	 * offset 758
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 760
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 762
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 763
	 */
	uint8_t alignmentFill_at_763[1];
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	 * units: mv
	 * offset 764
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	 * units: mv
	 * offset 766
	 */
	uint16_t wastegatePositionMax;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	 * units: mv
	 * offset 768
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	 * units: mv
	 * offset 770
	 */
	uint16_t idlePositionMax;
	/**
	 * Secondary TTL channel baud rate
	 * units: BPs
	 * offset 772
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	 * units: CR
	 * offset 776
	 */
	float compressionRatio;
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 780
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * units: g/s
	 * offset 784
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 786
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 788
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 789
	 */
	uint8_t alignmentFill_at_789[1];
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 790
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 792
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * units: RPM
	 * offset 793
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionMinRpm;
	/**
	 * units: %
	 * offset 794
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionMinLoad;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 795
	 */
	uint8_t alignmentFill_at_795[1];
	/**
	offset 796 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 796 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 796 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 796 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 796 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 796 bit 5 */
	bool rethrowHardFault : 1 {};
	/**
	offset 796 bit 6 */
	bool isHip9011Enabled : 1 {};
	/**
	offset 796 bit 7 */
	bool requireFootOnBrakeToCrank : 1 {};
	/**
	offset 796 bit 8 */
	bool verboseQuad : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 796 bit 9 */
	bool useStepperIdle : 1 {};
	/**
	offset 796 bit 10 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 796 bit 11 */
	bool useTpicAdvancedMode : 1 {};
	/**
	offset 796 bit 12 */
	bool lambdaProtectionEnable : 1 {};
	/**
	offset 796 bit 13 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 796 bit 14 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 796 bit 15 */
	bool etb1configured : 1 {};
	/**
	offset 796 bit 16 */
	bool etb2configured : 1 {};
	/**
	 * Useful for individual intakes
	offset 796 bit 17 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 796 bit 18 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 796 bit 19 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 796 bit 20 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 796 bit 21 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 796 bit 22 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 796 bit 23 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 796 bit 24 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 796 bit 25 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 796 bit 26 */
	bool pauseEtbControl : 1 {};
	/**
	offset 796 bit 27 */
	bool alignEngineSnifferAtTDC : 1 {};
	/**
	offset 796 bit 28 */
	bool verboseKLine : 1 {};
	/**
	offset 796 bit 29 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 796 bit 30 */
	bool enableAemXSeries : 1 {};
	/**
	offset 796 bit 31 */
	bool unusedBit_227_31 : 1 {};
	/**
	 * offset 800
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 808
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 809
	 */
	uint8_t alignmentFill_at_809[1];
	/**
	 * offset 810
	 */
	Gpio hip9011CsPin;
	/**
	 * offset 812
	 */
	Gpio hip9011IntHoldPin;
	/**
	 * offset 814
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 815
	 */
	uint8_t alignmentFill_at_815[1];
	/**
	 * offset 816
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	 * units: v
	 * offset 820
	 */
	uint8_t mc33_hvolt;
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	 * units: kPa
	 * offset 821
	 */
	uint8_t minimumBoostClosedLoopMap;
	/**
	 * Optional Radiator Fan used with A/C
	 * offset 822
	 */
	output_pin_e acFanPin;
	/**
	 * offset 824
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * offset 825
	 */
	spi_device_e l9779spiDevice;
	/**
	 * units: volts
	 * offset 826
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	 * offset 834
	 */
	imu_type_e imuType;
	/**
	 * units: multiplier
	 * offset 835
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 843
	 */
	uint8_t alignmentFill_at_843[1];
	/**
	 * units: kg
	 * offset 844
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling, i.e. coasting detection threshold.
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	 * units: RPM
	 * offset 846
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	 * units: ms
	 * offset 848
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 850
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 852
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 854
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 856
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 858
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 859
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 860
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 876
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * offset 878
	 */
	Gpio test557pin;
	/**
	 * offset 880
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 881
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	 * units: :1
	 * offset 882
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	 * units: %
	 * offset 883
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	 * units: hz
	 * offset 884
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 886
	 */
	adc_channel_e idlePositionChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 887
	 */
	uint8_t alignmentFill_at_887[1];
	/**
	 * offset 888
	 */
	Gpio debugMapAveraging;
	/**
	 * offset 890
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 892
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 893
	 */
	uint8_t alignmentFill_at_893[1];
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 894
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 896
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 898
	 */
	uint8_t alignmentFill_at_898[2];
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	 * units: count
	 * offset 900
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	 * units: %
	 * offset 904
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * units: %
	 * offset 906
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * units: ADC
	 * offset 908
	 */
	uint16_t tps1SecondaryMin;
	/**
	 * units: ADC
	 * offset 910
	 */
	uint16_t tps1SecondaryMax;
	/**
	 * units: rpm
	 * offset 912
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	 * units: Seconds
	 * offset 914
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 916
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 918
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 919
	 */
	uint8_t alignmentFill_at_919[1];
	/**
	 * offset 920
	 */
	output_pin_e acRelayPin;
	/**
	 * units: %
	 * offset 922
	 */
	uint8_t lambdaProtectionMinTps;
	/**
	 * Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed
	 * units: s
	 * offset 923
	 */
	scaled_channel<uint8_t, 10, 1> lambdaProtectionTimeout;
	/**
	 * units: RPM
	 * offset 924
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionRestoreRpm;
	/**
	 * offset 925
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 926
	 */
	uint8_t alignmentFill_at_926[2];
	/**
	 * offset 928
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 960
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 962
	 */
	Gpio spi1misoPin;
	/**
	 * offset 964
	 */
	Gpio spi1sckPin;
	/**
	 * offset 966
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 968
	 */
	Gpio spi2misoPin;
	/**
	 * offset 970
	 */
	Gpio spi2sckPin;
	/**
	 * offset 972
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 974
	 */
	Gpio spi3misoPin;
	/**
	 * offset 976
	 */
	Gpio spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 978
	 */
	Gpio cdmInputPin;
	/**
	 * offset 980
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEFI console Sensor Sniffer mode
	 * offset 981
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 982
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 983
	 */
	uint8_t alignmentFill_at_983[1];
	/**
	offset 984 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 984 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 984 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 984 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 984 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 984 bit 5 */
	bool canInputBCM : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	offset 984 bit 6 */
	bool consumeObdSensors : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 984 bit 7 */
	bool enableCanVss : 1 {};
	/**
	offset 984 bit 8 */
	bool enableInnovateLC2 : 1 {};
	/**
	offset 984 bit 9 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 984 bit 10 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 984 bit 11 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 984 bit 12 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	 * enable vvt_details
	offset 984 bit 13 */
	bool verboseVVTDecoding : 1 {};
	/**
	 * get invertCamVVTSignal
	offset 984 bit 14 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * In Alpha-N mode, compensate for air temperature.
	offset 984 bit 15 */
	bool alphaNUseIat : 1 {};
	/**
	offset 984 bit 16 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 984 bit 17 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 984 bit 18 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 984 bit 19 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 984 bit 20 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 984 bit 21 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 984 bit 22 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 984 bit 23 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 984 bit 24 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 984 bit 25 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 984 bit 26 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 984 bit 27 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 984 bit 28 */
	bool tcuEnabled : 1 {};
	/**
	offset 984 bit 29 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coils
	offset 984 bit 30 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 984 bit 31 */
	bool verboseCan2 : 1 {};
	/**
	 * offset 988
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 1004
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 1006
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1007
	 */
	uint8_t alignmentFill_at_1007[1];
	/**
	 * offset 1008
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 1010
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 1012
	 */
	pid_s boostPid;
	/**
	 * offset 1032
	 */
	boostType_e boostType;
	/**
	 * offset 1033
	 */
	pin_input_mode_e ignitionKeyDigitalPinMode;
	/**
	 * offset 1034
	 */
	Gpio ignitionKeyDigitalPin;
	/**
	 * units: Hz
	 * offset 1036
	 */
	int boostPwmFrequency;
	/**
	 * offset 1040
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1041
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1042
	 */
	uint8_t alignmentFill_at_1042[2];
	/**
	 * Launch disabled above this speed if setting is above zero
	 * units: Kph
	 * offset 1044
	 */
	int launchSpeedThreshold;
	/**
	 * Range from Launch RPM for Timing Retard to activate
	 * units: RPM
	 * offset 1048
	 */
	int launchTimingRpmRange;
	/**
	 * Extra Fuel Added
	 * units: %
	 * offset 1052
	 */
	int launchFuelAdded;
	/**
	 * Duty Cycle for the Boost Solenoid
	 * units: %
	 * offset 1056
	 */
	int launchBoostDuty;
	/**
	 * Range from Launch RPM to activate Hard Cut
	 * units: RPM
	 * offset 1060
	 */
	int hardCutRpmRange;
	/**
	 * offset 1064
	 */
	float turbochargerFilter;
	/**
	 * offset 1068
	 */
	int launchTpsThreshold;
	/**
	 * offset 1072
	 */
	float launchActivateDelay;
	/**
	 * offset 1076
	 */
	stft_s stft;
	/**
	 * offset 1100
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1116
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1148
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1180
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1212
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * offset 1224
	 */
	dc_function_e etbFunctions[ETB_COUNT];
	/**
	 * offset 1226
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1227
	 */
	uint8_t alignmentFill_at_1227[1];
	/**
	 * offset 1228
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1230
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1231
	 */
	uint8_t alignmentFill_at_1231[1];
	/**
	 * offset 1232
	 */
	Gpio drv8860_miso;
	/**
	 * units: volt
	 * offset 1234
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * offset 1250
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1266
	 */
	uint8_t alignmentFill_at_1266[2];
	/**
	 * Angle between cam sensor and VVT zero position
	 * set vvt_offset X
	 * units: value
	 * offset 1268
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1284
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1316
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	 * units: ADC
	 * offset 1380
	 */
	uint16_t tps2SecondaryMin;
	/**
	 * units: ADC
	 * offset 1382
	 */
	uint16_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1384 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1384 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * On even fire engines with even number of cylinders we go wasted spark during cranking. Use this setting to disable wasted spark cranking on odd fire engines.
	offset 1384 bit 2 */
	bool oddFireEngine : 1 {};
	/**
	offset 1384 bit 3 */
	bool boardUseTachPullUp : 1 {};
	/**
	offset 1384 bit 4 */
	bool boardUseTempPullUp : 1 {};
	/**
	offset 1384 bit 5 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1384 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1384 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1384 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write. See also can1ListenMode
	offset 1384 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1384 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1384 bit 11 */
	bool boardUse2stepPullDown : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1384 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1384 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1384 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1384 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1384 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1384 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1384 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1384 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1384 bit 20 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 1384 bit 21 */
	bool boardUseCamPullDown : 1 {};
	/**
	offset 1384 bit 22 */
	bool boardUseCamVrPullUp : 1 {};
	/**
	offset 1384 bit 23 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 1384 bit 24 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 1384 bit 25 */
	bool boardUseD4PullDown : 1 {};
	/**
	offset 1384 bit 26 */
	bool boardUseD5PullDown : 1 {};
	/**
	 * Are you a developer troubleshooting TS over CAN ISO/TP?
	offset 1384 bit 27 */
	bool verboseIsoTp : 1 {};
	/**
	offset 1384 bit 28 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1384 bit 29 */
	bool launchActivateInverted : 1 {};
	/**
	offset 1384 bit 30 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1384 bit 31 */
	bool skippedWheelOnCam : 1 {};
	/**
	 * offset 1388
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1389
	 */
	uint8_t alignmentFill_at_1389[1];
	/**
	 * A/C button input
	 * offset 1390
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1392
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	 * units: %
	 * offset 1393
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1394
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1395
	 */
	uint8_t alignmentFill_at_1395[1];
	/**
	offset 1396 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1396 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1396 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1396 bit 3 */
	bool complexWallModel : 1 {};
	/**
	 * RPM is measured based on last 720 degrees while instant RPM is measured based on the last 90 degrees of crank revolution
	offset 1396 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1396 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1396 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1396 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1396 bit 8 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1396 bit 9 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	 * enable trigger_details
	offset 1396 bit 10 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1396 bit 11 */
	bool isManualSpinningMode : 1 {};
	/**
	offset 1396 bit 12 */
	bool cutFuelInAcr : 1 {};
	/**
	offset 1396 bit 13 */
	bool hondaK : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1396 bit 14 */
	bool twoWireBatchIgnition : 1 {};
	/**
	offset 1396 bit 15 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1396 bit 16 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1396 bit 17 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1396 bit 18 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1396 bit 19 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1396 bit 20 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1396 bit 21 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1396 bit 22 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1396 bit 23 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors. WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1396 bit 24 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1396 bit 25 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1396 bit 26 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1396 bit 27 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	offset 1396 bit 28 */
	bool sdTriggerLog : 1 {};
	/**
	offset 1396 bit 29 */
	bool ALSActivateInverted : 1 {};
	/**
	offset 1396 bit 30 */
	bool stepper_dc_use_two_wires : 1 {};
	/**
	offset 1396 bit 31 */
	bool watchOutForLinearTime : 1 {};
	/**
	 * units: count
	 * offset 1400
	 */
	uint32_t engineChartSize;
	/**
	 * units: mult
	 * offset 1404
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1408
	 */
	Gpio camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * Idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	 * units: RPM
	 * offset 1416
	 */
	int16_t acIdleRpmTarget;
	/**
	 * set warningPeriod X
	 * units: seconds
	 * offset 1418
	 */
	int16_t warningPeriod;
	/**
	 * units: angle
	 * offset 1420
	 */
	float knockDetectionWindowStart;
	/**
	 * units: angle
	 * offset 1424
	 */
	float knockDetectionWindowEnd;
	/**
	 * units: ms
	 * offset 1428
	 */
	float idleStepperReactionTime;
	/**
	 * units: count
	 * offset 1432
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	 * units: sec
	 * offset 1436
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	 * units: index
	 * offset 1440
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1444
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	 * units: Seconds
	 * offset 1448
	 */
	float wwaeTau;
	/**
	 * offset 1452
	 */
	pid_s alternatorControl;
	/**
	 * offset 1472
	 */
	pid_s etb;
	/**
	 * offset 1492
	 */
	Gpio triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * RPM range above upper limit for extra air taper
	 * units: RPM
	 * offset 1496
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1498
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * set tps2_min X
	 * units: ADC
	 * offset 1500
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps2_max X
	 * units: ADC
	 * offset 1502
	 */
	int16_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1504
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1506
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1507
	 */
	uint8_t alignmentFill_at_1507[1];
	/**
	 * offset 1508
	 */
	Gpio mc33816_flag0;
	/**
	 * units: Pulse
	 * offset 1510
	 */
	uint8_t tachPulsePerRev;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1511
	 */
	uint8_t alignmentFill_at_1511[1];
	/**
	 * kPa value which is too low to be true
	 * units: kPa
	 * offset 1512
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	 * units: kPa
	 * offset 1516
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	 * units: ms
	 * offset 1520
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	 * units: ms
	 * offset 1522
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1524
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	 * units: Fraction
	 * offset 1544
	 */
	float wwaeBeta;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1548
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1550
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1552
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * offset 1556
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1558
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * units: voltage
	 * offset 1560
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * units: voltage
	 * offset 1564
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	 * units: seconds
	 * offset 1568
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	 * units: RPM
	 * offset 1570
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	 * units: Volts
	 * offset 1572
	 */
	float targetVBatt;
	/**
	 * See Over/Undervoltage Shutdown/Retry bit in documentation
	offset 1576 bit 0 */
	bool mc33810DisableRecoveryMode : 1 {};
	/**
	offset 1576 bit 1 */
	bool mc33810Gpgd0Mode : 1 {};
	/**
	offset 1576 bit 2 */
	bool mc33810Gpgd1Mode : 1 {};
	/**
	offset 1576 bit 3 */
	bool mc33810Gpgd2Mode : 1 {};
	/**
	offset 1576 bit 4 */
	bool mc33810Gpgd3Mode : 1 {};
	/**
	offset 1576 bit 5 */
	bool enableExtendedCanBroadcast : 1 {};
	/**
	offset 1576 bit 6 */
	bool luaCanRxWorkaround : 1 {};
	/**
	offset 1576 bit 7 */
	bool unusedBit_495_7 : 1 {};
	/**
	offset 1576 bit 8 */
	bool unusedBit_495_8 : 1 {};
	/**
	offset 1576 bit 9 */
	bool unusedBit_495_9 : 1 {};
	/**
	offset 1576 bit 10 */
	bool unusedBit_495_10 : 1 {};
	/**
	offset 1576 bit 11 */
	bool useAuxSpeedForSlipRatio : 1 {};
	/**
	 * VSS and auxSpeed1 or auxSpeed1 with auxSpeed2?
	offset 1576 bit 12 */
	bool useVssAsSecondWheelSpeed : 1 {};
	/**
	offset 1576 bit 13 */
	bool is_enabled_spi_5 : 1 {};
	/**
	offset 1576 bit 14 */
	bool is_enabled_spi_6 : 1 {};
	/**
	offset 1576 bit 15 */
	bool unusedBit_503_15 : 1 {};
	/**
	offset 1576 bit 16 */
	bool unusedBit_503_16 : 1 {};
	/**
	offset 1576 bit 17 */
	bool unusedBit_503_17 : 1 {};
	/**
	offset 1576 bit 18 */
	bool unusedBit_508_18 : 1 {};
	/**
	offset 1576 bit 19 */
	bool unusedBit_508_19 : 1 {};
	/**
	offset 1576 bit 20 */
	bool unusedBit_508_20 : 1 {};
	/**
	offset 1576 bit 21 */
	bool unusedBit_508_21 : 1 {};
	/**
	offset 1576 bit 22 */
	bool unusedBit_508_22 : 1 {};
	/**
	offset 1576 bit 23 */
	bool unusedBit_508_23 : 1 {};
	/**
	offset 1576 bit 24 */
	bool unusedBit_508_24 : 1 {};
	/**
	offset 1576 bit 25 */
	bool unusedBit_508_25 : 1 {};
	/**
	offset 1576 bit 26 */
	bool unusedBit_508_26 : 1 {};
	/**
	offset 1576 bit 27 */
	bool unusedBit_508_27 : 1 {};
	/**
	offset 1576 bit 28 */
	bool unusedBit_508_28 : 1 {};
	/**
	offset 1576 bit 29 */
	bool unusedBit_508_29 : 1 {};
	/**
	offset 1576 bit 30 */
	bool unusedBit_508_30 : 1 {};
	/**
	offset 1576 bit 31 */
	bool unusedBit_508_31 : 1 {};
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	 * units: cycles
	 * offset 1580
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * IAC Value added when coasting and transitioning into idle.
	 * units: percent
	 * offset 1582
	 */
	int16_t iacByTpsTaper;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * offset 1584
	 */
	Gpio auxSerialTxPin;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * offset 1586
	 */
	Gpio auxSerialRxPin;
	/**
	 * offset 1588
	 */
	Gpio accelerometerCsPin;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	 * units: sec
	 * offset 1590
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: kph
	 * offset 1591
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: kph
	 * offset 1592
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	 * units: sec
	 * offset 1593
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1594
	 */
	uint8_t alignmentFill_at_1594[2];
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	 * units: roc
	 * offset 1596
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1600
	 */
	brain_input_pin_e auxSpeedSensorInputPin[AUX_SPEED_SENSOR_COUNT];
	/**
	 * offset 1604
	 */
	uint8_t totalGearsCount;
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 1605
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1606
	 */
	debug_mode_e debugMode;
	/**
	 * Additional idle % when fan #1 is active
	 * units: %
	 * offset 1607
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * Band rate for primary TTL
	 * units: BPs
	 * offset 1608
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	 * units: roc
	 * offset 1612
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	 * units: coeff
	 * offset 1616
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * units: BPs
	 * offset 1620
	 */
	uint32_t auxSerialSpeed;
	/**
	 * units: voltage
	 * offset 1624
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	 * units: voltage
	 * offset 1628
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * set can_baudrate
	 * offset 1632
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1633
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1634
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1635
	 */
	load_override_e afrOverrideMode;
	/**
	 * units: A
	 * offset 1636
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	 * units: A
	 * offset 1637
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	 * units: us
	 * offset 1638
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	 * units: ms
	 * offset 1639
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1640 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Allow OpenBLT on Primary CAN
	offset 1640 bit 1 */
	bool canOpenBLT : 1 {};
	/**
	 * Allow OpenBLT on Secondary CAN
	offset 1640 bit 2 */
	bool can2OpenBLT : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (defualt, cc/min) or mass flow (g/s).
	offset 1640 bit 3 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1640 bit 4 */
	bool boardUseCanTerminator : 1 {};
	/**
	offset 1640 bit 5 */
	bool kLineDoHondaSend : 1 {};
	/**
	 * ListenMode is about acknowledging CAN traffic on the protocol level. Different from canWriteEnabled
	offset 1640 bit 6 */
	bool can1ListenMode : 1 {};
	/**
	offset 1640 bit 7 */
	bool can2ListenMode : 1 {};
	/**
	offset 1640 bit 8 */
	bool unusedBit_560_8 : 1 {};
	/**
	offset 1640 bit 9 */
	bool unusedBit_560_9 : 1 {};
	/**
	offset 1640 bit 10 */
	bool unusedBit_560_10 : 1 {};
	/**
	offset 1640 bit 11 */
	bool unusedBit_560_11 : 1 {};
	/**
	offset 1640 bit 12 */
	bool unusedBit_560_12 : 1 {};
	/**
	offset 1640 bit 13 */
	bool unusedBit_560_13 : 1 {};
	/**
	offset 1640 bit 14 */
	bool unusedBit_560_14 : 1 {};
	/**
	offset 1640 bit 15 */
	bool unusedBit_560_15 : 1 {};
	/**
	offset 1640 bit 16 */
	bool unusedBit_560_16 : 1 {};
	/**
	offset 1640 bit 17 */
	bool unusedBit_560_17 : 1 {};
	/**
	offset 1640 bit 18 */
	bool unusedBit_560_18 : 1 {};
	/**
	offset 1640 bit 19 */
	bool unusedBit_560_19 : 1 {};
	/**
	offset 1640 bit 20 */
	bool unusedBit_560_20 : 1 {};
	/**
	offset 1640 bit 21 */
	bool unusedBit_560_21 : 1 {};
	/**
	offset 1640 bit 22 */
	bool unusedBit_560_22 : 1 {};
	/**
	offset 1640 bit 23 */
	bool unusedBit_560_23 : 1 {};
	/**
	offset 1640 bit 24 */
	bool unusedBit_560_24 : 1 {};
	/**
	offset 1640 bit 25 */
	bool unusedBit_560_25 : 1 {};
	/**
	offset 1640 bit 26 */
	bool unusedBit_560_26 : 1 {};
	/**
	offset 1640 bit 27 */
	bool unusedBit_560_27 : 1 {};
	/**
	offset 1640 bit 28 */
	bool unusedBit_560_28 : 1 {};
	/**
	offset 1640 bit 29 */
	bool unusedBit_560_29 : 1 {};
	/**
	offset 1640 bit 30 */
	bool unusedBit_560_30 : 1 {};
	/**
	offset 1640 bit 31 */
	bool unusedBit_560_31 : 1 {};
	/**
	 * offset 1644
	 */
	uint8_t camDecoder2jzPosition;
	/**
	 * offset 1645
	 */
	mc33810maxDwellTimer_e mc33810maxDwellTimer;
	/**
	 * Duration of each test pulse
	 * units: ms
	 * offset 1646
	 */
	scaled_channel<uint16_t, 100, 1> benchTestOnTime;
	/**
	 * units: %
	 * offset 1648
	 */
	uint8_t lambdaProtectionRestoreTps;
	/**
	 * units: %
	 * offset 1649
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionRestoreLoad;
	/**
	 * offset 1650
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1651
	 */
	uint8_t alignmentFill_at_1651[1];
	/**
	 * set_can2_tx_pin X
	 * offset 1652
	 */
	Gpio can2TxPin;
	/**
	 * set_can2_rx_pin X
	 * offset 1654
	 */
	Gpio can2RxPin;
	/**
	 * offset 1656
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1657
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1658
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1659
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1660
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1662
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1663
	 */
	uint8_t alignmentFill_at_1663[1];
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	 * units: kPa (absolute)
	 * offset 1664
	 */
	float boostCutPressure;
	/**
	 * units: kg/h
	 * offset 1668
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16];
	/**
	 * units: ratio
	 * offset 1684
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16];
	/**
	 * Fixed timing, useful for TDC testing
	 * units: deg
	 * offset 1700
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1704
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1708
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * units: value
	 * offset 1712
	 */
	float egoValueShift;
	/**
	 * VVT output solenoid pin for this cam
	 * offset 1716
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * offset 1724
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1725
	 */
	uint8_t alignmentFill_at_1725[3];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	 * units: percent
	 * offset 1728
	 */
	int crankingIACposition;
	/**
	 * offset 1732
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 1736
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 1740
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 1744
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 1748
	 */
	pwm_freq_t vvtOutputFrequency;
	/**
	 * Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.
	 * units: deg BTDC
	 * offset 1750
	 */
	int8_t minimumIgnitionTiming;
	/**
	 * Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC
	 * units: deg BTDC
	 * offset 1751
	 */
	int8_t maximumIgnitionTiming;
	/**
	 * units: Hz
	 * offset 1752
	 */
	int alternatorPwmFrequency;
	/**
	 * set vvt_mode X
	 * offset 1756
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #2 is active
	 * units: %
	 * offset 1758
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	 * units: sec
	 * offset 1759
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1760
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT];
	/**
	 * offset 1768
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 1792
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1793
	 */
	pin_output_mode_e accelerometerCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 1794
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 1795
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	 * units: kPa
	 * offset 1796
	 */
	float fuelReferencePressure;
	/**
	 * units: mult
	 * offset 1800
	 */
	float postCrankingFactor[CRANKING_ENRICH_COUNT][CRANKING_ENRICH_COUNT];
	/**
	 * units: count
	 * offset 1944
	 */
	float postCrankingDurationBins[CRANKING_ENRICH_COUNT];
	/**
	 * units: C
	 * offset 1968
	 */
	float postCrankingCLTBins[CRANKING_ENRICH_COUNT];
	/**
	 * offset 1992
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 2024
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * units: Deg
	 * offset 2056
	 */
	int16_t knockSamplingDuration;
	/**
	 * units: Hz
	 * offset 2058
	 */
	int16_t etbFreq;
	/**
	 * offset 2060
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 2080
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	 * units: %
	 * offset 2081
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	 * units: %
	 * offset 2082
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 2083
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder ignition and fueling timing correction for uneven engines
	 * units: deg
	 * offset 2084
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	 * units: seconds
	 * offset 2132
	 */
	float idlePidActivationTime;
	/**
	 * offset 2136
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 2137
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 2138
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 2139
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 2140
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 2141
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 2142
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 2143
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 2144
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 2145
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 2146
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 2148
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 2150
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * offset 2152
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2153
	 */
	uint8_t alignmentFill_at_2153[3];
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 2156
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 2196
	 */
	float injectorCorrectionPolynomial[8];
	/**
	 * units: C
	 * offset 2228
	 */
	int8_t primeBins[PRIME_CURVE_COUNT];
	/**
	 * offset 2236
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2256
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2257
	 */
	uint8_t alignmentFill_at_2257[1];
	/**
	 * offset 2258
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: deg C
	 * offset 2260
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: deg C
	 * offset 2261
	 */
	uint8_t fan2OffTemperature;
	/**
	 * offset 2262
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2264
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2266
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2267
	 */
	uint8_t alignmentFill_at_2267[1];
	/**
	 * offset 2268
	 */
	Gpio mc33816_cs;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2270
	 */
	uint8_t alignmentFill_at_2270[2];
	/**
	 * units: hz
	 * offset 2272
	 */
	float auxFrequencyFilter;
	/**
	 * units: RPM
	 * offset 2276
	 */
	int16_t vvtControlMinRpm;
	/**
	 * offset 2278
	 */
	sent_input_pin_e sentInputPins[SENT_INPUT_COUNT];
	/**
	 * units: %
	 * offset 2280
	 */
	int8_t launchFuelAdderPercent;
	/**
	 * Time required to detect a stuck throttle.
	 * units: sec
	 * offset 2281
	 */
	scaled_channel<uint8_t, 50, 1> etbJamTimeout;
	/**
	 * By the way ETB PID runs at 500hz, length in 1/500 of second here.
	 * offset 2282
	 */
	uint16_t etbExpAverageLength;
	/**
	 * offset 2284
	 */
	float etbDutyThreshold;
	/**
	 * This sets the RPM above which fuel cut is active.
	 * units: rpm
	 * offset 2288
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	 * units: rpm
	 * offset 2290
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	 * units: %
	 * offset 2292
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	 * units: C
	 * offset 2294
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * units: %
	 * offset 2296
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	 * units: kPa
	 * offset 2298
	 */
	int16_t coastingFuelCutMap;
	/**
	 * offset 2300
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2320
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * units: C
	 * offset 2340
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	 * units: RPM
	 * offset 2344
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * offset 2352
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	 * offset 2448
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 2512
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2640
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2644
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	 * units: kg/h
	 * offset 2648
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2652
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2656
	 */
	float tChargeAirDecrLimit;
	/**
	 * offset 2660
	 */
	tChargeMode_e tChargeMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2661
	 */
	uint8_t alignmentFill_at_2661[3];
	/**
	 * offset 2664
	 */
	float hip9011Gain;
	/**
	 * iTerm min value
	 * offset 2668
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2670
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2672
	 */
	pid_s idleTimingPid;
	/**
	 * When entering idle, and the PID settings are aggressive, it's good to make a soft entry upon entering closed loop
	 * offset 2692
	 */
	float idleTimingSoftEntryTime;
	/**
	 * By the way ETB PID runs at 500hz, length in 1/500 of second here.
	 * offset 2696
	 */
	int16_t etbRocExpAverageLength;
	/**
	 * A delay in cycles between fuel-enrich. portions
	 * units: cycles
	 * offset 2698
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	 * units: coef
	 * offset 2700
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2704
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2705
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2706
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2708
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	 * units: :1
	 * offset 2709
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2710
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	 * units: %
	 * offset 2712
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2716
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	 * units: mg
	 * offset 2728
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[PRIME_CURVE_COUNT];
	/**
	 * Trigger comparator center point voltage
	 * units: V
	 * offset 2736
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	 * units: V
	 * offset 2737
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	 * units: V
	 * offset 2738
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	 * units: RPM
	 * offset 2739
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * units: ratio
	 * offset 2740
	 */
	scaled_channel<uint16_t, 100, 1> tractionControlSlipBins[TRACTION_CONTROL_ETB_DROP_SIZE];
	/**
	 * units: RPM
	 * offset 2752
	 */
	uint8_t tractionControlSpeedBins[TRACTION_CONTROL_ETB_DROP_SIZE];
	/**
	 * offset 2758
	 */
	int8_t disableFan1AtSpeed;
	/**
	 * offset 2759
	 */
	int8_t disableFan2AtSpeed;
	/**
	 * offset 2760
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2762
	 */
	uint8_t alignmentFill_at_2762[2];
	/**
	 * offset 2764
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	 * Boost Current
	 * units: mA
	 * offset 3196
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	 * units: mA
	 * offset 3198
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	 * units: mA
	 * offset 3200
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	 * units: us
	 * offset 3202
	 */
	uint16_t mc33_t_max_boost;
	/**
	 * units: us
	 * offset 3204
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	 * units: us
	 * offset 3206
	 */
	uint16_t mc33_t_peak_tot;
	/**
	 * units: us
	 * offset 3208
	 */
	uint16_t mc33_t_bypass;
	/**
	 * units: us
	 * offset 3210
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	 * units: us
	 * offset 3212
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 3214
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 3215
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 3216
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 3217
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	 * units: dB
	 * offset 3223
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3239
	 */
	uint8_t alignmentFill_at_3239[1];
	/**
	 * units: ratio
	 * offset 3240
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	 * units: ratio
	 * offset 3312
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	 * units: rpm
	 * offset 3384
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	 * units: sec
	 * offset 3385
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	 * units: sec
	 * offset 3386
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3387
	 */
	uint8_t alignmentFill_at_3387[1];
	/**
	 * units: mg
	 * offset 3388
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	 * units: multiplier
	 * offset 3390
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	/**
	 * units: %
	 * offset 3394
	 */
	uint8_t etbJamIntegratorLimit;
	/**
	 * units: lobes/cam
	 * offset 3395
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 3396
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * offset 3397
	 */
	uint8_t auxiliarySetting1;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	 * units: deg
	 * offset 3398
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3399
	 */
	uint8_t alignmentFill_at_3399[1];
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	 * units: cc
	 * offset 3400
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	 * units: deg
	 * offset 3402
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 3403
	 */
	uint8_t issFilterReciprocal;
	/**
	 * units: %/kPa
	 * offset 3404
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	 * units: %/kPa/lobe
	 * offset 3406
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	 * units: kPa/s
	 * offset 3408
	 */
	uint16_t hpfpTargetDecay;
	/**
	 * units: %
	 * offset 3410
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	 * units: deg
	 * offset 3426
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	 * units: volts
	 * offset 3442
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	 * units: ms
	 * offset 3450
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	 * units: kPa
	 * offset 3466
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	 * units: load
	 * offset 3666
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	 * units: RPM
	 * offset 3686
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	 * units: %
	 * offset 3696
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	 * units: cc/lobe
	 * offset 3796
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	 * units: RPM
	 * offset 3816
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	 * offset 3826
	 */
	output_pin_e stepper_raw_output[4];
	/**
	 * units: ratio
	 * offset 3834
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[TCU_GEAR_COUNT];
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	 * units: ms
	 * offset 3854
	 */
	uint16_t vvtActivationDelayMs;
	/**
	 * units: deg C
	 * offset 3856
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	/**
	 * offset 3864
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3872
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	/**
	 * units: kPa
	 * offset 3880
	 */
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	/**
	 * offset 3888
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 3896
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	/**
	 * units: Nm
	 * offset 3904
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_SIZE];
	/**
	 * units: RPM
	 * offset 3940
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE];
	/**
	 * units: Load
	 * offset 3952
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE];
	/**
	 * offset 3964
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3965
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * units: deg
	 * offset 3966
	 */
	uint16_t acrDisablePhase;
	/**
	 * offset 3968
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3988
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 4008
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 4010
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4011
	 */
	uint8_t alignmentFill_at_4011[1];
	/**
	 * offset 4012
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 4014
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4015
	 */
	uint8_t alignmentFill_at_4015[1];
	/**
	 * offset 4016
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 4018
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 4020
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4021
	 */
	uint8_t alignmentFill_at_4021[1];
	/**
	 * offset 4022
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 4024
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 4026
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4027
	 */
	uint8_t alignmentFill_at_4027[1];
	/**
	 * offset 4028
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * offset 4030
	 */
	output_pin_e acrPin2;
	/**
	 * units: %
	 * offset 4032
	 */
	float etbMinimumPosition;
	/**
	 * offset 4036
	 */
	uint16_t tuneHidingKey;
	/**
	 * offset 4038
	 */
	vin_number_t vinNumber;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4055
	 */
	uint8_t alignmentFill_at_4055[1];
	/**
	 * offset 4056
	 */
	uint16_t highSpeedOffsets[HIGH_SPEED_COUNT];
	/**
	 * offset 4120
	 */
	float etbDutyShutdownThreshold;
	/**
	 * offset 4124
	 */
	fuel_pressure_sensor_mode_e fuelPressureSensorMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4125
	 */
	uint8_t alignmentFill_at_4125[1];
	/**
	 * offset 4126
	 */
	switch_input_pin_e luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT];
	/**
	 * units: RPM
	 * offset 4142
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	/**
	 * units: rpm
	 * offset 4146
	 */
	int16_t ALSMinRPM;
	/**
	 * units: rpm
	 * offset 4148
	 */
	int16_t ALSMaxRPM;
	/**
	 * units: sec
	 * offset 4150
	 */
	int16_t ALSMaxDuration;
	/**
	 * units: C
	 * offset 4152
	 */
	int8_t ALSMinCLT;
	/**
	 * units: C
	 * offset 4153
	 */
	int8_t ALSMaxCLT;
	/**
	 * offset 4154
	 */
	uint8_t alsMinTimeBetween;
	/**
	 * offset 4155
	 */
	uint8_t alsEtbPosition;
	/**
	 * units: %
	 * offset 4156
	 */
	uint8_t acRelayAlternatorDutyAdder;
	/**
	 * offset 4157
	 */
	SentEtbType sentEtbType;
	/**
	 * offset 4158
	 */
	uint16_t customSentTpsMin;
	/**
	 * units: %
	 * offset 4160
	 */
	int ALSIdleAdd;
	/**
	 * units: %
	 * offset 4164
	 */
	int ALSEtbAdd;
	/**
	 * offset 4168
	 */
	float ALSSkipRatio;
	/**
	 * units: %
	 * offset 4172
	 */
	uint8_t ALSMaxDriverThrottleIntent;
	/**
	 * offset 4173
	 */
	pin_input_mode_e ALSActivatePinMode;
	/**
	 * For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	 * units: %
	 * offset 4174
	 */
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	/**
	 * For Toyota ETCS-i, use ~69%
	 * units: %
	 * offset 4175
	 */
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	/**
	 * offset 4176
	 */
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT];
	/**
	 * offset 4184
	 */
	uint16_t customSentTpsMax;
	/**
	 * offset 4186
	 */
	uint16_t kLineBaudRate;
	/**
	 * offset 4188
	 */
	CanGpioType canGpioType;
	/**
	 * offset 4189
	 */
	UiMode uiMode;
	/**
	 * Crank angle ATDC of first lobe peak
	 * units: deg
	 * offset 4190
	 */
	int16_t hpfpPeakPos;
	/**
	 * units: us
	 * offset 4192
	 */
	int16_t kLinePeriodUs;
	/**
	 * Window that the correction will be added throughout (example, if rpm limit is 7000, and rpmSoftLimitWindowSize is 200, the corrections activate at 6800RPM, creating a 200rpm window)
	 * units: RPM
	 * offset 4194
	 */
	scaled_channel<uint8_t, 1, 10> rpmSoftLimitWindowSize;
	/**
	 * Degrees of timing REMOVED from actual timing during soft RPM limit window
	 * units: deg
	 * offset 4195
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitTimingRetard;
	/**
	 * % of fuel ADDED during window
	 * units: %
	 * offset 4196
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitFuelAdded;
	/**
	 * Hysterisis: if the hard limit is 7200rpm and rpmHardLimitHyst is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached
	 * units: RPM
	 * offset 4197
	 */
	scaled_channel<uint8_t, 1, 10> rpmHardLimitHyst;
	/**
	 * Time between bench test pulses
	 * units: ms
	 * offset 4198
	 */
	scaled_channel<uint16_t, 10, 1> benchTestOffTime;
	/**
	 * Hysterisis: if hard cut is 240kpa, and boostCutPressureHyst is 20, when the ECU sees 240kpa, fuel/ign will cut, and stay cut until 240-20=220kpa is reached
	 * units: kPa (absolute)
	 * offset 4200
	 */
	scaled_channel<uint8_t, 2, 1> boostCutPressureHyst;
	/**
	 * Boost duty cycle added by gear
	 * units: %
	 * offset 4201
	 */
	scaled_channel<uint8_t, 2, 1> gearBasedOpenLoopBoostAdder[TCU_GEAR_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4211
	 */
	uint8_t alignmentFill_at_4211[1];
	/**
	 * How many test bench pulses do you want
	 * offset 4212
	 */
	uint32_t benchTestCount;
	/**
	 * How long initial IAC adder is held before starting to decay.
	 * units: seconds
	 * offset 4216
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsHoldTime;
	/**
	 * How long it takes to remove initial IAC adder to return to normal idle.
	 * units: seconds
	 * offset 4217
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsDecayTime;
	/**
	 * offset 4218
	 */
	switch_input_pin_e tcu_rangeInput[RANGE_INPUT_COUNT];
	/**
	 * offset 4230
	 */
	pin_input_mode_e tcu_rangeInputMode[RANGE_INPUT_COUNT];
	/**
	 * Scale the reported vehicle speed value from CAN. Example: Parameter set to 1.1, CAN VSS reports 50kph, ECU will report 55kph instead.
	 * units: ratio
	 * offset 4236
	 */
	scaled_channel<uint16_t, 10000, 1> canVssScaling;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4238
	 */
	uint8_t alignmentFill_at_4238[2];
	/**
	 * offset 4240
	 */
	ThermistorConf oilTempSensor;
	/**
	 * offset 4272
	 */
	ThermistorConf fuelTempSensor;
	/**
	 * offset 4304
	 */
	ThermistorConf ambientTempSensor;
	/**
	 * offset 4336
	 */
	ThermistorConf compressorDischargeTemperature;
	/**
	 * Place the sensor before the throttle, but after any turbocharger/supercharger and intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 4368
	 */
	adc_channel_e throttleInletPressureChannel;
	/**
	 * Place the sensor after the turbocharger/supercharger, but before any intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 4369
	 */
	adc_channel_e compressorDischargePressureChannel;
	/**
	 * offset 4370
	 */
	Gpio dacOutputPins[DAC_OUTPUT_COUNT];
	/**
	 * offset 4374
	 */
	output_pin_e speedometerOutputPin;
	/**
	 * Number of speedometer pulses per kilometer travelled.
	 * offset 4376
	 */
	uint16_t speedometerPulsePerKm;
	/**
	 * offset 4378
	 */
	uint8_t simulatorCamPosition[CAM_INPUTS_COUNT];
	/**
	 * offset 4382
	 */
	adc_channel_e ignKeyAdcChannel;
	/**
	 * offset 4383
	 */
	pin_mode_e spi6MisoMode;
	/**
	 * units: ratio
	 * offset 4384
	 */
	float triggerVVTGapOverrideFrom[VVT_TRACKING_LENGTH];
	/**
	 * units: ratio
	 * offset 4400
	 */
	float triggerVVTGapOverrideTo[VVT_TRACKING_LENGTH];
	/**
	 * units: %
	 * offset 4416
	 */
	int8_t tractionControlEtbDrop[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE];
	/**
	 * If injector duty cycle hits this value, instantly cut fuel.
	 * units: %
	 * offset 4452
	 */
	uint8_t maxInjectorDutyInstant;
	/**
	 * If injector duty cycle hits this value for the specified delay time, cut fuel.
	 * units: %
	 * offset 4453
	 */
	uint8_t maxInjectorDutySustained;
	/**
	 * Timeout period for duty cycle over the sustained limit to trigger duty cycle protection.
	 * units: sec
	 * offset 4454
	 */
	scaled_channel<uint8_t, 10, 1> maxInjectorDutySustainedTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4455
	 */
	uint8_t alignmentFill_at_4455[1];
	/**
	 * offset 4456
	 */
	output_pin_e injectionPinsStage2[MAX_CYLINDER_COUNT];
	/**
	 * units: Deg
	 * offset 4480
	 */
	int8_t tractionControlTimingDrop[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE];
	/**
	 * units: %
	 * offset 4516
	 */
	int8_t tractionControlIgnitionSkip[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE];
	/**
	 * offset 4552
	 */
	float auxSpeed1Multiplier;
	/**
	 * offset 4556
	 */
	float brakeMeanEffectivePressureDifferential;
	/**
	 * offset 4560
	 */
	Gpio spi4mosiPin;
	/**
	 * offset 4562
	 */
	Gpio spi4misoPin;
	/**
	 * offset 4564
	 */
	Gpio spi4sckPin;
	/**
	 * offset 4566
	 */
	Gpio spi5mosiPin;
	/**
	 * offset 4568
	 */
	Gpio spi5misoPin;
	/**
	 * offset 4570
	 */
	Gpio spi5sckPin;
	/**
	 * offset 4572
	 */
	Gpio spi6mosiPin;
	/**
	 * offset 4574
	 */
	Gpio spi6misoPin;
	/**
	 * offset 4576
	 */
	Gpio spi6sckPin;
	/**
	 * offset 4578
	 */
	pin_mode_e spi4SckMode;
	/**
	 * offset 4579
	 */
	pin_mode_e spi4MosiMode;
	/**
	 * offset 4580
	 */
	pin_mode_e spi4MisoMode;
	/**
	 * offset 4581
	 */
	pin_mode_e spi5SckMode;
	/**
	 * offset 4582
	 */
	pin_mode_e spi5MosiMode;
	/**
	 * offset 4583
	 */
	pin_mode_e spi5MisoMode;
	/**
	 * offset 4584
	 */
	pin_mode_e spi6SckMode;
	/**
	 * offset 4585
	 */
	pin_mode_e spi6MosiMode;
	/**
	 * units: units
	 * offset 4586
	 */
	uint8_t unusedOftenChangesDuringFirmwareUpdate[120];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 4706
	 */
	uint8_t alignmentFill_at_4706[2];
};
static_assert(sizeof(engine_configuration_s) == 4708);

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
	 * units: Load
	 * offset 128
	 */
	uint16_t loadBins[8];
	/**
	 * units: RPM
	 * offset 144
	 */
	uint16_t rpmBins[8];
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
	scaled_channel<int16_t, 10, 1> blendBins[8];
	/**
	 * units: %
	 * offset 178
	 */
	scaled_channel<uint8_t, 2, 1> blendValues[8];
	/**
	 * need 4 byte alignment
	 * units: units
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
	 * units: target TPS position
	 * offset 4708
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	 * units: ETB duty cycle bias
	 * offset 4740
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	 * units: %
	 * offset 4772
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	 * units: Load
	 * offset 4836
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	 * units: RPM
	 * offset 4844
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	 * units: RPM
	 * offset 4852
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	 * units: ms
	 * offset 4868
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * CLT-based target RPM for automatic idle controller
	 * units: C
	 * offset 4884
	 */
	scaled_channel<int8_t, 1, 2> cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * units: RPM
	 * offset 4900
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * CLT-based timing correction
	 * units: C
	 * offset 4916
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	 * units: degree
	 * offset 4948
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * units: x
	 * offset 4980
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	 * units: y
	 * offset 5044
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	 * units: x
	 * offset 5108
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	 * units: y
	 * offset 5172
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	 * units: x
	 * offset 5236
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	 * units: y
	 * offset 5268
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	 * units: x
	 * offset 5300
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	 * units: y
	 * offset 5332
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	 * units: x
	 * offset 5364
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	 * units: y
	 * offset 5396
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	 * units: x
	 * offset 5428
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	 * units: y
	 * offset 5460
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	 * units: kPa
	 * offset 5492
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	 * units: RPM
	 * offset 5508
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	 * units: ratio
	 * offset 5524
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * units: Ratio
	 * offset 5588
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	 * units: %
	 * offset 5620
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: RPM
	 * offset 5652
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: deg
	 * offset 5660
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * RPM-based idle position for coasting
	 * units: RPM
	 * offset 5668
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE];
	/**
	 * RPM-based idle position for coasting
	 * units: %
	 * offset 5684
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 5700
	 */
	warning_message_t warning_message;
	/**
	 * units: C
	 * offset 5820
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	 * units: Seconds
	 * offset 5852
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	 * units: %
	 * offset 5884
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	 * units: Seconds
	 * offset 5916
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 5948
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT];
	/**
	 * units: RPM
	 * offset 6012
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 6020
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT];
	/**
	 * units: %
	 * offset 6084
	 */
	uint8_t boostTpsBins[BOOST_LOAD_COUNT];
	/**
	 * units: %
	 * offset 6092
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	 * units: %
	 * offset 6156
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	 * units: RPM
	 * offset 6164
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	 * units: C
	 * offset 6172
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	 * units: %
	 * offset 6204
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: RPM
	 * offset 6236
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: deg
	 * offset 6244
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * units: RPM
	 * offset 6276
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	/**
	 * units: load
	 * offset 6280
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE];
	/**
	 * units: %
	 * offset 6284
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	/**
	 * offset 6316
	 */
	lua_script_t luaScript;
	/**
	 * units: C
	 * offset 14316
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 14380
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	 * units: C
	 * offset 14444
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 14508
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 14572
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	 * units: C
	 * offset 14604
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 14636
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	 * units: counter
	 * offset 14668
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	 * units: C
	 * offset 14700
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	 * units: ratio
	 * offset 14764
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	 * units: kg/hour
	 * offset 14828
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	 * units: V
	 * offset 14956
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	 * units: deg
	 * offset 15084
	 */
	scaled_channel<int8_t, 10, 1> ignitionIatCorrTable[8][8];
	/**
	 * units: C
	 * offset 15148
	 */
	int8_t ignitionIatCorrTempBins[8];
	/**
	 * units: Load
	 * offset 15156
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[8];
	/**
	 * units: deg
	 * offset 15164
	 */
	int16_t injectionPhase[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	 * units: Load
	 * offset 15676
	 */
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 15708
	 */
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	 * units: onoff
	 * offset 15740
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	 * units: kPa
	 * offset 15800
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	 * units: % TPS
	 * offset 16312
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 16344
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	 * units: value
	 * offset 16376
	 */
	int8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	 * units: L
	 * offset 16440
	 */
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 16456
	 */
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	/**
	 * units: value
	 * offset 16472
	 */
	int8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	 * units: L
	 * offset 16536
	 */
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 16552
	 */
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	/**
	 * units: deg
	 * offset 16568
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT];
	/**
	 * units: Load
	 * offset 17080
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 17112
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	 * units: %
	 * offset 17144
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	 * units: kPa
	 * offset 17656
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 17688
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	 * units: lambda
	 * offset 17720
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	 * offset 17976
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 18008
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	 * units: value
	 * offset 18040
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	 * units: from
	 * offset 18296
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * units: to
	 * offset 18328
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * units: value
	 * offset 18360
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	 * units: L
	 * offset 18616
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 18632
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	 * units: value
	 * offset 18648
	 */
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	 * units: L
	 * offset 18712
	 */
	int16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 18728
	 */
	int16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	/**
	 * units: value
	 * offset 18744
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	 * units: L
	 * offset 18808
	 */
	int16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 18824
	 */
	int16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	 * units: value
	 * offset 18840
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][TABLE_4_RPM];
	/**
	 * units: L
	 * offset 18920
	 */
	int16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 18936
	 */
	int16_t scriptTable4RpmBins[TABLE_4_RPM];
	/**
	 * offset 18956
	 */
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	/**
	 * units: rpm
	 * offset 18964
	 */
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 18972
	 */
	cyl_trim_s ignTrims[12];
	/**
	 * offset 19164
	 */
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	/**
	 * units: rpm
	 * offset 19172
	 */
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 19180
	 */
	cyl_trim_s fuelTrims[12];
	/**
	 * units: ratio
	 * offset 19372
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	/**
	 * units: Airmass
	 * offset 19388
	 */
	scaled_channel<uint8_t, 50, 1> tcu_pcAirmassBins[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19396
	 */
	uint8_t tcu_pcValsR[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19404
	 */
	uint8_t tcu_pcValsN[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19412
	 */
	uint8_t tcu_pcVals1[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19420
	 */
	uint8_t tcu_pcVals2[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19428
	 */
	uint8_t tcu_pcVals3[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19436
	 */
	uint8_t tcu_pcVals4[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19444
	 */
	uint8_t tcu_pcVals12[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19452
	 */
	uint8_t tcu_pcVals23[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19460
	 */
	uint8_t tcu_pcVals34[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19468
	 */
	uint8_t tcu_pcVals21[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19476
	 */
	uint8_t tcu_pcVals32[TCU_MAGIC_SIZE];
	/**
	 * units: %
	 * offset 19484
	 */
	uint8_t tcu_pcVals43[TCU_MAGIC_SIZE];
	/**
	 * units: TPS
	 * offset 19492
	 */
	uint8_t tcu_tccTpsBins[8];
	/**
	 * units: MPH
	 * offset 19500
	 */
	uint8_t tcu_tccLockSpeed[8];
	/**
	 * units: MPH
	 * offset 19508
	 */
	uint8_t tcu_tccUnlockSpeed[8];
	/**
	 * units: KPH
	 * offset 19516
	 */
	uint8_t tcu_32SpeedBins[8];
	/**
	 * units: %
	 * offset 19524
	 */
	uint8_t tcu_32Vals[8];
	/**
	 * units: %
	 * offset 19532
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[ETB2_TRIM_SIZE][ETB2_TRIM_SIZE];
	/**
	 * units: %
	 * offset 19568
	 */
	uint8_t throttle2TrimTpsBins[ETB2_TRIM_SIZE];
	/**
	 * units: RPM
	 * offset 19574
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[ETB2_TRIM_SIZE];
	/**
	 * units: deg
	 * offset 19580
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[6][6];
	/**
	 * units: %
	 * offset 19616
	 */
	uint8_t maxKnockRetardLoadBins[6];
	/**
	 * units: RPM
	 * offset 19622
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[6];
	/**
	 * units: deg
	 * offset 19628
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[4][4];
	/**
	 * units: TPS
	 * offset 19660
	 */
	uint16_t alsIgnRetardLoadBins[4];
	/**
	 * units: RPM
	 * offset 19668
	 */
	uint16_t alsIgnRetardrpmBins[4];
	/**
	 * units: percent
	 * offset 19676
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[4][4];
	/**
	 * units: TPS
	 * offset 19708
	 */
	uint16_t alsFuelAdjustmentLoadBins[4];
	/**
	 * units: RPM
	 * offset 19716
	 */
	uint16_t alsFuelAdjustmentrpmBins[4];
	/**
	 * units: ratio
	 * offset 19724
	 */
	scaled_channel<int16_t, 1, 10> ALSIgnSkipTable[4][4];
	/**
	 * units: TPS
	 * offset 19756
	 */
	uint16_t alsIgnSkipLoadBins[4];
	/**
	 * units: RPM
	 * offset 19764
	 */
	uint16_t alsIgnSkiprpmBins[4];
	/**
	 * offset 19772
	 */
	blend_table_s ignBlends[IGN_BLEND_COUNT];
	/**
	 * offset 20524
	 */
	blend_table_s veBlends[VE_BLEND_COUNT];
	/**
	 * units: %
	 * offset 21276
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaBins[12];
	/**
	 * In units of g/s normalized to choked flow conditions
	 * units: g/s
	 * offset 21300
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaValues[12];
	/**
	 * offset 21324
	 */
	blend_table_s boostOpenLoopBlends[BOOST_BLEND_COUNT];
	/**
	 * offset 21700
	 */
	blend_table_s boostClosedLoopBlends[BOOST_BLEND_COUNT];
	/**
	 * units: level
	 * offset 22076
	 */
	uint8_t tcu_rangeP[6];
	/**
	 * units: level
	 * offset 22082
	 */
	uint8_t tcu_rangeR[6];
	/**
	 * units: level
	 * offset 22088
	 */
	uint8_t tcu_rangeN[6];
	/**
	 * units: level
	 * offset 22094
	 */
	uint8_t tcu_rangeD[6];
	/**
	 * units: level
	 * offset 22100
	 */
	uint8_t tcu_rangeM[6];
	/**
	 * units: level
	 * offset 22106
	 */
	uint8_t tcu_rangeM3[6];
	/**
	 * units: level
	 * offset 22112
	 */
	uint8_t tcu_rangeM2[6];
	/**
	 * units: level
	 * offset 22118
	 */
	uint8_t tcu_rangeM1[6];
	/**
	 * units: level
	 * offset 22124
	 */
	uint8_t tcu_rangePlus[6];
	/**
	 * units: level
	 * offset 22130
	 */
	uint8_t tcu_rangeMinus[6];
	/**
	 * units: level
	 * offset 22136
	 */
	uint8_t tcu_rangeLow[6];
	/**
	 * units: lambda
	 * offset 22142
	 */
	scaled_channel<uint8_t, 100, 1> lambdaMaxDeviationTable[4][4];
	/**
	 * offset 22158
	 */
	uint16_t lambdaMaxDeviationLoadBins[4];
	/**
	 * units: RPM
	 * offset 22166
	 */
	uint16_t lambdaMaxDeviationRpmBins[4];
	/**
	 * units: %
	 * offset 22174
	 */
	uint8_t injectorStagingTable[INJ_STAGING_COUNT][INJ_STAGING_COUNT];
	/**
	 * offset 22210
	 */
	uint16_t injectorStagingLoadBins[INJ_STAGING_COUNT];
	/**
	 * units: RPM
	 * offset 22222
	 */
	uint16_t injectorStagingRpmBins[INJ_STAGING_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 22234
	 */
	uint8_t alignmentFill_at_22234[2];
};
static_assert(sizeof(persistent_config_s) == 22236);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt Tue Feb 27 01:11:06 UTC 2024
