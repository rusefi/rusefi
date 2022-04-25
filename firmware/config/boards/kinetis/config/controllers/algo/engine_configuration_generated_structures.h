// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on kinetis_gen_config.bat integration/rusefi_config.txt Mon Apr 25 03:58:59 UTC 2022
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
	 * need 4 byte alignment
	units
	 * offset 6
	 */
	uint8_t alignmentFill_at_6[2];
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
	 * offset 12
	 */
	adc_channel_e hwChannel;
	/**
	unit
	 * offset 13
	 */
	uint8_t alignmentFill[3];
};
static_assert(sizeof(air_pressure_sensor_config_s) == 16);

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
static_assert(sizeof(MAP_sensor_config_s) == 144);

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
	 * offset 28
	 */
	uint8_t unused28[24];
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
	bool useEeprom : 1 {};
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
	 * Print incoming and outgoing first bus CAN messages in rusEFI console
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
	 * offset 184
	 */
	float unused184;
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
	index
	 * offset 500
	 */
	int weHaveUnusedHere;
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
	 * need 4 byte alignment
	units
	 * offset 521
	 */
	uint8_t alignmentFill_at_521[3];
	/**
	 * This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	 * offset 524
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 525
	 */
	uint8_t alignmentFill_at_525[3];
	/**
	 * This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	 * offset 528
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * need 4 byte alignment
	units
	 * offset 529
	 */
	uint8_t alignmentFill_at_529[3];
	/**
	 * Second throttle body position sensor, single channel so far
	 * set_analog_input_pin tps2 X
	 * offset 532
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 533
	 */
	uint8_t alignmentFill_at_533[3];
	/**
	 * 0.1 is a good default value
	x
	 * offset 536
	 */
	float idle_derivativeFilterLoss;
	/**
	 * just a temporary solution
	angle
	 * offset 540
	 */
	int trailingSparkAngle;
	/**
	 * offset 544
	 */
	trigger_config_s trigger;
	/**
	 * offset 560
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * Single value to be used in event of a failed MAP sensor 
	 * This value is only used for speed density fueling calculations.
	kPa
	 * offset 561
	 */
	uint8_t failedMapFallback;
	/**
	 * Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost.
	%
	 * offset 562
	 */
	uint8_t boostControlSafeDutyCycle;
	/**
	 * need 4 byte alignment
	units
	 * offset 563
	 */
	uint8_t alignmentFill_at_563[1];
	/**
	 * offset 564
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 565
	 */
	uint8_t alignmentFill_at_565[3];
	/**
	 * set global_fuel_correction X
	coef
	 * offset 568
	 */
	float globalFuelCorrection;
	/**
	volts
	 * offset 572
	 */
	float adcVcc;
	/**
	Deg
	 * offset 576
	 */
	float mapCamDetectionAnglePosition;
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * offset 580
	 */
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	/**
	 * offset 581
	 */
	uint8_t camInputsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 584
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position first channel
	 * See throttlePedalPositionSecondAdcChannel for second channel
	 * See also tps1_1AdcChannel
	 * set_analog_input_pin pps X
	 * See throttlePedalUpVoltage and throttlePedalWOTVoltage
	 * offset 604
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 605
	 */
	uint8_t alignmentFill_at_605[3];
	/**
	 * offset 608
	 */
	brain_pin_e tle6240_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 609
	 */
	uint8_t alignmentFill_at_609[3];
	/**
	 * offset 612
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 613
	 */
	uint8_t alignmentFill_at_613[3];
	/**
	 * Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	 * offset 616
	 */
	switch_input_pin_e throttlePedalUpPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 617
	 */
	uint8_t alignmentFill_at_617[3];
	/**
	 * @see hasBaroSensor
	 * offset 620
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 636
	 */
	idle_hardware_s idle;
	/**
	 * Value between 0 and 100 used in Manual mode
	%
	 * offset 656
	 */
	float manIdlePosition;
	/**
	 * Ignition timing to remove when a knock event occurs.
	%
	 * offset 660
	 */
	uint8_t knockRetardAggression;
	/**
	 * After a knock event, reapply timing at this rate.
	deg/s
	 * offset 661
	 */
	uint8_t knockRetardReapplyRate;
	/**
	 * Maximum amount of knock retard.
	deg
	 * offset 662
	 */
	uint8_t knockRetardMaximum;
	/**
	 * Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	 * offset 663
	 */
	uint8_t vssFilterReciprocal;
	/**
	 * Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	ratio
	 * offset 664
	 */
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	/**
	 * Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	count
	 * offset 666
	 */
	uint8_t vssToothCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 667
	 */
	uint8_t alignmentFill_at_667[1];
	/**
	 * offset 668
	 */
	brain_pin_e l9779_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 669
	 */
	uint8_t alignmentFill_at_669[3];
	/**
	 * Same RPM is used for two ways of producing simulated RPM. See also triggerSimulatorPins (with wires)
	 * See also directSelfStimulation (no wires, bypassing input hardware)
	 * rpm X
	 * TODO: rename to triggerSimulatorRpm
	Rpm
	 * offset 672
	 */
	int triggerSimulatorFrequency;
	/**
	 * offset 676
	 */
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 688
	 */
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	/**
	 * offset 700
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 701
	 */
	uint8_t alignmentFill_at_701[3];
	/**
	 * offset 704
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 705
	 */
	uint8_t alignmentFill_at_705[3];
	/**
	 * offset 708
	 */
	brain_pin_e HD44780_rs;
	/**
	 * need 4 byte alignment
	units
	 * offset 709
	 */
	uint8_t alignmentFill_at_709[3];
	/**
	 * offset 712
	 */
	brain_pin_e HD44780_e;
	/**
	 * need 4 byte alignment
	units
	 * offset 713
	 */
	uint8_t alignmentFill_at_713[3];
	/**
	 * offset 716
	 */
	brain_pin_e HD44780_db4;
	/**
	 * need 4 byte alignment
	units
	 * offset 717
	 */
	uint8_t alignmentFill_at_717[3];
	/**
	 * offset 720
	 */
	brain_pin_e HD44780_db5;
	/**
	 * need 4 byte alignment
	units
	 * offset 721
	 */
	uint8_t alignmentFill_at_721[3];
	/**
	 * offset 724
	 */
	brain_pin_e HD44780_db6;
	/**
	 * need 4 byte alignment
	units
	 * offset 725
	 */
	uint8_t alignmentFill_at_725[3];
	/**
	 * offset 728
	 */
	brain_pin_e HD44780_db7;
	/**
	 * need 4 byte alignment
	units
	 * offset 729
	 */
	uint8_t alignmentFill_at_729[3];
	/**
	 * offset 732
	 */
	brain_pin_e gps_rx_pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 733
	 */
	uint8_t alignmentFill_at_733[3];
	/**
	 * offset 736
	 */
	brain_pin_e gps_tx_pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 737
	 */
	uint8_t alignmentFill_at_737[3];
	/**
	 * offset 740
	 */
	output_pin_e fuelPumpPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 741
	 */
	uint8_t alignmentFill_at_741[3];
	/**
	 * offset 744
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 745
	 */
	uint8_t alignmentFill_at_745[3];
	/**
	 * Check engine light, also malfunction indicator light. Always blinks once on boot.
	 * offset 748
	 */
	output_pin_e malfunctionIndicatorPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 749
	 */
	uint8_t alignmentFill_at_749[3];
	/**
	 * offset 752
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 753
	 */
	uint8_t alignmentFill_at_753[3];
	/**
	 * offset 756
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 757
	 */
	uint8_t alignmentFill_at_757[3];
	/**
	 * offset 760
	 */
	output_pin_e fanPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 761
	 */
	uint8_t alignmentFill_at_761[3];
	/**
	 * Some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 764
	 */
	switch_input_pin_e clutchDownPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 765
	 */
	uint8_t alignmentFill_at_765[3];
	/**
	 * offset 768
	 */
	output_pin_e alternatorControlPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 769
	 */
	uint8_t alignmentFill_at_769[3];
	/**
	 * offset 772
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 773
	 */
	uint8_t alignmentFill_at_773[3];
	/**
	 * offset 776
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 777
	 */
	uint8_t alignmentFill_at_777[3];
	/**
	 * offset 780
	 */
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 784
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 785
	 */
	uint8_t alignmentFill_at_785[3];
	/**
	 * set_cj125_heater_pin XXX
	 * offset 788
	 */
	brain_pin_e wboHeaterPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 789
	 */
	uint8_t alignmentFill_at_789[3];
	/**
	 * set_cj125_cs_pin XXX
	 * offset 792
	 */
	brain_pin_e cj125CsPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 793
	 */
	uint8_t alignmentFill_at_793[3];
	/**
	 * offset 796
	 */
	spi_device_e max31855spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 797
	 */
	uint8_t alignmentFill_at_797[3];
	/**
	 * offset 800
	 */
	brain_pin_e debugTriggerSync;
	/**
	 * need 4 byte alignment
	units
	 * offset 801
	 */
	uint8_t alignmentFill_at_801[3];
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 804
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 805
	 */
	uint8_t alignmentFill_at_805[3];
	/**
	 * offset 808
	 */
	brain_pin_e mc33972_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 809
	 */
	uint8_t alignmentFill_at_809[3];
	/**
	 * offset 812
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 813
	 */
	uint8_t alignmentFill_at_813[3];
	/**
	 * Useful in Research&Development phase
	 * offset 816
	 */
	adc_channel_e auxFastSensor1_adcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 817
	 */
	uint8_t alignmentFill_at_817[3];
	/**
	 * First throttle body, second sensor.
	 * offset 820
	 */
	adc_channel_e tps1_2AdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 821
	 */
	uint8_t alignmentFill_at_821[3];
	/**
	 * Second throttle body, second sensor.
	 * offset 824
	 */
	adc_channel_e tps2_2AdcChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 825
	 */
	uint8_t alignmentFill_at_825[3];
	/**
	 * Electronic throttle pedal position input
	 * Second channel
	 * See also tps1_1AdcChannel
	 * See throttlePedalSecondaryUpVoltage and throttlePedalSecondaryWOTVoltage
	 * offset 828
	 */
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	/**
	%
	 * offset 829
	 */
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 837
	 */
	uint8_t alignmentFill_at_837[3];
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 840
	 */
	ego_sensor_e afr_type;
	/**
	 * 0.1 is a good default value
	x
	 * offset 844
	 */
	float idle_antiwindupFreq;
	/**
	 * offset 848
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 851
	 */
	uint8_t alignmentFill_at_851[1];
	/**
	 * offset 852
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 853
	 */
	uint8_t alignmentFill_at_853[3];
	/**
	 * offset 856
	 */
	output_pin_e tachOutputPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 857
	 */
	uint8_t alignmentFill_at_857[3];
	/**
	 * offset 860
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 861
	 */
	uint8_t alignmentFill_at_861[3];
	/**
	 * offset 864
	 */
	output_pin_e mainRelayPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 865
	 */
	uint8_t alignmentFill_at_865[3];
	/**
	 * offset 868
	 */
	brain_pin_e sdCardCsPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 869
	 */
	uint8_t alignmentFill_at_869[3];
	/**
	 * set_can_tx_pin X
	 * offset 872
	 */
	brain_pin_e canTxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 873
	 */
	uint8_t alignmentFill_at_873[3];
	/**
	 * set_can_rx_pin X
	 * offset 876
	 */
	brain_pin_e canRxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 877
	 */
	uint8_t alignmentFill_at_877[3];
	/**
	 * offset 880
	 */
	pin_input_mode_e throttlePedalUpPinMode;
	/**
	 * Additional idle % while A/C is active
	%
	 * offset 881
	 */
	uint8_t acIdleExtraOffset;
	/**
	 * Ratio between the wheels and your transmission output.
	ratio
	 * offset 882
	 */
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	/**
	 * offset 884
	 */
	brain_input_pin_e tcuInputSpeedSensorPin;
	/**
	 * offset 885
	 */
	uint8_t tcuInputSpeedSensorTeeth;
	/**
	 * Voltage when the wastegate is closed.
	 * You probably don't have one of these!
	mv
	 * offset 886
	 */
	uint16_t wastegatePositionMin;
	/**
	 * Voltage when the wastegate is fully open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 888
	 */
	uint16_t wastegatePositionMax;
	/**
	 * Voltage when the idle valve is closed.
	 * You probably don't have one of these!
	mv
	 * offset 890
	 */
	uint16_t idlePositionMin;
	/**
	 * Voltage when the idle valve is open.
	 * You probably don't have one of these!
	 * 1 volt = 1000 units
	mv
	 * offset 892
	 */
	uint16_t idlePositionMax;
	/**
	 * need 4 byte alignment
	units
	 * offset 894
	 */
	uint8_t alignmentFill_at_894[2];
	/**
	 * Secondary TTL channel baud rate
	BPs
	 * offset 896
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * Just for reference really, not taken into account by any logic at this point
	CR
	 * offset 900
	 */
	float compressionRatio;
	/**
	 * Each rusEFI piece can provide synthetic trigger signal for external ECU. Sometimes these wires are routed back into trigger inputs of the same rusEFI board.
	 * See also directSelfStimulation which is different.
	 * offset 904
	 */
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 907
	 */
	uint8_t alignmentFill_at_907[1];
	/**
	 * offset 908
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 911
	 */
	uint8_t alignmentFill_at_911[1];
	/**
	 * Narrow band o2 heater, not used for CJ125. 'ON' if engine is running, 'OFF' if stopped or cranking. See wboHeaterPin
	 * offset 912
	 */
	output_pin_e o2heaterPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 913
	 */
	uint8_t alignmentFill_at_913[3];
	/**
	 * offset 916
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	 * need 4 byte alignment
	units
	 * offset 917
	 */
	uint8_t alignmentFill_at_917[3];
	/**
	offset 920 bit 0 */
	bool is_enabled_spi_1 : 1 {};
	/**
	offset 920 bit 1 */
	bool is_enabled_spi_2 : 1 {};
	/**
	offset 920 bit 2 */
	bool is_enabled_spi_3 : 1 {};
	/**
	 * enable sd/disable sd
	offset 920 bit 3 */
	bool isSdCardEnabled : 1 {};
	/**
	 * Use 11 bit (standard) or 29 bit (extended) IDs for rusEFI verbose CAN format.
	offset 920 bit 4 */
	bool rusefiVerbose29b : 1 {};
	/**
	offset 920 bit 5 */
	bool isEngineControlEnabled : 1 {};
	/**
	offset 920 bit 6 */
	bool isHip9011Enabled : 1 {};
	/**
	offset 920 bit 7 */
	bool isVerboseAlternator : 1 {};
	/**
	offset 920 bit 8 */
	bool useSerialPort : 1 {};
	/**
	 * This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	offset 920 bit 9 */
	bool useStepperIdle : 1 {};
	/**
	offset 920 bit 10 */
	bool enabledStep1Limiter : 1 {};
	/**
	offset 920 bit 11 */
	bool useTpicAdvancedMode : 1 {};
	/**
	offset 920 bit 12 */
	bool useLcdScreen : 1 {};
	/**
	offset 920 bit 13 */
	bool verboseTLE8888 : 1 {};
	/**
	 * CAN broadcast using custom rusEFI protocol
	 * enable can_broadcast/disable can_broadcast
	offset 920 bit 14 */
	bool enableVerboseCanTx : 1 {};
	/**
	 * This will cause the alternator to be operated in a basic on or off mode, this is the simplest alternator control.
	offset 920 bit 15 */
	bool onOffAlternatorLogic : 1 {};
	/**
	 * enable cj125/disable cj125
	offset 920 bit 16 */
	bool isCJ125Enabled : 1 {};
	/**
	 * Use rise or fall signal front
	 * get vvtCamSensorUseRise
	offset 920 bit 17 */
	bool vvtCamSensorUseRise : 1 {};
	/**
	 * Useful for individual intakes
	offset 920 bit 18 */
	bool measureMapOnlyInOneCylinder : 1 {};
	/**
	offset 920 bit 19 */
	bool stepperForceParkingEveryRestart : 1 {};
	/**
	 * If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	offset 920 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1 {};
	/**
	 * This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	offset 920 bit 21 */
	bool coastingFuelCutEnabled : 1 {};
	/**
	 * This setting allows the ECU to open the IAC during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars. Used in Auto-PID Idle mode.
	offset 920 bit 22 */
	bool useIacTableForCoasting : 1 {};
	/**
	offset 920 bit 23 */
	bool useNoiselessTriggerDecoder : 1 {};
	/**
	offset 920 bit 24 */
	bool useIdleTimingPidControl : 1 {};
	/**
	 * Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	offset 920 bit 25 */
	bool disableEtbWhenEngineStopped : 1 {};
	/**
	offset 920 bit 26 */
	bool is_enabled_spi_4 : 1 {};
	/**
	 * Disable the electronic throttle motor and DC idle motor for testing.
	 * This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	offset 920 bit 27 */
	bool pauseEtbControl : 1 {};
	/**
	offset 920 bit 28 */
	bool alignEngineSnifferAtTDC : 1 {};
	/**
	 * This setting allows the ETB to act as the idle air control valve and move to regulate the airflow at idle.
	offset 920 bit 29 */
	bool useETBforIdleControl : 1 {};
	/**
	offset 920 bit 30 */
	bool idleIncrementalPidCic : 1 {};
	/**
	 * AEM X-Series or rusEFI Wideband
	offset 920 bit 31 */
	bool enableAemXSeries : 1 {};
	/**
	 * offset 924
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 928
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 929
	 */
	uint8_t alignmentFill_at_929[3];
	/**
	 * offset 932
	 */
	brain_pin_e hip9011CsPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 933
	 */
	uint8_t alignmentFill_at_933[3];
	/**
	 * offset 936
	 */
	brain_pin_e hip9011IntHoldPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 937
	 */
	uint8_t alignmentFill_at_937[3];
	/**
	 * offset 940
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 941
	 */
	uint8_t alignmentFill_at_941[3];
	/**
	 * offset 944
	 */
	uint32_t verboseCanBaseAddress;
	/**
	v
	 * offset 948
	 */
	uint8_t mc33_hvolt;
	/**
	 * Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	kPa
	 * offset 949
	 */
	uint8_t minimumBoostClosedLoopMap;
	/**
	 * need 4 byte alignment
	units
	 * offset 950
	 */
	uint8_t alignmentFill_at_950[2];
	/**
	 * Optional Radiator Fan used with A/C
	 * offset 952
	 */
	output_pin_e acFanPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 953
	 */
	uint8_t alignmentFill_at_953[3];
	/**
	 * offset 956
	 */
	pin_output_mode_e acFanPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 957
	 */
	uint8_t alignmentFill_at_957[3];
	/**
	 * offset 960
	 */
	spi_device_e l9779spiDevice;
	/**
	volts
	 * offset 961
	 */
	uint8_t dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 969
	 */
	uint8_t alignmentFill_at_969[3];
	/**
	 * offset 972
	 */
	imu_type_e imuType;
	/**
	multiplier
	 * offset 973
	 */
	uint8_t dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 981
	 */
	uint8_t alignmentFill_at_981[1];
	/**
	kg
	 * offset 982
	 */
	uint16_t vehicleWeight;
	/**
	 * How far above idle speed do we consider idling?
	 * For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	RPM
	 * offset 984
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	ms
	 * offset 986
	 */
	uint16_t applyNonlinearBelowPulse;
	/**
	 * offset 988
	 */
	brain_pin_e lps25BaroSensorScl;
	/**
	 * need 4 byte alignment
	units
	 * offset 989
	 */
	uint8_t alignmentFill_at_989[3];
	/**
	 * offset 992
	 */
	brain_pin_e lps25BaroSensorSda;
	/**
	 * need 4 byte alignment
	units
	 * offset 993
	 */
	uint8_t alignmentFill_at_993[3];
	/**
	 * offset 996
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 997
	 */
	uint8_t alignmentFill_at_997[3];
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 1000
	 */
	switch_input_pin_e clutchUpPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1001
	 */
	uint8_t alignmentFill_at_1001[3];
	/**
	 * offset 1004
	 */
	InjectorNonlinearMode injectorNonlinearMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1005
	 */
	uint8_t alignmentFill_at_1005[3];
	/**
	 * offset 1008
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1009
	 */
	uint8_t alignmentFill_at_1009[3];
	/**
	 * offset 1012
	 */
	brain_pin_e max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * Continental/GM flex fuel sensor, 50-150hz type
	 * offset 1020
	 */
	brain_input_pin_e flexSensorPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1021
	 */
	uint8_t alignmentFill_at_1021[3];
	/**
	 * offset 1024
	 */
	brain_pin_e test557pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1025
	 */
	uint8_t alignmentFill_at_1025[3];
	/**
	 * offset 1028
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1029
	 */
	uint8_t alignmentFill_at_1029[3];
	/**
	 * offset 1032
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	:1
	 * offset 1033
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	/**
	 * Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	%
	 * offset 1034
	 */
	uint8_t etbMaximumPosition;
	/**
	 * need 4 byte alignment
	units
	 * offset 1035
	 */
	uint8_t alignmentFill_at_1035[1];
	/**
	 * SD card logging period, in milliseconds
	ms
	 * offset 1036
	 */
	int16_t sdCardPeriodMs;
	/**
	 * need 4 byte alignment
	units
	 * offset 1038
	 */
	uint8_t alignmentFill_at_1038[2];
	/**
	 * offset 1040
	 */
	adc_channel_e idlePositionSensor;
	/**
	 * need 4 byte alignment
	units
	 * offset 1041
	 */
	uint8_t alignmentFill_at_1041[3];
	/**
	 * offset 1044
	 */
	brain_pin_e debugMapAveraging;
	/**
	 * need 4 byte alignment
	units
	 * offset 1045
	 */
	uint8_t alignmentFill_at_1045[3];
	/**
	 * offset 1048
	 */
	output_pin_e starterRelayDisablePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1049
	 */
	uint8_t alignmentFill_at_1049[3];
	/**
	 * On some vehicles we can disable starter once engine is already running
	 * offset 1052
	 */
	pin_output_mode_e starterRelayDisablePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1053
	 */
	uint8_t alignmentFill_at_1053[3];
	/**
	 * Some Subaru and some Mazda use double-solenoid idle air valve
	 * offset 1056
	 */
	output_pin_e secondSolenoidPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1057
	 */
	uint8_t alignmentFill_at_1057[3];
	/**
	 * See also starterControlPin
	 * offset 1060
	 */
	switch_input_pin_e startStopButtonPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1061
	 */
	uint8_t alignmentFill_at_1061[3];
	/**
	 * This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	count
	 * offset 1064
	 */
	int mapMinBufferLength;
	/**
	 * Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	%
	 * offset 1068
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	%
	 * offset 1070
	 */
	int16_t stepperParkingExtraSteps;
	/**
	ADC
	 * offset 1072
	 */
	uint16_t tps1SecondaryMin;
	/**
	ADC
	 * offset 1074
	 */
	uint16_t tps1SecondaryMax;
	/**
	rpm
	 * offset 1076
	 */
	int16_t antiLagRpmTreshold;
	/**
	 * Maximum time to crank starter when start/stop button is pressed
	Seconds
	 * offset 1078
	 */
	uint16_t startCrankingDuration;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 1080
	 */
	brain_pin_e triggerErrorPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1081
	 */
	uint8_t alignmentFill_at_1081[3];
	/**
	 * offset 1084
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1085
	 */
	uint8_t alignmentFill_at_1085[3];
	/**
	 * offset 1088
	 */
	output_pin_e acRelayPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1089
	 */
	uint8_t alignmentFill_at_1089[3];
	/**
	 * offset 1092
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1093
	 */
	uint8_t alignmentFill_at_1093[3];
	/**
	 * offset 1096
	 */
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	/**
	 * offset 1128
	 */
	brain_pin_e spi1mosiPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1129
	 */
	uint8_t alignmentFill_at_1129[3];
	/**
	 * offset 1132
	 */
	brain_pin_e spi1misoPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1133
	 */
	uint8_t alignmentFill_at_1133[3];
	/**
	 * offset 1136
	 */
	brain_pin_e spi1sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1137
	 */
	uint8_t alignmentFill_at_1137[3];
	/**
	 * offset 1140
	 */
	brain_pin_e spi2mosiPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1141
	 */
	uint8_t alignmentFill_at_1141[3];
	/**
	 * offset 1144
	 */
	brain_pin_e spi2misoPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1145
	 */
	uint8_t alignmentFill_at_1145[3];
	/**
	 * offset 1148
	 */
	brain_pin_e spi2sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1149
	 */
	uint8_t alignmentFill_at_1149[3];
	/**
	 * offset 1152
	 */
	brain_pin_e spi3mosiPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1153
	 */
	uint8_t alignmentFill_at_1153[3];
	/**
	 * offset 1156
	 */
	brain_pin_e spi3misoPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1157
	 */
	uint8_t alignmentFill_at_1157[3];
	/**
	 * offset 1160
	 */
	brain_pin_e spi3sckPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1161
	 */
	uint8_t alignmentFill_at_1161[3];
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 1164
	 */
	brain_pin_e cdmInputPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1165
	 */
	uint8_t alignmentFill_at_1165[3];
	/**
	 * offset 1168
	 */
	brain_pin_e joystickCenterPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1169
	 */
	uint8_t alignmentFill_at_1169[3];
	/**
	 * offset 1172
	 */
	brain_pin_e joystickAPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1173
	 */
	uint8_t alignmentFill_at_1173[3];
	/**
	 * offset 1176
	 */
	brain_pin_e joystickBPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1177
	 */
	uint8_t alignmentFill_at_1177[3];
	/**
	 * offset 1180
	 */
	brain_pin_e joystickCPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1181
	 */
	uint8_t alignmentFill_at_1181[3];
	/**
	 * offset 1184
	 */
	brain_pin_e joystickDPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1185
	 */
	uint8_t alignmentFill_at_1185[3];
	/**
	 * offset 1188
	 */
	uart_device_e consoleUartDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1189
	 */
	uint8_t alignmentFill_at_1189[3];
	/**
	 * rusEFI console Sensor Sniffer mode
	 * offset 1192
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 1196
	 */
	maf_sensor_type_e mafSensorType;
	/**
	offset 1200 bit 0 */
	bool clutchUpPinInverted : 1 {};
	/**
	offset 1200 bit 1 */
	bool clutchDownPinInverted : 1 {};
	/**
	 * If enabled we use two H-bridges to drive stepper idle air valve
	offset 1200 bit 2 */
	bool useHbridgesToDriveIdleStepper : 1 {};
	/**
	offset 1200 bit 3 */
	bool multisparkEnable : 1 {};
	/**
	offset 1200 bit 4 */
	bool enableLaunchRetard : 1 {};
	/**
	offset 1200 bit 5 */
	bool unfinishedenableLaunchBoost : 1 {};
	/**
	offset 1200 bit 6 */
	bool unfinishedlaunchDisableBySpeed : 1 {};
	/**
	 * Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	offset 1200 bit 7 */
	bool enableCanVss : 1 {};
	/**
	offset 1200 bit 8 */
	bool enableInnovateLC2 : 1 {};
	/**
	offset 1200 bit 9 */
	bool showHumanReadableWarning : 1 {};
	/**
	 * If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise. Use of this mode is required if you have a narrowband O2 sensor.
	offset 1200 bit 10 */
	bool stftIgnoreErrorMagnitude : 1 {};
	/**
	offset 1200 bit 11 */
	bool tempBooleanForVerySpecialCases : 1 {};
	/**
	offset 1200 bit 12 */
	bool enableSoftwareKnock : 1 {};
	/**
	 * enable vvt_details
	offset 1200 bit 13 */
	bool verboseVVTDecoding : 1 {};
	/**
	 * get invertCamVVTSignal
	offset 1200 bit 14 */
	bool invertCamVVTSignal : 1 {};
	/**
	 * This property is useful if using rusEFI as TCM or BCM only
	 * enable consumeObdSensors
	offset 1200 bit 15 */
	bool consumeObdSensors : 1 {};
	/**
	offset 1200 bit 16 */
	bool knockBankCyl1 : 1 {};
	/**
	offset 1200 bit 17 */
	bool knockBankCyl2 : 1 {};
	/**
	offset 1200 bit 18 */
	bool knockBankCyl3 : 1 {};
	/**
	offset 1200 bit 19 */
	bool knockBankCyl4 : 1 {};
	/**
	offset 1200 bit 20 */
	bool knockBankCyl5 : 1 {};
	/**
	offset 1200 bit 21 */
	bool knockBankCyl6 : 1 {};
	/**
	offset 1200 bit 22 */
	bool knockBankCyl7 : 1 {};
	/**
	offset 1200 bit 23 */
	bool knockBankCyl8 : 1 {};
	/**
	offset 1200 bit 24 */
	bool knockBankCyl9 : 1 {};
	/**
	offset 1200 bit 25 */
	bool knockBankCyl10 : 1 {};
	/**
	offset 1200 bit 26 */
	bool knockBankCyl11 : 1 {};
	/**
	offset 1200 bit 27 */
	bool knockBankCyl12 : 1 {};
	/**
	offset 1200 bit 28 */
	bool tcuEnabled : 1 {};
	/**
	offset 1200 bit 29 */
	bool canBroadcastUseChannelTwo : 1 {};
	/**
	 * If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coilss
	offset 1200 bit 30 */
	bool useRawOutputToDriveIdleStepper : 1 {};
	/**
	 * Print incoming and outgoing second bus CAN messages in rusEFI console
	offset 1200 bit 31 */
	bool verboseCan2 : 1 {};
	/**
	 * offset 1204
	 */
	dc_io etbIo[ETB_COUNT];
	/**
	 * Wastegate control Solenoid
	 * offset 1236
	 */
	output_pin_e boostControlPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1237
	 */
	uint8_t alignmentFill_at_1237[3];
	/**
	 * offset 1240
	 */
	pin_output_mode_e boostControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1241
	 */
	uint8_t alignmentFill_at_1241[3];
	/**
	 * offset 1244
	 */
	switch_input_pin_e antiLagActivatePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1245
	 */
	uint8_t alignmentFill_at_1245[3];
	/**
	 * offset 1248
	 */
	switch_input_pin_e launchActivatePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1249
	 */
	uint8_t alignmentFill_at_1249[3];
	/**
	 * offset 1252
	 */
	pid_s boostPid;
	/**
	 * offset 1272
	 */
	boostType_e boostType;
	/**
	Hz
	 * offset 1276
	 */
	int boostPwmFrequency;
	/**
	 * offset 1280
	 */
	launchActivationMode_e launchActivationMode;
	/**
	 * offset 1284
	 */
	antiLagActivationMode_e antiLagActivationMode;
	/**
	 * Disabled above this speed
	Kph
	 * offset 1288
	 */
	int launchSpeedThreshold;
	/**
	 * Range from Launch Rpm for Timing Retard to activate
	RPM
	 * offset 1292
	 */
	int launchTimingRpmRange;
	/**
	 * Extra Fuel Added
	%
	 * offset 1296
	 */
	int launchFuelAdded;
	/**
	 * Duty Cycle for the Boost Solenoid
	%
	 * offset 1300
	 */
	int launchBoostDuty;
	/**
	 * RPM Range for Hard Cut
	rpm
	 * offset 1304
	 */
	int hardCutRpmRange;
	/**
	rpm
	 * offset 1308
	 */
	int launchAdvanceRpmRange;
	/**
	rpm
	 * offset 1312
	 */
	int launchTpsTreshold;
	/**
	rpm
	 * offset 1316
	 */
	float launchActivateDelay;
	/**
	 * offset 1320
	 */
	stft_s stft;
	/**
	 * offset 1344
	 */
	dc_io stepperDcIo[DC_PER_STEPPER];
	/**
	 * For example, BMW, GM or Chevrolet
	 * REQUIRED for rusEFI Online
	 * offset 1376
	 */
	vehicle_info_t engineMake;
	/**
	 * For example, LS1 or NB2
	 * REQUIRED for rusEFI Online
	 * offset 1408
	 */
	vehicle_info_t engineCode;
	/**
	 * For example, Hunchback or Orange Miata
	 * Vehicle name has to be unique between your vehicles.
	 * REQUIRED for rusEFI Online
	 * offset 1440
	 */
	vehicle_info_t vehicleName;
	/**
	 * offset 1472
	 */
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1478
	 */
	uint8_t alignmentFill_at_1478[2];
	/**
	 * offset 1480
	 */
	etb_function_e etbFunctions[ETB_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1482
	 */
	uint8_t alignmentFill_at_1482[2];
	/**
	 * offset 1484
	 */
	spi_device_e drv8860spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 1485
	 */
	uint8_t alignmentFill_at_1485[3];
	/**
	 * offset 1488
	 */
	brain_pin_e drv8860_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 1489
	 */
	uint8_t alignmentFill_at_1489[3];
	/**
	 * offset 1492
	 */
	pin_output_mode_e drv8860_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1493
	 */
	uint8_t alignmentFill_at_1493[3];
	/**
	 * offset 1496
	 */
	brain_pin_e drv8860_miso;
	/**
	 * need 4 byte alignment
	units
	 * offset 1497
	 */
	uint8_t alignmentFill_at_1497[1];
	/**
	volt
	 * offset 1498
	 */
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1514
	 */
	uint8_t alignmentFill_at_1514[2];
	/**
	 * offset 1516
	 */
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	/**
	 * Angle between cam sensor and VVT zero position
	 * set vvt_offset X
	value
	 * offset 1524
	 */
	float vvtOffsets[CAM_INPUTS_COUNT];
	/**
	 * offset 1528
	 */
	float vvtOffsetsPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * offset 1540
	 */
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	/**
	 * offset 1572
	 */
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	/**
	ADC
	 * offset 1636
	 */
	uint16_t tps2SecondaryMin;
	/**
	ADC
	 * offset 1638
	 */
	uint16_t tps2SecondaryMax;
	/**
	offset 1640 bit 0 */
	bool disablePrimaryUart : 1 {};
	/**
	 * Enables lambda sensor closed loop feedback for fuelling.
	offset 1640 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	/**
	 * Print details into rusEFI console
	 * enable verbose_idle
	offset 1640 bit 2 */
	bool isVerboseIAC : 1 {};
	/**
	offset 1640 bit 3 */
	bool boardUseTachPullUp : 1 {};
	/**
	offset 1640 bit 4 */
	bool boardUseTempPullUp : 1 {};
	/**
	 * This options enables data for 'engine sniffer' tab in console, which comes at some CPU price
	offset 1640 bit 5 */
	bool isEngineChartEnabled : 1 {};
	/**
	 * Sometimes we have a performance issue while printing error
	offset 1640 bit 6 */
	bool silentTriggerError : 1 {};
	/**
	offset 1640 bit 7 */
	bool useLinearCltSensor : 1 {};
	/**
	 * enable can_read/disable can_read
	offset 1640 bit 8 */
	bool canReadEnabled : 1 {};
	/**
	 * enable can_write/disable can_write
	offset 1640 bit 9 */
	bool canWriteEnabled : 1 {};
	/**
	offset 1640 bit 10 */
	bool useLinearIatSensor : 1 {};
	/**
	offset 1640 bit 11 */
	bool boardUse2stepPullDown : 1 {};
	/**
	 * Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	offset 1640 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1 {};
	/**
	 * This enables smart alternator control and activates the extra alternator settings.
	offset 1640 bit 13 */
	bool isAlternatorControlEnabled : 1 {};
	/**
	 * This setting flips the signal from the primary engine speed sensor.
	offset 1640 bit 14 */
	bool invertPrimaryTriggerSignal : 1 {};
	/**
	 * This setting flips the signal from the secondary engine speed sensor.
	offset 1640 bit 15 */
	bool invertSecondaryTriggerSignal : 1 {};
	/**
	offset 1640 bit 16 */
	bool cutFuelOnHardLimit : 1 {};
	/**
	offset 1640 bit 17 */
	bool cutSparkOnHardLimit : 1 {};
	/**
	offset 1640 bit 18 */
	bool launchFuelCutEnable : 1 {};
	/**
	 * This is the Cut Mode normally used
	offset 1640 bit 19 */
	bool launchSparkCutEnable : 1 {};
	/**
	offset 1640 bit 20 */
	bool boardUseCrankPullUp : 1 {};
	/**
	offset 1640 bit 21 */
	bool boardUseCamPullDown : 1 {};
	/**
	offset 1640 bit 22 */
	bool boardUseCamVrPullUp : 1 {};
	/**
	offset 1640 bit 23 */
	bool boardUseD2PullDown : 1 {};
	/**
	offset 1640 bit 24 */
	bool boardUseD3PullDown : 1 {};
	/**
	offset 1640 bit 25 */
	bool boardUseD4PullDown : 1 {};
	/**
	offset 1640 bit 26 */
	bool boardUseD5PullDown : 1 {};
	/**
	 * Sometimes we just have to shut the engine down. Use carefully!
	offset 1640 bit 27 */
	bool useFSIO5ForCriticalIssueEngineStop : 1 {};
	/**
	 * Sometimes we have to miss injection on purpose to attract driver's attention
	offset 1640 bit 28 */
	bool useFSIO4ForSeriousEngineWarning : 1 {};
	/**
	offset 1640 bit 29 */
	bool launchActivateInverted : 1 {};
	/**
	offset 1640 bit 30 */
	bool twoStroke : 1 {};
	/**
	 * Where is your primary skipped wheel located?
	offset 1640 bit 31 */
	bool skippedWheelOnCam : 1 {};
	/**
	 * offset 1644
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * need 4 byte alignment
	units
	 * offset 1645
	 */
	uint8_t alignmentFill_at_1645[3];
	/**
	 * A/C button input
	 * offset 1648
	 */
	switch_input_pin_e acSwitch;
	/**
	 * need 4 byte alignment
	units
	 * offset 1649
	 */
	uint8_t alignmentFill_at_1649[3];
	/**
	 * offset 1652
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * Expected neutral position
	%
	 * offset 1653
	 */
	uint8_t etbNeutralPosition;
	/**
	 * need 4 byte alignment
	units
	 * offset 1654
	 */
	uint8_t alignmentFill_at_1654[2];
	/**
	 * See also idleRpmPid
	 * offset 1656
	 */
	idle_mode_e idleMode;
	/**
	 * Enable fuel injection - This is default off for new projects as a safety feature, set to "true" to enable fuel injection and further injector settings.
	offset 1660 bit 0 */
	bool isInjectionEnabled : 1 {};
	/**
	 * Enable ignition - This is default off for new projects as a safety feature, set to "true" to enable ignition and further ignition settings.
	offset 1660 bit 1 */
	bool isIgnitionEnabled : 1 {};
	/**
	 * When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	offset 1660 bit 2 */
	bool isCylinderCleanupEnabled : 1 {};
	/**
	 * Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	offset 1660 bit 3 */
	bool complexWallModel : 1 {};
	/**
	offset 1660 bit 4 */
	bool alwaysInstantRpm : 1 {};
	/**
	offset 1660 bit 5 */
	bool isMapAveragingEnabled : 1 {};
	/**
	 * If enabled, use separate temperature multiplier table for cranking idle position.
	 * If disabled, use normal running multiplier table applied to the cranking base position.
	offset 1660 bit 6 */
	bool overrideCrankingIacSetting : 1 {};
	/**
	 * This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	offset 1660 bit 7 */
	bool useSeparateAdvanceForIdle : 1 {};
	/**
	offset 1660 bit 8 */
	bool unused1476b8 : 1 {};
	/**
	offset 1660 bit 9 */
	bool isWaveAnalyzerEnabled : 1 {};
	/**
	 * This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	offset 1660 bit 10 */
	bool useSeparateVeForIdle : 1 {};
	/**
	 * enable trigger_details
	offset 1660 bit 11 */
	bool verboseTriggerSynchDetails : 1 {};
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1660 bit 12 */
	bool isManualSpinningMode : 1 {};
	/**
	 * This is needed if your coils are individually wired and you wish to use batch injection.
	 * enable two_wire_batch_injection
	offset 1660 bit 13 */
	bool twoWireBatchInjection : 1 {};
	/**
	 * VR sensors are only precise on rising front
	 * enable trigger_only_front
	offset 1660 bit 14 */
	bool useOnlyRisingEdgeForTrigger : 1 {};
	/**
	 * This is needed if your coils are individually wired (COP) and you wish to use batch ignition (Wasted Spark).
	offset 1660 bit 15 */
	bool twoWireBatchIgnition : 1 {};
	/**
	offset 1660 bit 16 */
	bool useFixedBaroCorrFromMap : 1 {};
	/**
	 * In Constant mode, timing is automatically tapered to running as RPM increases.
	 * In Table mode, the "Cranking ignition advance" table is used directly.
	offset 1660 bit 17 */
	bool useSeparateAdvanceForCranking : 1 {};
	/**
	 * This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	 * You probably don't need this.
	offset 1660 bit 18 */
	bool useAdvanceCorrectionsForCranking : 1 {};
	/**
	 * Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	offset 1660 bit 19 */
	bool flexCranking : 1 {};
	/**
	offset 1660 bit 20 */
	bool unused1476b20 : 1 {};
	/**
	 * This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	offset 1660 bit 21 */
	bool useIacPidMultTable : 1 {};
	/**
	offset 1660 bit 22 */
	bool isBoostControlEnabled : 1 {};
	/**
	 * Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	offset 1660 bit 23 */
	bool launchSmoothRetard : 1 {};
	/**
	offset 1660 bit 24 */
	bool unused_1484_bit_24 : 1 {};
	/**
	offset 1660 bit 25 */
	bool unused_1484_bit_25 : 1 {};
	/**
	offset 1660 bit 26 */
	bool unused_1484_bit_26 : 1 {};
	/**
	offset 1660 bit 27 */
	bool unused_1484_bit_27 : 1 {};
	/**
	offset 1660 bit 28 */
	bool unused_1484_bit_28 : 1 {};
	/**
	offset 1660 bit 29 */
	bool unused_1484_bit_29 : 1 {};
	/**
	offset 1660 bit 30 */
	bool unused_1484_bit_30 : 1 {};
	/**
	offset 1660 bit 31 */
	bool unused_1484_bit_31 : 1 {};
	/**
	count
	 * offset 1664
	 */
	uint32_t engineChartSize;
	/**
	mult
	 * offset 1668
	 */
	float turboSpeedSensorMultiplier;
	/**
	 * offset 1672
	 */
	brain_pin_e camInputsDebug[CAM_INPUTS_COUNT];
	/**
	 * offset 1673
	 */
	uint8_t camInputsDebugPadding[CAM_INPUTS_COUNT_padding];
	/**
	 * Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	RPM
	 * offset 1676
	 */
	int16_t acIdleRpmBump;
	/**
	 * set warningPeriod X
	seconds
	 * offset 1678
	 */
	int16_t warningPeriod;
	/**
	angle
	 * offset 1680
	 */
	float knockDetectionWindowStart;
	/**
	angle
	 * offset 1684
	 */
	float knockDetectionWindowEnd;
	/**
	ms
	 * offset 1688
	 */
	float idleStepperReactionTime;
	/**
	count
	 * offset 1692
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	sec
	 * offset 1696
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	index
	 * offset 1700
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	kPa
	 * offset 1704
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	RPM
	 * offset 1720
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	ratio
	 * offset 1736
	 */
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	/**
	 * Cranking fuel correction coefficient based on TPS
	Ratio
	 * offset 1800
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	%
	 * offset 1832
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * Duration in ms or duty cycle depending on selected mode
	 * offset 1864
	 */
	float tachPulseDuractionMs;
	/**
	 * Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	Seconds
	 * offset 1868
	 */
	float wwaeTau;
	/**
	 * offset 1872
	 */
	pid_s alternatorControl;
	/**
	 * offset 1892
	 */
	pid_s etb;
	/**
	 * offset 1912
	 */
	brain_pin_e triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1915
	 */
	uint8_t alignmentFill_at_1915[1];
	/**
	 * offset 1916
	 */
	brain_input_pin_e turboSpeedSensorInputPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1917
	 */
	uint8_t alignmentFill_at_1917[1];
	/**
	 * Closed throttle#2. todo: extract these two fields into a structure
	 * See also tps2_1AdcChannel
	 * set tps2_min X
	ADC
	 * offset 1918
	 */
	int16_t tps2Min;
	/**
	 * Full throttle#2. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps2_max X
	ADC
	 * offset 1920
	 */
	int16_t tps2Max;
	/**
	 * need 4 byte alignment
	units
	 * offset 1922
	 */
	uint8_t alignmentFill_at_1922[2];
	/**
	 * See also startStopButtonPin
	 * offset 1924
	 */
	output_pin_e starterControlPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1925
	 */
	uint8_t alignmentFill_at_1925[3];
	/**
	 * offset 1928
	 */
	pin_input_mode_e startStopButtonMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 1929
	 */
	uint8_t alignmentFill_at_1929[3];
	/**
	 * offset 1932
	 */
	brain_pin_e mc33816_flag0;
	/**
	Pulse
	 * offset 1933
	 */
	uint8_t tachPulsePerRev;
	/**
	 * need 4 byte alignment
	units
	 * offset 1934
	 */
	uint8_t alignmentFill_at_1934[2];
	/**
	 * kPa value which is too low to be true
	kPa
	 * offset 1936
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	kPa
	 * offset 1940
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * How long to wait for the spark to fire before recharging the coil for another spark.
	ms
	 * offset 1944
	 */
	uint16_t multisparkSparkDuration;
	/**
	 * This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	ms
	 * offset 1946
	 */
	uint16_t multisparkDwell;
	/**
	 * See cltIdleRpmBins
	 * offset 1948
	 */
	pid_s idleRpmPid;
	/**
	 * 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	Fraction
	 * offset 1968
	 */
	float wwaeBeta;
	/**
	 * blue LED on many rusEFI boards.
	 * Blue Communication LED which is expected to blink at 50% duty cycle during normal board operation.
	 * If USB communication cable is connected Blue LED starts to blink faster.
	 * offset 1972
	 */
	brain_pin_e communicationLedPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1973
	 */
	uint8_t alignmentFill_at_1973[3];
	/**
	 * Green LED on many rusEFI boards.
	 * Off if engine is stopped, blinks if engine is cranking, solid if engine is running.
	 * offset 1976
	 */
	brain_pin_e runningLedPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1977
	 */
	uint8_t alignmentFill_at_1977[3];
	/**
	 * See also EFI_CONSOLE_RX_BRAIN_PIN
	 * offset 1980
	 */
	brain_pin_e binarySerialTxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1981
	 */
	uint8_t alignmentFill_at_1981[3];
	/**
	 * offset 1984
	 */
	brain_pin_e binarySerialRxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1985
	 */
	uint8_t alignmentFill_at_1985[3];
	/**
	 * offset 1988
	 */
	brain_pin_e auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 1990
	 */
	uint8_t alignmentFill_at_1990[2];
	/**
	 * offset 1992
	 */
	switch_input_pin_e tcuUpshiftButtonPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1993
	 */
	uint8_t alignmentFill_at_1993[3];
	/**
	 * offset 1996
	 */
	switch_input_pin_e tcuDownshiftButtonPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 1997
	 */
	uint8_t alignmentFill_at_1997[3];
	/**
	voltage
	 * offset 2000
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 2004
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	seconds
	 * offset 2008
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	RPM
	 * offset 2010
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * CLT-based target RPM for automatic idle controller
	C
	 * offset 2012
	 */
	float cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	RPM
	 * offset 2076
	 */
	scaled_channel<uint8_t, 1, 50> cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * This is the target battery voltage the alternator PID control will attempt to maintain
	Volts
	 * offset 2092
	 */
	float targetVBatt;
	/**
	 * Turns off alternator output above specified TPS, enabling this reduced parasitic drag on the engine at full load.
	%
	 * offset 2096
	 */
	float alternatorOffAboveTps;
	/**
	 * This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	cycles
	 * offset 2100
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	percent
	 * offset 2102
	 */
	int16_t iacByTpsTaper;
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_tx_pin X
	 * offset 2104
	 */
	brain_pin_e auxSerialTxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2105
	 */
	uint8_t alignmentFill_at_2105[3];
	/**
	 * offset 2108
	 */
	brain_pin_e warningLedPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2109
	 */
	uint8_t alignmentFill_at_2109[3];
	/**
	 * Auxiliary sensor serial, not to be confused with secondary calibration serial
	 * set_aux_rx_pin X
	 * offset 2112
	 */
	brain_pin_e auxSerialRxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2113
	 */
	uint8_t alignmentFill_at_2113[3];
	/**
	 * offset 2116
	 */
	brain_pin_e LIS302DLCsPin;
	/**
	 * How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	sec
	 * offset 2117
	 */
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	/**
	 * Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 2118
	 */
	uint8_t coastingFuelCutVssLow;
	/**
	 * Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	kph
	 * offset 2119
	 */
	uint8_t coastingFuelCutVssHigh;
	/**
	 * Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	sec
	 * offset 2120
	 */
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	/**
	 * need 4 byte alignment
	units
	 * offset 2121
	 */
	uint8_t alignmentFill_at_2121[3];
	/**
	 * Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	roc
	 * offset 2124
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * offset 2128
	 */
	brain_input_pin_e auxSpeedSensorInputPin[2];
	/**
	 * offset 2130
	 */
	uint8_t totalGearsCount;
	/**
	 * need 4 byte alignment
	units
	 * offset 2131
	 */
	uint8_t alignmentFill_at_2131[1];
	/**
	 * Sets what part of injection's is controlled by the injection phase table.
	 * offset 2132
	 */
	InjectionTimingMode injectionTimingMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2133
	 */
	uint8_t alignmentFill_at_2133[3];
	/**
	 * Band rate for primary TTL
	BPs
	 * offset 2136
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	roc
	 * offset 2140
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	coeff
	 * offset 2144
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	coeff
	 * offset 2148
	 */
	float slowAdcAlpha;
	/**
	 * See http://rusefi.com/s/debugmode
	 * 
	 * set debug_mode X
	 * offset 2152
	 */
	debug_mode_e debugMode;
	/**
	BPs
	 * offset 2156
	 */
	uint32_t auxSerialSpeed;
	/**
	voltage
	 * offset 2160
	 */
	float throttlePedalSecondaryUpVoltage;
	/**
	 * Pedal in the floor
	voltage
	 * offset 2164
	 */
	float throttlePedalSecondaryWOTVoltage;
	/**
	 * set can_baudrate
	 * offset 2168
	 */
	can_baudrate_e canBaudRate;
	/**
	 * need 4 byte alignment
	units
	 * offset 2169
	 */
	uint8_t alignmentFill_at_2169[3];
	/**
	 * Override the Y axis (load) value used for the VE table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2172
	 */
	ve_override_e veOverrideMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2173
	 */
	uint8_t alignmentFill_at_2173[3];
	/**
	 * offset 2176
	 */
	can_baudrate_e can2BaudRate;
	/**
	 * need 4 byte alignment
	units
	 * offset 2177
	 */
	uint8_t alignmentFill_at_2177[3];
	/**
	 * Override the Y axis (load) value used for the AFR table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2180
	 */
	load_override_e afrOverrideMode;
	/**
	A
	 * offset 2181
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	/**
	A
	 * offset 2182
	 */
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	/**
	 * How long to deactivate power when hold current is reached before applying power again
	us
	 * offset 2183
	 */
	uint8_t mc33_hpfp_i_hold_off;
	/**
	 * Maximum amount of time the solenoid can be active before assuming a programming error
	ms
	 * offset 2184
	 */
	uint8_t mc33_hpfp_max_hold;
	/**
	 * need 4 byte alignment
	units
	 * offset 2185
	 */
	uint8_t alignmentFill_at_2185[3];
	/**
	offset 2188 bit 0 */
	bool unused1740b0 : 1 {};
	/**
	offset 2188 bit 1 */
	bool unused1740b1 : 1 {};
	/**
	offset 2188 bit 2 */
	bool unused1740b2 : 1 {};
	/**
	 * Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	offset 2188 bit 3 */
	bool stepperDcInvertedPins : 1 {};
	/**
	offset 2188 bit 4 */
	bool unused1127 : 1 {};
	/**
	offset 2188 bit 5 */
	bool unused1128 : 1 {};
	/**
	offset 2188 bit 6 */
	bool unused1129 : 1 {};
	/**
	offset 2188 bit 7 */
	bool unused1130 : 1 {};
	/**
	offset 2188 bit 8 */
	bool unusedBit_648_8 : 1 {};
	/**
	offset 2188 bit 9 */
	bool unusedBit_648_9 : 1 {};
	/**
	offset 2188 bit 10 */
	bool unusedBit_648_10 : 1 {};
	/**
	offset 2188 bit 11 */
	bool unusedBit_648_11 : 1 {};
	/**
	offset 2188 bit 12 */
	bool unusedBit_648_12 : 1 {};
	/**
	offset 2188 bit 13 */
	bool unusedBit_648_13 : 1 {};
	/**
	offset 2188 bit 14 */
	bool unusedBit_648_14 : 1 {};
	/**
	offset 2188 bit 15 */
	bool unusedBit_648_15 : 1 {};
	/**
	offset 2188 bit 16 */
	bool unusedBit_648_16 : 1 {};
	/**
	offset 2188 bit 17 */
	bool unusedBit_648_17 : 1 {};
	/**
	offset 2188 bit 18 */
	bool unusedBit_648_18 : 1 {};
	/**
	offset 2188 bit 19 */
	bool unusedBit_648_19 : 1 {};
	/**
	offset 2188 bit 20 */
	bool unusedBit_648_20 : 1 {};
	/**
	offset 2188 bit 21 */
	bool unusedBit_648_21 : 1 {};
	/**
	offset 2188 bit 22 */
	bool unusedBit_648_22 : 1 {};
	/**
	offset 2188 bit 23 */
	bool unusedBit_648_23 : 1 {};
	/**
	offset 2188 bit 24 */
	bool unusedBit_648_24 : 1 {};
	/**
	offset 2188 bit 25 */
	bool unusedBit_648_25 : 1 {};
	/**
	offset 2188 bit 26 */
	bool unusedBit_648_26 : 1 {};
	/**
	offset 2188 bit 27 */
	bool unusedBit_648_27 : 1 {};
	/**
	offset 2188 bit 28 */
	bool unusedBit_648_28 : 1 {};
	/**
	offset 2188 bit 29 */
	bool unusedBit_648_29 : 1 {};
	/**
	offset 2188 bit 30 */
	bool unusedBit_648_30 : 1 {};
	/**
	offset 2188 bit 31 */
	bool unusedBit_648_31 : 1 {};
	/**
	ms
	 * offset 2192
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOffTime;
	/**
	count
	 * offset 2193
	 */
	uint8_t benchTestCount;
	/**
	ms
	 * offset 2194
	 */
	scaled_channel<uint8_t, 1, 5> benchTestOnTime;
	/**
	 * need 4 byte alignment
	units
	 * offset 2195
	 */
	uint8_t alignmentFill_at_2195[1];
	/**
	 * offset 2196
	 */
	pin_input_mode_e launchActivatePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2197
	 */
	uint8_t alignmentFill_at_2197[3];
	/**
	 * set_can2_tx_pin X
	 * offset 2200
	 */
	brain_pin_e can2TxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2201
	 */
	uint8_t alignmentFill_at_2201[3];
	/**
	 * set_can2_rx_pin X
	 * offset 2204
	 */
	brain_pin_e can2RxPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2205
	 */
	uint8_t alignmentFill_at_2205[3];
	/**
	 * offset 2208
	 */
	pin_output_mode_e starterControlPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2209
	 */
	uint8_t alignmentFill_at_2209[3];
	/**
	 * offset 2212
	 */
	adc_channel_e wastegatePositionSensor;
	/**
	 * need 4 byte alignment
	units
	 * offset 2213
	 */
	uint8_t alignmentFill_at_2213[3];
	/**
	 * Override the Y axis (load) value used for the ignition table.
	 * Advanced users only: If you aren't sure you need this, you probably don't need this.
	 * offset 2216
	 */
	load_override_e ignOverrideMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2217
	 */
	uint8_t alignmentFill_at_2217[3];
	/**
	 * Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	 * offset 2220
	 */
	injector_pressure_type_e injectorPressureType;
	/**
	 * need 4 byte alignment
	units
	 * offset 2221
	 */
	uint8_t alignmentFill_at_2221[3];
	/**
	 * offset 2224
	 */
	output_pin_e hpfpValvePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2225
	 */
	uint8_t alignmentFill_at_2225[3];
	/**
	 * offset 2228
	 */
	pin_output_mode_e hpfpValvePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2229
	 */
	uint8_t alignmentFill_at_2229[3];
	/**
	 * MAP value above which fuel is cut in case of overboost.
	 * Set to 0 to disable overboost cut.
	kPa (absolute)
	 * offset 2232
	 */
	float boostCutPressure;
	/**
	kg/h
	 * offset 2236
	 */
	uint8_t tchargeBins[16];
	/**
	ratio
	 * offset 2252
	 */
	uint8_t tchargeValues[16];
	/**
	counter
	 * offset 2268
	 */
	float unusedMapAccelTaperBins[8];
	/**
	 * Fixed timing, useful for TDC testing
	deg
	 * offset 2300
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	v
	 * offset 2304
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	v
	 * offset 2308
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	value
	 * offset 2312
	 */
	float egoValueShift;
	/**
	 * offset 2316
	 */
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	/**
	 * need 4 byte alignment
	units
	 * offset 2317
	 */
	uint8_t alignmentFill_at_2317[3];
	/**
	 * offset 2320
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 2321
	 */
	uint8_t alignmentFill_at_2321[3];
	/**
	 * offset 2324
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2325
	 */
	uint8_t alignmentFill_at_2325[3];
	/**
	 * offset 2328
	 */
	pin_output_mode_e sdCardCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2329
	 */
	uint8_t alignmentFill_at_2329[3];
	/**
	 * This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	percent
	 * offset 2332
	 */
	int crankingIACposition;
	/**
	 * offset 2336
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 2340
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 2344
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 2348
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 2352
	 */
	pwm_freq_t vvtOutputFrequency[CAMS_PER_BANK];
	/**
	 * Additional idle % when fan #1 is active
	%
	 * offset 2354
	 */
	uint8_t fan1ExtraIdle;
	/**
	 * need 4 byte alignment
	units
	 * offset 2355
	 */
	uint8_t alignmentFill_at_2355[1];
	/**
	Hz
	 * offset 2356
	 */
	int alternatorPwmFrequency;
	/**
	 * Narrow Band WBO Approximation
	V
	 * offset 2360
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	ratio
	 * offset 2392
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * set vvt_mode X
	 * offset 2424
	 */
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	/**
	 * offset 2425
	 */
	uint8_t vvtModePadding[CAMS_PER_BANK_padding];
	/**
	 * Additional idle % when fan #2 is active
	%
	 * offset 2426
	 */
	uint8_t fan2ExtraIdle;
	/**
	 * Delay to allow fuel pressure to build before firing the priming pulse.
	sec
	 * offset 2427
	 */
	scaled_channel<uint8_t, 100, 1> primingDelay;
	/**
	 * offset 2428
	 */
	adc_channel_e auxAnalogInputs[AUX_ANALOG_INPUT_COUNT];
	/**
	 * offset 2436
	 */
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	/**
	 * CLT-based timing correction
	C
	 * offset 2448
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	degree
	 * offset 2480
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2512
	 */
	tle8888_mode_e tle8888mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2513
	 */
	uint8_t alignmentFill_at_2513[3];
	/**
	 * offset 2516
	 */
	pin_output_mode_e LIS302DLCsPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2517
	 */
	uint8_t alignmentFill_at_2517[3];
	/**
	 * None = I have a MAP-referenced fuel pressure regulator
	 * Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	 * Sensed rail pressure = I have a fuel pressure sensor
	 * offset 2520
	 */
	injector_compensation_mode_e injectorCompensationMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2521
	 */
	uint8_t alignmentFill_at_2521[3];
	/**
	 * offset 2524
	 */
	pin_output_mode_e fan2PinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2525
	 */
	uint8_t alignmentFill_at_2525[3];
	/**
	 * This is the pressure at which your injector flow is known.
	 * For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	kPa
	 * offset 2528
	 */
	float fuelReferencePressure;
	/**
	 * Fuel multiplier (enrichment) immediately after engine start
	mult
	 * offset 2532
	 */
	float postCrankingFactor;
	/**
	 * Time over which to taper out after start enrichment
	seconds
	 * offset 2536
	 */
	float postCrankingDurationSec;
	/**
	 * offset 2540
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * offset 2572
	 */
	ThermistorConf auxTempSensor2;
	/**
	Deg
	 * offset 2604
	 */
	int16_t knockSamplingDuration;
	/**
	Hz
	 * offset 2606
	 */
	int16_t etbFreq;
	/**
	 * offset 2608
	 */
	pid_s etbWastegatePid;
	/**
	 * For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	 * offset 2628
	 */
	stepper_num_micro_steps_e stepperNumMicroSteps;
	/**
	 * Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	%
	 * offset 2629
	 */
	uint8_t stepperMinDutyCycle;
	/**
	 * Use to limit the max.current through the stepper motor (100% = no limit)
	%
	 * offset 2630
	 */
	uint8_t stepperMaxDutyCycle;
	/**
	 * need 4 byte alignment
	units
	 * offset 2631
	 */
	uint8_t alignmentFill_at_2631[1];
	/**
	 * offset 2632
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 2633
	 */
	uint8_t alignmentFill_at_2633[3];
	/**
	 * per-cylinder timing correction
	deg
	 * offset 2636
	 */
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	/**
	seconds
	 * offset 2684
	 */
	float idlePidActivationTime;
	/**
	 * offset 2688
	 */
	pin_mode_e spi1SckMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2689
	 */
	uint8_t alignmentFill_at_2689[3];
	/**
	 * Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	 * offset 2692
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2693
	 */
	uint8_t alignmentFill_at_2693[3];
	/**
	 * offset 2696
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2697
	 */
	uint8_t alignmentFill_at_2697[3];
	/**
	 * offset 2700
	 */
	pin_mode_e spi2SckMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2701
	 */
	uint8_t alignmentFill_at_2701[3];
	/**
	 * offset 2704
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2705
	 */
	uint8_t alignmentFill_at_2705[3];
	/**
	 * offset 2708
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2709
	 */
	uint8_t alignmentFill_at_2709[3];
	/**
	 * offset 2712
	 */
	pin_mode_e spi3SckMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2713
	 */
	uint8_t alignmentFill_at_2713[3];
	/**
	 * offset 2716
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2717
	 */
	uint8_t alignmentFill_at_2717[3];
	/**
	 * offset 2720
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2721
	 */
	uint8_t alignmentFill_at_2721[3];
	/**
	 * offset 2724
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2725
	 */
	uint8_t alignmentFill_at_2725[3];
	/**
	 * ResetB
	 * offset 2728
	 */
	brain_pin_e mc33816_rstb;
	/**
	 * need 4 byte alignment
	units
	 * offset 2729
	 */
	uint8_t alignmentFill_at_2729[3];
	/**
	 * offset 2732
	 */
	brain_pin_e mc33816_driven;
	/**
	 * need 4 byte alignment
	units
	 * offset 2733
	 */
	uint8_t alignmentFill_at_2733[3];
	/**
	 * Brake pedal switch
	 * offset 2736
	 */
	switch_input_pin_e brakePedalPin;
	/**
	 * need 4 byte alignment
	units
	 * offset 2737
	 */
	uint8_t alignmentFill_at_2737[3];
	/**
	 * lambda input
	 * offset 2740
	 */
	adc_channel_e cj125ua;
	/**
	 * need 4 byte alignment
	units
	 * offset 2741
	 */
	uint8_t alignmentFill_at_2741[3];
	/**
	 * heater input
	 * offset 2744
	 */
	adc_channel_e cj125ur;
	/**
	 * need 4 byte alignment
	units
	 * offset 2745
	 */
	uint8_t alignmentFill_at_2745[3];
	/**
	 * offset 2748
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 2749
	 */
	uint8_t alignmentFill_at_2749[3];
	/**
	 * offset 2752
	 */
	pid_s auxPid[CAMS_PER_BANK];
	/**
	 * offset 2772
	 */
	float injectorCorrectionPolynomial[8];
	/**
	C
	 * offset 2804
	 */
	int8_t primeBins[8];
	/**
	 * offset 2812
	 */
	linear_sensor_s oilPressure;
	/**
	 * offset 2832
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 2833
	 */
	uint8_t alignmentFill_at_2833[3];
	/**
	 * offset 2836
	 */
	output_pin_e fan2Pin;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsius
	deg C
	 * offset 2837
	 */
	uint8_t fan2OnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsius
	deg C
	 * offset 2838
	 */
	uint8_t fan2OffTemperature;
	/**
	 * need 4 byte alignment
	units
	 * offset 2839
	 */
	uint8_t alignmentFill_at_2839[1];
	/**
	x
	 * offset 2840
	 */
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 2904
	 */
	float scriptCurve1[SCRIPT_CURVE_16];
	/**
	x
	 * offset 2968
	 */
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	/**
	y
	 * offset 3032
	 */
	float scriptCurve2[SCRIPT_CURVE_16];
	/**
	x
	 * offset 3096
	 */
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3128
	 */
	float scriptCurve3[SCRIPT_CURVE_8];
	/**
	x
	 * offset 3160
	 */
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3192
	 */
	float scriptCurve4[SCRIPT_CURVE_8];
	/**
	 * offset 3224
	 */
	brain_pin_e stepperEnablePin;
	/**
	 * need 4 byte alignment
	units
	 * offset 3225
	 */
	uint8_t alignmentFill_at_3225[3];
	/**
	 * offset 3228
	 */
	brain_pin_e tle8888_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 3229
	 */
	uint8_t alignmentFill_at_3229[3];
	/**
	 * offset 3232
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 3233
	 */
	uint8_t alignmentFill_at_3233[3];
	/**
	 * offset 3236
	 */
	brain_pin_e mc33816_cs;
	/**
	 * need 4 byte alignment
	units
	 * offset 3237
	 */
	uint8_t alignmentFill_at_3237[3];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	RPM
	 * offset 3240
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	deg
	 * offset 3256
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * todo: more comments
	 * offset 3272
	 */
	brain_pin_e servoOutputPins[SERVO_COUNT];
	/**
	 * This sets the RPM above which fuel cut is active.
	rpm
	 * offset 3280
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	rpm
	 * offset 3282
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	%
	 * offset 3284
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is disabled when the engine is cold.
	C
	 * offset 3286
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	%
	 * offset 3288
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * MAP value above which fuel injection is re-enabled.
	kPa
	 * offset 3290
	 */
	int16_t coastingFuelCutMap;
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	C
	 * offset 3292
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	%
	 * offset 3356
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 3420
	 */
	linear_sensor_s highPressureFuel;
	/**
	 * offset 3440
	 */
	linear_sensor_s lowPressureFuel;
	/**
	 * CLT-based target RPM for hard limit depending on CLT like on Lexus LFA
	C
	 * offset 3460
	 */
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 3464
	 */
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	/**
	 * offset 3472
	 */
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	/**
	x
	 * offset 3568
	 */
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3600
	 */
	float scriptCurve5[SCRIPT_CURVE_8];
	/**
	x
	 * offset 3632
	 */
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	/**
	y
	 * offset 3664
	 */
	float scriptCurve6[SCRIPT_CURVE_8];
	/**
	 * offset 3696
	 */
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	/**
	 * offset 3760
	 */
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	/**
	 * Heat transfer coefficient at zero flow.
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 3888
	 */
	float tChargeAirCoefMin;
	/**
	 * Heat transfer coefficient at high flow, as defined by "max air flow".
	 * 0 means the air charge is fully heated to the same temperature as CLT.
	 * 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	 * offset 3892
	 */
	float tChargeAirCoefMax;
	/**
	 * High flow point for heat transfer estimation.
	 * Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	kg/h
	 * offset 3896
	 */
	float tChargeAirFlowMax;
	/**
	 * Maximum allowed rate of increase allowed for the estimated charge temperature
	deg/sec
	 * offset 3900
	 */
	float tChargeAirIncrLimit;
	/**
	 * Maximum allowed rate of decrease allowed for the estimated charge temperature
	deg/sec
	 * offset 3904
	 */
	float tChargeAirDecrLimit;
	/**
	 * offset 3908
	 */
	tChargeMode_e tChargeMode;
	/**
	 * target TPS value, 0 to 100%
	 * TODO: use int8 data date once we template interpolation method
	target TPS position
	 * offset 3912
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
	ETB duty cycle bias
	 * offset 3944
	 */
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	/**
	 * offset 3976
	 */
	float hip9011Gain;
	/**
	 * iTerm min value
	 * offset 3980
	 */
	int16_t etb_iTermMin;
	/**
	 * iTerm max value
	 * offset 3982
	 */
	int16_t etb_iTermMax;
	/**
	 * See useIdleTimingPidControl
	 * offset 3984
	 */
	pid_s idleTimingPid;
	/**
	 * If the RPM closer to target than this value, disable timing correction to prevent oscillation
	RPM
	 * offset 4004
	 */
	int16_t idleTimingPidDeadZone;
	/**
	 * A delay in cycles between fuel-enrich. portions
	cycles
	 * offset 4006
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	coef
	 * offset 4008
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 4012
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 4013
	 */
	uint8_t alignmentFill_at_4013[3];
	/**
	 * offset 4016
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * need 4 byte alignment
	units
	 * offset 4017
	 */
	uint8_t alignmentFill_at_4017[1];
	/**
	 * iTerm min value
	 * offset 4018
	 */
	int16_t idlerpmpid_iTermMin;
	/**
	 * offset 4020
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	 * E0 = 14.7
	 * E10 = 14.1
	 * E85 = 9.9
	 * E100 = 9.0
	:1
	 * offset 4021
	 */
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	/**
	 * iTerm max value
	 * offset 4022
	 */
	int16_t idlerpmpid_iTermMax;
	/**
	 * This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	%
	 * offset 4024
	 */
	float etbIdleThrottleRange;
	/**
	 * Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	 * offset 4028
	 */
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	/**
	mg
	 * offset 4040
	 */
	scaled_channel<uint8_t, 1, 5> primeValues[8];
	/**
	 * Trigger comparator center point voltage
	V
	 * offset 4048
	 */
	uint8_t triggerCompCenterVolt;
	/**
	 * Trigger comparator hysteresis voltage (Min)
	V
	 * offset 4049
	 */
	uint8_t triggerCompHystMin;
	/**
	 * Trigger comparator hysteresis voltage (Max)
	V
	 * offset 4050
	 */
	uint8_t triggerCompHystMax;
	/**
	 * VR-sensor saturation RPM
	RPM
	 * offset 4051
	 */
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	/**
	 * offset 4052
	 */
	pid_s idleRpmPid2;
	/**
	%
	 * offset 4072
	 */
	uint8_t iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	/**
	Load
	 * offset 4136
	 */
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	/**
	RPM
	 * offset 4144
	 */
	scaled_channel<uint8_t, 1, 50> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	/**
	 * set can_vss X
	 * offset 4152
	 */
	can_vss_nbc_e canVssNbcType;
	/**
	 * offset 4156
	 */
	gppwm_channel gppwm[GPPWM_CHANNELS];
	/**
	mA
	 * offset 4524
	 */
	uint16_t mc33_i_boost;
	/**
	mA
	 * offset 4526
	 */
	uint16_t mc33_i_peak;
	/**
	mA
	 * offset 4528
	 */
	uint16_t mc33_i_hold;
	/**
	us
	 * offset 4530
	 */
	uint16_t mc33_t_max_boost;
	/**
	us
	 * offset 4532
	 */
	uint16_t mc33_t_peak_off;
	/**
	us
	 * offset 4534
	 */
	uint16_t mc33_t_peak_tot;
	/**
	us
	 * offset 4536
	 */
	uint16_t mc33_t_bypass;
	/**
	us
	 * offset 4538
	 */
	uint16_t mc33_t_hold_off;
	/**
	us
	 * offset 4540
	 */
	uint16_t mc33_t_hold_tot;
	/**
	 * need 4 byte alignment
	units
	 * offset 4542
	 */
	uint8_t alignmentFill_at_4542[2];
	/**
	 * offset 4544
	 */
	pin_input_mode_e tcuUpshiftButtonPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4545
	 */
	uint8_t alignmentFill_at_4545[3];
	/**
	 * offset 4548
	 */
	pin_input_mode_e tcuDownshiftButtonPinMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4549
	 */
	uint8_t alignmentFill_at_4549[3];
	/**
	 * offset 4552
	 */
	pin_input_mode_e acSwitchMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 4553
	 */
	uint8_t alignmentFill_at_4553[3];
	/**
	 * offset 4556
	 */
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	/**
	 * Knock sensor output knock detection threshold depending on current RPM.
	dB
	 * offset 4562
	 */
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * need 4 byte alignment
	units
	 * offset 4578
	 */
	uint8_t alignmentFill_at_4578[2];
	/**
	from
	 * offset 4580
	 */
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	/**
	Percent
	 * offset 4652
	 */
	int8_t fuelTrim[MAX_CYLINDER_COUNT];
	/**
	to
	 * offset 4664
	 */
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	/**
	lobes/cam
	 * offset 4736
	 */
	uint8_t hpfpCamLobes;
	/**
	 * need 4 byte alignment
	units
	 * offset 4737
	 */
	uint8_t alignmentFill_at_4737[3];
	/**
	 * offset 4740
	 */
	hpfp_cam_e hpfpCam;
	/**
	 * Crank angle ATDC of first lobe peak
	deg
	 * offset 4741
	 */
	uint8_t hpfpPeakPos;
	/**
	 * If the requested activation time is below this angle, don't bother running the pump
	deg
	 * offset 4742
	 */
	uint8_t hpfpMinAngle;
	/**
	 * need 4 byte alignment
	units
	 * offset 4743
	 */
	uint8_t alignmentFill_at_4743[1];
	/**
	 * Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	cc
	 * offset 4744
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	/**
	 * How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	deg
	 * offset 4746
	 */
	uint8_t hpfpActivationAngle;
	/**
	 * offset 4747
	 */
	uint8_t issFilterReciprocal;
	/**
	%/kPa
	 * offset 4748
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	/**
	%/kPa/lobe
	 * offset 4750
	 */
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	/**
	 * The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	kPa/s
	 * offset 4752
	 */
	uint16_t hpfpTargetDecay;
	/**
	%
	 * offset 4754
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	/**
	deg
	 * offset 4770
	 */
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	/**
	volts
	 * offset 4786
	 */
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	/**
	ms
	 * offset 4794
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	/**
	kPa
	 * offset 4810
	 */
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	/**
	load
	 * offset 5010
	 */
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	/**
	RPM
	 * offset 5030
	 */
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	/**
	%
	 * offset 5040
	 */
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	/**
	cc/lobe
	 * offset 5140
	 */
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	/**
	RPM
	 * offset 5160
	 */
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	/**
	TPS
	 * offset 5170
	 */
	uint8_t unusedTpsBins[24];
	/**
	 * need 4 byte alignment
	units
	 * offset 5194
	 */
	uint8_t alignmentFill_at_5194[2];
	/**
	 * offset 5196
	 */
	output_pin_e stepper_raw_output[4];
	/**
	ratio
	 * offset 5200
	 */
	scaled_channel<uint16_t, 100, 1> gearRatio[GEARS_COUNT];
	/**
	 * We need to give engine time to build oil pressure without diverting it to VVT
	ms
	 * offset 5216
	 */
	uint16_t vvtActivationDelayMs;
	/**
	RPM
	 * offset 5218
	 */
	uint16_t unusedShort;
	/**
	deg C
	 * offset 5220
	 */
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	/**
	 * offset 5228
	 */
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	/**
	 * offset 5236
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	/**
	kPa
	 * offset 5244
	 */
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	/**
	 * offset 5252
	 */
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	/**
	 * offset 5260
	 */
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	/**
	Nm
	 * offset 5268
	 */
	scaled_channel<uint8_t, 1, 10> torqueTable[TORQUE_CURVE_SIZE][TORQUE_CURVE_SIZE];
	/**
	RPM
	 * offset 5304
	 */
	uint16_t torqueRpmBins[TORQUE_CURVE_SIZE];
	/**
	Load
	 * offset 5316
	 */
	uint16_t torqueLoadBins[TORQUE_CURVE_SIZE];
	/**
	 * offset 5328
	 */
	GearControllerMode gearControllerMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5329
	 */
	uint8_t alignmentFill_at_5329[3];
	/**
	 * offset 5332
	 */
	TransmissionControllerMode transmissionControllerMode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5333
	 */
	uint8_t alignmentFill_at_5333[3];
	/**
	 * offset 5336
	 */
	linear_sensor_s auxLinear1;
	/**
	 * offset 5356
	 */
	linear_sensor_s auxLinear2;
	/**
	 * offset 5376
	 */
	output_pin_e tcu_tcc_onoff_solenoid;
	/**
	 * need 4 byte alignment
	units
	 * offset 5377
	 */
	uint8_t alignmentFill_at_5377[3];
	/**
	 * offset 5380
	 */
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5381
	 */
	uint8_t alignmentFill_at_5381[3];
	/**
	 * offset 5384
	 */
	output_pin_e tcu_tcc_pwm_solenoid;
	/**
	 * need 4 byte alignment
	units
	 * offset 5385
	 */
	uint8_t alignmentFill_at_5385[3];
	/**
	 * offset 5388
	 */
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5389
	 */
	uint8_t alignmentFill_at_5389[3];
	/**
	 * offset 5392
	 */
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	/**
	 * need 4 byte alignment
	units
	 * offset 5394
	 */
	uint8_t alignmentFill_at_5394[2];
	/**
	 * offset 5396
	 */
	output_pin_e tcu_pc_solenoid_pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 5397
	 */
	uint8_t alignmentFill_at_5397[3];
	/**
	 * offset 5400
	 */
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5401
	 */
	uint8_t alignmentFill_at_5401[3];
	/**
	 * offset 5404
	 */
	pwm_freq_t tcu_pc_solenoid_freq;
	/**
	 * need 4 byte alignment
	units
	 * offset 5406
	 */
	uint8_t alignmentFill_at_5406[2];
	/**
	 * offset 5408
	 */
	output_pin_e tcu_32_solenoid_pin;
	/**
	 * need 4 byte alignment
	units
	 * offset 5409
	 */
	uint8_t alignmentFill_at_5409[3];
	/**
	 * offset 5412
	 */
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	/**
	 * need 4 byte alignment
	units
	 * offset 5413
	 */
	uint8_t alignmentFill_at_5413[3];
	/**
	 * offset 5416
	 */
	pwm_freq_t tcu_32_solenoid_freq;
	/**
	units
	 * offset 5418
	 */
	uint8_t mainUnusedEnd[222];
};
static_assert(sizeof(engine_configuration_s) == 5640);

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
	 * offset 5640
	 */
	error_message_t warning_message;
	/**
	C
	 * offset 5760
	 */
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	Seconds
	 * offset 5792
	 */
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	/**
	%
	 * offset 5824
	 */
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	/**
	Seconds
	 * offset 5856
	 */
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	/**
	 * offset 5888
	 */
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	RPM
	 * offset 5952
	 */
	scaled_channel<uint8_t, 1, 50> boostRpmBins[BOOST_RPM_COUNT];
	/**
	 * offset 5960
	 */
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	/**
	%
	 * offset 6024
	 */
	scaled_channel<uint8_t, 1, 1> boostTpsBins[BOOST_LOAD_COUNT];
	/**
	%
	 * offset 6032
	 */
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	/**
	%
	 * offset 6096
	 */
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	/**
	RPM
	 * offset 6104
	 */
	scaled_channel<uint8_t, 1, 50> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	C
	 * offset 6112
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	%
	 * offset 6144
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	RPM
	 * offset 6176
	 */
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	deg
	 * offset 6184
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	RPM
	 * offset 6216
	 */
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	/**
	load
	 * offset 6220
	 */
	scaled_channel<uint8_t, 1, 1> idleVeLoadBins[IDLE_VE_SIZE];
	/**
	%
	 * offset 6224
	 */
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	/**
	 * offset 6256
	 */
	lua_script_t luaScript;
	/**
	C
	 * offset 14256
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	ratio
	 * offset 14320
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	C
	 * offset 14384
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 14448
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	ratio
	 * offset 14512
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	C
	 * offset 14544
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	ratio
	 * offset 14576
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	counter
	 * offset 14608
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	C
	 * offset 14640
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	ratio
	 * offset 14704
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * Also known as MAF transfer function.
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft3/m
	kg/hour
	 * offset 14768
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	V
	 * offset 15792
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	deg
	 * offset 16816
	 */
	scaled_channel<int16_t, 10, 1> ignitionIatCorrTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Temperature
	 * offset 17328
	 */
	float ignitionIatCorrLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 17392
	 */
	uint16_t ignitionIatCorrRpmBins[IGN_RPM_COUNT];
	/**
	deg
	 * offset 17424
	 */
	int16_t injectionPhase[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 17936
	 */
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 17968
	 */
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	onoff
	 * offset 18000
	 */
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	/**
	 * offset 18060
	 */
	float unused17440;
	/**
	kPa
	 * offset 18064
	 */
	uint16_t mapEstimateTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	% TPS
	 * offset 18576
	 */
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 18608
	 */
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 18640
	 */
	uint8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 18704
	 */
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 18720
	 */
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 18736
	 */
	uint8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 18800
	 */
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 18816
	 */
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	/**
	deg
	 * offset 18832
	 */
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	/**
	Load
	 * offset 19344
	 */
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	RPM
	 * offset 19376
	 */
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	/**
	%
	 * offset 19408
	 */
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	kPa
	 * offset 19920
	 */
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 19952
	 */
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	/**
	lambda
	 * offset 19984
	 */
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	/**
	 * offset 20240
	 */
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	/**
	RPM
	 * offset 20272
	 */
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	/**
	value
	 * offset 20304
	 */
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	/**
	from
	 * offset 20560
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	to
	 * offset 20592
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	value
	 * offset 20624
	 */
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20880
	 */
	uint16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20896
	 */
	uint16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 20912
	 */
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 20976
	 */
	uint16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 20992
	 */
	uint16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 21008
	 */
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 21072
	 */
	uint16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 21088
	 */
	uint16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	/**
	value
	 * offset 21104
	 */
	uint8_t scriptTable4[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	/**
	L
	 * offset 21168
	 */
	uint16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	/**
	RPM
	 * offset 21184
	 */
	uint16_t scriptTable4RpmBins[SCRIPT_TABLE_8];
	/**
	 * offset 21200
	 */
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 21208
	 */
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 21216
	 */
	cyl_trim_s ignTrims[12];
	/**
	 * offset 21408
	 */
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	/**
	rpm
	 * offset 21416
	 */
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	/**
	 * offset 21424
	 */
	cyl_trim_s fuelTrims[12];
	/**
	ratio
	 * offset 21616
	 */
	uint16_t crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	/**
	Airmass
	 * offset 21632
	 */
	uint8_t tcu_pcAirmassBins[8];
	/**
	%
	 * offset 21640
	 */
	uint8_t tcu_pcValsR[8];
	/**
	%
	 * offset 21648
	 */
	uint8_t tcu_pcValsN[8];
	/**
	%
	 * offset 21656
	 */
	uint8_t tcu_pcVals1[8];
	/**
	%
	 * offset 21664
	 */
	uint8_t tcu_pcVals2[8];
	/**
	%
	 * offset 21672
	 */
	uint8_t tcu_pcVals3[8];
	/**
	%
	 * offset 21680
	 */
	uint8_t tcu_pcVals4[8];
	/**
	%
	 * offset 21688
	 */
	uint8_t tcu_pcVals12[8];
	/**
	%
	 * offset 21696
	 */
	uint8_t tcu_pcVals23[8];
	/**
	%
	 * offset 21704
	 */
	uint8_t tcu_pcVals34[8];
	/**
	%
	 * offset 21712
	 */
	uint8_t tcu_pcVals21[8];
	/**
	%
	 * offset 21720
	 */
	uint8_t tcu_pcVals32[8];
	/**
	%
	 * offset 21728
	 */
	uint8_t tcu_pcVals43[8];
	/**
	TPS
	 * offset 21736
	 */
	uint8_t tcu_tccTpsBins[8];
	/**
	MPH
	 * offset 21744
	 */
	uint8_t tcu_tccLockSpeed[8];
	/**
	MPH
	 * offset 21752
	 */
	uint8_t tcu_tccUnlockSpeed[8];
	/**
	KPH
	 * offset 21760
	 */
	uint8_t tcu_32SpeedBins[8];
	/**
	%
	 * offset 21768
	 */
	uint8_t tcu_32Vals[8];
};
static_assert(sizeof(persistent_config_s) == 21776);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on kinetis_gen_config.bat integration/rusefi_config.txt Mon Apr 25 03:58:59 UTC 2022
