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
	int8_t maxAdd;
	/**
	 * Maximum % that the short term fuel trim can remove
	 * units: %
	 * offset 1
	 */
	int8_t maxRemove;
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
	 * When tuning by MAP the units are kPa, e.g. 30 would mean 30kPa. When tuning TPS, 30 would be 30%
	 * units: load
	 * offset 2
	 */
	uint16_t maxOverrunLoad;
	/**
	 * Above this engine load, the power region is active
	 * When tuning by MAP the units are kPa
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
	 * units: C
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
	uint8_t alignmentFill_at_6[2] = {};
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
	 * offset 28
	 */
	scaled_channel<int16_t, 100, 1> battLagCorrTable[VBAT_INJECTOR_CURVE_PRESSURE_SIZE][VBAT_INJECTOR_CURVE_SIZE] = {};
};
static_assert(sizeof(injector_s) == 60);

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
	 * offset 20
	 */
	uint16_t unusedWastgate0;
	/**
	 * offset 22
	 */
	uint16_t unusedWastgate1;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	 * units: mv
	 * offset 24
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	 * units: mv
	 * offset 26
	 */
	uint16_t idlePositionMax;
	/**
	 * offset 28
	 */
	output_pin_e mainRelayPin;
	/**
	 * offset 30
	 */
	Gpio sdCardCsPin;
	/**
	 * offset 32
	 */
	Gpio canTxPin;
	/**
	 * offset 34
	 */
	Gpio canRxPin;
	/**
	 * Pin that activates the reduction/cut for shifting. Sometimes shared with the Launch Control pin
	 * offset 36
	 */
	switch_input_pin_e torqueReductionTriggerPin;
	/**
	 * units: %
	 * offset 38
	 */
	int8_t launchFuelAdderPercent;
	/**
	 * Time after which the throttle is considered jammed.
	 * units: sec
	 * offset 39
	 */
	scaled_channel<uint8_t, 50, 1> etbJamTimeout;
	/**
	 * offset 40
	 */
	output_pin_e tachOutputPin;
	/**
	 * offset 42
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * Additional idle % while A/C is active
	 * units: %
	 * offset 43
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	 * units: deg
	 * offset 44
	 */
	uint8_t multisparkMaxSparkingAngle;
	/**
	 * Configures the maximum number of extra sparks to fire (does not include main spark)
	 * units: count
	 * offset 45
	 */
	uint8_t multisparkMaxExtraSparkCount;
	/**
	 * units: RPM
	 * offset 46
	 */
	int16_t vvtControlMinRpm;
	/**
	 * offset 48
	 */
	injector_s injector;
	/**
	 * offset 108
	 */
	injector_s injectorSecondary;
	/**
	 * Does the vehicle have a turbo or supercharger?
	offset 168 bit 0 */
	bool isForcedInduction : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 168 bit 1 */
	bool useFordRedundantTps : 1 {};
	/**
	offset 168 bit 2 */
	bool enableKline : 1 {};
	/**
	offset 168 bit 3 */
	bool overrideTriggerGaps : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 168 bit 4 */
	bool enableFan1WithAc : 1 {};
	/**
	 * Turn on this fan when AC is on.
	offset 168 bit 5 */
	bool enableFan2WithAc : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 168 bit 6 */
	bool disableFan1WhenStopped : 1 {};
	/**
	 * Inhibit operation of this fan while the engine is not running.
	offset 168 bit 7 */
	bool disableFan2WhenStopped : 1 {};
	/**
	 * Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	offset 168 bit 8 */
	bool enableTrailingSparks : 1 {};
	/**
	 * TLE7209 and L6205 use two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	offset 168 bit 9 */
	bool etb_use_two_wires : 1 {};
	/**
	 * Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	offset 168 bit 10 */
	bool isDoubleSolenoidIdle : 1 {};
	/**
	offset 168 bit 11 */
	bool useEeprom : 1 {};
	/**
	 * Switch between Industrial and Cic PID implementation
	offset 168 bit 12 */
	bool useCicPidForIdle : 1 {};
	/**
	offset 168 bit 13 */
	bool useTLE8888_cranking_hack : 1 {};
	/**
	offset 168 bit 14 */
	bool kickStartCranking : 1 {};
	/**
	 * This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	offset 168 bit 15 */
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	/**
	offset 168 bit 16 */
	bool launchControlEnabled : 1 {};
	/**
	offset 168 bit 17 */
	bool tempPumpLimitCheck : 1 {};
	/**
	offset 168 bit 18 */
	bool antiLagEnabled : 1 {};
	/**
	 * For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	offset 168 bit 19 */
	bool useRunningMathForCranking : 1 {};
	/**
	 * Shall we display real life signal or just the part consumed by trigger decoder.
	 * Applies to both trigger and cam/vvt input.
	offset 168 bit 20 */
	bool displayLogicLevelsInEngineSniffer : 1 {};
	/**
	offset 168 bit 21 */
	bool useTLE8888_stepper : 1 {};
	/**
	offset 168 bit 22 */
	bool usescriptTableForCanSniffingFiltering : 1 {};
	/**
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
	offset 168 bit 23 */
	bool verboseCan : 1 {};
	/**
	 * Experimental setting that will cause a misfire
	 * DO NOT ENABLE.
	offset 168 bit 24 */
	bool artificialTestMisfire : 1 {};
	/**
	 * On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	offset 168 bit 25 */
	bool useFordRedundantPps : 1 {};
	/**
	offset 168 bit 26 */
	bool cltSensorPulldown : 1 {};
	/**
	offset 168 bit 27 */
	bool iatSensorPulldown : 1 {};
	/**
	offset 168 bit 28 */
	bool allowIdenticalPps : 1 {};
	/**
	offset 168 bit 29 */
	bool overrideVvtTriggerGaps : 1 {};
	/**
	 * If enabled - use onboard SPI Accelerometer, otherwise listen for CAN messages
	offset 168 bit 30 */
	bool useSpiImu : 1 {};
	/**
	offset 168 bit 31 */
	bool enableStagedInjection : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * set tps_min X
	 * units: ADC
	 * offset 172
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * set tps_max X
	 * units: ADC
	 * offset 174
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 176
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 178
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 180
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	 * units: ms
	 * offset 188
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	 * units: rpm
	 * offset 192
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	 * units: rpm
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
	 * offset 336
	 */
	ThermistorConf clt;
	/**
	 * offset 368
	 */
	ThermistorConf iat;
	/**
	 * units: deg
	 * offset 400
	 */
	float launchTimingRetard;
	/**
	 * Maximum commanded airmass for the idle controller.
	 * units: mg
	 * offset 404
	 */
	scaled_channel<uint8_t, 1, 2> idleMaximumAirmass;
	/**
	 * Zero value means do not detect tuning
	 * units: seconds
	 * offset 405
	 */
	uint8_t tuningDetector;
	/**
	 * iTerm min value
	 * offset 406
	 */
	int16_t alternator_iTermMin;
	/**
	 * iTerm max value
	 * offset 408
	 */
	int16_t alternator_iTermMax;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 410
	 */
	uint8_t alignmentFill_at_410[2] = {};
	/**
	 * @@DISPLACEMENT_TOOLTIP@@
	 * units: L
	 * offset 412
	 */
	scaled_channel<float, 1000, 1> displacement;
	/**
	 * units: RPM
	 * offset 416
	 */
	uint16_t triggerSimulatorRpm;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 418
	 */
	uint8_t alignmentFill_at_418[2] = {};
	/**
	 * Number of cylinder the engine has.
	 * offset 420
	 */
	uint32_t cylindersCount;
	/**
	 * offset 424
	 */
	firing_order_e firingOrder;
	/**
	 * offset 425
	 */
	uint8_t justATempTest;
	/**
	 * Delta kPa for MAP sync
	 * units: kPa
	 * offset 426
	 */
	uint8_t mapSyncThreshold;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 427
	 */
	uint8_t alignmentFill_at_427[1] = {};
	/**
	 * @@CYLINDER_BORE_TOOLTIP@@
	 * units: mm
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
	 * units: %
	 * offset 433
	 */
	uint8_t ALSMaxTPS;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 434
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups.
	 * offset 435
	 */
	injection_mode_e injectionMode;
	/**
	 * Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 436
	 */
	uint16_t boostControlMinRpm;
	/**
	 * Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 438
	 */
	uint8_t boostControlMinTps;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 439
	 */
	uint8_t alignmentFill_at_439[1] = {};
	/**
	 * Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 440
	 */
	uint16_t boostControlMinMap;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 442
	 */
	uint8_t alignmentFill_at_442[2] = {};
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	 * units: deg
	 * offset 444
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 448
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 449
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	 * units: kph
	 * offset 450
	 */
	uint8_t maxIdleVss;
	/**
	 * Allowed range around detection position
	 * offset 451
	 */
	uint8_t camDecoder2jzPrecision;
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	 * units: kPa
	 * offset 452
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing
	 * Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 454
	 */
	timing_mode_e timingMode;
	/**
	 * offset 455
	 */
	can_nbc_e canNbcType;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	 * units: RPM
	 * offset 456
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
	 * offset 460
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	 * units: coef
	 * offset 464
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	 * units: coef
	 * offset 468
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: deg C
	 * offset 472
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: deg C
	 * offset 476
	 */
	float fanOffTemperature;
	/**
	 * offset 480
	 */
	output_pin_e acrPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 482
	 */
	uint8_t alignmentFill_at_482[2] = {};
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	 * units: revs/km
	 * offset 484
	 */
	float driveWheelRevPerKm;
	/**
	 * CANbus thread period in ms
	 * units: ms
	 * offset 488
	 */
	int canSleepPeriodMs;
	/**
	 * units: index
	 * offset 492
	 */
	int byFirmwareVersion;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
	 * offset 496
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 497
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 498
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * offset 499
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 500
	 */
	float idle_derivativeFilterLoss;
	/**
	 * offset 504
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	 * units: %
	 * offset 516
	 */
	float airByRpmTaper;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	 * units: %
	 * offset 520
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 521
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 522
	 */
	uint8_t acrRevolutions;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 523
	 */
	uint8_t alignmentFill_at_523[1] = {};
	/**
	 * offset 524
	 */
	int calibrationBirthday;
	/**
	 * units: volts
	 * offset 528
	 */
	float adcVcc;
	/**
	 * Magic engine phase: we compare instant MAP at X to instant MAP at x+360 angle in one complete cycle
	 * units: Deg
	 * offset 532
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 536
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 544
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 564
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * TPS/PPS error threshold
	 * units: %
	 * offset 565
	 */
	scaled_channel<uint8_t, 10, 1> etbSplit;
	/**
	 * offset 566
	 */
	Gpio tle6240_cs;
	/**
	 * offset 568
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * offset 569
	 */
	pin_output_mode_e mc33810_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 570
	 */
	uint8_t alignmentFill_at_570[2] = {};
	/**
	 * @see hasBaroSensor
	 * offset 572
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 584
	 */
	idle_hardware_s idle;
	/**
	 * Ignition timing to remove when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 596
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	 * units: deg/s
	 * offset 597
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 598
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * offset 599
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	 * units: ratio
	 * offset 600
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 602
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	 * units: count
	 * offset 603
	 */
	uint8_t vssToothCount;
	/**
	 * Override the Y axis (load) value used for only the Idle VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 604
	 */
	ve_override_e idleVeOverrideMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 605
	 */
	uint8_t alignmentFill_at_605[1] = {};
	/**
	 * offset 606
	 */
	Gpio l9779_cs;
	/**
	 * offset 608
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 632
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT] = {};
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
	output_pin_e fuelPumpPin;
	/**
	 * offset 660
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * How many consecutive VVT gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 661
	 */
	int8_t gapVvtTrackingLengthOverride;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 662
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 664
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 665
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 666
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 668
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 670
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 672
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 673
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 674
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT] = {};
	/**
	 * offset 682
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 683
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 684
	 */
	Gpio debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 686
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 687
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 688
	 */
	Gpio mc33972_cs;
	/**
	 * offset 690
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 691
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 692
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 693
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 694
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 695
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 696
	 */
	Gpio mc33810_cs[C_MC33810_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 698
	 */
	uint8_t alignmentFill_at_698[2] = {};
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 700
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 704
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT] = {};
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	 * units: us
	 * offset 708
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * Ratio between the wheels and your transmission output.
	 * units: ratio
	 * offset 710
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 712
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 714
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 715
	 */
	uint8_t alignmentFill_at_715[1] = {};
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 716
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * units: g/s
	 * offset 720
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 722
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT] = {};
	/**
	 * offset 724
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 725
	 */
	uint8_t alignmentFill_at_725[1] = {};
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 726
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 728
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * units: RPM
	 * offset 729
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionMinRpm;
	/**
	 * units: %
	 * offset 730
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionMinLoad;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 731
	 */
	uint8_t alignmentFill_at_731[1] = {};
	/**
	offset 732 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 732 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 732 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 732 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 732 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 732 bit 5 */
	bool rethrowHardFault : 1 {};
	/**
	offset 732 bit 6 */
	bool requireFootOnBrakeToCrank : 1 {};
	/**
	offset 732 bit 7 */
	bool verboseQuad : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 732 bit 8 */
	bool useStepperIdle : 1 {};
	/**
	offset 732 bit 9 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 732 bit 10 */
	bool lambdaProtectionEnable : 1 {};
	/**
	offset 732 bit 11 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	offset 732 bit 12 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 732 bit 13 */
	bool externalRusEfiGdiModule : 1 {};
	/**
	offset 732 bit 14 */
	bool flipWboChannels : 1 {};
	/**
	 * Useful for individual intakes
	offset 732 bit 15 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 732 bit 16 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 732 bit 17 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 732 bit 18 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 732 bit 19 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 732 bit 20 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 732 bit 21 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 732 bit 22 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 732 bit 23 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 732 bit 24 */
	bool pauseEtbControl : 1 {};
	/**
	offset 732 bit 25 */
	bool tpsTpsPercentMode : 1 {};
	/**
	offset 732 bit 26 */
	bool verboseKLine : 1 {};
	/**
	offset 732 bit 27 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 732 bit 28 */
	bool enableAemXSeries : 1 {};
	/**
	offset 732 bit 29 */
	bool modeledFlowIdle : 1 {};
	/**
	offset 732 bit 30 */
	bool unusedBit_227_30 : 1 {};
	/**
	offset 732 bit 31 */
	bool unusedBit_227_31 : 1 {};
	/**
	 * offset 736
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT] = {};
	/**
	 * offset 744
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 745
	 */
	uint8_t alignmentFill_at_745[3] = {};
	/**
	 * offset 748
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	 * units: v
	 * offset 752
	 */
	uint8_t mc33_hvolt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 753
	 */
	uint8_t alignmentFill_at_753[1] = {};
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	 * units: kPa
	 * offset 754
	 */
	uint16_t minimumBoostClosedLoopMap;
	/**
	 * units: %
	 * offset 756
	 */
	int8_t initialIgnitionCutPercent;
	/**
	 * units: %
	 * offset 757
	 */
	int8_t finalIgnitionCutPercentBeforeLaunch;
	/**
	 * offset 758
	 */
	gppwm_channel_e boostOpenLoopYAxis;
	/**
	 * offset 759
	 */
	spi_device_e l9779spiDevice;
	/**
	 * offset 760
	 */
	imu_type_e imuType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 761
	 */
	uint8_t alignmentFill_at_761[1] = {};
	/**
	 * How far above idle speed do we consider idling, i.e. coasting detection threshold.
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	 * units: RPM
	 * offset 762
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	 * units: ms
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
	Gpio max31855_cs[EGT_CHANNEL_COUNT] = {};
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 792
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * Since torque reduction pin is usually shared with launch control, most people have an RPM where behavior under that is Launch Control, over that is Flat Shift/Torque Reduction
	 * units: rpm
	 * offset 794
	 */
	uint16_t torqueReductionArmingRpm;
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
	 * units: :1
	 * offset 798
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	 * units: %
	 * offset 799
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	 * units: hz
	 * offset 800
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 802
	 */
	adc_channel_e idlePositionChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 803
	 */
	uint8_t alignmentFill_at_803[1] = {};
	/**
	 * offset 804
	 */
	uint16_t launchCorrectionsEndRpm;
	/**
	 * offset 806
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 808
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 809
	 */
	uint8_t alignmentFill_at_809[1] = {};
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 810
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 812
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * units: RPM
	 * offset 814
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionRestoreRpm;
	/**
	 * offset 815
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	 * units: count
	 * offset 816
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	 * units: %
	 * offset 820
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * units: %
	 * offset 822
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * units: ADC
	 * offset 824
	 */
	uint16_t tps1SecondaryMin;
	/**
	 * units: ADC
	 * offset 826
	 */
	uint16_t tps1SecondaryMax;
	/**
	 * units: rpm
	 * offset 828
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	 * units: Seconds
	 * offset 830
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 832
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 834
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 835
	 */
	uint8_t alignmentFill_at_835[1] = {};
	/**
	 * offset 836
	 */
	output_pin_e acRelayPin;
	/**
	 * units: %
	 * offset 838
	 */
	uint8_t lambdaProtectionMinTps;
	/**
	 * Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed
	 * units: s
	 * offset 839
	 */
	scaled_channel<uint8_t, 10, 1> lambdaProtectionTimeout;
	/**
	 * offset 840
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT] = {};
	/**
	 * offset 872
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 874
	 */
	Gpio spi1misoPin;
	/**
	 * offset 876
	 */
	Gpio spi1sckPin;
	/**
	 * offset 878
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 880
	 */
	Gpio spi2misoPin;
	/**
	 * offset 882
	 */
	Gpio spi2sckPin;
	/**
	 * offset 884
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 886
	 */
	Gpio spi3misoPin;
	/**
	 * offset 888
	 */
	Gpio spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 890
	 */
	Gpio cdmInputPin;
	/**
	 * offset 892
	 */
	uart_device_e consoleUartDevice;
	/**
	 * offset 893
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 894
	 */
	uint8_t alignmentFill_at_894[2] = {};
	/**
	 * Ramp the idle target down from the entry threshold over N seconds when returning to idle. Helps prevent overshooting (below) the idle target while returning to idle from coasting.
	offset 896 bit 0 */
	bool idleReturnTargetRamp : 1 {};
	/**
	offset 896 bit 1 */
	bool useInjectorFlowLinearizationTable : 1 {};
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
	bool canInputBCM : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	offset 896 bit 6 */
	bool consumeObdSensors : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 896 bit 7 */
	bool enableCanVss : 1 {};
	/**
	offset 896 bit 8 */
	bool suppressWboWorkaround7048 : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 896 bit 9 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 896 bit 10 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 896 bit 11 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 896 bit 12 */
	bool verboseVVTDecoding : 1 {};
	/**
	offset 896 bit 13 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * In Alpha-N mode, compensate for air temperature.
	offset 896 bit 14 */
	bool alphaNUseIat : 1 {};
	/**
	offset 896 bit 15 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 896 bit 16 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 896 bit 17 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 896 bit 18 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 896 bit 19 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 896 bit 20 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 896 bit 21 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 896 bit 22 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 896 bit 23 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 896 bit 24 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 896 bit 25 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 896 bit 26 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 896 bit 27 */
	bool tcuEnabled : 1 {};
	/**
	offset 896 bit 28 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coils
	offset 896 bit 29 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 896 bit 30 */
	bool verboseCan2 : 1 {};
	/**
	offset 896 bit 31 */
	bool unusedBit_326_31 : 1 {};
	/**
	 * offset 900
	 */
	dc_io etbIo[ETB_COUNT] = {};
	/**
	 * Wastegate control Solenoid, set to 'NONE' if you are using DC wastegate
	 * offset 916
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 918
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 919
	 */
	uint8_t alignmentFill_at_919[1] = {};
	/**
	 * offset 920
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 922
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 924
	 */
	pid_s boostPid;
	/**
	 * offset 944
	 */
	boostType_e boostType;
	/**
	 * offset 945
	 */
	pin_input_mode_e ignitionKeyDigitalPinMode;
	/**
	 * offset 946
	 */
	Gpio ignitionKeyDigitalPin;
	/**
	 * units: Hz
	 * offset 948
	 */
	int boostPwmFrequency;
	/**
	 * offset 952
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 953
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	 * units: sec
	 * offset 954
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	 * units: sec
	 * offset 955
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Launch disabled above this speed if setting is above zero
	 * units: Kph
	 * offset 956
	 */
	int launchSpeedThreshold;
	/**
	 * Starting Launch RPM window to activate (subtracts from Launch RPM)
	 * units: RPM
	 * offset 960
	 */
	int launchRpmWindow;
	/**
	 * units: ms
	 * offset 964
	 */
	float triggerEventsTimeoutMs;
	/**
	 * offset 968
	 */
	float mapExpAverageAlpha;
	/**
	 * offset 972
	 */
	float magicNumberAvailableForDevTricks;
	/**
	 * offset 976
	 */
	float turbochargerFilter;
	/**
	 * offset 980
	 */
	int launchTpsThreshold;
	/**
	 * offset 984
	 */
	float launchActivateDelay;
	/**
	 * offset 988
	 */
	stft_s stft;
	/**
	 * offset 1016
	 */
	dc_io stepperDcIo[DC_PER_STEPPER] = {};
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1032
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1064
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1096
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1128
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT] = {};
	/**
	 * offset 1140
	 */
	dc_function_e etbFunctions[ETB_COUNT] = {};
	/**
	 * offset 1142
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1143
	 */
	uint8_t alignmentFill_at_1143[1] = {};
	/**
	 * offset 1144
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1146
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1147
	 */
	uint8_t alignmentFill_at_1147[1] = {};
	/**
	 * offset 1148
	 */
	Gpio drv8860_miso;
	/**
	 * offset 1150
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1166
	 */
	uint8_t alignmentFill_at_1166[2] = {};
	/**
	 * Angle between cam sensor and VVT zero position
	 * units: value
	 * offset 1168
	 */
	float vvtOffsets[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1184
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT] = {};
	/**
	 * offset 1216
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS] = {};
	/**
	 * units: ADC
	 * offset 1280
	 */
	uint16_t tps2SecondaryMin;
	/**
	 * units: ADC
	 * offset 1282
	 */
	uint16_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1284 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1284 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Write SD card log even when powered by USB
	offset 1284 bit 2 */
	bool alwaysWriteSdCard : 1 {};
	/**
	 * Second harmonic (aka double) is usually quieter background noise
	offset 1284 bit 3 */
	bool knockDetectionUseDoubleFrequency : 1 {};
	/**
	offset 1284 bit 4 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1284 bit 5 */
	bool silentTriggerError : 1 {};
	/**
	offset 1284 bit 6 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1284 bit 7 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write. See also can1ListenMode
	offset 1284 bit 8 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1284 bit 9 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1284 bit 10 */
	bool enableOilPressureProtect : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1284 bit 11 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1284 bit 12 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1284 bit 13 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1284 bit 14 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1284 bit 15 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1284 bit 16 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1284 bit 17 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1284 bit 18 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1284 bit 19 */
	bool torqueReductionEnabled : 1 {};
	/**
	offset 1284 bit 20 */
	bool camSyncOnSecondCrankRevolution : 1 {};
	/**
	offset 1284 bit 21 */
	bool limitTorqueReductionTime : 1 {};
	/**
	 * Are you a developer troubleshooting TS over CAN ISO/TP?
	offset 1284 bit 22 */
	bool verboseIsoTp : 1 {};
	/**
	 * In this mode only trigger events go into engine sniffer and not coils/injectors etc
	offset 1284 bit 23 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1284 bit 24 */
	bool unused1308_24 : 1 {};
	/**
	offset 1284 bit 25 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1284 bit 26 */
	bool skippedWheelOnCam : 1 {};
	/**
	offset 1284 bit 27 */
	bool unusedBit_397_27 : 1 {};
	/**
	offset 1284 bit 28 */
	bool unusedBit_397_28 : 1 {};
	/**
	offset 1284 bit 29 */
	bool unusedBit_397_29 : 1 {};
	/**
	offset 1284 bit 30 */
	bool unusedBit_397_30 : 1 {};
	/**
	offset 1284 bit 31 */
	bool unusedBit_397_31 : 1 {};
	/**
	 * A/C button input
	 * offset 1288
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1290
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	 * units: %
	 * offset 1291
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1292
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1293
	 */
	uint8_t alignmentFill_at_1293[3] = {};
	/**
	offset 1296 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1296 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1296 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1296 bit 3 */
	bool complexWallModel : 1 {};
	/**
	 * RPM is measured based on last 720 degrees while instant RPM is measured based on the last 90 degrees of crank revolution
	offset 1296 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1296 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1296 bit 6 */
	bool unusedOverrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra advance at low idle speeds will prevent stalling and extra retard at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1296 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1296 bit 8 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1296 bit 9 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	offset 1296 bit 10 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Measure actual dt for PID instead of pre-defined
	offset 1296 bit 11 */
	bool etbUsePreciseTiming : 1 {};
	/**
	offset 1296 bit 12 */
	bool hondaK : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1296 bit 13 */
	bool twoWireBatchIgnition : 1 {};
	/**
	 * Read MAP sensor on ECU start-up to use as baro value.
	offset 1296 bit 14 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1296 bit 15 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT and PID idle).
	 * You probably don't need this.
	offset 1296 bit 16 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1296 bit 17 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1296 bit 18 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1296 bit 19 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1296 bit 20 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1296 bit 21 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1296 bit 22 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors.
	 * WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1296 bit 23 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1296 bit 24 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1296 bit 25 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1296 bit 26 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	offset 1296 bit 27 */
	bool sdTriggerLog : 1 {};
	/**
	offset 1296 bit 28 */
	bool unused1320_28 : 1 {};
	/**
	offset 1296 bit 29 */
	bool stepper_dc_use_two_wires : 1 {};
	/**
	offset 1296 bit 30 */
	bool watchOutForLinearTime : 1 {};
	/**
	offset 1296 bit 31 */
	bool unusedBit_438_31 : 1 {};
	/**
	 * units: count
	 * offset 1300
	 */
	uint32_t engineChartSize;
	/**
	 * units: mult
	 * offset 1304
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1308
	 */
	Gpio camInputsDebug[CAM_INPUTS_COUNT] = {};
	/**
	 * Idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	 * units: RPM
	 * offset 1316
	 */
	int16_t acIdleRpmTarget;
	/**
	 * set warningPeriod X
	 * units: seconds
	 * offset 1318
	 */
	int16_t warningPeriod;
	/**
	 * units: angle
	 * offset 1320
	 */
	float knockDetectionWindowStart;
	/**
	 * units: ms
	 * offset 1324
	 */
	float idleStepperReactionTime;
	/**
	 * units: count
	 * offset 1328
	 */
	int idleStepperTotalSteps;
	/**
	 * Pedal position to realize that we need to reduce torque when the trigger pin is uuuh triggered
	 * offset 1332
	 */
	int torqueReductionArmingApp;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1336
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	 * units: Seconds
	 * offset 1340
	 */
	float wwaeTau;
	/**
	 * offset 1344
	 */
	pid_s alternatorControl;
	/**
	 * offset 1364
	 */
	pid_s etb;
	/**
	 * offset 1384
	 */
	Gpio triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT] = {};
	/**
	 * RPM range above upper limit for extra air taper
	 * units: RPM
	 * offset 1388
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1390
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * units: ADC
	 * offset 1392
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * units: ADC
	 * offset 1394
	 */
	int16_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1396
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1398
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1399
	 */
	uint8_t alignmentFill_at_1399[1] = {};
	/**
	 * offset 1400
	 */
	Gpio mc33816_flag0;
	/**
	 * offset 1402
	 */
	scaled_channel<uint16_t, 1000, 1> tachPulsePerRev;
	/**
	 * kPa value which is too low to be true
	 * units: kPa
	 * offset 1404
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	 * units: kPa
	 * offset 1408
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	 * units: ms
	 * offset 1412
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	 * units: ms
	 * offset 1414
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1416
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	 * units: Fraction
	 * offset 1436
	 */
	float wwaeBeta;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1440
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1442
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1444
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT] = {};
	/**
	 * offset 1448
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1450
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * units: voltage
	 * offset 1452
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * units: voltage
	 * offset 1456
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	 * units: seconds
	 * offset 1460
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	 * units: RPM
	 * offset 1462
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * See Over/Undervoltage Shutdown/Retry bit in documentation
	offset 1464 bit 0 */
	bool mc33810DisableRecoveryMode : 1 {};
	/**
	offset 1464 bit 1 */
	bool mc33810Gpgd0Mode : 1 {};
	/**
	offset 1464 bit 2 */
	bool mc33810Gpgd1Mode : 1 {};
	/**
	offset 1464 bit 3 */
	bool mc33810Gpgd2Mode : 1 {};
	/**
	offset 1464 bit 4 */
	bool mc33810Gpgd3Mode : 1 {};
	/**
	 * Send out board statistics
	offset 1464 bit 5 */
	bool enableExtendedCanBroadcast : 1 {};
	/**
	 * global_can_data performance hack
	offset 1464 bit 6 */
	bool luaCanRxWorkaround : 1 {};
	/**
	offset 1464 bit 7 */
	bool flexSensorInverted : 1 {};
	/**
	offset 1464 bit 8 */
	bool useHardSkipInTraction : 1 {};
	/**
	 * Use Aux Speed 1 as one of speeds for wheel slip ratio?
	offset 1464 bit 9 */
	bool useAuxSpeedForSlipRatio : 1 {};
	/**
	 * VSS and Aux Speed 1 or Aux Speed 1 with Aux Speed 2?
	offset 1464 bit 10 */
	bool useVssAsSecondWheelSpeed : 1 {};
	/**
	offset 1464 bit 11 */
	bool is_enabled_spi_5 : 1 {};
	/**
	offset 1464 bit 12 */
	bool is_enabled_spi_6 : 1 {};
	/**
	 * AEM X-Series EGT gauge kit or rusEFI EGT sensor from Wideband controller
	offset 1464 bit 13 */
	bool enableAemXSeriesEgt : 1 {};
	/**
	offset 1464 bit 14 */
	bool startRequestPinInverted : 1 {};
	/**
	offset 1464 bit 15 */
	bool tcu_rangeSensorPulldown : 1 {};
	/**
	offset 1464 bit 16 */
	bool devBit01 : 1 {};
	/**
	offset 1464 bit 17 */
	bool devBit0 : 1 {};
	/**
	offset 1464 bit 18 */
	bool devBit1 : 1 {};
	/**
	offset 1464 bit 19 */
	bool devBit2 : 1 {};
	/**
	offset 1464 bit 20 */
	bool devBit3 : 1 {};
	/**
	offset 1464 bit 21 */
	bool devBit4 : 1 {};
	/**
	offset 1464 bit 22 */
	bool devBit5 : 1 {};
	/**
	offset 1464 bit 23 */
	bool devBit6 : 1 {};
	/**
	offset 1464 bit 24 */
	bool devBit7 : 1 {};
	/**
	offset 1464 bit 25 */
	bool invertExhaustCamVVTSignal : 1 {};
	/**
	 * "Available via TS Plugin see https://rusefi.com/s/knock"
	offset 1464 bit 26 */
	bool enableKnockSpectrogram : 1 {};
	/**
	offset 1464 bit 27 */
	bool enableKnockSpectrogramFilter : 1 {};
	/**
	offset 1464 bit 28 */
	bool unusedBit_505_28 : 1 {};
	/**
	offset 1464 bit 29 */
	bool unusedBit_505_29 : 1 {};
	/**
	offset 1464 bit 30 */
	bool unusedBit_505_30 : 1 {};
	/**
	offset 1464 bit 31 */
	bool unusedBit_505_31 : 1 {};
	/**
	 * offset 1468
	 */
	int16_t unusedafterCrankingIACtaperDuration;
	/**
	 * This value is an added for base idle value. Idle Value added when coasting and transitioning into idle.
	 * units: percent
	 * offset 1470
	 */
	int16_t iacByTpsTaper;
	/**
	 * offset 1472
	 */
	Gpio accelerometerCsPin;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: kph
	 * offset 1474
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: kph
	 * offset 1475
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	 * units: roc
	 * offset 1476
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1480
	 */
	brain_input_pin_e auxSpeedSensorInputPin[AUX_SPEED_SENSOR_COUNT] = {};
	/**
	 * offset 1484
	 */
	uint8_t totalGearsCount;
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 1485
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1486
	 */
	debug_mode_e debugMode;
	/**
	 * Additional idle % when fan #1 is active
	 * units: %
	 * offset 1487
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * Band rate for primary TTL
	 * units: BPs
	 * offset 1488
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	 * units: roc
	 * offset 1492
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	 * units: coeff
	 * offset 1496
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * units: voltage
	 * offset 1500
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	 * units: voltage
	 * offset 1504
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * offset 1508
	 */
	can_baudrate_e canBaudRate;
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1509
	 */
	ve_override_e veOverrideMode;
	/**
	 * offset 1510
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1511
	 */
	load_override_e afrOverrideMode;
	/**
	 * units: A
	 * offset 1512
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	 * units: A
	 * offset 1513
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	 * units: us
	 * offset 1514
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	 * units: ms
	 * offset 1515
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1516 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Allow OpenBLT on Primary CAN
	offset 1516 bit 1 */
	bool canOpenBLT : 1 {};
	/**
	 * Allow OpenBLT on Secondary CAN
	offset 1516 bit 2 */
	bool can2OpenBLT : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (default, cc/min) or mass flow (g/s).
	offset 1516 bit 3 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1516 bit 4 */
	bool boardUseCanTerminator : 1 {};
	/**
	offset 1516 bit 5 */
	bool kLineDoHondaSend : 1 {};
	/**
	 * ListenMode is about acknowledging CAN traffic on the protocol level. Different from canWriteEnabled
	offset 1516 bit 6 */
	bool can1ListenMode : 1 {};
	/**
	offset 1516 bit 7 */
	bool can2ListenMode : 1 {};
	/**
	offset 1516 bit 8 */
	bool unusedBit_541_8 : 1 {};
	/**
	offset 1516 bit 9 */
	bool unusedBit_541_9 : 1 {};
	/**
	offset 1516 bit 10 */
	bool unusedBit_541_10 : 1 {};
	/**
	offset 1516 bit 11 */
	bool unusedBit_541_11 : 1 {};
	/**
	offset 1516 bit 12 */
	bool unusedBit_541_12 : 1 {};
	/**
	offset 1516 bit 13 */
	bool unusedBit_541_13 : 1 {};
	/**
	offset 1516 bit 14 */
	bool unusedBit_541_14 : 1 {};
	/**
	offset 1516 bit 15 */
	bool unusedBit_541_15 : 1 {};
	/**
	offset 1516 bit 16 */
	bool unusedBit_541_16 : 1 {};
	/**
	offset 1516 bit 17 */
	bool unusedBit_541_17 : 1 {};
	/**
	offset 1516 bit 18 */
	bool unusedBit_541_18 : 1 {};
	/**
	offset 1516 bit 19 */
	bool unusedBit_541_19 : 1 {};
	/**
	offset 1516 bit 20 */
	bool unusedBit_541_20 : 1 {};
	/**
	offset 1516 bit 21 */
	bool unusedBit_541_21 : 1 {};
	/**
	offset 1516 bit 22 */
	bool unusedBit_541_22 : 1 {};
	/**
	offset 1516 bit 23 */
	bool unusedBit_541_23 : 1 {};
	/**
	offset 1516 bit 24 */
	bool unusedBit_541_24 : 1 {};
	/**
	offset 1516 bit 25 */
	bool unusedBit_541_25 : 1 {};
	/**
	offset 1516 bit 26 */
	bool unusedBit_541_26 : 1 {};
	/**
	offset 1516 bit 27 */
	bool unusedBit_541_27 : 1 {};
	/**
	offset 1516 bit 28 */
	bool unusedBit_541_28 : 1 {};
	/**
	offset 1516 bit 29 */
	bool unusedBit_541_29 : 1 {};
	/**
	offset 1516 bit 30 */
	bool unusedBit_541_30 : 1 {};
	/**
	offset 1516 bit 31 */
	bool unusedBit_541_31 : 1 {};
	/**
	 * Angle of tooth detection within engine phase cycle
	 * units: angle
	 * offset 1520
	 */
	uint8_t camDecoder2jzPosition;
	/**
	 * offset 1521
	 */
	mc33810maxDwellTimer_e mc33810maxDwellTimer;
	/**
	 * Duration of each test pulse
	 * units: ms
	 * offset 1522
	 */
	scaled_channel<uint16_t, 100, 1> benchTestOnTime;
	/**
	 * units: %
	 * offset 1524
	 */
	uint8_t lambdaProtectionRestoreTps;
	/**
	 * units: %
	 * offset 1525
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionRestoreLoad;
	/**
	 * offset 1526
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1527
	 */
	uint8_t alignmentFill_at_1527[1] = {};
	/**
	 * offset 1528
	 */
	Gpio can2TxPin;
	/**
	 * offset 1530
	 */
	Gpio can2RxPin;
	/**
	 * offset 1532
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1533
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1534
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1535
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1536
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1538
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1539
	 */
	uint8_t alignmentFill_at_1539[1] = {};
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	 * units: kPa (absolute)
	 * offset 1540
	 */
	float boostCutPressure;
	/**
	 * units: kg/h
	 * offset 1544
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16] = {};
	/**
	 * units: ratio
	 * offset 1560
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16] = {};
	/**
	 * Fixed timing, useful for TDC testing
	 * units: deg
	 * offset 1576
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1580
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1584
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * units: value
	 * offset 1588
	 */
	float egoValueShift;
	/**
	 * VVT output solenoid pin for this cam
	 * offset 1592
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 1600
	 */
	int unusedcrankingIACposition;
	/**
	 * offset 1604
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMinTps;
	/**
	 * offset 1605
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMinRpmMaxTps;
	/**
	 * offset 1606
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMinTps;
	/**
	 * offset 1607
	 */
	scaled_channel<uint8_t, 200, 1> tChargeMaxRpmMaxTps;
	/**
	 * offset 1608
	 */
	pwm_freq_t vvtOutputFrequency;
	/**
	 * Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.
	 * units: deg BTDC
	 * offset 1610
	 */
	int8_t minimumIgnitionTiming;
	/**
	 * Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC
	 * units: deg BTDC
	 * offset 1611
	 */
	int8_t maximumIgnitionTiming;
	/**
	 * units: Hz
	 * offset 1612
	 */
	int alternatorPwmFrequency;
	/**
	 * set vvt_mode X
	 * offset 1616
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK] = {};
	/**
	 * Additional idle % when fan #2 is active
	 * units: %
	 * offset 1618
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	 * units: sec
	 * offset 1619
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1620
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT] = {};
	/**
	 * offset 1628
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 1652
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1653
	 */
	pin_output_mode_e accelerometerCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 *  HPFP fuel mass compensation = manual mode for GDI engines
	 * offset 1654
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 1655
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	 * This is gauge pressure/in reference to atmospheric.
	 * units: kPa
	 * offset 1656
	 */
	float fuelReferencePressure;
	/**
	 * offset 1660
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1692
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * units: Deg
	 * offset 1724
	 */
	int16_t knockSamplingDuration;
	/**
	 * units: Hz
	 * offset 1726
	 */
	int16_t etbFreq;
	/**
	 * offset 1728
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 1748
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	 * units: %
	 * offset 1749
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	 * units: %
	 * offset 1750
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 1751
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder ignition and fueling timing correction for uneven engines
	 * units: deg
	 * offset 1752
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: seconds
	 * offset 1800
	 */
	float idlePidActivationTime;
	/**
	 * offset 1804
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 1805
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 1806
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 1807
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 1808
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 1809
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 1810
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 1811
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 1812
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 1813
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 1814
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 1816
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 1818
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 1820
	 */
	pid_s auxPid[CAMS_PER_BANK] = {};
	/**
	 * offset 1860
	 */
	float injectorCorrectionPolynomial[8] = {};
	/**
	 * units: C
	 * offset 1892
	 */
	scaled_channel<int8_t, 1, 5> primeBins[PRIME_CURVE_COUNT] = {};
	/**
	 * offset 1900
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 1920
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1921
	 */
	uint8_t alignmentFill_at_1921[1] = {};
	/**
	 * offset 1922
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: deg C
	 * offset 1924
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: deg C
	 * offset 1925
	 */
	uint8_t fan2OffTemperature;
	/**
	 * offset 1926
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 1928
	 */
	Gpio tle8888_cs;
	/**
	 * offset 1930
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1931
	 */
	uint8_t alignmentFill_at_1931[1] = {};
	/**
	 * offset 1932
	 */
	Gpio mc33816_cs;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1934
	 */
	uint8_t alignmentFill_at_1934[2] = {};
	/**
	 * units: hz
	 * offset 1936
	 */
	float auxFrequencyFilter;
	/**
	 * offset 1940
	 */
	sent_input_pin_e sentInputPins[SENT_INPUT_COUNT] = {};
	/**
	 * This sets the RPM above which fuel cut is active.
	 * units: rpm
	 * offset 1942
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	 * units: rpm
	 * offset 1944
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	 * units: %
	 * offset 1946
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	 * units: C
	 * offset 1948
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * units: %
	 * offset 1950
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	 * units: kPa
	 * offset 1952
	 */
	int16_t coastingFuelCutMap;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1954
	 */
	uint8_t alignmentFill_at_1954[2] = {};
	/**
	 * offset 1956
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 1976
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * offset 1996
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT] = {};
	/**
	 * offset 2092
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT] = {};
	/**
	 * offset 2156
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT] = {};
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2284
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2288
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	 * units: kg/h
	 * offset 2292
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2296
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2300
	 */
	float tChargeAirDecrLimit;
	/**
	 * iTerm min value
	 * offset 2304
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2306
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2308
	 */
	pid_s idleTimingPid;
	/**
	 * When entering idle, and the PID settings are aggressive, it's good to make a soft entry upon entering closed loop
	 * offset 2328
	 */
	float idleTimingSoftEntryTime;
	/**
	 * offset 2332
	 */
	pin_input_mode_e torqueReductionTriggerPinMode;
	/**
	 * offset 2333
	 */
	torqueReductionActivationMode_e torqueReductionActivationMode;
	/**
	 * A delay in cycles between fuel-enrich. portions
	 * units: cycles
	 * offset 2334
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	 * units: coef
	 * offset 2336
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2340
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2341
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2342
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2344
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	 * units: :1
	 * offset 2345
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2346
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	 * units: %
	 * offset 2348
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2352
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: mg
	 * offset 2364
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[PRIME_CURVE_COUNT] = {};
	/**
	 * Trigger comparator center point voltage
	 * units: V
	 * offset 2372
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	 * units: V
	 * offset 2373
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	 * units: V
	 * offset 2374
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	 * units: RPM
	 * offset 2375
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * units: ratio
	 * offset 2376
	 */
	scaled_channel<uint16_t, 100, 1> tractionControlSlipBins[TRACTION_CONTROL_ETB_DROP_SIZE] = {};
	/**
	 * units: RPM
	 * offset 2388
	 */
	uint8_t tractionControlSpeedBins[TRACTION_CONTROL_ETB_DROP_SIZE] = {};
	/**
	 * offset 2394
	 */
	int8_t disableFan1AtSpeed;
	/**
	 * offset 2395
	 */
	int8_t disableFan2AtSpeed;
	/**
	 * offset 2396
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2398
	 */
	uint8_t alignmentFill_at_2398[2] = {};
	/**
	 * offset 2400
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS] = {};
	/**
	 * Boost Current
	 * units: mA
	 * offset 2832
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	 * units: mA
	 * offset 2834
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	 * units: mA
	 * offset 2836
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	 * units: us
	 * offset 2838
	 */
	uint16_t mc33_t_max_boost;
	/**
	 * units: us
	 * offset 2840
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	 * units: us
	 * offset 2842
	 */
	uint16_t mc33_t_peak_tot;
	/**
	 * units: us
	 * offset 2844
	 */
	uint16_t mc33_t_bypass;
	/**
	 * units: us
	 * offset 2846
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	 * units: us
	 * offset 2848
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 2850
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 2851
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 2852
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 2853
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2859
	 */
	uint8_t alignmentFill_at_2859[1] = {};
	/**
	 * units: ratio
	 * offset 2860
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 2932
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH] = {};
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	 * units: rpm
	 * offset 3004
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	 * units: sec
	 * offset 3005
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	 * units: sec
	 * offset 3006
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * offset 3007
	 */
	tChargeMode_e tChargeMode;
	/**
	 * units: mg
	 * offset 3008
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	 * Threshold in ETB error (target vs. actual) above which the jam timer is started. If the timer reaches the time specified in the jam detection timeout period, the throttle is considered jammed, and engine operation limited.
	 * units: %
	 * offset 3010
	 */
	uint8_t etbJamDetectThreshold;
	/**
	 * units: lobes/cam
	 * offset 3011
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 3012
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Low engine speed for A/C. Larger engines can survive lower values
	 * units: RPM
	 * offset 3013
	 */
	scaled_channel<int8_t, 1, 10> acLowRpmLimit;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	 * units: deg
	 * offset 3014
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3015
	 */
	uint8_t alignmentFill_at_3015[1] = {};
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	 * units: cc
	 * offset 3016
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	 * https://rusefi.com/forum/viewtopic.php?t=2192
	 * units: deg
	 * offset 3018
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 3019
	 */
	uint8_t issFilterReciprocal;
	/**
	 * units: %/kPa
	 * offset 3020
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	 * units: %/kPa/lobe
	 * offset 3022
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * iTerm min value
	 * offset 3024
	 */
	int16_t hpfpPid_iTermMin;
	/**
	 * iTerm max value
	 * offset 3026
	 */
	int16_t hpfpPid_iTermMax;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	 * units: kPa/s
	 * offset 3028
	 */
	uint16_t hpfpTargetDecay;
	/**
	 * offset 3030
	 */
	output_pin_e stepper_raw_output[4] = {};
	/**
	 * units: ratio
	 * offset 3038
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[TCU_GEAR_COUNT] = {};
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	 * units: ms
	 * offset 3058
	 */
	uint16_t vvtActivationDelayMs;
	/**
	 * offset 3060
	 */
	GearControllerMode gearControllerMode;
	/**
	 * offset 3061
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * During revolution where ACR should be disabled at what specific angle to disengage
	 * units: deg
	 * offset 3062
	 */
	uint16_t acrDisablePhase;
	/**
	 * offset 3064
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 3084
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 3104
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * offset 3106
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3107
	 */
	uint8_t alignmentFill_at_3107[1] = {};
	/**
	 * offset 3108
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * offset 3110
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3111
	 */
	uint8_t alignmentFill_at_3111[1] = {};
	/**
	 * offset 3112
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * offset 3114
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * offset 3116
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3117
	 */
	uint8_t alignmentFill_at_3117[1] = {};
	/**
	 * offset 3118
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * offset 3120
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * offset 3122
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3123
	 */
	uint8_t alignmentFill_at_3123[1] = {};
	/**
	 * offset 3124
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	 * offset 3126
	 */
	output_pin_e acrPin2;
	/**
	 * Set a minimum allowed target position to avoid slamming/driving against the hard mechanical stop in the throttle.
	 * units: %
	 * offset 3128
	 */
	scaled_channel<uint8_t, 10, 1> etbMinimumPosition;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3129
	 */
	uint8_t alignmentFill_at_3129[1] = {};
	/**
	 * offset 3130
	 */
	uint16_t tuneHidingKey;
	/**
	 * Individual characters are accessible using vin(index) Lua function
	 * offset 3132
	 */
	vin_number_t vinNumber;
	/**
	 * units: C
	 * offset 3149
	 */
	int8_t torqueReductionActivationTemperature;
	/**
	 * offset 3150
	 */
	uint16_t highSpeedOffsets[HIGH_SPEED_COUNT] = {};
	/**
	 * offset 3214
	 */
	fuel_pressure_sensor_mode_e fuelPressureSensorMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3215
	 */
	uint8_t alignmentFill_at_3215[1] = {};
	/**
	 * offset 3216
	 */
	switch_input_pin_e luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * units: rpm
	 * offset 3232
	 */
	int16_t ALSMinRPM;
	/**
	 * units: rpm
	 * offset 3234
	 */
	int16_t ALSMaxRPM;
	/**
	 * units: sec
	 * offset 3236
	 */
	int16_t ALSMaxDuration;
	/**
	 * units: C
	 * offset 3238
	 */
	int8_t ALSMinCLT;
	/**
	 * units: C
	 * offset 3239
	 */
	int8_t ALSMaxCLT;
	/**
	 * offset 3240
	 */
	uint8_t alsMinTimeBetween;
	/**
	 * offset 3241
	 */
	uint8_t alsEtbPosition;
	/**
	 * units: %
	 * offset 3242
	 */
	uint8_t acRelayAlternatorDutyAdder;
	/**
	 * If you have SENT TPS sensor please select type. For analog TPS leave None
	 * offset 3243
	 */
	SentEtbType sentEtbType;
	/**
	 * offset 3244
	 */
	uint16_t customSentTpsMin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3246
	 */
	uint8_t alignmentFill_at_3246[2] = {};
	/**
	 * units: %
	 * offset 3248
	 */
	int ALSIdleAdd;
	/**
	 * units: %
	 * offset 3252
	 */
	int ALSEtbAdd;
	/**
	 * offset 3256
	 */
	float ALSSkipRatio;
	/**
	 * Hysterisis: if Pressure High Disable is 240kpa, and acPressureEnableHyst is 20, when the ECU sees 240kpa, A/C will be disabled, and stay disabled until 240-20=220kpa is reached
	 * units: kPa (absolute)
	 * offset 3260
	 */
	scaled_channel<uint8_t, 2, 1> acPressureEnableHyst;
	/**
	 * offset 3261
	 */
	pin_input_mode_e ALSActivatePinMode;
	/**
	 * For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	 * units: %
	 * offset 3262
	 */
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	/**
	 * For Toyota ETCS-i, use ~69%
	 * units: %
	 * offset 3263
	 */
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	/**
	 * offset 3264
	 */
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT] = {};
	/**
	 * offset 3272
	 */
	uint16_t customSentTpsMax;
	/**
	 * offset 3274
	 */
	uint16_t kLineBaudRate;
	/**
	 * offset 3276
	 */
	CanGpioType canGpioType;
	/**
	 * offset 3277
	 */
	UiMode uiMode;
	/**
	 * Crank angle ATDC of first lobe peak
	 * units: deg
	 * offset 3278
	 */
	int16_t hpfpPeakPos;
	/**
	 * units: us
	 * offset 3280
	 */
	int16_t kLinePeriodUs;
	/**
	 * Window that the correction will be added throughout (example, if rpm limit is 7000, and rpmSoftLimitWindowSize is 200, the corrections activate at 6800RPM, creating a 200rpm window)
	 * units: RPM
	 * offset 3282
	 */
	scaled_channel<uint8_t, 1, 10> rpmSoftLimitWindowSize;
	/**
	 * Degrees of timing REMOVED from actual timing during soft RPM limit window
	 * units: deg
	 * offset 3283
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitTimingRetard;
	/**
	 * % of fuel ADDED during window
	 * units: %
	 * offset 3284
	 */
	scaled_channel<uint8_t, 5, 1> rpmSoftLimitFuelAdded;
	/**
	 * Hysterisis: if the hard limit is 7200rpm and rpmHardLimitHyst is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached
	 * units: RPM
	 * offset 3285
	 */
	scaled_channel<uint8_t, 1, 10> rpmHardLimitHyst;
	/**
	 * Time between bench test pulses
	 * units: ms
	 * offset 3286
	 */
	scaled_channel<uint16_t, 10, 1> benchTestOffTime;
	/**
	 * Hysterisis: if hard cut is 240kpa, and boostCutPressureHyst is 20, when the ECU sees 240kpa, fuel/ign will cut, and stay cut until 240-20=220kpa is reached
	 * units: kPa (absolute)
	 * offset 3288
	 */
	scaled_channel<uint8_t, 2, 1> boostCutPressureHyst;
	/**
	 * Boost duty cycle modified by gear
	 * units: %
	 * offset 3289
	 */
	scaled_channel<int8_t, 2, 1> gearBasedOpenLoopBoostAdder[TCU_GEAR_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3299
	 */
	uint8_t alignmentFill_at_3299[1] = {};
	/**
	 * How many test bench pulses do you want
	 * offset 3300
	 */
	uint32_t benchTestCount;
	/**
	 * How long initial idle adder is held before starting to decay.
	 * units: seconds
	 * offset 3304
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsHoldTime;
	/**
	 * How long it takes to remove initial IAC adder to return to normal idle.
	 * units: seconds
	 * offset 3305
	 */
	scaled_channel<uint8_t, 10, 1> iacByTpsDecayTime;
	/**
	 * offset 3306
	 */
	switch_input_pin_e tcu_rangeInput[RANGE_INPUT_COUNT] = {};
	/**
	 * offset 3318
	 */
	pin_input_mode_e tcu_rangeInputMode[RANGE_INPUT_COUNT] = {};
	/**
	 * Scale the reported vehicle speed value from CAN. Example: Parameter set to 1.1, CAN VSS reports 50kph, ECU will report 55kph instead.
	 * units: ratio
	 * offset 3324
	 */
	scaled_channel<uint16_t, 10000, 1> canVssScaling;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3326
	 */
	uint8_t alignmentFill_at_3326[2] = {};
	/**
	 * offset 3328
	 */
	ThermistorConf oilTempSensor;
	/**
	 * offset 3360
	 */
	ThermistorConf fuelTempSensor;
	/**
	 * offset 3392
	 */
	ThermistorConf ambientTempSensor;
	/**
	 * offset 3424
	 */
	ThermistorConf compressorDischargeTemperature;
	/**
	 * Place the sensor before the throttle, but after any turbocharger/supercharger and intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3456
	 */
	adc_channel_e throttleInletPressureChannel;
	/**
	 * Place the sensor after the turbocharger/supercharger, but before any intercoolers if fitted. Uses the same calibration as the MAP sensor.
	 * offset 3457
	 */
	adc_channel_e compressorDischargePressureChannel;
	/**
	 * offset 3458
	 */
	Gpio dacOutputPins[DAC_OUTPUT_COUNT] = {};
	/**
	 * offset 3462
	 */
	output_pin_e speedometerOutputPin;
	/**
	 * Number of speedometer pulses per kilometer travelled.
	 * offset 3464
	 */
	uint16_t speedometerPulsePerKm;
	/**
	 * offset 3466
	 */
	uint8_t simulatorCamPosition[CAM_INPUTS_COUNT] = {};
	/**
	 * offset 3470
	 */
	adc_channel_e ignKeyAdcChannel;
	/**
	 * offset 3471
	 */
	pin_mode_e spi6MisoMode;
	/**
	 * units: ratio
	 * offset 3472
	 */
	float triggerVVTGapOverrideFrom[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: ratio
	 * offset 3488
	 */
	float triggerVVTGapOverrideTo[VVT_TRACKING_LENGTH] = {};
	/**
	 * units: %
	 * offset 3504
	 */
	int8_t tractionControlEtbDrop[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE] = {};
	/**
	 * If injector duty cycle hits this value, instantly cut fuel.
	 * units: %
	 * offset 3540
	 */
	uint8_t maxInjectorDutyInstant;
	/**
	 * If injector duty cycle hits this value for the specified delay time, cut fuel.
	 * units: %
	 * offset 3541
	 */
	uint8_t maxInjectorDutySustained;
	/**
	 * Timeout period for duty cycle over the sustained limit to trigger duty cycle protection.
	 * units: sec
	 * offset 3542
	 */
	scaled_channel<uint8_t, 10, 1> maxInjectorDutySustainedTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3543
	 */
	uint8_t alignmentFill_at_3543[1] = {};
	/**
	 * offset 3544
	 */
	output_pin_e injectionPinsStage2[MAX_CYLINDER_COUNT] = {};
	/**
	 * units: Deg
	 * offset 3568
	 */
	int8_t tractionControlTimingDrop[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE] = {};
	/**
	 * units: %
	 * offset 3604
	 */
	int8_t tractionControlIgnitionSkip[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE] = {};
	/**
	 * offset 3640
	 */
	float auxSpeed1Multiplier;
	/**
	 * offset 3644
	 */
	float brakeMeanEffectivePressureDifferential;
	/**
	 * offset 3648
	 */
	Gpio spi4mosiPin;
	/**
	 * offset 3650
	 */
	Gpio spi4misoPin;
	/**
	 * offset 3652
	 */
	Gpio spi4sckPin;
	/**
	 * offset 3654
	 */
	Gpio spi5mosiPin;
	/**
	 * offset 3656
	 */
	Gpio spi5misoPin;
	/**
	 * offset 3658
	 */
	Gpio spi5sckPin;
	/**
	 * offset 3660
	 */
	Gpio spi6mosiPin;
	/**
	 * offset 3662
	 */
	Gpio spi6misoPin;
	/**
	 * offset 3664
	 */
	Gpio spi6sckPin;
	/**
	 * offset 3666
	 */
	pin_mode_e spi4SckMode;
	/**
	 * offset 3667
	 */
	pin_mode_e spi4MosiMode;
	/**
	 * offset 3668
	 */
	pin_mode_e spi4MisoMode;
	/**
	 * offset 3669
	 */
	pin_mode_e spi5SckMode;
	/**
	 * offset 3670
	 */
	pin_mode_e spi5MosiMode;
	/**
	 * offset 3671
	 */
	pin_mode_e spi5MisoMode;
	/**
	 * offset 3672
	 */
	pin_mode_e spi6SckMode;
	/**
	 * offset 3673
	 */
	pin_mode_e spi6MosiMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3674
	 */
	uint8_t alignmentFill_at_3674[2] = {};
	/**
	 * Secondary TTL channel baud rate
	 * units: BPs
	 * offset 3676
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 3680
	 */
	Gpio camSimulatorPin;
	/**
	 * offset 3682
	 */
	pin_output_mode_e camSimulatorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3683
	 */
	uint8_t alignmentFill_at_3683[1] = {};
	/**
	 * offset 3684
	 */
	int anotherCiTest;
	/**
	 * offset 3688
	 */
	uint32_t device_uid[3] = {};
	/**
	 * offset 3700
	 */
	adc_channel_e tcu_rangeAnalogInput[RANGE_INPUT_COUNT] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3706
	 */
	uint8_t alignmentFill_at_3706[2] = {};
	/**
	 * units: Ohm
	 * offset 3708
	 */
	float tcu_rangeSensorBiasResistor;
	/**
	 * offset 3712
	 */
	MsIoBox_config_s msIoBox0;
	/**
	 * Nominal coil charge current, 0.25A step
	 * units: A
	 * offset 3716
	 */
	scaled_channel<uint8_t, 4, 1> mc33810Nomi;
	/**
	 * Maximum coil charge current, 1A step
	 * units: A
	 * offset 3717
	 */
	uint8_t mc33810Maxi;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3718
	 */
	uint8_t alignmentFill_at_3718[2] = {};
	/**
	 * offset 3720
	 */
	linear_sensor_s acPressure;
	/**
	 * value of A/C pressure in kPa before that compressor is disengaged
	 * units: kPa
	 * offset 3740
	 */
	uint16_t minAcPressure;
	/**
	 * value of A/C pressure in kPa after that compressor is disengaged
	 * units: kPa
	 * offset 3742
	 */
	uint16_t maxAcPressure;
	/**
	 * Delay before cutting fuel due to low oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 3744
	 */
	scaled_channel<uint8_t, 10, 1> minimumOilPressureTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3745
	 */
	uint8_t alignmentFill_at_3745[3] = {};
	/**
	 * offset 3748
	 */
	linear_sensor_s auxLinear3;
	/**
	 * offset 3768
	 */
	linear_sensor_s auxLinear4;
	/**
	 * Below TPS value all knock suppression will be disabled.
	 * units: %
	 * offset 3788
	 */
	scaled_channel<uint8_t, 1, 1> knockSuppressMinTps;
	/**
	 * Fuel to odd when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 3789
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimAggression;
	/**
	 * After a knock event, reapply fuel at this rate.
	 * units: 1%/s
	 * offset 3790
	 */
	scaled_channel<uint8_t, 10, 1> knockFuelTrimReapplyRate;
	/**
	 * Fuel trim when knock, max 30%
	 * units: %
	 * offset 3791
	 */
	scaled_channel<uint8_t, 1, 1> knockFuelTrim;
	/**
	 * units: sense
	 * offset 3792
	 */
	float knockSpectrumSensitivity;
	/**
	 * "Estimated knock frequency, ignore cylinderBore if this one > 0"
	 * units: Hz
	 * offset 3796
	 */
	float knockFrequency;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 3800
	 */
	injector_compensation_mode_e secondaryInjectorCompensationMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3801
	 */
	uint8_t alignmentFill_at_3801[3] = {};
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	 * units: kPa
	 * offset 3804
	 */
	float secondaryInjectorFuelReferencePressure;
	/**
	 * SENT input connected to ETB
	 * offset 3808
	 */
	SentInput EtbSentInput;
	/**
	 * SENT input used for high pressure fuel sensor
	 * offset 3809
	 */
	SentInput FuelHighPressureSentInput;
	/**
	 * If you have SENT High Pressure Fuel Sensor please select type. For analog TPS leave None
	 * offset 3810
	 */
	SentFuelHighPressureType FuelHighPressureSentType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3811
	 */
	uint8_t alignmentFill_at_3811[1] = {};
	/**
	offset 3812 bit 0 */
	bool nitrousControlEnabled : 1 {};
	/**
	offset 3812 bit 1 */
	bool unusedFancy2 : 1 {};
	/**
	offset 3812 bit 2 */
	bool unusedFancy3 : 1 {};
	/**
	offset 3812 bit 3 */
	bool unusedFancy4 : 1 {};
	/**
	offset 3812 bit 4 */
	bool unusedFancy5 : 1 {};
	/**
	offset 3812 bit 5 */
	bool unusedFancy6 : 1 {};
	/**
	offset 3812 bit 6 */
	bool unusedFancy7 : 1 {};
	/**
	offset 3812 bit 7 */
	bool unusedFancy8 : 1 {};
	/**
	offset 3812 bit 8 */
	bool unusedFancy9 : 1 {};
	/**
	offset 3812 bit 9 */
	bool unusedFancy10 : 1 {};
	/**
	offset 3812 bit 10 */
	bool unusedFancy11 : 1 {};
	/**
	offset 3812 bit 11 */
	bool unusedFancy12 : 1 {};
	/**
	offset 3812 bit 12 */
	bool unusedFancy13 : 1 {};
	/**
	offset 3812 bit 13 */
	bool unusedFancy14 : 1 {};
	/**
	offset 3812 bit 14 */
	bool unusedFancy15 : 1 {};
	/**
	offset 3812 bit 15 */
	bool unusedFancy16 : 1 {};
	/**
	offset 3812 bit 16 */
	bool unusedFancy17 : 1 {};
	/**
	offset 3812 bit 17 */
	bool unusedFancy18 : 1 {};
	/**
	offset 3812 bit 18 */
	bool unusedFancy19 : 1 {};
	/**
	offset 3812 bit 19 */
	bool unusedFancy20 : 1 {};
	/**
	offset 3812 bit 20 */
	bool unusedFancy21 : 1 {};
	/**
	offset 3812 bit 21 */
	bool unusedFancy22 : 1 {};
	/**
	offset 3812 bit 22 */
	bool unusedFancy23 : 1 {};
	/**
	offset 3812 bit 23 */
	bool unusedFancy24 : 1 {};
	/**
	offset 3812 bit 24 */
	bool unusedFancy25 : 1 {};
	/**
	offset 3812 bit 25 */
	bool unusedFancy26 : 1 {};
	/**
	offset 3812 bit 26 */
	bool unusedFancy27 : 1 {};
	/**
	offset 3812 bit 27 */
	bool unusedFancy28 : 1 {};
	/**
	offset 3812 bit 28 */
	bool unusedFancy29 : 1 {};
	/**
	offset 3812 bit 29 */
	bool unusedFancy30 : 1 {};
	/**
	offset 3812 bit 30 */
	bool unusedFancy31 : 1 {};
	/**
	offset 3812 bit 31 */
	bool unusedFancy32 : 1 {};
	/**
	 * offset 3816
	 */
	nitrous_arming_method_e nitrousControlArmingMethod;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3817
	 */
	uint8_t alignmentFill_at_3817[1] = {};
	/**
	 * Pin that activates nitrous control
	 * offset 3818
	 */
	switch_input_pin_e nitrousControlTriggerPin;
	/**
	 * offset 3820
	 */
	pin_input_mode_e nitrousControlTriggerPinMode;
	/**
	 * offset 3821
	 */
	lua_gauge_e nitrousLuaGauge;
	/**
	 * offset 3822
	 */
	lua_gauge_meaning_e nitrousLuaGaugeMeaning;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3823
	 */
	uint8_t alignmentFill_at_3823[1] = {};
	/**
	 * offset 3824
	 */
	float nitrousLuaGaugeArmingValue;
	/**
	 * offset 3828
	 */
	int nitrousMinimumTps;
	/**
	 * units: deg C
	 * offset 3832
	 */
	uint8_t nitrousMinimumClt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3833
	 */
	uint8_t alignmentFill_at_3833[1] = {};
	/**
	 * units: kPa
	 * offset 3834
	 */
	int16_t nitrousMaximumMap;
	/**
	 * units: afr
	 * offset 3836
	 */
	scaled_channel<uint8_t, 10, 1> nitrousMaximumAfr;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3837
	 */
	uint8_t alignmentFill_at_3837[1] = {};
	/**
	 * units: rpm
	 * offset 3838
	 */
	uint16_t nitrousActivationRpm;
	/**
	 * units: rpm
	 * offset 3840
	 */
	uint16_t nitrousDeactivationRpm;
	/**
	 * units: rpm
	 * offset 3842
	 */
	uint16_t nitrousDeactivationRpmWindow;
	/**
	 * Retard timing by this amount during DFCO. Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: deg
	 * offset 3844
	 */
	uint8_t dfcoRetardDeg;
	/**
	 * Smooths the transition back from fuel cut. After fuel is restored, ramp timing back in over the period specified.
	 * units: s
	 * offset 3845
	 */
	scaled_channel<uint8_t, 10, 1> dfcoRetardRampInTime;
	/**
	 * offset 3846
	 */
	output_pin_e nitrousRelayPin;
	/**
	 * offset 3848
	 */
	pin_output_mode_e nitrousRelayPinMode;
	/**
	 * units: %
	 * offset 3849
	 */
	int8_t nitrousFuelAdderPercent;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3850
	 */
	uint8_t alignmentFill_at_3850[2] = {};
	/**
	 * Retard timing to remove from actual final timing (after all corrections) due to additional air.
	 * units: deg
	 * offset 3852
	 */
	float nitrousIgnitionRetard;
	/**
	 * units: Kph
	 * offset 3856
	 */
	uint16_t nitrousMinimumVehicleSpeed;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3858
	 */
	uint8_t alignmentFill_at_3858[2] = {};
	/**
	 * Exponential Average Alpha filtering parameter
	 * offset 3860
	 */
	float fuelLevelAveragingAlpha;
	/**
	 * How often do we update fuel level gauge
	 * units: seconds
	 * offset 3864
	 */
	float fuelLevelUpdatePeriodSec;
	/**
	 * Error below specified value
	 * units: v
	 * offset 3868
	 */
	float fuelLevelLowThresholdVoltage;
	/**
	 * Error above specified value
	 * units: v
	 * offset 3872
	 */
	float fuelLevelHighThresholdVoltage;
	/**
	 * offset 3876
	 */
	float afrExpAverageAlpha;
	/**
	 * Compensates for trigger delay due to belt stretch, or other electromechanical issues. beware that raising this value advances ignition timing!
	 * units: uS
	 * offset 3880
	 */
	scaled_channel<uint8_t, 1, 1> sparkHardwareLatencyCorrection;
	/**
	 * Delay before cutting fuel due to extra high oil pressure. Use this to ignore short pressure blips and sensor noise.
	 * units: sec
	 * offset 3881
	 */
	scaled_channel<uint8_t, 10, 1> maxOilPressureTimeout;
	/**
	 * units: kg/h
	 * offset 3882
	 */
	scaled_channel<uint16_t, 100, 1> idleFlowEstimateFlow[8] = {};
	/**
	 * units: %
	 * offset 3898
	 */
	scaled_channel<uint8_t, 2, 1> idleFlowEstimatePosition[8] = {};
	/**
	 * units: mg
	 * offset 3906
	 */
	int8_t airmassToTimingBins[8] = {};
	/**
	 * units: deg
	 * offset 3914
	 */
	int8_t airmassToTimingValues[8] = {};
	/**
	 * idle return target ramp duration
	 * units: seconds
	 * offset 3922
	 */
	scaled_channel<uint8_t, 10, 1> idleReturnTargetRampDuration;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3923
	 */
	uint8_t alignmentFill_at_3923[1] = {};
	/**
	 * Voltage when the wastegate is fully open
	 * units: v
	 * offset 3924
	 */
	float wastegatePositionOpenedVoltage;
	/**
	 * Voltage when the wastegate is closed
	 * units: v
	 * offset 3928
	 */
	float wastegatePositionClosedVoltage;
	/**
	 * units: units
	 * offset 3932
	 */
	uint8_t unusedOftenChangesDuringFirmwareUpdate[END_OF_CALIBRATION_PADDING] = {};
};
static_assert(sizeof(engine_configuration_s) == 3976);

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
	 * offset 3976
	 */
	float tmfTable[TMF_SIZE][TMF_SIZE] = {};
	/**
	 * offset 3992
	 */
	float tmfRatioBins[TMF_SIZE] = {};
	/**
	 * offset 4000
	 */
	float tmfOpeningBins[TMF_SIZE] = {};
	/**
	 * units: Nm
	 * offset 4008
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 4044
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE] = {};
	/**
	 * units: Load
	 * offset 4056
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE] = {};
	/**
	 * units: mult
	 * offset 4068
	 */
	float postCrankingFactor[CRANKING_ENRICH_COUNT][CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: count
	 * offset 4212
	 */
	uint16_t postCrankingDurationBins[CRANKING_ENRICH_COUNT] = {};
	/**
	 * units: C
	 * offset 4224
	 */
	int16_t postCrankingCLTBins[CRANKING_ENRICH_COUNT] = {};
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	 * units: target TPS position
	 * offset 4236
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: ETB duty cycle bias
	 * offset 4268
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * target Wastegate value, 0 to 100%
	 * units: target DC position
	 * offset 4300
	 */
	int8_t dcWastegateBiasBins[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: DC wastegate duty cycle bias
	 * offset 4308
	 */
	scaled_channel<int16_t, 100, 1> dcWastegateBiasValues[ETB_BIAS_CURVE_LENGTH] = {};
	/**
	 * units: %
	 * offset 4324
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE] = {};
	/**
	 * units: Load
	 * offset 4388
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE] = {};
	/**
	 * units: RPM
	 * offset 4396
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_SIZE] = {};
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	 * units: RPM
	 * offset 4404
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: ms
	 * offset 4420
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE] = {};
	/**
	 * CLT-based target RPM for automatic idle controller
	 * units: C
	 * offset 4436
	 */
	scaled_channel<int8_t, 1, 2> cltIdleRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * See idleRpmPid
	 * units: RPM
	 * offset 4452
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE] = {};
	/**
	 * units: deg
	 * offset 4468
	 */
	scaled_channel<int16_t, 10, 1> ignitionCltCorrTable[CLT_TIMING_CURVE_SIZE][CLT_TIMING_CURVE_SIZE] = {};
	/**
	 * CLT-based timing correction
	 * units: C
	 * offset 4518
	 */
	scaled_channel<int8_t, 1, 5> ignitionCltCorrTempBins[CLT_TIMING_CURVE_SIZE] = {};
	/**
	 * units: Load
	 * offset 4523
	 */
	scaled_channel<uint8_t, 1, 5> ignitionCltCorrLoadBins[CLT_TIMING_CURVE_SIZE] = {};
	/**
	 * units: x
	 * offset 4528
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 4592
	 */
	float scriptCurve1[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 4656
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16] = {};
	/**
	 * units: y
	 * offset 4720
	 */
	float scriptCurve2[SCRIPT_CURVE_16] = {};
	/**
	 * units: x
	 * offset 4784
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 4816
	 */
	float scriptCurve3[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 4848
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 4880
	 */
	float scriptCurve4[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 4912
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 4944
	 */
	float scriptCurve5[SCRIPT_CURVE_8] = {};
	/**
	 * units: x
	 * offset 4976
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8] = {};
	/**
	 * units: y
	 * offset 5008
	 */
	float scriptCurve6[SCRIPT_CURVE_8] = {};
	/**
	 * units: kPa
	 * offset 5040
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5056
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE] = {};
	/**
	 * units: ratio
	 * offset 5072
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE] = {};
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * units: Ratio
	 * offset 5136
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 5168
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: RPM
	 * offset 5200
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: deg
	 * offset 5208
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: RPM
	 * offset 5216
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE] = {};
	/**
	 * RPM-based idle position for coasting
	 * units: %
	 * offset 5232
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE] = {};
	/**
	 * offset 5248
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * units: RPM
	 * offset 5312
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT] = {};
	/**
	 * offset 5320
	 */
	uint16_t boostOpenLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * offset 5336
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT] = {};
	/**
	 * offset 5400
	 */
	uint16_t boostClosedLoopLoadBins[BOOST_LOAD_COUNT] = {};
	/**
	 * units: %
	 * offset 5416
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE] = {};
	/**
	 * units: %
	 * offset 5480
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5488
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE] = {};
	/**
	 * CLT-based cranking position % for simple manual idle controller
	 * units: C
	 * offset 5496
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based cranking position % for simple manual idle controller
	 * units: percent
	 * offset 5528
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 5560
	 */
	float afterCrankingIACtaperDurationBins[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	 * Should be 100 once tune is better
	 * units: cycles
	 * offset 5584
	 */
	uint16_t afterCrankingIACtaperDuration[CLT_CRANKING_TAPER_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: RPM
	 * offset 5596
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: deg
	 * offset 5604
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 5636
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE] = {};
	/**
	 * units: load
	 * offset 5640
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE] = {};
	/**
	 * units: %
	 * offset 5644
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE] = {};
	/**
	 * offset 5676
	 */
	lua_script_t luaScript;
	/**
	 * units: C
	 * offset 13676
	 */
	float cltFuelCorrBins[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 13740
	 */
	float cltFuelCorr[CLT_FUEL_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 13804
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 13868
	 */
	float iatFuelCorr[IAT_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 13932
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 13964
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: counter
	 * offset 13996
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 14028
	 */
	int16_t crankingCycleFuelCltBins[CRANKING_CYCLE_CLT_SIZE] = {};
	/**
	 * units: mult
	 * offset 14036
	 */
	float crankingCycleFuelCoef[CRANKING_CYCLE_CLT_SIZE][CRANKING_CURVE_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: C
	 * offset 14164
	 */
	float cltIdleCorrBins[CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * CLT-based idle position for simple manual idle controller
	 * units: %
	 * offset 14196
	 */
	float cltIdleCorrTable[CLT_IDLE_TABLE_RPM_SIZE][CLT_IDLE_TABLE_CLT_SIZE] = {};
	/**
	 * units: RPM
	 * offset 14260
	 */
	scaled_channel<uint8_t, 1, 100> rpmIdleCorrBins[CLT_IDLE_TABLE_RPM_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 14262
	 */
	uint8_t alignmentFill_at_14262[2] = {};
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	 * units: kg/hour
	 * offset 14264
	 */
	float mafDecoding[MAF_DECODING_COUNT] = {};
	/**
	 * units: V
	 * offset 14392
	 */
	float mafDecodingBins[MAF_DECODING_COUNT] = {};
	/**
	 * units: deg
	 * offset 14520
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IAT_IGN_CORR_COUNT][IAT_IGN_CORR_COUNT] = {};
	/**
	 * units: C
	 * offset 14648
	 */
	int8_t ignitionIatCorrTempBins[IAT_IGN_CORR_COUNT] = {};
	/**
	 * units: Load
	 * offset 14656
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[IAT_IGN_CORR_COUNT] = {};
	/**
	 * units: deg
	 * offset 14664
	 */
	int16_t injectionPhase[INJ_PHASE_LOAD_COUNT][INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: Load
	 * offset 15176
	 */
	uint16_t injPhaseLoadBins[INJ_PHASE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 15208
	 */
	uint16_t injPhaseRpmBins[INJ_PHASE_RPM_COUNT] = {};
	/**
	 * units: onoff
	 * offset 15240
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT] = {};
	/**
	 * units: kPa
	 * offset 15300
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[MAP_EST_LOAD_COUNT][MAP_EST_RPM_COUNT] = {};
	/**
	 * units: % TPS
	 * offset 15812
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[MAP_EST_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 15844
	 */
	uint16_t mapEstimateRpmBins[MAP_EST_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 15876
	 */
	int8_t vvtTable1[VVT_TABLE_SIZE][VVT_TABLE_SIZE] = {};
	/**
	 * units: L
	 * offset 15940
	 */
	uint16_t vvtTable1LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 15956
	 */
	uint16_t vvtTable1RpmBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: value
	 * offset 15972
	 */
	int8_t vvtTable2[VVT_TABLE_SIZE][VVT_TABLE_SIZE] = {};
	/**
	 * units: L
	 * offset 16036
	 */
	uint16_t vvtTable2LoadBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 16052
	 */
	uint16_t vvtTable2RpmBins[VVT_TABLE_SIZE] = {};
	/**
	 * units: deg
	 * offset 16068
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT] = {};
	/**
	 * units: Load
	 * offset 16580
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 16612
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT] = {};
	/**
	 * units: %
	 * offset 16644
	 */
	scaled_channel<uint16_t, 10, 1> veTable[VE_LOAD_COUNT][VE_RPM_COUNT] = {};
	/**
	 * units: {bitStringValue(fuelUnits, fuelAlgorithm) }
	 * offset 17156
	 */
	uint16_t veLoadBins[VE_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 17188
	 */
	uint16_t veRpmBins[VE_RPM_COUNT] = {};
	/**
	 * units: lambda
	 * offset 17220
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT] = {};
	/**
	 * offset 17476
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT] = {};
	/**
	 * units: RPM
	 * offset 17508
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT] = {};
	/**
	 * units: value
	 * offset 17540
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: from
	 * offset 17796
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: to
	 * offset 17828
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE] = {};
	/**
	 * units: value
	 * offset 17860
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8] = {};
	/**
	 * units: L
	 * offset 18116
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 18132
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: value
	 * offset 18148
	 */
	float scriptTable2[TABLE_2_LOAD_SIZE][TABLE_2_RPM_SIZE] = {};
	/**
	 * units: L
	 * offset 18404
	 */
	int16_t scriptTable2LoadBins[TABLE_2_LOAD_SIZE] = {};
	/**
	 * units: RPM
	 * offset 18420
	 */
	int16_t scriptTable2RpmBins[TABLE_2_RPM_SIZE] = {};
	/**
	 * units: value
	 * offset 18436
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8] = {};
	/**
	 * units: L
	 * offset 18500
	 */
	int16_t scriptTable3LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 18516
	 */
	int16_t scriptTable3RpmBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: value
	 * offset 18532
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][TABLE_4_RPM] = {};
	/**
	 * units: L
	 * offset 18612
	 */
	int16_t scriptTable4LoadBins[SCRIPT_TABLE_8] = {};
	/**
	 * units: RPM
	 * offset 18628
	 */
	int16_t scriptTable4RpmBins[TABLE_4_RPM] = {};
	/**
	 * offset 18648
	 */
	uint16_t ignTrimLoadBins[IGN_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 18656
	 */
	uint16_t ignTrimRpmBins[IGN_TRIM_SIZE] = {};
	/**
	 * offset 18664
	 */
	ign_cyl_trim_s ignTrims[12] = {};
	/**
	 * offset 18856
	 */
	uint16_t fuelTrimLoadBins[FUEL_TRIM_SIZE] = {};
	/**
	 * units: rpm
	 * offset 18864
	 */
	uint16_t fuelTrimRpmBins[FUEL_TRIM_SIZE] = {};
	/**
	 * offset 18872
	 */
	fuel_cyl_trim_s fuelTrims[12] = {};
	/**
	 * units: ratio
	 * offset 19064
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE] = {};
	/**
	 * units: Airmass
	 * offset 19080
	 */
	scaled_channel<uint8_t, 1, 5> tcu_pcAirmassBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19088
	 */
	uint8_t tcu_pcValsR[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19096
	 */
	uint8_t tcu_pcValsN[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19104
	 */
	uint8_t tcu_pcVals1[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19112
	 */
	uint8_t tcu_pcVals2[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19120
	 */
	uint8_t tcu_pcVals3[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19128
	 */
	uint8_t tcu_pcVals4[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19136
	 */
	uint8_t tcu_pcVals12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19144
	 */
	uint8_t tcu_pcVals23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19152
	 */
	uint8_t tcu_pcVals34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19160
	 */
	uint8_t tcu_pcVals21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19168
	 */
	uint8_t tcu_pcVals32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: %
	 * offset 19176
	 */
	uint8_t tcu_pcVals43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: TPS
	 * offset 19184
	 */
	uint8_t tcu_tccTpsBins[8] = {};
	/**
	 * units: MPH
	 * offset 19192
	 */
	uint8_t tcu_tccLockSpeed[8] = {};
	/**
	 * units: MPH
	 * offset 19200
	 */
	uint8_t tcu_tccUnlockSpeed[8] = {};
	/**
	 * units: KPH
	 * offset 19208
	 */
	uint8_t tcu_32SpeedBins[8] = {};
	/**
	 * units: %
	 * offset 19216
	 */
	uint8_t tcu_32Vals[8] = {};
	/**
	 * units: %
	 * offset 19224
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[ETB2_TRIM_SIZE][ETB2_TRIM_SIZE] = {};
	/**
	 * units: %
	 * offset 19260
	 */
	uint8_t throttle2TrimTpsBins[ETB2_TRIM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 19266
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[ETB2_TRIM_SIZE] = {};
	/**
	 * units: deg
	 * offset 19272
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[KNOCK_TABLE_SIZE][KNOCK_TABLE_SIZE] = {};
	/**
	 * units: %
	 * offset 19308
	 */
	uint8_t maxKnockRetardLoadBins[KNOCK_TABLE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 19314
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[KNOCK_TABLE_SIZE] = {};
	/**
	 * units: deg
	 * offset 19320
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[ALS_SIZE][ALS_SIZE] = {};
	/**
	 * units: TPS
	 * offset 19352
	 */
	uint16_t alsIgnRetardLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 19360
	 */
	uint16_t alsIgnRetardrpmBins[ALS_SIZE] = {};
	/**
	 * units: percent
	 * offset 19368
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[ALS_SIZE][ALS_SIZE] = {};
	/**
	 * units: TPS
	 * offset 19400
	 */
	uint16_t alsFuelAdjustmentLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 19408
	 */
	uint16_t alsFuelAdjustmentrpmBins[ALS_SIZE] = {};
	/**
	 * units: ratio
	 * offset 19416
	 */
	scaled_channel<int16_t, 1, 10> ALSIgnSkipTable[ALS_SIZE][ALS_SIZE] = {};
	/**
	 * units: TPS
	 * offset 19448
	 */
	uint16_t alsIgnSkipLoadBins[ALS_SIZE] = {};
	/**
	 * units: RPM
	 * offset 19456
	 */
	uint16_t alsIgnSkiprpmBins[ALS_SIZE] = {};
	/**
	 * offset 19464
	 */
	blend_table_s ignBlends[IGN_BLEND_COUNT] = {};
	/**
	 * offset 20216
	 */
	blend_table_s veBlends[VE_BLEND_COUNT] = {};
	/**
	 * units: %
	 * offset 20968
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaBins[THR_EST_SIZE] = {};
	/**
	 * In units of g/s normalized to choked flow conditions
	 * units: g/s
	 * offset 20992
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaValues[THR_EST_SIZE] = {};
	/**
	 * offset 21016
	 */
	blend_table_s boostOpenLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * offset 21392
	 */
	blend_table_s boostClosedLoopBlends[BOOST_BLEND_COUNT] = {};
	/**
	 * units: level
	 * offset 21768
	 */
	float tcu_rangeP[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21792
	 */
	float tcu_rangeR[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21816
	 */
	float tcu_rangeN[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21840
	 */
	float tcu_rangeD[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21864
	 */
	float tcu_rangeM[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21888
	 */
	float tcu_rangeM3[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21912
	 */
	float tcu_rangeM2[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21936
	 */
	float tcu_rangeM1[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21960
	 */
	float tcu_rangePlus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 21984
	 */
	float tcu_rangeMinus[RANGE_INPUT_COUNT] = {};
	/**
	 * units: level
	 * offset 22008
	 */
	float tcu_rangeLow[RANGE_INPUT_COUNT] = {};
	/**
	 * units: lambda
	 * offset 22032
	 */
	scaled_channel<uint8_t, 100, 1> lambdaMaxDeviationTable[LAM_SIZE][LAM_SIZE] = {};
	/**
	 * offset 22048
	 */
	uint16_t lambdaMaxDeviationLoadBins[LAM_SIZE] = {};
	/**
	 * units: RPM
	 * offset 22056
	 */
	uint16_t lambdaMaxDeviationRpmBins[LAM_SIZE] = {};
	/**
	 * units: %
	 * offset 22064
	 */
	uint8_t injectorStagingTable[INJ_STAGING_COUNT][INJ_STAGING_COUNT] = {};
	/**
	 * offset 22100
	 */
	uint16_t injectorStagingLoadBins[INJ_STAGING_COUNT] = {};
	/**
	 * units: RPM
	 * offset 22112
	 */
	uint16_t injectorStagingRpmBins[INJ_STAGING_COUNT] = {};
	/**
	 * units: deg C
	 * offset 22124
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 22132
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 22140
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: kPa
	 * offset 22148
	 */
	uint8_t wwMapBins[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 22156
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * offset 22164
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE] = {};
	/**
	 * units: %
	 * offset 22172
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: deg
	 * offset 22188
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE] = {};
	/**
	 * units: volts
	 * offset 22204
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: ms
	 * offset 22212
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE] = {};
	/**
	 * units: kPa
	 * offset 22228
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE] = {};
	/**
	 * units: load
	 * offset 22428
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 22448
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE] = {};
	/**
	 * units: %
	 * offset 22458
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: cc/lobe
	 * offset 22558
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: RPM
	 * offset 22578
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE] = {};
	/**
	 * units: %
	 * offset 22588
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensation[HPFP_FUEL_MASS_COMPENSATION_SIZE][HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 22716
	 */
	scaled_channel<uint16_t, 100, 1> hpfpFuelMassCompensationFuelMass[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: bar
	 * offset 22732
	 */
	scaled_channel<uint16_t, 10, 1> hpfpFuelMassCompensationFuelPressure[HPFP_FUEL_MASS_COMPENSATION_SIZE] = {};
	/**
	 * units: ms
	 * offset 22748
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearization[FLOW_LINEARIZATION_PRESSURE_SIZE][FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: fuel mass/mg
	 * offset 22756
	 */
	scaled_channel<uint16_t, 100, 1> injectorFlowLinearizationFuelMassBins[FLOW_LINEARIZATION_MASS_SIZE] = {};
	/**
	 * units: bar
	 * offset 22760
	 */
	scaled_channel<uint16_t, 10, 1> injectorFlowLinearizationPressureBins[FLOW_LINEARIZATION_PRESSURE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 22764
	 */
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	 * units: dB
	 * offset 22796
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 22812
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: multiplier
	 * offset 22816
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE] = {};
	/**
	 * units: C
	 * offset 22820
	 */
	scaled_channel<int8_t, 1, 5> cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 22824
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE] = {};
	/**
	 * units: volt
	 * offset 22832
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: %
	 * offset 22848
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT] = {};
	/**
	 * units: volts
	 * offset 22856
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE] = {};
	/**
	 * units: multiplier
	 * offset 22864
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE] = {};
	/**
	 * units: %
	 * offset 22872
	 */
	scaled_channel<uint8_t, 1, 1> tcu_shiftTpsBins[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 22880
	 */
	uint8_t tcu_shiftSpeed12[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 22888
	 */
	uint8_t tcu_shiftSpeed23[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 22896
	 */
	uint8_t tcu_shiftSpeed34[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 22904
	 */
	uint8_t tcu_shiftSpeed21[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 22912
	 */
	uint8_t tcu_shiftSpeed32[TCU_TABLE_WIDTH] = {};
	/**
	 * units: MPH
	 * offset 22920
	 */
	uint8_t tcu_shiftSpeed43[TCU_TABLE_WIDTH] = {};
	/**
	 * units: ms
	 * offset 22928
	 */
	float tcu_shiftTime;
	/**
	 * units: Volts
	 * offset 22932
	 */
	scaled_channel<int16_t, 10, 1> alternatorVoltageTargetTable[ALTERNATOR_VOLTAGE_TARGET_SIZE][ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: Load
	 * offset 22964
	 */
	uint16_t alternatorVoltageTargetLoadBins[ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: RPM
	 * offset 22972
	 */
	uint16_t alternatorVoltageTargetRpmBins[ALTERNATOR_VOLTAGE_TARGET_SIZE] = {};
	/**
	 * units: C
	 * offset 22980
	 */
	float cltBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 23000
	 */
	float cltBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 23020
	 */
	float iatBoostCorrBins[BOOST_CURVE_SIZE] = {};
	/**
	 * units: ratio
	 * offset 23040
	 */
	float iatBoostCorr[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 23060
	 */
	float cltBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 23080
	 */
	float cltBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * units: C
	 * offset 23100
	 */
	float iatBoostAdderBins[BOOST_CURVE_SIZE] = {};
	/**
	 * offset 23120
	 */
	float iatBoostAdder[BOOST_CURVE_SIZE] = {};
	/**
	 * units: RPM
	 * offset 23140
	 */
	scaled_channel<uint8_t, 1, 100> minimumOilPressureBins[8] = {};
	/**
	 * units: kPa
	 * offset 23148
	 */
	scaled_channel<uint8_t, 1, 10> minimumOilPressureValues[8] = {};
	/**
	 * offset 23156
	 */
	blend_table_s targetAfrBlends[TARGET_AFR_BLEND_COUNT] = {};
	/**
	 * units: RPM
	 * offset 23532
	 */
	scaled_channel<uint8_t, 1, 100> trimRpmBins[FUEL_TRIM_RPM_COUNT] = {};
	/**
	 * offset 23540
	 */
	uint16_t trimLoadBins[FUEL_TRIM_LOAD_COUNT] = {};
	/**
	 * @@DYNO_RPM_STEP_TOOLTIP@@
	 * units: Rpm
	 * offset 23556
	 */
	scaled_channel<uint8_t, 1, 1> dynoRpmStep;
	/**
	 * @@DYNO_SAE_TEMPERATURE_C_TOOLTIP@@
	 * units: C
	 * offset 23557
	 */
	scaled_channel<int8_t, 1, 1> dynoSaeTemperatureC;
	/**
	 * @@DYNO_SAE_RELATIVE_HUMIDITY_TOOLTIP@@
	 * units: %
	 * offset 23558
	 */
	scaled_channel<uint8_t, 1, 1> dynoSaeRelativeHumidity;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 23559
	 */
	uint8_t alignmentFill_at_23559[1] = {};
	/**
	 * @@DYNO_SAE_BARO_TOOLTIP@@
	 * units: KPa
	 * offset 23560
	 */
	scaled_channel<float, 1, 1> dynoSaeBaro;
	/**
	 * @@DYNO_CAR_WHEEL_DIA_INCH_TOOLTIP@@
	 * units: Inch
	 * offset 23564
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelDiaInch;
	/**
	 * @@DYNO_CAR_WHEEL_ASPECT_RATIO_TOOLTIP@@
	 * units: Aspect Ratio (height)
	 * offset 23565
	 */
	scaled_channel<int8_t, 1, 1> dynoCarWheelAspectRatio;
	/**
	 * @@DYNO_CAR_WHEEL_TIRE_WIDTH_TOOLTIP@@
	 * units: Width mm
	 * offset 23566
	 */
	scaled_channel<int16_t, 1, 1> dynoCarWheelTireWidthMm;
	/**
	 * @@DYNO_CAR_GEAR_PRIMARY_REDUCTION_TOOLTIP@@
	 * units: Units
	 * offset 23568
	 */
	scaled_channel<float, 1, 1> dynoCarGearPrimaryReduction;
	/**
	 * @@DYNO_CAR_GEAR_RATIO_TOOLTIP@@
	 * units: Units
	 * offset 23572
	 */
	scaled_channel<float, 1, 1> dynoCarGearRatio;
	/**
	 * @@DYNO_CAR_GEAR_FINAL_DRIVE_TOOLTIP@@
	 * units: Units
	 * offset 23576
	 */
	scaled_channel<float, 1, 1> dynoCarGearFinalDrive;
	/**
	 * @@DYNO_CAR_CAR_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 23580
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCarMassKg;
	/**
	 * @@DYNO_CAR_CARGO_MASS_TOOLTIP@@
	 * units: Kg
	 * offset 23582
	 */
	scaled_channel<int16_t, 1, 1> dynoCarCargoMassKg;
	/**
	 * @@DYNO_CAR_COEFF_OF_DRAG_TOOLTIP@@
	 * units: Coeff
	 * offset 23584
	 */
	scaled_channel<float, 1, 1> dynoCarCoeffOfDrag;
	/**
	 * @@DYNO_CAR_FRONTAL_AREA_TOOLTIP@@
	 * units: m2
	 * offset 23588
	 */
	scaled_channel<float, 1, 1> dynoCarFrontalAreaM2;
	/**
	 * units: deg
	 * offset 23592
	 */
	scaled_channel<int8_t, 10, 1> trailingSparkTable[TRAILING_SPARK_SIZE][TRAILING_SPARK_SIZE] = {};
	/**
	 * units: rpm
	 * offset 23608
	 */
	scaled_channel<uint8_t, 1, 50> trailingSparkRpmBins[TRAILING_SPARK_SIZE] = {};
	/**
	 * units: Load
	 * offset 23612
	 */
	scaled_channel<uint8_t, 1, 5> trailingSparkLoadBins[TRAILING_SPARK_SIZE] = {};
	/**
	 * units: RPM
	 * offset 23616
	 */
	scaled_channel<uint8_t, 1, 100> maximumOilPressureBins[4] = {};
	/**
	 * units: kPa
	 * offset 23620
	 */
	scaled_channel<uint8_t, 1, 10> maximumOilPressureValues[4] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 23624
	 */
	gppwm_channel_e torqueReductionCutXaxis;
	/**
	 * How many % of ignition events will be cut
	 * units: %
	 * offset 23625
	 */
	int8_t torqueReductionIgnitionCutTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 23637
	 */
	uint8_t alignmentFill_at_23637[1] = {};
	/**
	 * offset 23638
	 */
	int16_t torqueReductionCutXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 23650
	 */
	int8_t torqueReductionCutGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 23652
	 */
	gppwm_channel_e torqueReductionTimeXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 23653
	 */
	uint8_t alignmentFill_at_23653[3] = {};
	/**
	 * For how long after the pin has been triggered will the cut/reduction stay active. After that, even if the pin is still triggered, torque is re-introduced
	 * units: ms
	 * offset 23656
	 */
	float torqueReductionTimeTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 23704
	 */
	int16_t torqueReductionTimeXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 23716
	 */
	int8_t torqueReductionTimeGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * Selects the X axis to use for the table.
	 * offset 23718
	 */
	gppwm_channel_e torqueReductionIgnitionRetardXaxis;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 23719
	 */
	uint8_t alignmentFill_at_23719[1] = {};
	/**
	 * How many degrees of timing advance will be reduced during the Torque Reduction Time
	 * units: deg
	 * offset 23720
	 */
	float torqueReductionIgnitionRetardTable[TORQUE_TABLE_Y_SIZE][TORQUE_TABLE_X_SIZE] = {};
	/**
	 * offset 23768
	 */
	int16_t torqueReductionIgnitionRetardXBins[TORQUE_TABLE_X_SIZE] = {};
	/**
	 * units: gear N°
	 * offset 23780
	 */
	int8_t torqueReductionIgnitionRetardGearBins[TORQUE_TABLE_Y_SIZE] = {};
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 23782
	 */
	uint8_t alignmentFill_at_23782[2] = {};
};
static_assert(sizeof(persistent_config_s) == 23784);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt
