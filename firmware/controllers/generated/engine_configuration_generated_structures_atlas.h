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
	uint8_t alignmentFill_at_1[1];
	/**
	 * Below this engine load, the overrun region is active
	 * units: load
	 * offset 2
	 */
	uint16_t maxOverrunLoad;
	/**
	 * Above this engine load, the power region is active
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
	uint8_t alignmentFill_at_11[1];
	/**
	 * offset 12
	 */
	stft_cell_cfg_s cellCfgs[STFT_CELL_COUNT];
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
	uint8_t alignmentFill_at_2[2];
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
	 * offset 10
	 */
	scaled_channel<int16_t, 2, 1> loadBins[GPPWM_LOAD_COUNT];
	/**
	 * offset 26
	 */
	int16_t rpmBins[GPPWM_RPM_COUNT];
	/**
	 * units: duty
	 * offset 34
	 */
	scaled_channel<uint8_t, 2, 1> table[GPPWM_LOAD_COUNT][GPPWM_RPM_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 66
	 */
	uint8_t alignmentFill_at_66[2];
};
static_assert(sizeof(gppwm_channel) == 68);

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
	 * This is your injector flow at the fuel pressure used in the vehicle
	 * See units setting below
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
	 * Just for reference really, not taken into account by any logic at this point
	 * units: CR
	 * offset 16
	 */
	float compressionRatio;
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	 * units: mv
	 * offset 20
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	 * units: mv
	 * offset 22
	 */
	uint16_t wastegatePositionMax;
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
	 * Value between 0 and 100 used in Manual mode
	 * units: %
	 * offset 44
	 */
	float manIdlePosition;
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
	 * units: RPM
	 * offset 50
	 */
	int16_t vvtControlMinRpm;
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
	 * TLE7209 and L6205 use two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
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
	 * Maximum absolute ignition timing while smothering
	 * units: deg
	 * offset 192
	 */
	angle_t maxAdvanceSmoothing;
	/**
	 * Minimum absolute ignition timing while smothering
	 * units: deg
	 * offset 196
	 */
	angle_t minAdvanceSmoothing;
	/**
	 * Maximum ignition timing increase while smothering
	 * units: deg
	 * offset 200
	 */
	angle_t increaseAdvanceSmoothing;
	/**
	 * Maximum ignition timing decrease while smothering
	 * units: deg
	 * offset 204
	 */
	angle_t decreaseAdvanceSmoothing;
	/**
	 * Engine cicles that the advance correction will stay on
	 * units: cicles
	 * offset 208
	 */
	int16_t timeoutAdvanceSmoothing;
	/**
	 * Minimum engine load that corrections will occour
	 * units: %
	 * offset 210
	 */
	scaled_channel<int16_t, 1, 1> deltaLoadSmoothingThreshold;
	/**
	 * Activate this to increment or decrement ignition angle after fast TPS variation, can improve drivability;
	offset 212 bit 0 */
	bool enableAdvanceSmoothing : 1 {};
	/**
	offset 212 bit 1 */
	bool unusedBit_70_1 : 1 {};
	/**
	offset 212 bit 2 */
	bool unusedBit_70_2 : 1 {};
	/**
	offset 212 bit 3 */
	bool unusedBit_70_3 : 1 {};
	/**
	offset 212 bit 4 */
	bool unusedBit_70_4 : 1 {};
	/**
	offset 212 bit 5 */
	bool unusedBit_70_5 : 1 {};
	/**
	offset 212 bit 6 */
	bool unusedBit_70_6 : 1 {};
	/**
	offset 212 bit 7 */
	bool unusedBit_70_7 : 1 {};
	/**
	offset 212 bit 8 */
	bool unusedBit_70_8 : 1 {};
	/**
	offset 212 bit 9 */
	bool unusedBit_70_9 : 1 {};
	/**
	offset 212 bit 10 */
	bool unusedBit_70_10 : 1 {};
	/**
	offset 212 bit 11 */
	bool unusedBit_70_11 : 1 {};
	/**
	offset 212 bit 12 */
	bool unusedBit_70_12 : 1 {};
	/**
	offset 212 bit 13 */
	bool unusedBit_70_13 : 1 {};
	/**
	offset 212 bit 14 */
	bool unusedBit_70_14 : 1 {};
	/**
	offset 212 bit 15 */
	bool unusedBit_70_15 : 1 {};
	/**
	offset 212 bit 16 */
	bool unusedBit_70_16 : 1 {};
	/**
	offset 212 bit 17 */
	bool unusedBit_70_17 : 1 {};
	/**
	offset 212 bit 18 */
	bool unusedBit_70_18 : 1 {};
	/**
	offset 212 bit 19 */
	bool unusedBit_70_19 : 1 {};
	/**
	offset 212 bit 20 */
	bool unusedBit_70_20 : 1 {};
	/**
	offset 212 bit 21 */
	bool unusedBit_70_21 : 1 {};
	/**
	offset 212 bit 22 */
	bool unusedBit_70_22 : 1 {};
	/**
	offset 212 bit 23 */
	bool unusedBit_70_23 : 1 {};
	/**
	offset 212 bit 24 */
	bool unusedBit_70_24 : 1 {};
	/**
	offset 212 bit 25 */
	bool unusedBit_70_25 : 1 {};
	/**
	offset 212 bit 26 */
	bool unusedBit_70_26 : 1 {};
	/**
	offset 212 bit 27 */
	bool unusedBit_70_27 : 1 {};
	/**
	offset 212 bit 28 */
	bool unusedBit_70_28 : 1 {};
	/**
	offset 212 bit 29 */
	bool unusedBit_70_29 : 1 {};
	/**
	offset 212 bit 30 */
	bool unusedBit_70_30 : 1 {};
	/**
	offset 212 bit 31 */
	bool unusedBit_70_31 : 1 {};
	/**
	 * Strenght of advance correction
	 * units: %
	 * offset 216
	 */
	uint8_t strenghtAdvanceSmoothing;
	/**
	 * Theshold to enable fan 1 after ac turned on
	 * units: sec
	 * offset 217
	 */
	uint8_t Fan1AcThresholdOn;
	/**
	 * Theshold to disable fan 1 after ac tuned off
	 * units: sec
	 * offset 218
	 */
	uint8_t Fan1AcThresholdOff;
	/**
	 * Theshold to enable fan 2 after ac turned on
	 * units: sec
	 * offset 219
	 */
	uint8_t Fan2AcThresholdOn;
	/**
	 * Theshold to disable fan 2 after ac tuned off
	 * units: sec
	 * offset 220
	 */
	uint8_t Fan2AcThresholdOff;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 221
	 */
	uint8_t alignmentFill_at_221[3];
	/**
	 * Use idle ignition table while coasting;
	offset 224 bit 0 */
	bool useIdleAdvanceWhileCoasting : 1 {};
	/**
	offset 224 bit 1 */
	bool unusedBit_108_1 : 1 {};
	/**
	offset 224 bit 2 */
	bool unusedBit_108_2 : 1 {};
	/**
	offset 224 bit 3 */
	bool unusedBit_108_3 : 1 {};
	/**
	offset 224 bit 4 */
	bool unusedBit_108_4 : 1 {};
	/**
	offset 224 bit 5 */
	bool unusedBit_108_5 : 1 {};
	/**
	offset 224 bit 6 */
	bool unusedBit_108_6 : 1 {};
	/**
	offset 224 bit 7 */
	bool unusedBit_108_7 : 1 {};
	/**
	offset 224 bit 8 */
	bool unusedBit_108_8 : 1 {};
	/**
	offset 224 bit 9 */
	bool unusedBit_108_9 : 1 {};
	/**
	offset 224 bit 10 */
	bool unusedBit_108_10 : 1 {};
	/**
	offset 224 bit 11 */
	bool unusedBit_108_11 : 1 {};
	/**
	offset 224 bit 12 */
	bool unusedBit_108_12 : 1 {};
	/**
	offset 224 bit 13 */
	bool unusedBit_108_13 : 1 {};
	/**
	offset 224 bit 14 */
	bool unusedBit_108_14 : 1 {};
	/**
	offset 224 bit 15 */
	bool unusedBit_108_15 : 1 {};
	/**
	offset 224 bit 16 */
	bool unusedBit_108_16 : 1 {};
	/**
	offset 224 bit 17 */
	bool unusedBit_108_17 : 1 {};
	/**
	offset 224 bit 18 */
	bool unusedBit_108_18 : 1 {};
	/**
	offset 224 bit 19 */
	bool unusedBit_108_19 : 1 {};
	/**
	offset 224 bit 20 */
	bool unusedBit_108_20 : 1 {};
	/**
	offset 224 bit 21 */
	bool unusedBit_108_21 : 1 {};
	/**
	offset 224 bit 22 */
	bool unusedBit_108_22 : 1 {};
	/**
	offset 224 bit 23 */
	bool unusedBit_108_23 : 1 {};
	/**
	offset 224 bit 24 */
	bool unusedBit_108_24 : 1 {};
	/**
	offset 224 bit 25 */
	bool unusedBit_108_25 : 1 {};
	/**
	offset 224 bit 26 */
	bool unusedBit_108_26 : 1 {};
	/**
	offset 224 bit 27 */
	bool unusedBit_108_27 : 1 {};
	/**
	offset 224 bit 28 */
	bool unusedBit_108_28 : 1 {};
	/**
	offset 224 bit 29 */
	bool unusedBit_108_29 : 1 {};
	/**
	offset 224 bit 30 */
	bool unusedBit_108_30 : 1 {};
	/**
	offset 224 bit 31 */
	bool unusedBit_108_31 : 1 {};
	/**
	 * Closed throttle, 1 volt = 200 units.
	 * See also tps1_1AdcChannel
	 * set tps_min X
	 * units: ADC
	 * offset 228
	 */
	int16_t tpsMin;
	/**
	 * Full throttle.
	 * See also tps1_1AdcChannel
	 * set tps_max X
	 * units: ADC
	 * offset 230
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection: what throttle % is unrealistically low?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 232
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection: what throttle % is unrealistically high?
	 * Also used for accelerator pedal error detection if so equipped.
	 * units: %
	 * offset 234
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 236
	 */
	cranking_parameters_s cranking;
	/**
	 * Dwell duration while cranking
	 * units: ms
	 * offset 244
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * Once engine speed passes this value, start reducing ETB angle.
	 * units: rpm
	 * offset 248
	 */
	uint16_t etbRevLimitStart;
	/**
	 * This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	 * units: rpm
	 * offset 250
	 */
	uint16_t etbRevLimitRange;
	/**
	 * @see isMapAveragingEnabled
	 * offset 252
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 392
	 */
	ThermistorConf clt;
	/**
	 * offset 424
	 */
	ThermistorConf iat;
	/**
	 * units: deg
	 * offset 456
	 */
	float launchTimingRetard;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	 * units: integer
	 * offset 460
	 */
	int hip9011PrescalerAndSDO;
	/**
	 * iTerm min value
	 * offset 464
	 */
	int16_t alternator_iTermMin;
	/**
	 * iTerm max value
	 * offset 466
	 */
	int16_t alternator_iTermMax;
	/**
	 * @@DISPLACEMENT_TOOLTIP@@
	 * units: L
	 * offset 468
	 */
	scaled_channel<uint16_t, 1000, 1> displacement;
	/**
	 * units: RPM
	 * offset 470
	 */
	uint16_t triggerSimulatorRpm;
	/**
	 * Number of cylinder the engine has.
	 * offset 472
	 */
	uint32_t cylindersCount;
	/**
	 * offset 476
	 */
	firing_order_e firingOrder;
	/**
	 * offset 477
	 */
	uint8_t justATempTest;
	/**
	 * Delta kPa for MAP sync
	 * units: kPa
	 * offset 478
	 */
	uint8_t mapSyncThreshold;
	/**
	 * How many % of ignition events will be cut
	 * units: %
	 * offset 479
	 */
	int8_t torqueReductionIgnitionCut;
	/**
	 * @@CYLINDER_BORE_TOOLTIP@@
	 * units: mm
	 * offset 480
	 */
	float cylinderBore;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * Alpha-N means drive by TPS commonly only used for NA engines
	 * Speed Density requires MAP sensor and is the default choice for may installs
	 * MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	 * offset 484
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * units: %
	 * offset 485
	 */
	uint8_t ALSMaxTPS;
	/**
	 * This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	 * offset 486
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups.
	 * offset 487
	 */
	injection_mode_e injectionMode;
	/**
	 * Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 488
	 */
	uint16_t boostControlMinRpm;
	/**
	 * Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 490
	 */
	uint8_t boostControlMinTps;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 491
	 */
	uint8_t alignmentFill_at_491[1];
	/**
	 * Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	 * offset 492
	 */
	uint16_t boostControlMinMap;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 494
	 */
	uint8_t alignmentFill_at_494[2];
	/**
	 * Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	 * There is tapering towards running timing advance
	 * set cranking_timing_angle X
	 * units: deg
	 * offset 496
	 */
	angle_t crankingTimingAngle;
	/**
	 * Single coil = distributor
	 * Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	 * Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	 * Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	 * set ignition_mode X
	 * offset 500
	 */
	ignition_mode_e ignitionMode;
	/**
	 * How many consecutive gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 501
	 */
	int8_t gapTrackingLengthOverride;
	/**
	 * Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	 * units: kph
	 * offset 502
	 */
	uint8_t maxIdleVss;
	/**
	 * offset 503
	 */
	uint8_t camDecoder2jzPrecision;
	/**
	 * Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	 * units: kPa
	 * offset 504
	 */
	uint16_t minOilPressureAfterStart;
	/**
	 * Dynamic uses the timing map to decide the ignition timing
	 * Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	 * offset 506
	 */
	timing_mode_e timingMode;
	/**
	 * offset 507
	 */
	can_nbc_e canNbcType;
	/**
	 * This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful when adjusting distributor location.
	 * units: RPM
	 * offset 508
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
	 * offset 512
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	 * units: coef
	 * offset 516
	 */
	float analogInputDividerCoefficient;
	/**
	 * This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	 * units: coef
	 * offset 520
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: deg C
	 * offset 524
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: deg C
	 * offset 528
	 */
	float fanOffTemperature;
	/**
	 * offset 532
	 */
	output_pin_e acrPin;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 534
	 */
	uint8_t alignmentFill_at_534[2];
	/**
	 * Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	 * units: revs/km
	 * offset 536
	 */
	float driveWheelRevPerKm;
	/**
	 * CANbus thread period in ms
	 * units: ms
	 * offset 540
	 */
	int canSleepPeriodMs;
	/**
	 * units: index
	 * offset 544
	 */
	int byFirmwareVersion;
	/**
	 * First throttle body, first sensor. See also pedalPositionAdcChannel
	 * offset 548
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 549
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 550
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * offset 551
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 552
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	 * units: angle
	 * offset 556
	 */
	int trailingSparkAngle;
	/**
	 * offset 560
	 */
	trigger_config_s trigger;
	/**
	 * Extra air taper amount
	 * units: %
	 * offset 572
	 */
	float airByRpmTaper;
	/**
	 * offset 576
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	 * units: %
	 * offset 577
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * offset 578
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 579
	 */
	uint8_t acrRevolutions;
	/**
	 * offset 580
	 */
	int calibrationBirthday;
	/**
	 * units: volts
	 * offset 584
	 */
	float adcVcc;
	/**
	 * Magic engine phase: we compare instant MAP at X to instant MAP at x+360 angle in one complete cycle
	 * units: Deg
	 * offset 588
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 592
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 600
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 620
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * TPS/PPS error threshold
	 * units: %
	 * offset 621
	 */
	scaled_channel<uint8_t, 10, 1> etbSplit;
	/**
	 * offset 622
	 */
	Gpio tle6240_cs;
	/**
	 * offset 624
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * offset 625
	 */
	pin_output_mode_e mc33810_csPinMode;
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 626
	 */
	switch_input_pin_e unusedThrottlePedalUpPin;
	/**
	 * @see hasBaroSensor
	 * offset 628
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 640
	 */
	idle_hardware_s idle;
	/**
	 * Ignition timing to remove when a knock event occurs. Advice: 5% (mild), 10% (turbo/high comp.), 15% (high knock, e.g. GDI), 20% (spicy lump),
	 * units: %
	 * offset 652
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	 * units: deg/s
	 * offset 653
	 */
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	/**
	 * Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	 * offset 654
	 */
	engineSyncCam_e engineSyncCam;
	/**
	 * offset 655
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	 * units: ratio
	 * offset 656
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 658
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	 * units: count
	 * offset 659
	 */
	uint8_t vssToothCount;
	/**
	 * Override the Y axis (load) value used for only the Idle VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 660
	 */
	ve_override_e idleVeOverrideMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 661
	 */
	uint8_t alignmentFill_at_661[1];
	/**
	 * offset 662
	 */
	Gpio l9779_cs;
	/**
	 * offset 664
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 688
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 712
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 713
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 714
	 */
	output_pin_e fuelPumpPin;
	/**
	 * offset 716
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * How many consecutive VVT gap rations have to match expected ranges for sync to happen
	 * units: count
	 * offset 717
	 */
	int8_t gapVvtTrackingLengthOverride;
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 718
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * offset 720
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 721
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 722
	 */
	output_pin_e fanPin;
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 724
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * offset 726
	 */
	output_pin_e alternatorControlPin;
	/**
	 * offset 728
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 729
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 730
	 */
	Gpio digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 738
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 739
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 740
	 */
	Gpio debugTriggerSync;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 742
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 743
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 744
	 */
	Gpio mc33972_cs;
	/**
	 * offset 746
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * Useful in Research&Development phase
	 * offset 747
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * First throttle body, second sensor.
	 * offset 748
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * Second throttle body, second sensor.
	 * offset 749
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 750
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 751
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 752
	 */
	Gpio mc33810_cs[C_MC33810_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 754
	 */
	uint8_t alignmentFill_at_754[2];
	/**
	 * 0.1 is a good default value
	 * units: x
	 * offset 756
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 760
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	 * units: us
	 * offset 764
	 */
	uint16_t mc33_t_min_boost;
	/**
	 * offset 766
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * offset 767
	 */
	pin_input_mode_e unusedThrottlePedalUpPinMode;
	/**
	 * Ratio between the wheels and your transmission output.
	 * units: ratio
	 * offset 768
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 770
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 772
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 773
	 */
	uint8_t alignmentFill_at_773[1];
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 774
	 */
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * units: g/s
	 * offset 778
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	/**
	 * offset 780
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 782
	 */
	adc_channel_e maf2AdcChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 783
	 */
	uint8_t alignmentFill_at_783[1];
	/**
	 * On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	 * offset 784
	 */
	output_pin_e o2heaterPin;
	/**
	 * offset 786
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * units: RPM
	 * offset 787
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionMinRpm;
	/**
	 * units: %
	 * offset 788
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionMinLoad;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 789
	 */
	uint8_t alignmentFill_at_789[3];
	/**
	offset 792 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 792 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 792 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 792 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 792 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 792 bit 5 */
	bool rethrowHardFault : 1 {};
	/**
	offset 792 bit 6 */
	bool isHip9011Enabled : 1 {};
	/**
	offset 792 bit 7 */
	bool requireFootOnBrakeToCrank : 1 {};
	/**
	offset 792 bit 8 */
	bool verboseQuad : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 792 bit 9 */
	bool useStepperIdle : 1 {};
	/**
	offset 792 bit 10 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 792 bit 11 */
	bool useTpicAdvancedMode : 1 {};
	/**
	offset 792 bit 12 */
	bool lambdaProtectionEnable : 1 {};
	/**
	offset 792 bit 13 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 792 bit 14 */
	bool enableVerboseCanTx : 1 {};
	/**
	offset 792 bit 15 */
	bool externalRusEfiGdiModule : 1 {};
	/**
	offset 792 bit 16 */
	bool flipWboChannels : 1 {};
	/**
	 * Useful for individual intakes
	offset 792 bit 17 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 792 bit 18 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 792 bit 19 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 792 bit 20 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	offset 792 bit 21 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 792 bit 22 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 792 bit 23 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 792 bit 24 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 792 bit 25 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 792 bit 26 */
	bool pauseEtbControl : 1 {};
	/**
	offset 792 bit 27 */
	bool tpsTpsPercentMode : 1 {};
	/**
	offset 792 bit 28 */
	bool verboseKLine : 1 {};
	/**
	offset 792 bit 29 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 792 bit 30 */
	bool enableAemXSeries : 1 {};
	/**
	offset 792 bit 31 */
	bool unused32nd : 1 {};
	/**
	 * offset 796
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 804
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 805
	 */
	uint8_t alignmentFill_at_805[1];
	/**
	 * offset 806
	 */
	Gpio hip9011CsPin;
	/**
	 * offset 808
	 */
	Gpio hip9011IntHoldPin;
	/**
	 * offset 810
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 811
	 */
	uint8_t alignmentFill_at_811[1];
	/**
	 * offset 812
	 */
	uint32_t verboseCanBaseAddress;
	/**
	 * Boost Voltage
	 * units: v
	 * offset 816
	 */
	uint8_t mc33_hvolt;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 817
	 */
	uint8_t alignmentFill_at_817[1];
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	 * units: kPa
	 * offset 818
	 */
	uint16_t minimumBoostClosedLoopMap;
	/**
	 * units: %
	 * offset 820
	 */
	int8_t initialIgnitionCutPercent;
	/**
	 * units: %
	 * offset 821
	 */
	int8_t finalIgnitionCutPercentBeforeLaunch;
	/**
	 * offset 822
	 */
	gppwm_channel_e boostOpenLoopYAxis;
	/**
	 * offset 823
	 */
	spi_device_e l9779spiDevice;
	/**
	 * offset 824
	 */
	imu_type_e imuType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 825
	 */
	uint8_t alignmentFill_at_825[1];
	/**
	 * units: kg
	 * offset 826
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling, i.e. coasting detection threshold.
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	 * units: RPM
	 * offset 828
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	 * units: ms
	 * offset 830
	 */
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	/**
	 * offset 832
	 */
	Gpio lps25BaroSensorScl;
	/**
	 * offset 834
	 */
	Gpio lps25BaroSensorSda;
	/**
	 * offset 836
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 838
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * offset 840
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * offset 841
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 842
	 */
	Gpio max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 858
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * Since torque reduction pin is usually shared with launch control, most people have an RPM where behavior under that is Launch Control, over that is Flat Shift/Torque Reduction
	 * units: rpm
	 * offset 860
	 */
	uint16_t torqueReductionArmingRpm;
	/**
	 * offset 862
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 863
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	 * units: :1
	 * offset 864
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	 * units: %
	 * offset 865
	 */
	uint8_t etbMaximumPosition;
	/**
	 * Rate the ECU will log to the SD card, in hz (log lines per second).
	 * units: hz
	 * offset 866
	 */
	uint16_t sdCardLogFrequency;
	/**
	 * offset 868
	 */
	adc_channel_e idlePositionChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 869
	 */
	uint8_t alignmentFill_at_869[1];
	/**
	 * offset 870
	 */
	uint16_t launchCorrectionsEndRpm;
	/**
	 * offset 872
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 874
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 875
	 */
	uint8_t alignmentFill_at_875[1];
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 876
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * See also starterControlPin
	 * offset 878
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * units: RPM
	 * offset 880
	 */
	scaled_channel<uint8_t, 1, 100> lambdaProtectionRestoreRpm;
	/**
	 * offset 881
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 882
	 */
	uint8_t alignmentFill_at_882[2];
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	 * units: count
	 * offset 884
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	 * units: %
	 * offset 888
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * units: %
	 * offset 890
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * units: ADC
	 * offset 892
	 */
	uint16_t tps1SecondaryMin;
	/**
	 * units: ADC
	 * offset 894
	 */
	uint16_t tps1SecondaryMax;
	/**
	 * units: rpm
	 * offset 896
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	 * units: Seconds
	 * offset 898
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 900
	 */
	Gpio triggerErrorPin;
	/**
	 * offset 902
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 903
	 */
	uint8_t alignmentFill_at_903[1];
	/**
	 * offset 904
	 */
	output_pin_e acRelayPin;
	/**
	 * units: %
	 * offset 906
	 */
	uint8_t lambdaProtectionMinTps;
	/**
	 * Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed
	 * units: s
	 * offset 907
	 */
	scaled_channel<uint8_t, 10, 1> lambdaProtectionTimeout;
	/**
	 * offset 908
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 940
	 */
	Gpio spi1mosiPin;
	/**
	 * offset 942
	 */
	Gpio spi1misoPin;
	/**
	 * offset 944
	 */
	Gpio spi1sckPin;
	/**
	 * offset 946
	 */
	Gpio spi2mosiPin;
	/**
	 * offset 948
	 */
	Gpio spi2misoPin;
	/**
	 * offset 950
	 */
	Gpio spi2sckPin;
	/**
	 * offset 952
	 */
	Gpio spi3mosiPin;
	/**
	 * offset 954
	 */
	Gpio spi3misoPin;
	/**
	 * offset 956
	 */
	Gpio spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 958
	 */
	Gpio cdmInputPin;
	/**
	 * offset 960
	 */
	uart_device_e consoleUartDevice;
	/**
	 * rusEFI console Sensor Sniffer mode
	 * offset 961
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 962
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 963
	 */
	uint8_t alignmentFill_at_963[1];
	/**
	offset 964 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 964 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 964 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 964 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 964 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 964 bit 5 */
	bool canInputBCM : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	offset 964 bit 6 */
	bool consumeObdSensors : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 964 bit 7 */
	bool enableCanVss : 1 {};
	/**
	offset 964 bit 8 */
	bool suppressWboWorkaround7048 : 1 {};
	/**
	offset 964 bit 9 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	offset 964 bit 10 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 964 bit 11 */
	bool vvtBooleanForVerySpecialCases : 1 {};
	/**
	offset 964 bit 12 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	 * enable vvt_details
	offset 964 bit 13 */
	bool verboseVVTDecoding : 1 {};
	/**
	offset 964 bit 14 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * In Alpha-N mode, compensate for air temperature.
	offset 964 bit 15 */
	bool alphaNUseIat : 1 {};
	/**
	offset 964 bit 16 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 964 bit 17 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 964 bit 18 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 964 bit 19 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 964 bit 20 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 964 bit 21 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 964 bit 22 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 964 bit 23 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 964 bit 24 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 964 bit 25 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 964 bit 26 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 964 bit 27 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 964 bit 28 */
	bool tcuEnabled : 1 {};
	/**
	offset 964 bit 29 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coils
	offset 964 bit 30 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 964 bit 31 */
	bool verboseCan2 : 1 {};
	/**
	 * offset 968
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 984
	 */
	output_pin_e boostControlPin;
	/**
	 * offset 986
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 987
	 */
	uint8_t alignmentFill_at_987[1];
	/**
	 * offset 988
	 */
	switch_input_pin_e ALSActivatePin;
	/**
	 * offset 990
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * offset 992
	 */
	pid_s boostPid;
	/**
	 * offset 1012
	 */
	boostType_e boostType;
	/**
	 * offset 1013
	 */
	pin_input_mode_e ignitionKeyDigitalPinMode;
	/**
	 * offset 1014
	 */
	Gpio ignitionKeyDigitalPin;
	/**
	 * How long it takes to enter standby mode, in seconds. Any value below 60 will cause the ECU to not enter standby, with an average consumption of 80mA
	 * units: s
	 * offset 1016
	 */
	uint16_t standbyTimeout;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1018
	 */
	uint8_t alignmentFill_at_1018[2];
	/**
	 * units: Hz
	 * offset 1020
	 */
	int boostPwmFrequency;
	/**
	 * offset 1024
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1025
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	 * units: sec
	 * offset 1026
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	 * units: sec
	 * offset 1027
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * Pause closed loop fueling after acceleration fuel occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel accel.
	 * units: sec
	 * offset 1028
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterAccelTime;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1029
	 */
	uint8_t alignmentFill_at_1029[3];
	/**
	 * Launch disabled above this speed if setting is above zero
	 * units: Kph
	 * offset 1032
	 */
	int launchSpeedThreshold;
	/**
	 * Starting Launch RPM window to activate (subtracts from Launch RPM)
	 * units: RPM
	 * offset 1036
	 */
	int launchRpmWindow;
	/**
	 * units: ms
	 * offset 1040
	 */
	float triggerEventsTimeoutMs;
	/**
	 * offset 1044
	 */
	float mapExpAverageAlpha;
	/**
	 * offset 1048
	 */
	float magicNumberAvailableForDevTricks;
	/**
	 * offset 1052
	 */
	float turbochargerFilter;
	/**
	 * offset 1056
	 */
	int launchTpsThreshold;
	/**
	 * offset 1060
	 */
	float launchActivateDelay;
	/**
	 * offset 1064
	 */
	stft_s stft;
	/**
	 * offset 1092
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1108
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1140
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1172
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1204
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * offset 1216
	 */
	dc_function_e etbFunctions[ETB_COUNT];
	/**
	 * offset 1218
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1219
	 */
	uint8_t alignmentFill_at_1219[1];
	/**
	 * offset 1220
	 */
	Gpio drv8860_cs;
	/**
	 * offset 1222
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1223
	 */
	uint8_t alignmentFill_at_1223[1];
	/**
	 * offset 1224
	 */
	Gpio drv8860_miso;
	/**
	 * offset 1226
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1242
	 */
	uint8_t alignmentFill_at_1242[2];
	/**
	 * Angle between cam sensor and VVT zero position
	 * units: value
	 * offset 1244
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1260
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1292
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	 * units: ADC
	 * offset 1356
	 */
	uint16_t tps2SecondaryMin;
	/**
	 * units: ADC
	 * offset 1358
	 */
	uint16_t tps2SecondaryMax;
	/**
	 * Select which bus the wideband controller is attached to.
	offset 1360 bit 0 */
	bool widebandOnSecondBus : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1360 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * On even fire engines with even number of cylinders we go wasted spark during cranking. Use this setting to disable wasted spark cranking on odd fire engines.
	offset 1360 bit 2 */
	bool oddFireEngine : 1 {};
	/**
	 * Write SD card log even when powered by USB
	offset 1360 bit 3 */
	bool alwaysWriteSdCard : 1 {};
	/**
	 * Second harmonic (aka double) is usually quieter background noise
	offset 1360 bit 4 */
	bool knockDetectionUseDoubleFrequency : 1 {};
	/**
	offset 1360 bit 5 */
	bool yesUnderstandLocking : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1360 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1360 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1360 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write. See also can1ListenMode
	offset 1360 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1360 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1360 bit 11 */
	bool enableOilPressureProtect : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1360 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1360 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1360 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * https://wiki.rusefi.com/Trigger-Configuration-Guide
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1360 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1360 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	 * Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	offset 1360 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1360 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1360 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1360 bit 20 */
	bool torqueReductionEnabled : 1 {};
	/**
	offset 1360 bit 21 */
	bool torqueReductionTriggerPinInverted : 1 {};
	/**
	offset 1360 bit 22 */
	bool limitTorqueReductionTime : 1 {};
	/**
	 * Are you a developer troubleshooting TS over CAN ISO/TP?
	offset 1360 bit 23 */
	bool verboseIsoTp : 1 {};
	/**
	offset 1360 bit 24 */
	bool engineSnifferFocusOnInputs : 1 {};
	/**
	offset 1360 bit 25 */
	bool launchActivateInverted : 1 {};
	/**
	offset 1360 bit 26 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1360 bit 27 */
	bool skippedWheelOnCam : 1 {};
	/**
	offset 1360 bit 28 */
	bool unusedBit_486_28 : 1 {};
	/**
	offset 1360 bit 29 */
	bool unusedBit_486_29 : 1 {};
	/**
	offset 1360 bit 30 */
	bool unusedBit_486_30 : 1 {};
	/**
	offset 1360 bit 31 */
	bool unusedBit_486_31 : 1 {};
	/**
	 * offset 1364
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1365
	 */
	uint8_t alignmentFill_at_1365[1];
	/**
	 * A/C button input
	 * offset 1366
	 */
	switch_input_pin_e acSwitch;
	/**
	 * offset 1368
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	 * units: %
	 * offset 1369
	 */
	uint8_t etbNeutralPosition;
	/**
	 * See also idleRpmPid
	 * offset 1370
	 */
	idle_mode_e idleMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1371
	 */
	uint8_t alignmentFill_at_1371[1];
	/**
	offset 1372 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	offset 1372 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1372 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1372 bit 3 */
	bool complexWallModel : 1 {};
	/**
	 * RPM is measured based on last 720 degrees while instant RPM is measured based on the last 90 degrees of crank revolution
	offset 1372 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1372 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1372 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra advance at low idle speeds will prevent stalling and extra retard at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1372 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1372 bit 8 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1372 bit 9 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * Verbose info in console below engineSnifferRpmThreshold
	 * enable trigger_details
	offset 1372 bit 10 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	offset 1372 bit 11 */
	bool cutFuelInAcr : 1 {};
	/**
	offset 1372 bit 12 */
	bool hondaK : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1372 bit 13 */
	bool twoWireBatchIgnition : 1 {};
	/**
	 * Read MAP sensor on ECU start-up to use as baro value.
	offset 1372 bit 14 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1372 bit 15 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT and PID idle).
	 * You probably don't need this.
	offset 1372 bit 16 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1372 bit 17 */
	bool flexCranking : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1372 bit 18 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1372 bit 19 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1372 bit 20 */
	bool launchSmoothRetard : 1 {};
	/**
	 * Some engines are OK running semi-random sequential while other engine require phase synchronization
	offset 1372 bit 21 */
	bool isPhaseSyncRequiredForIgnition : 1 {};
	/**
	 * If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	offset 1372 bit 22 */
	bool useCltBasedRpmLimit : 1 {};
	/**
	 * If enabled, don't wait for engine start to heat O2 sensors.
	 * WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	offset 1372 bit 23 */
	bool forceO2Heating : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1372 bit 24 */
	bool invertVvtControlIntake : 1 {};
	/**
	 * If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	offset 1372 bit 25 */
	bool invertVvtControlExhaust : 1 {};
	/**
	offset 1372 bit 26 */
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	/**
	 * 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	offset 1372 bit 27 */
	bool sdTriggerLog : 1 {};
	/**
	offset 1372 bit 28 */
	bool ALSActivateInverted : 1 {};
	/**
	offset 1372 bit 29 */
	bool stepper_dc_use_two_wires : 1 {};
	/**
	offset 1372 bit 30 */
	bool watchOutForLinearTime : 1 {};
	/**
	offset 1372 bit 31 */
	bool unusedBit_528_31 : 1 {};
	/**
	 * units: count
	 * offset 1376
	 */
	uint32_t engineChartSize;
	/**
	 * units: mult
	 * offset 1380
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1384
	 */
	Gpio camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * Idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	 * units: RPM
	 * offset 1392
	 */
	int16_t acIdleRpmTarget;
	/**
	 * set warningPeriod X
	 * units: seconds
	 * offset 1394
	 */
	int16_t warningPeriod;
	/**
	 * units: angle
	 * offset 1396
	 */
	float knockDetectionWindowStart;
	/**
	 * units: angle
	 * offset 1400
	 */
	float knockDetectionWindowEnd;
	/**
	 * units: ms
	 * offset 1404
	 */
	float idleStepperReactionTime;
	/**
	 * units: count
	 * offset 1408
	 */
	int idleStepperTotalSteps;
	/**
	 * Pedal position to realize that we need to reduce torque when the trigger pin is uuuh triggered
	 * offset 1412
	 */
	int torqueReductionArmingApp;
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1416
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	 * units: Seconds
	 * offset 1420
	 */
	float wwaeTau;
	/**
	 * offset 1424
	 */
	pid_s alternatorControl;
	/**
	 * offset 1444
	 */
	pid_s etb;
	/**
	 * offset 1464
	 */
	Gpio triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * RPM range above upper limit for extra air taper
	 * units: RPM
	 * offset 1468
	 */
	int16_t airTaperRpmRange;
	/**
	 * offset 1470
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * units: ADC
	 * offset 1472
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * units: ADC
	 * offset 1474
	 */
	int16_t tps2Max;
	/**
	 * See also startStopButtonPin
	 * offset 1476
	 */
	output_pin_e starterControlPin;
	/**
	 * offset 1478
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1479
	 */
	uint8_t alignmentFill_at_1479[1];
	/**
	 * offset 1480
	 */
	Gpio mc33816_flag0;
	/**
	 * units: Pulse
	 * offset 1482
	 */
	uint8_t tachPulsePerRev;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1483
	 */
	uint8_t alignmentFill_at_1483[1];
	/**
	 * kPa value which is too low to be true
	 * units: kPa
	 * offset 1484
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	 * units: kPa
	 * offset 1488
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	 * units: ms
	 * offset 1492
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	 * units: ms
	 * offset 1494
	 */
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1496
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	 * units: Fraction
	 * offset 1516
	 */
	float wwaeBeta;
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1520
	 */
	Gpio binarySerialTxPin;
	/**
	 * offset 1522
	 */
	Gpio binarySerialRxPin;
	/**
	 * offset 1524
	 */
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * offset 1528
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * offset 1530
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * units: voltage
	 * offset 1532
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * units: voltage
	 * offset 1536
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on IGN voltage detection turn fuel pump on to build fuel pressure
	 * units: seconds
	 * offset 1540
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	 * units: RPM
	 * offset 1542
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * For how long after the pin has been triggered will the cut/reduction stay active. After that, even if the pin is still triggered, torque is re-introduced
	 * units: ms
	 * offset 1544
	 */
	float torqueReductionTime;
	/**
	 * See Over/Undervoltage Shutdown/Retry bit in documentation
	offset 1548 bit 0 */
	bool mc33810DisableRecoveryMode : 1 {};
	/**
	offset 1548 bit 1 */
	bool mc33810Gpgd0Mode : 1 {};
	/**
	offset 1548 bit 2 */
	bool mc33810Gpgd1Mode : 1 {};
	/**
	offset 1548 bit 3 */
	bool mc33810Gpgd2Mode : 1 {};
	/**
	offset 1548 bit 4 */
	bool mc33810Gpgd3Mode : 1 {};
	/**
	 * Send out board statistics
	offset 1548 bit 5 */
	bool enableExtendedCanBroadcast : 1 {};
	/**
	 * global_can_data performance hack
	offset 1548 bit 6 */
	bool luaCanRxWorkaround : 1 {};
	/**
	offset 1548 bit 7 */
	bool flexSensorInverted : 1 {};
	/**
	offset 1548 bit 8 */
	bool useHardSkipInTraction : 1 {};
	/**
	 * Use Aux Speed 1 as one of speeds for wheel slip ratio?
	offset 1548 bit 9 */
	bool useAuxSpeedForSlipRatio : 1 {};
	/**
	 * VSS and Aux Speed 1 or Aux Speed 1 with Aux Speed 2?
	offset 1548 bit 10 */
	bool useVssAsSecondWheelSpeed : 1 {};
	/**
	offset 1548 bit 11 */
	bool is_enabled_spi_5 : 1 {};
	/**
	offset 1548 bit 12 */
	bool is_enabled_spi_6 : 1 {};
	/**
	 * AEM X-Series EGT gauge kit or rusEFI EGT sensor from Wideband controller
	offset 1548 bit 13 */
	bool enableAemXSeriesEgt : 1 {};
	/**
	offset 1548 bit 14 */
	bool startRequestPinInverted : 1 {};
	/**
	offset 1548 bit 15 */
	bool tcu_rangeSensorPulldown : 1 {};
	/**
	offset 1548 bit 16 */
	bool brakePedalPinInverted : 1 {};
	/**
	offset 1548 bit 17 */
	bool devBit0 : 1 {};
	/**
	offset 1548 bit 18 */
	bool devBit1 : 1 {};
	/**
	offset 1548 bit 19 */
	bool devBit2 : 1 {};
	/**
	offset 1548 bit 20 */
	bool devBit3 : 1 {};
	/**
	offset 1548 bit 21 */
	bool devBit4 : 1 {};
	/**
	offset 1548 bit 22 */
	bool devBit5 : 1 {};
	/**
	offset 1548 bit 23 */
	bool devBit6 : 1 {};
	/**
	offset 1548 bit 24 */
	bool devBit7 : 1 {};
	/**
	offset 1548 bit 25 */
	bool invertExhaustCamVVTSignal : 1 {};
	/**
	 * "Available via TS Plugin see https://rusefi.com/s/knock"
	offset 1548 bit 26 */
	bool enableKnockSpectrogram : 1 {};
	/**
	offset 1548 bit 27 */
	bool enableKnockSpectrogramFilter : 1 {};
	/**
	offset 1548 bit 28 */
	bool unusedBit_598_28 : 1 {};
	/**
	offset 1548 bit 29 */
	bool unusedBit_598_29 : 1 {};
	/**
	offset 1548 bit 30 */
	bool unusedBit_598_30 : 1 {};
	/**
	offset 1548 bit 31 */
	bool unusedBit_598_31 : 1 {};
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.\Should be 100 once tune is better
	 * units: cycles
	 * offset 1552
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * IAC Value added when coasting and transitioning into idle.
	 * units: percent
	 * offset 1554
	 */
	int16_t iacByTpsTaper;
	/**
	 * offset 1556
	 */
	Gpio accelerometerCsPin;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: kph
	 * offset 1558
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	 * units: kph
	 * offset 1559
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	 * units: roc
	 * offset 1560
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 1564
	 */
	brain_input_pin_e auxSpeedSensorInputPin[AUX_SPEED_SENSOR_COUNT];
	/**
	 * offset 1568
	 */
	uint8_t totalGearsCount;
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 1569
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * See http://rusefi.com/s/debugmode
	 * offset 1570
	 */
	debug_mode_e debugMode;
	/**
	 * Additional idle % when fan #1 is active
	 * units: %
	 * offset 1571
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * Band rate for primary TTL
	 * units: BPs
	 * offset 1572
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	 * units: roc
	 * offset 1576
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	 * units: coeff
	 * offset 1580
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * How many degrees of timing advance will be reduced during the Torque Reduction Time
	 * units: deg
	 * offset 1584
	 */
	float torqueReductionIgnitionRetard;
	/**
	 * units: voltage
	 * offset 1588
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	 * units: voltage
	 * offset 1592
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
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
	 * units: A
	 * offset 1600
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	 * units: A
	 * offset 1601
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	 * units: us
	 * offset 1602
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	 * units: ms
	 * offset 1603
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 1604 bit 0 */
	bool stepperDcInvertedPins : 1 {};
	/**
	 * Allow OpenBLT on Primary CAN
	offset 1604 bit 1 */
	bool canOpenBLT : 1 {};
	/**
	 * Allow OpenBLT on Secondary CAN
	offset 1604 bit 2 */
	bool can2OpenBLT : 1 {};
	/**
	 * Select whether to configure injector flow in volumetric flow (default, cc/min) or mass flow (g/s).
	offset 1604 bit 3 */
	bool injectorFlowAsMassFlow : 1 {};
	/**
	offset 1604 bit 4 */
	bool boardUseCanTerminator : 1 {};
	/**
	offset 1604 bit 5 */
	bool kLineDoHondaSend : 1 {};
	/**
	 * ListenMode is about acknowledging CAN traffic on the protocol level. Different from canWriteEnabled
	offset 1604 bit 6 */
	bool can1ListenMode : 1 {};
	/**
	offset 1604 bit 7 */
	bool can2ListenMode : 1 {};
	/**
	offset 1604 bit 8 */
	bool unusedBit_635_8 : 1 {};
	/**
	offset 1604 bit 9 */
	bool unusedBit_635_9 : 1 {};
	/**
	offset 1604 bit 10 */
	bool unusedBit_635_10 : 1 {};
	/**
	offset 1604 bit 11 */
	bool unusedBit_635_11 : 1 {};
	/**
	offset 1604 bit 12 */
	bool unusedBit_635_12 : 1 {};
	/**
	offset 1604 bit 13 */
	bool unusedBit_635_13 : 1 {};
	/**
	offset 1604 bit 14 */
	bool unusedBit_635_14 : 1 {};
	/**
	offset 1604 bit 15 */
	bool unusedBit_635_15 : 1 {};
	/**
	offset 1604 bit 16 */
	bool unusedBit_635_16 : 1 {};
	/**
	offset 1604 bit 17 */
	bool unusedBit_635_17 : 1 {};
	/**
	offset 1604 bit 18 */
	bool unusedBit_635_18 : 1 {};
	/**
	offset 1604 bit 19 */
	bool unusedBit_635_19 : 1 {};
	/**
	offset 1604 bit 20 */
	bool unusedBit_635_20 : 1 {};
	/**
	offset 1604 bit 21 */
	bool unusedBit_635_21 : 1 {};
	/**
	offset 1604 bit 22 */
	bool unusedBit_635_22 : 1 {};
	/**
	offset 1604 bit 23 */
	bool unusedBit_635_23 : 1 {};
	/**
	offset 1604 bit 24 */
	bool unusedBit_635_24 : 1 {};
	/**
	offset 1604 bit 25 */
	bool unusedBit_635_25 : 1 {};
	/**
	offset 1604 bit 26 */
	bool unusedBit_635_26 : 1 {};
	/**
	offset 1604 bit 27 */
	bool unusedBit_635_27 : 1 {};
	/**
	offset 1604 bit 28 */
	bool unusedBit_635_28 : 1 {};
	/**
	offset 1604 bit 29 */
	bool unusedBit_635_29 : 1 {};
	/**
	offset 1604 bit 30 */
	bool unusedBit_635_30 : 1 {};
	/**
	offset 1604 bit 31 */
	bool unusedBit_635_31 : 1 {};
	/**
	 * offset 1608
	 */
	uint8_t camDecoder2jzPosition;
	/**
	 * offset 1609
	 */
	mc33810maxDwellTimer_e mc33810maxDwellTimer;
	/**
	 * Duration of each test pulse
	 * units: ms
	 * offset 1610
	 */
	scaled_channel<uint16_t, 100, 1> benchTestOnTime;
	/**
	 * units: %
	 * offset 1612
	 */
	uint8_t lambdaProtectionRestoreTps;
	/**
	 * units: %
	 * offset 1613
	 */
	scaled_channel<uint8_t, 1, 10> lambdaProtectionRestoreLoad;
	/**
	 * offset 1614
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1615
	 */
	uint8_t alignmentFill_at_1615[1];
	/**
	 * offset 1616
	 */
	Gpio can2TxPin;
	/**
	 * offset 1618
	 */
	Gpio can2RxPin;
	/**
	 * offset 1620
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * offset 1621
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 1622
	 */
	load_override_e ignOverrideMode;
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 1623
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * offset 1624
	 */
	output_pin_e hpfpValvePin;
	/**
	 * offset 1626
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1627
	 */
	uint8_t alignmentFill_at_1627[1];
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	 * units: kPa (absolute)
	 * offset 1628
	 */
	float boostCutPressure;
	/**
	 * units: kg/h
	 * offset 1632
	 */
	scaled_channel<uint8_t, 1, 5> tchargeBins[16];
	/**
	 * units: ratio
	 * offset 1648
	 */
	scaled_channel<uint8_t, 100, 1> tchargeValues[16];
	/**
	 * Fixed timing, useful for TDC testing
	 * units: deg
	 * offset 1664
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1668
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * units: v
	 * offset 1672
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * units: value
	 * offset 1676
	 */
	float egoValueShift;
	/**
	 * VVT output solenoid pin for this cam
	 * offset 1680
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	 * units: percent
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
	pwm_freq_t vvtOutputFrequency;
	/**
	 * Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.
	 * units: deg BTDC
	 * offset 1710
	 */
	int8_t minimumIgnitionTiming;
	/**
	 * Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC
	 * units: deg BTDC
	 * offset 1711
	 */
	int8_t maximumIgnitionTiming;
	/**
	 * units: Hz
	 * offset 1712
	 */
	int alternatorPwmFrequency;
	/**
	 * set vvt_mode X
	 * offset 1716
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #2 is active
	 * units: %
	 * offset 1718
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	 * units: sec
	 * offset 1719
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 1720
	 */
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT];
	/**
	 * offset 1728
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 1752
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * offset 1753
	 */
	pin_output_mode_e accelerometerCsPinMode;
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 1754
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * offset 1755
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	 * units: kPa
	 * offset 1756
	 */
	float fuelReferencePressure;
	/**
	 * offset 1760
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 1792
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * units: Deg
	 * offset 1824
	 */
	int16_t knockSamplingDuration;
	/**
	 * units: Hz
	 * offset 1826
	 */
	int16_t etbFreq;
	/**
	 * offset 1828
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 1848
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	 * units: %
	 * offset 1849
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	 * units: %
	 * offset 1850
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * offset 1851
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * per-cylinder ignition and fueling timing correction for uneven engines
	 * units: deg
	 * offset 1852
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	 * units: seconds
	 * offset 1900
	 */
	float idlePidActivationTime;
	/**
	 * offset 1904
	 */
	pin_mode_e spi1SckMode;
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 1905
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 1906
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 1907
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 1908
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 1909
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 1910
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 1911
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 1912
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 1913
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 1914
	 */
	Gpio mc33816_rstb;
	/**
	 * offset 1916
	 */
	Gpio mc33816_driven;
	/**
	 * Brake pedal switch
	 * offset 1918
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * VVT output PID
	 * TODO: rename to vvtPid
	 * offset 1920
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 1960
	 */
	float injectorCorrectionPolynomial[8];
	/**
	 * units: C
	 * offset 1992
	 */
	int8_t primeBins[PRIME_CURVE_COUNT];
	/**
	 * offset 2000
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2020
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2021
	 */
	uint8_t alignmentFill_at_2021[1];
	/**
	 * offset 2022
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	 * units: deg C
	 * offset 2024
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	 * units: deg C
	 * offset 2025
	 */
	uint8_t fan2OffTemperature;
	/**
	 * offset 2026
	 */
	Gpio stepperEnablePin;
	/**
	 * offset 2028
	 */
	Gpio tle8888_cs;
	/**
	 * offset 2030
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2031
	 */
	uint8_t alignmentFill_at_2031[1];
	/**
	 * offset 2032
	 */
	Gpio mc33816_cs;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2034
	 */
	uint8_t alignmentFill_at_2034[2];
	/**
	 * units: hz
	 * offset 2036
	 */
	float auxFrequencyFilter;
	/**
	 * offset 2040
	 */
	sent_input_pin_e sentInputPins[SENT_INPUT_COUNT];
	/**
	 * This sets the RPM above which fuel cut is active.
	 * units: rpm
	 * offset 2042
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	 * units: rpm
	 * offset 2044
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	 * units: %
	 * offset 2046
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	 * units: C
	 * offset 2048
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * units: %
	 * offset 2050
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	 * units: kPa
	 * offset 2052
	 */
	int16_t coastingFuelCutMap;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2054
	 */
	uint8_t alignmentFill_at_2054[2];
	/**
	 * offset 2056
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 2076
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * offset 2096
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	 * offset 2192
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 2256
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2384
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 2388
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	 * units: kg/h
	 * offset 2392
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2396
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	 * units: deg/sec
	 * offset 2400
	 */
	float tChargeAirDecrLimit;
	/**
	 * offset 2404
	 */
	float hip9011Gain;
	/**
	 * iTerm min value
	 * offset 2408
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 2410
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 2412
	 */
	pid_s idleTimingPid;
	/**
	 * When entering idle, and the PID settings are aggressive, it's good to make a soft entry upon entering closed loop
	 * offset 2432
	 */
	float idleTimingSoftEntryTime;
	/**
	 * offset 2436
	 */
	pin_input_mode_e torqueReductionTriggerPinMode;
	/**
	 * offset 2437
	 */
	torqueReductionActivationMode_e torqueReductionActivationMode;
	/**
	 * A delay in cycles between fuel-enrich. portions
	 * units: cycles
	 * offset 2438
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	 * units: coef
	 * offset 2440
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 2444
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 2445
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * iTerm min value
	 * offset 2446
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 2448
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	 * units: :1
	 * offset 2449
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 2450
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	 * units: %
	 * offset 2452
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 2456
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	 * units: mg
	 * offset 2468
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[PRIME_CURVE_COUNT];
	/**
	 * Trigger comparator center point voltage
	 * units: V
	 * offset 2476
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	 * units: V
	 * offset 2477
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	 * units: V
	 * offset 2478
	 */
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	 * units: RPM
	 * offset 2479
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * units: ratio
	 * offset 2480
	 */
	scaled_channel<uint16_t, 100, 1> tractionControlSlipBins[TRACTION_CONTROL_ETB_DROP_SIZE];
	/**
	 * units: RPM
	 * offset 2492
	 */
	uint8_t tractionControlSpeedBins[TRACTION_CONTROL_ETB_DROP_SIZE];
	/**
	 * offset 2498
	 */
	int8_t disableFan1AtSpeed;
	/**
	 * offset 2499
	 */
	int8_t disableFan2AtSpeed;
	/**
	 * offset 2500
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2502
	 */
	uint8_t alignmentFill_at_2502[2];
	/**
	 * offset 2504
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	 * Boost Current
	 * units: mA
	 * offset 2776
	 */
	uint16_t mc33_i_boost;
	/**
	 * Peak Current
	 * units: mA
	 * offset 2778
	 */
	uint16_t mc33_i_peak;
	/**
	 * Hold Current
	 * units: mA
	 * offset 2780
	 */
	uint16_t mc33_i_hold;
	/**
	 * Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	 * units: us
	 * offset 2782
	 */
	uint16_t mc33_t_max_boost;
	/**
	 * units: us
	 * offset 2784
	 */
	uint16_t mc33_t_peak_off;
	/**
	 * Peak phase duration
	 * units: us
	 * offset 2786
	 */
	uint16_t mc33_t_peak_tot;
	/**
	 * units: us
	 * offset 2788
	 */
	uint16_t mc33_t_bypass;
	/**
	 * units: us
	 * offset 2790
	 */
	uint16_t mc33_t_hold_off;
	/**
	 * Hold phase duration
	 * units: us
	 * offset 2792
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * offset 2794
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * offset 2795
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * offset 2796
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * offset 2797
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2803
	 */
	uint8_t alignmentFill_at_2803[1];
	/**
	 * units: ratio
	 * offset 2804
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	 * units: ratio
	 * offset 2876
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	 * Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	 * units: rpm
	 * offset 2948
	 */
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	/**
	 * Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	 * units: sec
	 * offset 2949
	 */
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	/**
	 * Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	 * units: sec
	 * offset 2950
	 */
	scaled_channel<uint8_t, 10, 1> acDelay;
	/**
	 * offset 2951
	 */
	tChargeMode_e tChargeMode;
	/**
	 * units: mg
	 * offset 2952
	 */
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	/**
	 * Threshold in ETB error (target vs. actual) above which the jam timer is started. If the timer reaches the time specified in the jam detection timeout period, the throttle is considered jammed, and engine operation limited.
	 * units: %
	 * offset 2954
	 */
	uint8_t etbJamDetectThreshold;
	/**
	 * units: lobes/cam
	 * offset 2955
	 */
	uint8_t hpfpCamLobes;
	/**
	 * offset 2956
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Low engine speed for A/C. Larger engines can survive lower values
	 * units: RPM
	 * offset 2957
	 */
	scaled_channel<int8_t, 1, 10> acLowRpmLimit;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	 * units: deg
	 * offset 2958
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2959
	 */
	uint8_t alignmentFill_at_2959[1];
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	 * units: cc
	 * offset 2960
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	 * units: deg
	 * offset 2962
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 2963
	 */
	uint8_t issFilterReciprocal;
	/**
	 * units: %/kPa
	 * offset 2964
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	 * units: %/kPa/lobe
	 * offset 2966
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	 * units: kPa/s
	 * offset 2968
	 */
	uint16_t hpfpTargetDecay;
	/**
	 * offset 2970
	 */
	output_pin_e stepper_raw_output[4];
	/**
	 * units: ratio
	 * offset 2978
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[TCU_GEAR_COUNT];
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	 * units: ms
	 * offset 2998
	 */
	uint16_t vvtActivationDelayMs;
	/**
	 * units: Nm
	 * offset 3000
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_SIZE];
	/**
	 * units: RPM
	 * offset 3036
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE];
	/**
	 * units: Load
	 * offset 3048
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE];
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
	uint8_t alignmentFill_at_3107[1];
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
	uint8_t alignmentFill_at_3111[1];
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
	uint8_t alignmentFill_at_3117[1];
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
	uint8_t alignmentFill_at_3123[1];
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
	uint8_t alignmentFill_at_3129[1];
	/**
	 * offset 3130
	 */
	uint16_t tuneHidingKey;
	/**
	 * Individual charaters are accessible using vin(index) Lua function
	 * offset 3132
	 */
	vin_number_t vinNumber;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3149
	 */
	uint8_t alignmentFill_at_3149[1];
	/**
	 * offset 3150
	 */
	uint16_t highSpeedOffsets[HIGH_SPEED_COUNT];
	/**
	 * offset 3214
	 */
	fuel_pressure_sensor_mode_e fuelPressureSensorMode;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3215
	 */
	uint8_t alignmentFill_at_3215[1];
	/**
	 * offset 3216
	 */
	switch_input_pin_e luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT];
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
	uint8_t alignmentFill_at_3246[2];
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
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT];
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
	scaled_channel<int8_t, 2, 1> gearBasedOpenLoopBoostAdder[TCU_GEAR_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3299
	 */
	uint8_t alignmentFill_at_3299[1];
	/**
	 * How many test bench pulses do you want
	 * offset 3300
	 */
	uint32_t benchTestCount;
	/**
	 * How long initial IAC adder is held before starting to decay.
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
	switch_input_pin_e tcu_rangeInput[RANGE_INPUT_COUNT];
	/**
	 * offset 3318
	 */
	pin_input_mode_e tcu_rangeInputMode[RANGE_INPUT_COUNT];
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
	uint8_t alignmentFill_at_3326[2];
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
	Gpio dacOutputPins[DAC_OUTPUT_COUNT];
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
	uint8_t simulatorCamPosition[CAM_INPUTS_COUNT];
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
	float triggerVVTGapOverrideFrom[VVT_TRACKING_LENGTH];
	/**
	 * units: ratio
	 * offset 3488
	 */
	float triggerVVTGapOverrideTo[VVT_TRACKING_LENGTH];
	/**
	 * units: %
	 * offset 3504
	 */
	int8_t tractionControlEtbDrop[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE];
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
	uint8_t alignmentFill_at_3543[1];
	/**
	 * offset 3544
	 */
	output_pin_e injectionPinsStage2[MAX_CYLINDER_COUNT];
	/**
	 * units: Deg
	 * offset 3568
	 */
	int8_t tractionControlTimingDrop[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE];
	/**
	 * units: %
	 * offset 3604
	 */
	int8_t tractionControlIgnitionSkip[TRACTION_CONTROL_ETB_DROP_SIZE][TRACTION_CONTROL_ETB_DROP_SIZE];
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
	uint8_t alignmentFill_at_3674[2];
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
	uint8_t alignmentFill_at_3683[1];
	/**
	 * offset 3684
	 */
	int anotherCiTest;
	/**
	 * offset 3688
	 */
	uint32_t device_uid[3];
	/**
	 * offset 3700
	 */
	adc_channel_e tcu_rangeAnalogInput[RANGE_INPUT_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3706
	 */
	uint8_t alignmentFill_at_3706[2];
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
	uint8_t alignmentFill_at_3718[2];
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
	uint8_t alignmentFill_at_3745[3];
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
	uint8_t alignmentFill_at_3801[3];
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
	uint8_t alignmentFill_at_3811[1];
	/**
	offset 3812 bit 0 */
	bool nitrousControlEnabled : 1 {};
	/**
	offset 3812 bit 1 */
	bool nitrousControlTriggerPinInverted : 1 {};
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
	uint8_t alignmentFill_at_3817[1];
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
	uint8_t alignmentFill_at_3823[1];
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
	uint8_t alignmentFill_at_3833[1];
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
	uint8_t alignmentFill_at_3837[1];
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
	uint8_t alignmentFill_at_3850[2];
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
	uint8_t alignmentFill_at_3858[2];
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
	 * units: units
	 * offset 3868
	 */
	uint8_t unusedOftenChangesDuringFirmwareUpdate[END_OF_CALIBRATION_PADDING];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3926
	 */
	uint8_t alignmentFill_at_3926[2];
};
static_assert(sizeof(engine_configuration_s) == 3928);

// start of ign_cyl_trim_s
struct ign_cyl_trim_s {
	/**
	 * offset 0
	 */
	scaled_channel<int8_t, 5, 1> table[IGN_TRIM_SIZE][IGN_TRIM_SIZE];
};
static_assert(sizeof(ign_cyl_trim_s) == 16);

// start of fuel_cyl_trim_s
struct fuel_cyl_trim_s {
	/**
	 * offset 0
	 */
	scaled_channel<int8_t, 5, 1> table[FUEL_TRIM_SIZE][FUEL_TRIM_SIZE];
};
static_assert(sizeof(fuel_cyl_trim_s) == 16);

// start of blend_table_s
struct blend_table_s {
	/**
	 * offset 0
	 */
	scaled_channel<int16_t, 10, 1> table[BLEND_TABLE_COUNT][BLEND_TABLE_COUNT];
	/**
	 * units: Load
	 * offset 128
	 */
	uint16_t loadBins[BLEND_TABLE_COUNT];
	/**
	 * units: RPM
	 * offset 144
	 */
	uint16_t rpmBins[BLEND_TABLE_COUNT];
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
	 * units: mult
	 * offset 3928
	 */
	float postCrankingFactor[CRANKING_ENRICH_COUNT][CRANKING_ENRICH_COUNT];
	/**
	 * units: count
	 * offset 4072
	 */
	uint16_t postCrankingDurationBins[CRANKING_ENRICH_COUNT];
	/**
	 * units: C
	 * offset 4084
	 */
	int16_t postCrankingCLTBins[CRANKING_ENRICH_COUNT];
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	 * units: target TPS position
	 * offset 4096
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, open loop component of PID closed loop control
	 * units: ETB duty cycle bias
	 * offset 4128
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	 * units: %
	 * offset 4160
	 */
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	 * units: Load
	 * offset 4224
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	 * units: RPM
	 * offset 4232
	 */
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	 * units: RPM
	 * offset 4240
	 */
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	 * units: ms
	 * offset 4256
	 */
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * CLT-based target RPM for automatic idle controller
	 * units: C
	 * offset 4272
	 */
	scaled_channel<int8_t, 1, 2> cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * units: RPM
	 * offset 4288
	 */
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * CLT-based timing correction
	 * units: C
	 * offset 4304
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	 * units: degree
	 * offset 4336
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * units: x
	 * offset 4368
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	 * units: y
	 * offset 4432
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	 * units: x
	 * offset 4496
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	 * units: y
	 * offset 4560
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	 * units: x
	 * offset 4624
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	 * units: y
	 * offset 4656
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	 * units: x
	 * offset 4688
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	 * units: y
	 * offset 4720
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	 * units: x
	 * offset 4752
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	 * units: y
	 * offset 4784
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	 * units: x
	 * offset 4816
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	 * units: y
	 * offset 4848
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	 * units: kPa
	 * offset 4880
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	 * units: RPM
	 * offset 4896
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	 * units: ratio
	 * offset 4912
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * units: Ratio
	 * offset 4976
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	 * units: %
	 * offset 5008
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: RPM
	 * offset 5040
	 */
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * units: deg
	 * offset 5048
	 */
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * RPM-based idle position for coasting
	 * units: RPM
	 * offset 5056
	 */
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE];
	/**
	 * RPM-based idle position for coasting
	 * units: %
	 * offset 5072
	 */
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 5088
	 */
	warning_message_t warning_message;
	/**
	 * offset 5208
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT];
	/**
	 * units: RPM
	 * offset 5240
	 */
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 5244
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_LOAD_COUNT][BOOST_RPM_COUNT];
	/**
	 * offset 5276
	 */
	uint16_t boostLoadBins[BOOST_LOAD_COUNT];
	/**
	 * units: %
	 * offset 5292
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	 * units: %
	 * offset 5356
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	 * units: %
	 * offset 5364
	 */
	uint8_t pedalToTpsPedalSpeedBins[PEDAL_TO_TPS_SIZE];
	/**
	 * units: RPM
	 * offset 5372
	 */
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * units: %/s
	 * offset 5380
	 */
	scaled_channel<uint8_t, 1, 10> etbMaxSpeedOpen[PEDAL_TO_TPS_SIZE];
	/**
	 * units: %/s
	 * offset 5388
	 */
	scaled_channel<uint8_t, 1, 10> etbMaxSpeedClose[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	 * units: C
	 * offset 5396
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	 * units: %
	 * offset 5428
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: RPM
	 * offset 5460
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * units: deg
	 * offset 5468
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * units: RPM
	 * offset 5500
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	/**
	 * units: load
	 * offset 5504
	 */
	uint8_t idleVeLoadBins[IDLE_VE_SIZE];
	/**
	 * units: %
	 * offset 5508
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	/**
	 * offset 5540
	 */
	lua_script_t luaScript;
	/**
	 * units: C
	 * offset 13540
	 */
	float cltFuelCorrBins[CLT_FUEL_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 13604
	 */
	float cltFuelCorr[CLT_FUEL_CURVE_SIZE];
	/**
	 * units: C
	 * offset 13668
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 13732
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 13796
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	 * units: C
	 * offset 13828
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	 * units: counter
	 * offset 13860
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * units: C
	 * offset 13892
	 */
	int16_t crankingCycleFuelCltBins[CRANKING_CYCLE_CLT_SIZE];
	/**
	 * units: mult
	 * offset 13900
	 */
	float crankingCycleFuelCoef[CRANKING_CYCLE_CLT_SIZE][CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	 * units: C
	 * offset 14028
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	 * units: ratio
	 * offset 14092
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	 * units: kg/hour
	 * offset 14156
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	 * units: V
	 * offset 14284
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	 * units: deg
	 * offset 14412
	 */
	scaled_channel<int8_t, 10, 1> ignitionIatCorrTable[IAT_IGN_CORR_COUNT][IAT_IGN_CORR_COUNT];
	/**
	 * units: C
	 * offset 14476
	 */
	int8_t ignitionIatCorrTempBins[IAT_IGN_CORR_COUNT];
	/**
	 * units: Load
	 * offset 14484
	 */
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[IAT_IGN_CORR_COUNT];
	/**
	 * units: deg
	 * offset 14492
	 */
	int16_t injectionPhase[INJ_PHASE_LOAD_COUNT][INJ_PHASE_RPM_COUNT];
	/**
	 * units: Load
	 * offset 15004
	 */
	uint16_t injPhaseLoadBins[INJ_PHASE_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 15036
	 */
	uint16_t injPhaseRpmBins[INJ_PHASE_RPM_COUNT];
	/**
	 * units: onoff
	 * offset 15068
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	 * units: kPa
	 * offset 15128
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	 * units: % TPS
	 * offset 15640
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 15672
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	 * units: value
	 * offset 15704
	 */
	int8_t vvtTable1[VVT_TABLE_SIZE][VVT_TABLE_SIZE];
	/**
	 * units: L
	 * offset 15768
	 */
	uint16_t vvtTable1LoadBins[VVT_TABLE_SIZE];
	/**
	 * units: RPM
	 * offset 15784
	 */
	uint16_t vvtTable1RpmBins[VVT_TABLE_SIZE];
	/**
	 * units: value
	 * offset 15800
	 */
	int8_t vvtTable2[VVT_TABLE_SIZE][VVT_TABLE_SIZE];
	/**
	 * units: L
	 * offset 15864
	 */
	uint16_t vvtTable2LoadBins[VVT_TABLE_SIZE];
	/**
	 * units: RPM
	 * offset 15880
	 */
	uint16_t vvtTable2RpmBins[VVT_TABLE_SIZE];
	/**
	 * units: deg
	 * offset 15896
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT];
	/**
	 * units: Load
	 * offset 16152
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 16184
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	 * units: %
	 * offset 16200
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	 * units: {bitStringValue(fuelUnits, fuelAlgorithm) }
	 * offset 16712
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 16744
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	 * units: %
	 * offset 16776
	 */
	scaled_channel<uint16_t, 10, 1> ltftTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	 * units: 10x%/s
	 * offset 17288
	 */
	scaled_channel<uint16_t, 10, 1> ltftCorrectionRate[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	offset 17800 bit 0 */
	bool ltftEnabled : 1 {};
	/**
	offset 17800 bit 1 */
	bool unusedBit_93_1 : 1 {};
	/**
	offset 17800 bit 2 */
	bool unusedBit_93_2 : 1 {};
	/**
	offset 17800 bit 3 */
	bool unusedBit_93_3 : 1 {};
	/**
	offset 17800 bit 4 */
	bool unusedBit_93_4 : 1 {};
	/**
	offset 17800 bit 5 */
	bool unusedBit_93_5 : 1 {};
	/**
	offset 17800 bit 6 */
	bool unusedBit_93_6 : 1 {};
	/**
	offset 17800 bit 7 */
	bool unusedBit_93_7 : 1 {};
	/**
	offset 17800 bit 8 */
	bool unusedBit_93_8 : 1 {};
	/**
	offset 17800 bit 9 */
	bool unusedBit_93_9 : 1 {};
	/**
	offset 17800 bit 10 */
	bool unusedBit_93_10 : 1 {};
	/**
	offset 17800 bit 11 */
	bool unusedBit_93_11 : 1 {};
	/**
	offset 17800 bit 12 */
	bool unusedBit_93_12 : 1 {};
	/**
	offset 17800 bit 13 */
	bool unusedBit_93_13 : 1 {};
	/**
	offset 17800 bit 14 */
	bool unusedBit_93_14 : 1 {};
	/**
	offset 17800 bit 15 */
	bool unusedBit_93_15 : 1 {};
	/**
	offset 17800 bit 16 */
	bool unusedBit_93_16 : 1 {};
	/**
	offset 17800 bit 17 */
	bool unusedBit_93_17 : 1 {};
	/**
	offset 17800 bit 18 */
	bool unusedBit_93_18 : 1 {};
	/**
	offset 17800 bit 19 */
	bool unusedBit_93_19 : 1 {};
	/**
	offset 17800 bit 20 */
	bool unusedBit_93_20 : 1 {};
	/**
	offset 17800 bit 21 */
	bool unusedBit_93_21 : 1 {};
	/**
	offset 17800 bit 22 */
	bool unusedBit_93_22 : 1 {};
	/**
	offset 17800 bit 23 */
	bool unusedBit_93_23 : 1 {};
	/**
	offset 17800 bit 24 */
	bool unusedBit_93_24 : 1 {};
	/**
	offset 17800 bit 25 */
	bool unusedBit_93_25 : 1 {};
	/**
	offset 17800 bit 26 */
	bool unusedBit_93_26 : 1 {};
	/**
	offset 17800 bit 27 */
	bool unusedBit_93_27 : 1 {};
	/**
	offset 17800 bit 28 */
	bool unusedBit_93_28 : 1 {};
	/**
	offset 17800 bit 29 */
	bool unusedBit_93_29 : 1 {};
	/**
	offset 17800 bit 30 */
	bool unusedBit_93_30 : 1 {};
	/**
	offset 17800 bit 31 */
	bool unusedBit_93_31 : 1 {};
	/**
	 * units: value
	 * offset 17804
	 */
	uint8_t ltftCRC;
	/**
	 * Minimum temperature to start correcting ltft tables
	 * units: deg C
	 * offset 17805
	 */
	uint8_t ltftMinModTemp;
	/**
	 * Minimum temperature to start using ltft tables
	 * units: deg C
	 * offset 17806
	 */
	uint8_t ltftMinTemp;
	/**
	 * How much long term fuel trim should act to reduce short term fuel trim, 100 should keep stft in about 3%, 255 in 8% and 33 in 1%, and
	 * units: %
	 * offset 17807
	 */
	uint8_t ltftPermissivity;
	/**
	 * units: %
	 * offset 17808
	 */
	uint8_t ltftMaxCorrection;
	/**
	 * units: %
	 * offset 17809
	 */
	uint8_t ltftMinCorrection;
	/**
	 * units: lambda
	 * offset 17810
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
	/**
	 * offset 18066
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	 * units: RPM
	 * offset 18098
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 18130
	 */
	uint8_t alignmentFill_at_18130[2];
	/**
	 * units: value
	 * offset 18132
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	 * units: from
	 * offset 18388
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * units: to
	 * offset 18420
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * units: value
	 * offset 18452
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	 * units: L
	 * offset 18708
	 */
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 18724
	 */
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	 * units: value
	 * offset 18740
	 */
	float scriptTable2[TABLE_2_LOAD_SIZE][TABLE_2_RPM_SIZE];
	/**
	 * units: L
	 * offset 18996
	 */
	int16_t scriptTable2LoadBins[TABLE_2_LOAD_SIZE];
	/**
	 * units: RPM
	 * offset 19012
	 */
	int16_t scriptTable2RpmBins[TABLE_2_RPM_SIZE];
	/**
	 * units: value
	 * offset 19028
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	 * units: L
	 * offset 19092
	 */
	int16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 19108
	 */
	int16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	 * units: value
	 * offset 19124
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][TABLE_4_RPM];
	/**
	 * units: L
	 * offset 19204
	 */
	int16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	 * units: RPM
	 * offset 19220
	 */
	int16_t scriptTable4RpmBins[TABLE_4_RPM];
	/**
	 * offset 19240
	 */
	uint16_t ignTrimLoadBins[IGN_TRIM_SIZE];
	/**
	 * units: rpm
	 * offset 19248
	 */
	uint16_t ignTrimRpmBins[IGN_TRIM_SIZE];
	/**
	 * offset 19256
	 */
	ign_cyl_trim_s ignTrims[12];
	/**
	 * offset 19448
	 */
	uint16_t fuelTrimLoadBins[FUEL_TRIM_SIZE];
	/**
	 * units: rpm
	 * offset 19456
	 */
	uint16_t fuelTrimRpmBins[FUEL_TRIM_SIZE];
	/**
	 * offset 19464
	 */
	fuel_cyl_trim_s fuelTrims[12];
	/**
	 * units: ratio
	 * offset 19656
	 */
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	/**
	 * units: Airmass
	 * offset 19672
	 */
	scaled_channel<uint8_t, 1, 5> tcu_pcAirmassBins[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19680
	 */
	uint8_t tcu_pcValsR[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19688
	 */
	uint8_t tcu_pcValsN[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19696
	 */
	uint8_t tcu_pcVals1[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19704
	 */
	uint8_t tcu_pcVals2[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19712
	 */
	uint8_t tcu_pcVals3[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19720
	 */
	uint8_t tcu_pcVals4[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19728
	 */
	uint8_t tcu_pcVals12[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19736
	 */
	uint8_t tcu_pcVals23[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19744
	 */
	uint8_t tcu_pcVals34[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19752
	 */
	uint8_t tcu_pcVals21[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19760
	 */
	uint8_t tcu_pcVals32[TCU_TABLE_WIDTH];
	/**
	 * units: %
	 * offset 19768
	 */
	uint8_t tcu_pcVals43[TCU_TABLE_WIDTH];
	/**
	 * units: TPS
	 * offset 19776
	 */
	uint8_t tcu_tccTpsBins[8];
	/**
	 * units: MPH
	 * offset 19784
	 */
	uint8_t tcu_tccLockSpeed[8];
	/**
	 * units: MPH
	 * offset 19792
	 */
	uint8_t tcu_tccUnlockSpeed[8];
	/**
	 * units: KPH
	 * offset 19800
	 */
	uint8_t tcu_32SpeedBins[8];
	/**
	 * units: %
	 * offset 19808
	 */
	uint8_t tcu_32Vals[8];
	/**
	 * units: %
	 * offset 19816
	 */
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[ETB2_TRIM_SIZE][ETB2_TRIM_SIZE];
	/**
	 * units: %
	 * offset 19852
	 */
	uint8_t throttle2TrimTpsBins[ETB2_TRIM_SIZE];
	/**
	 * units: RPM
	 * offset 19858
	 */
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[ETB2_TRIM_SIZE];
	/**
	 * units: deg
	 * offset 19864
	 */
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[KNOCK_TABLE_SIZE][KNOCK_TABLE_SIZE];
	/**
	 * units: %
	 * offset 19900
	 */
	uint8_t maxKnockRetardLoadBins[KNOCK_TABLE_SIZE];
	/**
	 * units: RPM
	 * offset 19906
	 */
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[KNOCK_TABLE_SIZE];
	/**
	 * units: deg
	 * offset 19912
	 */
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[4][4];
	/**
	 * units: TPS
	 * offset 19944
	 */
	uint16_t alsIgnRetardLoadBins[4];
	/**
	 * units: RPM
	 * offset 19952
	 */
	uint16_t alsIgnRetardrpmBins[4];
	/**
	 * units: percent
	 * offset 19960
	 */
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[4][4];
	/**
	 * units: TPS
	 * offset 19992
	 */
	uint16_t alsFuelAdjustmentLoadBins[4];
	/**
	 * units: RPM
	 * offset 20000
	 */
	uint16_t alsFuelAdjustmentrpmBins[4];
	/**
	 * units: ratio
	 * offset 20008
	 */
	scaled_channel<int16_t, 1, 10> ALSIgnSkipTable[4][4];
	/**
	 * units: TPS
	 * offset 20040
	 */
	uint16_t alsIgnSkipLoadBins[4];
	/**
	 * units: RPM
	 * offset 20048
	 */
	uint16_t alsIgnSkiprpmBins[4];
	/**
	 * offset 20056
	 */
	blend_table_s ignBlends[IGN_BLEND_COUNT];
	/**
	 * offset 20432
	 */
	blend_table_s veBlends[VE_BLEND_COUNT];
	/**
	 * units: %
	 * offset 21184
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaBins[12];
	/**
	 * In units of g/s normalized to choked flow conditions
	 * units: g/s
	 * offset 21208
	 */
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaValues[12];
	/**
	 * offset 21232
	 */
	blend_table_s boostOpenLoopBlends[BOOST_BLEND_COUNT];
	/**
	 * offset 21608
	 */
	blend_table_s boostClosedLoopBlends[BOOST_BLEND_COUNT];
	/**
	 * units: level
	 * offset 21984
	 */
	float tcu_rangeP[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22008
	 */
	float tcu_rangeR[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22032
	 */
	float tcu_rangeN[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22056
	 */
	float tcu_rangeD[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22080
	 */
	float tcu_rangeM[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22104
	 */
	float tcu_rangeM3[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22128
	 */
	float tcu_rangeM2[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22152
	 */
	float tcu_rangeM1[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22176
	 */
	float tcu_rangePlus[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22200
	 */
	float tcu_rangeMinus[RANGE_INPUT_COUNT];
	/**
	 * units: level
	 * offset 22224
	 */
	float tcu_rangeLow[RANGE_INPUT_COUNT];
	/**
	 * units: lambda
	 * offset 22248
	 */
	scaled_channel<uint8_t, 100, 1> lambdaMaxDeviationTable[4][4];
	/**
	 * offset 22264
	 */
	uint16_t lambdaMaxDeviationLoadBins[4];
	/**
	 * units: RPM
	 * offset 22272
	 */
	uint16_t lambdaMaxDeviationRpmBins[4];
	/**
	 * units: %
	 * offset 22280
	 */
	uint8_t injectorStagingTable[INJ_STAGING_COUNT][INJ_STAGING_COUNT];
	/**
	 * offset 22316
	 */
	uint16_t injectorStagingLoadBins[INJ_STAGING_COUNT];
	/**
	 * units: RPM
	 * offset 22328
	 */
	uint16_t injectorStagingRpmBins[INJ_STAGING_COUNT];
	/**
	 * units: deg C
	 * offset 22340
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	/**
	 * offset 22348
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	/**
	 * offset 22356
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	/**
	 * units: kPa
	 * offset 22364
	 */
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	/**
	 * offset 22372
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 22380
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	/**
	 * units: %
	 * offset 22388
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	 * units: deg
	 * offset 22404
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	 * units: volts
	 * offset 22420
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	 * units: ms
	 * offset 22428
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	 * units: kPa
	 * offset 22444
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	 * units: load
	 * offset 22644
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	 * units: RPM
	 * offset 22664
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	 * units: %
	 * offset 22674
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	 * units: cc/lobe
	 * offset 22774
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	 * units: RPM
	 * offset 22794
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	 * units: RPM
	 * offset 22804
	 */
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	 * units: dB
	 * offset 22836
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * units: RPM
	 * offset 22852
	 */
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_RPM_CORR_TABLE];
	/**
	 * units: multiplier
	 * offset 22856
	 */
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_RPM_CORR_TABLE];
	/**
	 * units: C
	 * offset 22860
	 */
	scaled_channel<uint8_t, 1, 1> tpsAcelEctBins[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	/**
	 * units: multiplier
	 * offset 22868
	 */
	scaled_channel<uint8_t, 50, 1> tpsAcelEctValues[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	/**
	 * units: C
	 * offset 22876
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	 * units: RPM
	 * offset 22880
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * units: volt
	 * offset 22888
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * units: %
	 * offset 22904
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * units: volts
	 * offset 22912
	 */
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	 * units: multiplier
	 * offset 22920
	 */
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	 * units: %
	 * offset 22928
	 */
	scaled_channel<uint8_t, 1, 1> tcu_shiftTpsBins[TCU_TABLE_WIDTH];
	/**
	 * units: MPH
	 * offset 22936
	 */
	uint8_t tcu_shiftSpeed12[TCU_TABLE_WIDTH];
	/**
	 * units: MPH
	 * offset 22944
	 */
	uint8_t tcu_shiftSpeed23[TCU_TABLE_WIDTH];
	/**
	 * units: MPH
	 * offset 22952
	 */
	uint8_t tcu_shiftSpeed34[TCU_TABLE_WIDTH];
	/**
	 * units: MPH
	 * offset 22960
	 */
	uint8_t tcu_shiftSpeed21[TCU_TABLE_WIDTH];
	/**
	 * units: MPH
	 * offset 22968
	 */
	uint8_t tcu_shiftSpeed32[TCU_TABLE_WIDTH];
	/**
	 * units: MPH
	 * offset 22976
	 */
	uint8_t tcu_shiftSpeed43[TCU_TABLE_WIDTH];
	/**
	 * units: ms
	 * offset 22984
	 */
	float tcu_shiftTime;
	/**
	 * units: Volts
	 * offset 22988
	 */
	scaled_channel<int16_t, 10, 1> alternatorVoltageTargetTable[ALTERNATOR_VOLTAGE_TARGET_SIZE][ALTERNATOR_VOLTAGE_TARGET_SIZE];
	/**
	 * units: Load
	 * offset 23020
	 */
	uint16_t alternatorVoltageTargetLoadBins[ALTERNATOR_VOLTAGE_TARGET_SIZE];
	/**
	 * units: RPM
	 * offset 23028
	 */
	uint16_t alternatorVoltageTargetRpmBins[ALTERNATOR_VOLTAGE_TARGET_SIZE];
	/**
	 * units: C
	 * offset 23036
	 */
	float cltBoostCorrBins[BOOST_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 23056
	 */
	float cltBoostCorr[BOOST_CURVE_SIZE];
	/**
	 * units: C
	 * offset 23076
	 */
	float iatBoostCorrBins[BOOST_CURVE_SIZE];
	/**
	 * units: ratio
	 * offset 23096
	 */
	float iatBoostCorr[BOOST_CURVE_SIZE];
	/**
	 * units: C
	 * offset 23116
	 */
	float cltBoostAdderBins[BOOST_CURVE_SIZE];
	/**
	 * offset 23136
	 */
	float cltBoostAdder[BOOST_CURVE_SIZE];
	/**
	 * units: C
	 * offset 23156
	 */
	float iatBoostAdderBins[BOOST_CURVE_SIZE];
	/**
	 * offset 23176
	 */
	float iatBoostAdder[BOOST_CURVE_SIZE];
	/**
	 * units: RPM
	 * offset 23196
	 */
	scaled_channel<uint8_t, 1, 100> minimumOilPressureBins[8];
	/**
	 * units: kPa
	 * offset 23204
	 */
	scaled_channel<uint8_t, 1, 10> minimumOilPressureValues[8];
	/**
	 * offset 23212
	 */
	blend_table_s targetAfrBlends[TARGET_AFR_BLEND_COUNT];
};
static_assert(sizeof(persistent_config_s) == 23588);

// end
// this section was generated automatically by rusEFI tool config_definition-all.jar based on (unknown script) integration/rusefi_config.txt
