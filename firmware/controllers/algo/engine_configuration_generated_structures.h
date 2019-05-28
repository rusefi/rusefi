// this section was generated automatically by ConfigDefinition.jar based on integration\rusefi_config.txt Mon May 27 20:05:22 EDT 2019
// begin
#ifndef CONTROLLERS_ALGO_ENGINE_CONFIGURATION_GENERATED_STRUCTURES_H
#define CONTROLLERS_ALGO_ENGINE_CONFIGURATION_GENERATED_STRUCTURES_H
#include "rusefi_types.h"
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
	 * offset 14
	 */
	int16_t periodMs;
	/**
	 * Output min value
	 * offset 16
	 */
	int16_t minValue;
	/**
	 * Output max value
	 * offset 18
	 */
	int16_t maxValue;
	/** total size 20*/
};

typedef struct pid_s pid_s;

// start of cranking_parameters_s
struct cranking_parameters_s {
	/**
	 * Fuel squirt duration while cranking
	 * A number of curves adjust this value according to CLT/IAT/TPS etc
	 * offset 0
	 */
	float baseFuel;
	/**
	 * Cranking mode threshold. Special cranking logic controls fuel and spark while RPM is below this threshold
	 * set cranking_rpm X
	 * offset 4
	 */
	int16_t rpm;
	/**
	 * need 4 byte alignment
	 * offset 6
	 */
	uint8_t alignmentFill[2];
	/** total size 8*/
};

typedef struct cranking_parameters_s cranking_parameters_s;

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
	 * offset 3
	 */
	uint8_t alignmentFill;
	/** total size 4*/
};

typedef struct spi_pins spi_pins;

// start of air_pressure_sensor_config_s
struct air_pressure_sensor_config_s {
	/**
	 * kPa value at low volts
	 * offset 0
	 */
	float lowValue;
	/**
	 * kPa value at high volts
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
	 * offset 13
	 */
	uint8_t align[3];
	/** total size 16*/
};

typedef struct air_pressure_sensor_config_s air_pressure_sensor_config_s;

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
	 * @brief MAP averaging sampling start angle, by RPM
	 * offset 32
	 */
	float samplingAngle[MAP_ANGLE_SIZE];
	/**
	 * offset 64
	 */
	float samplingWindowBins[MAP_WINDOW_SIZE];
	/**
	 * @brief MAP averaging angle duration, by RPM
	 * offset 96
	 */
	float samplingWindow[MAP_WINDOW_SIZE];
	/**
	 * offset 128
	 */
	air_pressure_sensor_config_s sensor;
	/** total size 144*/
};

typedef struct MAP_sensor_config_s MAP_sensor_config_s;

/**
 * @brief Thermistor known values

*/
// start of thermistor_conf_s
struct thermistor_conf_s {
	/**
	 * these values are in Celcuus
	 * offset 0
	 */
	float tempC_1;
	/**
	 * offset 4
	 */
	float tempC_2;
	/**
	 * offset 8
	 */
	float tempC_3;
	/**
	 * offset 12
	 */
	float resistance_1;
	/**
	 * offset 16
	 */
	float resistance_2;
	/**
	 * offset 20
	 */
	float resistance_3;
	/**
	 * offset 24
	 */
	float bias_resistor;
	/** total size 28*/
};

typedef struct thermistor_conf_s thermistor_conf_s;

/**
 * @brief Oil pressure sensor interpolation

*/
// start of oil_pressure_config_s
struct oil_pressure_config_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	 * offset 1
	 */
	uint8_t align[3];
	/**
	 * offset 4
	 */
	float v1;
	/**
	 * offset 8
	 */
	float value1;
	/**
	 * offset 12
	 */
	float v2;
	/**
	 * offset 16
	 */
	float value2;
	/** total size 20*/
};

typedef struct oil_pressure_config_s oil_pressure_config_s;

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
	 * offset 29
	 */
	uint8_t alignmentFill[3];
	/** total size 32*/
};

typedef struct ThermistorConf ThermistorConf;

// start of injector_s
struct injector_s {
	/**
	 * cc/min, cubic centimeter per minute
	 * By the way, g/s = 0.125997881 * (lb/hr)
	 * g/s = 0.125997881 * (cc/min)/10.5
	 * g/s = 0.0119997981 * cc/min
	 * offset 0
	 */
	float flow;
	/**
	 * set_flat_injector_lag LAG
	 * set_injector_lag VOLTAGE LAG
	 * offset 4
	 */
	float battLagCorrBins[VBAT_INJECTOR_CURVE_SIZE];
	/**
	 * ms delay between injector open and close dead times
	 * offset 36
	 */
	float battLagCorr[VBAT_INJECTOR_CURVE_SIZE];
	/** total size 68*/
};

typedef struct injector_s injector_s;

// start of bi_quard_s
struct bi_quard_s {
	/**
	 * offset 0
	 */
	float a0;
	/**
	 * offset 4
	 */
	float a1;
	/**
	 * offset 8
	 */
	float a2;
	/**
	 * offset 12
	 */
	float b1;
	/**
	 * offset 16
	 */
	float b2;
	/** total size 20*/
};

typedef struct bi_quard_s bi_quard_s;

// start of specs_s
struct specs_s {
	/**
	 * Engine displacement, in liters
	 * see also cylindersCount
	 * offset 0
	 */
	float displacement;
	/**
	 * offset 4
	 */
	cylinders_count_t cylindersCount;
	/**
	 * offset 8
	 */
	firing_order_e firingOrder;
	/** total size 12*/
};

typedef struct specs_s specs_s;

/**
 * @brief Trigger wheel(s) configuration

*/
// start of trigger_config_s
struct trigger_config_s {
	/**
	 * set trigger_type X
	 * offset 0
	 */
	trigger_type_e type;
	/**
	offset 4 bit 0 */
	bool unusedCustomIsSynchronizationNeeded : 1;
	/**
	offset 4 bit 1 */
	bool unusedCustomNeedSecondTriggerInput : 1;
	/**
	 * This option could be used if your second trigger channel is broken
	offset 4 bit 2 */
	bool useOnlyFirstChannel : 1;
	/**
	 * offset 8
	 */
	int customTotalToothCount;
	/**
	 * offset 12
	 */
	int customSkippedToothCount;
	/** total size 16*/
};

typedef struct trigger_config_s trigger_config_s;

// start of afr_sensor_s
struct afr_sensor_s {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
	/**
	 * offset 1
	 */
	uint8_t alignAf[3];
	/**
	 * offset 4
	 */
	float v1;
	/**
	 * offset 8
	 */
	float value1;
	/**
	 * offset 12
	 */
	float v2;
	/**
	 * offset 16
	 */
	float value2;
	/** total size 20*/
};

typedef struct afr_sensor_s afr_sensor_s;

// start of idle_hardware_s
struct idle_hardware_s {
	/**
	 * offset 0
	 */
	int solenoidFrequency;
	/**
	 * offset 4
	 */
	brain_pin_e solenoidPin;
	/**
	 * offset 5
	 */
	brain_pin_e stepperDirectionPin;
	/**
	 * offset 6
	 */
	brain_pin_e stepperStepPin;
	/**
	 * offset 7
	 */
	pin_output_mode_e solenoidPinMode;
	/** total size 8*/
};

typedef struct idle_hardware_s idle_hardware_s;

// start of etb_io
struct etb_io {
	/**
	 * offset 0
	 */
	brain_pin_e directionPin1;
	/**
	 * offset 1
	 */
	brain_pin_e directionPin2;
	/**
	 * offset 2
	 */
	brain_pin_e controlPin1;
	/**
	 * currently UNUSED
	 * offset 3
	 */
	brain_pin_e controlPin2;
	/** total size 4*/
};

typedef struct etb_io etb_io;

// start of board_configuration_s
struct board_configuration_s {
	/**
	 * offset 0
	 */
	idle_hardware_s idle;
	/**
	 * value between 0 and 100 used in Manual mode
	 * offset 8
	 */
	float manIdlePosition;
	/**
	 * offset 12
	 */
	float mapFrequency0Kpa;
	/**
	 * offset 16
	 */
	float mapFrequency100Kpa;
	/**
	 * See also triggerSimulatorPins
	 * See also directSelfStimulation
	 * rpm X
	 * offset 20
	 */
	int triggerSimulatorFrequency;
	/**
	 * offset 24
	 */
	brain_pin_e injectionPins[INJECTION_PIN_COUNT];
	/**
	 * offset 36
	 */
	brain_pin_e ignitionPins[IGNITION_PIN_COUNT];
	/**
	 * offset 48
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 49
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 50
	 */
	brain_pin_e HD44780_rs;
	/**
	 * offset 51
	 */
	brain_pin_e HD44780_e;
	/**
	 * offset 52
	 */
	brain_pin_e HD44780_db4;
	/**
	 * offset 53
	 */
	brain_pin_e HD44780_db5;
	/**
	 * offset 54
	 */
	brain_pin_e HD44780_db6;
	/**
	 * offset 55
	 */
	brain_pin_e HD44780_db7;
	/**
	 * offset 56
	 */
	brain_pin_e gps_rx_pin;
	/**
	 * offset 57
	 */
	brain_pin_e gps_tx_pin;
	/**
	 * offset 58
	 */
	smart_pin_e fuelPumpPin;
	/**
	 * offset 59
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * offset 60
	 */
	smart_pin_e malfunctionIndicatorPin;
	/**
	 * offset 61
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 62
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 63
	 */
	smart_pin_e fanPin;
	/**
	 * some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 64
	 */
	brain_pin_e clutchDownPin;
	/**
	 * offset 65
	 */
	smart_pin_e alternatorControlPin;
	/**
	 * offset 66
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 67
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 68
	 */
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 72
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 73
	 */
	brain_pin_e wboHeaterPin;
	/**
	 * offset 74
	 */
	brain_pin_e cj125CsPin;
	/**
	 * offset 75
	 */
	uint8_t unusedEtbM;
	/**
	 * offset 76
	 */
	uint8_t unusedSpiPadding9;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 77
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 78
	 */
	brain_pin_e mc33972_cs;
	/**
	 * offset 79
	 */
	pin_output_mode_e mc33972_csPinMode;
	/**
	 * offset 80
	 */
	etb_io etb1;
	/**
	 * offset 84
	 */
	float fuelLevelEmptyTankVoltage;
	/**
	 * offset 88
	 */
	float fuelLevelFullTankVoltage;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 92
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 96
	 */
	float fuelClosedLoopAfrLowThreshold;
	/**
	 * offset 100
	 */
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	/**
	 * offset 103
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * This implementation produces one pulse per engine cycle. See also dizzySparkOutputPin.
	 * offset 104
	 */
	brain_pin_e tachOutputPin;
	/**
	 * offset 105
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * offset 106
	 */
	smart_pin_e mainRelayPin;
	/**
	 * offset 107
	 */
	brain_pin_e sdCardCsPin;
	/**
	 * offset 108
	 */
	brain_pin_e canTxPin;
	/**
	 * offset 109
	 */
	brain_pin_e canRxPin;
	/**
	 * offset 110
	 */
	brain_pin_e boardTestModeJumperPin;
	/**
	 * offset 111
	 */
	uint8_t unusedMa2;
	/**
	 * offset 112
	 */
	int idleThreadPeriodMs;
	/**
	 * offset 116
	 */
	int consoleLoopPeriodMs;
	/**
	 * offset 120
	 */
	int lcdThreadPeriodMs;
	/**
	 * offset 124
	 */
	int generalPeriodicThreadPeriodMs;
	/**
	 * offset 128
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 132
	 */
	can_device_mode_e canDeviceMode;
	/**
	 * See also directSelfStimulation
	 * offset 136
	 */
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 139
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * Narrow band o2 heater, not used for CJ125. See wboHeaterPin
	 * offset 142
	 */
	brain_pin_e o2heaterPin;
	/**
	 * offset 143
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	offset 144 bit 0 */
	bool is_enabled_spi_1 : 1;
	/**
	offset 144 bit 1 */
	bool is_enabled_spi_2 : 1;
	/**
	offset 144 bit 2 */
	bool is_enabled_spi_3 : 1;
	/**
	offset 144 bit 3 */
	bool isSdCardEnabled : 1;
	/**
	offset 144 bit 4 */
	bool isFastAdcEnabled : 1;
	/**
	offset 144 bit 5 */
	bool isEngineControlEnabled : 1;
	/**
	offset 144 bit 6 */
	bool isHip9011Enabled : 1;
	/**
	offset 144 bit 7 */
	bool isVerboseAlternator : 1;
	/**
	offset 144 bit 8 */
	bool useSerialPort : 1;
	/**
	offset 144 bit 9 */
	bool useStepperIdle : 1;
	/**
	offset 144 bit 10 */
	bool enabledStep1Limiter : 1;
	/**
	offset 144 bit 11 */
	bool useTpicAdvancedMode : 1;
	/**
	offset 144 bit 12 */
	bool useLcdScreen : 1;
	/**
	offset 144 bit 13 */
	bool unusedAnotherOne : 1;
	/**
	offset 144 bit 14 */
	bool useWarmupPidAfr : 1;
	/**
	offset 144 bit 15 */
	bool onOffAlternatorLogic : 1;
	/**
	offset 144 bit 16 */
	bool isCJ125Enabled : 1;
	/**
	 * Use rise or fall signal front
	offset 144 bit 17 */
	bool vvtCamSensorUseRise : 1;
	/**
	 * Useful for individual intakes
	offset 144 bit 18 */
	bool measureMapOnlyInOneCylinder : 1;
	/**
	offset 144 bit 19 */
	bool stepperForceParkingEveryRestart : 1;
	/**
	 * Smarter cranking logic.
	 * See also startOfCrankingPrimingPulse
	offset 144 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1;
	/**
	offset 144 bit 21 */
	bool coastingFuelCutEnabled : 1;
	/**
	offset 144 bit 22 */
	bool useIacTableForCoasting : 1;
	/**
	offset 144 bit 23 */
	bool useNoiselessTriggerDecoder : 1;
	/**
	offset 144 bit 24 */
	bool useIdleTimingPidControl : 1;
	/**
	offset 144 bit 25 */
	bool useTPSBasedVeTable : 1;
	/**
	offset 144 bit 26 */
	bool is_enabled_spi_4 : 1;
	/**
	offset 144 bit 27 */
	bool pauseEtbControl : 1;
	/**
	offset 144 bit 28 */
	bool alignEngineSnifferAtTDC : 1;
	/**
	offset 144 bit 29 */
	bool useETBforIdleControl : 1;
	/**
	offset 144 bit 30 */
	bool idleIncrementalPidCic : 1;
	/**
	offset 144 bit 31 */
	bool unused_board_984_31 : 1;
	/**
	 * offset 148
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 152
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * offset 153
	 */
	brain_pin_e hip9011CsPin;
	/**
	 * offset 154
	 */
	brain_pin_e hip9011IntHoldPin;
	/**
	 * offset 155
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * default or inverted input
	 * offset 156
	 */
	uint8_t logicAnalyzerMode[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 160
	 */
	int unrealisticRpmThreashold;
	/**
	 * offset 164
	 */
	pin_output_mode_e gpioPinModes[FSIO_COMMAND_COUNT];
	/**
	 * todo: more comments
	 * offset 180
	 */
	smart_pin_e fsioOutputPins[FSIO_COMMAND_COUNT];
	/**
	 * offset 196
	 */
	brain_pin_e max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * offset 204
	 */
	spi_device_e max31855spiDevice;
	/**
	 * offset 205
	 */
	uint8_t unusedSpiPadding2[3];
	/**
	 * offset 208
	 */
	uart_device_e consoleUartDevice;
	/**
	 * offset 209
	 */
	uint8_t unuseduartPadding1[3];
	/**
	 * offset 212
	 */
	int mapMinBufferLength;
	/**
	 * offset 216
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * offset 218
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * offset 220
	 */
	float nb2ratioFrom;
	/**
	 * offset 224
	 */
	float nb2ratioTo;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 228
	 */
	brain_pin_e triggerErrorPin;
	/**
	 * offset 229
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * offset 230
	 */
	smart_pin_e acRelayPin;
	/**
	 * offset 231
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * offset 232
	 */
	fsio_pwm_freq_t fsioFrequency[FSIO_COMMAND_COUNT];
	/**
	 * offset 264
	 */
	fsio_setting_t fsio_setting[FSIO_COMMAND_COUNT];
	/**
	 * offset 328
	 */
	brain_pin_e spi1mosiPin;
	/**
	 * offset 329
	 */
	brain_pin_e spi1misoPin;
	/**
	 * offset 330
	 */
	brain_pin_e spi1sckPin;
	/**
	 * offset 331
	 */
	brain_pin_e spi2mosiPin;
	/**
	 * offset 332
	 */
	brain_pin_e spi2misoPin;
	/**
	 * offset 333
	 */
	brain_pin_e spi2sckPin;
	/**
	 * offset 334
	 */
	brain_pin_e spi3mosiPin;
	/**
	 * offset 335
	 */
	brain_pin_e spi3misoPin;
	/**
	 * offset 336
	 */
	brain_pin_e spi3sckPin;
	/**
	 * Saab Combustion Detection Module knock signal input pin
	 * also known as Saab Ion Sensing Module
	 * offset 337
	 */
	brain_pin_e cdmInputPin;
	/**
	 * offset 338
	 */
	brain_pin_e joystickCenterPin;
	/**
	 * offset 339
	 */
	brain_pin_e joystickAPin;
	/**
	 * offset 340
	 */
	brain_pin_e joystickBPin;
	/**
	 * offset 341
	 */
	brain_pin_e joystickCPin;
	/**
	 * offset 342
	 */
	brain_pin_e joystickDPin;
	/**
	 * offset 343
	 */
	uint8_t unusedjoy;
	/**
	 * rusEfi console Sensor Sniffer mode
	 * offset 344
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 348
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * todo:not finshed
	 * These input pins allow us to pull toggle buttons state
	 * offset 352
	 */
	brain_pin_e fsioDigitalInputs[FSIO_COMMAND_COUNT];
	/**
	 * offset 368
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 369
	 */
	brain_pin_e clutchUpPin;
	/**
	 * offset 370
	 */
	brain_input_pin_e frequencyReportingMapInputPin;
	/**
	 * offset 371
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * offset 372
	 */
	float etbIdleRange;
	/**
	 * offset 376
	 */
	int unusedAtBoardConfigurationEnd[122];
	/** total size 864*/
};

typedef struct board_configuration_s board_configuration_s;

// start of engine_configuration_s
struct engine_configuration_s {
	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
	 * set engine_type X
	 * offset 0
	 */
	engine_type_e engineType;
	/**
	 * Engine sniffer would be disabled above this rpm
	 * set engineSnifferRpmThreshold X
	 * offset 4
	 */
	int engineSnifferRpmThreshold;
	/**
	 * offset 8
	 */
	injector_s injector;
	/**
	 * Should trigger emulator push data right into trigger handling logic, eliminating the need for physical jumper wires?
	 * See also triggerSimulatorPins
	 * PS: Funny name, right? :)
	offset 76 bit 0 */
	bool directSelfStimulation : 1;
	/**
	offset 76 bit 1 */
	bool activateAuxPid1 : 1;
	/**
	offset 76 bit 2 */
	bool isVerboseAuxPid1 : 1;
	/**
	offset 76 bit 3 */
	bool activateAuxPid2 : 1;
	/**
	offset 76 bit 4 */
	bool isVerboseAuxPid2 : 1;
	/**
	offset 76 bit 5 */
	bool activateAuxPid3 : 1;
	/**
	offset 76 bit 6 */
	bool isVerboseAuxPid3 : 1;
	/**
	offset 76 bit 7 */
	bool activateAuxPid4 : 1;
	/**
	offset 76 bit 8 */
	bool isVerboseAuxPid4 : 1;
	/**
	offset 76 bit 9 */
	bool useBiQuadAnalogFiltering : 1;
	/**
	offset 76 bit 10 */
	bool cj125isUaDivided : 1;
	/**
	offset 76 bit 11 */
	bool cj125isLsu49 : 1;
	/**
	offset 76 bit 12 */
	bool etb1_use_two_wires : 1;
	/**
	offset 76 bit 13 */
	bool etb2_use_two_wires : 1;
	/**
	offset 76 bit 14 */
	bool showSdCardWarning : 1;
	/**
	offset 76 bit 15 */
	bool issue_294_15 : 1;
	/**
	offset 76 bit 16 */
	bool issue_294_16 : 1;
	/**
	offset 76 bit 17 */
	bool issue_294_17 : 1;
	/**
	offset 76 bit 18 */
	bool issue_294_18 : 1;
	/**
	offset 76 bit 19 */
	bool issue_294_19 : 1;
	/**
	offset 76 bit 20 */
	bool issue_294_21 : 1;
	/**
	offset 76 bit 21 */
	bool issue_294_22 : 1;
	/**
	offset 76 bit 22 */
	bool issue_294_23 : 1;
	/**
	offset 76 bit 23 */
	bool issue_294_24 : 1;
	/**
	offset 76 bit 24 */
	bool issue_294_25 : 1;
	/**
	offset 76 bit 25 */
	bool issue_294_26 : 1;
	/**
	offset 76 bit 26 */
	bool issue_294_27 : 1;
	/**
	offset 76 bit 27 */
	bool issue_294_28 : 1;
	/**
	offset 76 bit 28 */
	bool issue_294_29 : 1;
	/**
	offset 76 bit 29 */
	bool issue_294_30 : 1;
	/**
	offset 76 bit 30 */
	bool issue_294_31 : 1;
	/**
	 * Closed throttle. todo: extract these two fields into a structure
	 * todo: we need two sets of TPS parameters - modern ETBs have two sensors
	 * See also tps1_1AdcChannel
	 * set tps_min X
	 * offset 80
	 */
	int16_t tpsMin;
	/**
	 * Full throtle. tpsMax value as 10 bit ADC value. Not Voltage!
	 * See also tps1_1AdcChannel
	 * set tps_max X
	 * offset 82
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection, what TPS % value is unrealistically low
	 * offset 84
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
	 * TPS error detection, what TPS % value is unrealistically high
	 * offset 86
	 */
	int16_t tpsErrorDetectionTooHigh;
	/**
	 * offset 88
	 */
	cranking_parameters_s cranking;
	/**
	 * offset 96
	 */
	float primingSquirtDurationMs;
	/**
	 * Used if useConstantDwellDuringCranking is TRUE
	 * offset 100
	 */
	float ignitionDwellForCrankingMs;
	/**
	 * While cranking (which causes battery voltage to drop) we can calculate dwell time in shaft
	 * degrees, not in absolute time as in running mode.
	 * set cranking_charge_angle X
	 * offset 104
	 */
	float crankingChargeAngle;
	/**
	 * @see hasMapSensor
	 * @see isMapAveragingEnabled
	 * offset 108
	 */
	MAP_sensor_config_s map;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 252
	 */
	ThermistorConf clt;
	/**
	 * offset 284
	 */
	ThermistorConf iat;
	/**
	 * A secondary Rev limit engaged by the driver to help launch the vehicle faster
	 * offset 316
	 */
	int step1rpm;
	/**
	 * offset 320
	 */
	int step1timing;
	/**
	 * value '6' for 8MHz hw osc
	 * read hip9011 datasheet for details
	 * todo split into two bit fields
	 * offset 324
	 */
	int hip9011PrescalerAndSDO;
	/**
	 * We calculate knock band based of cylinderBore
	 *  Use this to override - kHz knock band override
	 * offset 328
	 */
	float knockBandCustom;
	/**
	 * On single-coil or wasted spark setups you have to lower dwell at high RPM
	 * offset 332
	 */
	float sparkDwellRpmBins[DWELL_CURVE_SIZE];
	/**
	 * offset 364
	 */
	float sparkDwellValues[DWELL_CURVE_SIZE];
	/**
	 * offset 396
	 */
	specs_s specs;
	/**
	 * Cylinder diameter, in mm.
	 * offset 408
	 */
	float cylinderBore;
	/**
	 * Disable sensor sniffer above this rpm
	 * offset 412
	 */
	int sensorSnifferRpmThreshold;
	/**
	 * set rpm_hard_limit X
	 * offset 416
	 */
	int rpmHardLimit;
	/**
	 * This setting controls which fuel quantity control algorithm is used.
	 * See also useTPSAdvanceTable
	 * set algorithm X
	 * offset 420
	 */
	engine_load_mode_e fuelAlgorithm;
	/**
	 * offset 424
	 */
	injection_mode_e crankingInjectionMode;
	/**
	 * 'batched' means two injectors are wired together
	 * set injection_mode X
	 * See also twoWireBatchInjection
	 * offset 428
	 */
	injection_mode_e injectionMode;
	/**
	 * this is about deciding when the injector starts it's squirt
	 * See also injectionPhase map
	 * todo: do we need even need this since we have the map anyway?
	 * offset 432
	 */
	angle_t extraInjectionOffset;
	/**
	 * Timing advance while engine cranking
	 * set cranking_timing_angle X
	 * offset 436
	 */
	angle_t crankingTimingAngle;
	/**
	 * 'wasted' means one coil is driving two spark plugs in two cylinders, with one of the aparks not doing anything since it's happening on the exhaust cycle
	 * set ignition_mode X
	 * offset 440
	 */
	ignition_mode_e ignitionMode;
	/**
	 * this value could be used to offset the whole ignition timing table by a constant
	 * offset 444
	 */
	angle_t ignitionOffset;
	/**
	 * offset 448
	 */
	timing_mode_e timingMode;
	/**
	 * This value is used in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful for instance while adjusting distributor location
	 * offset 452
	 */
	angle_t fixedModeTiming;
	/**
	 * Angle between Top Dead Center (TDC) and the first trigger event.
	 * Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * set global_trigger_offset_angle X
	 * offset 456
	 */
	angle_t globalTriggerAngleOffset;
	/**
	 * Coefficient of input voltage dividers on your PCB
	 * offset 460
	 */
	float analogInputDividerCoefficient;
	/**
	 * Battery Voltage input resistor divider coefficient
	 * offset 464
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsuis
	 * offset 468
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsuis
	 * offset 472
	 */
	float fanOffTemperature;
	/**
	 * This coefficient translates vehicle speed input frequency (in Hz) into vehicle speed, km/h
	 * offset 476
	 */
	float vehicleSpeedCoef;
	/**
	 * set can_mode X
	 * offset 480
	 */
	can_nbc_e canNbcType;
	/**
	 * CANbus thread period, ms
	 * offset 484
	 */
	int canSleepPeriodMs;
	/**
	 * '4 stroke with symmetrical crank' is a pretty special case for example on Miata NB2
	 * See engineCycle
	 * set operation_mode X
	 * offset 488
	 */
	operation_mode_e operationMode;
	/**
	 * offset 492
	 */
	display_mode_e displayMode;
	/**
	 * offset 496
	 */
	log_format_e logFormat;
	/**
	 * offset 500
	 */
	int byFirmwareVersion;
	/**
	 * offset 504
	 */
	int HD44780width;
	/**
	 * offset 508
	 */
	int HD44780height;
	/**
	 * First TPS, single channel so far. See aslo pedalPositionAdcChannel
	 * offset 512
	 */
	adc_channel_e tps1_1AdcChannel;
	/**
	 * offset 513
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * offset 514
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * Second throttle body position sensor, single channel so far
	 * offset 515
	 */
	adc_channel_e tps2_1AdcChannel;
	/**
	 * offset 516
	 */
	int overrideCrankingIgnition;
	/**
	 * offset 520
	 */
	int sensorChartFrequency;
	/**
	 * offset 524
	 */
	trigger_config_s trigger;
	/**
	 * offset 540
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * offset 541
	 */
	adc_channel_e high_fuel_pressure_sensor_1;
	/**
	 * offset 542
	 */
	adc_channel_e high_fuel_pressure_sensor_2;
	/**
	 * offset 543
	 */
	uint8_t unusedSpiPadding1;
	/**
	 * set global_fuel_correction X
	 * offset 544
	 */
	float globalFuelCorrection;
	/**
	 * offset 548
	 */
	float adcVcc;
	/**
	 * maximum total number of degrees to subtract from ignition advance
	 * when knocking
	 * offset 552
	 */
	float maxKnockSubDeg;
	/**
	 * See hasMafSensor
	 * offset 556
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 557
	 */
	uint8_t unusedAlignMaf[3];
	/**
	 * offset 560
	 */
	afr_sensor_s afr;
	/**
	 * Electronic throttle pedal position input
	 * Single channel so far
	 * See also tps1_1AdcChannel
	 * offset 580
	 */
	adc_channel_e throttlePedalPositionAdcChannel;
	/**
	 * offset 581
	 */
	brain_pin_e tle6240_cs;
	/**
	 * offset 582
	 */
	pin_output_mode_e tle6240_csPinMode;
	/**
	 * offset 583
	 */
	uint8_t unusedt;
	/**
	 * @see hasBaroSensor
	 * offset 584
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 600
	 */
	board_configuration_s bc;
	/**
	offset 1464 bit 0 */
	bool vvtDisplayInverted : 1;
	/**
	offset 1464 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1;
	/**
	 * Print details into rusEfi console
	offset 1464 bit 2 */
	bool isVerboseIAC : 1;
	/**
	 * enable verbose_etb
	offset 1464 bit 3 */
	bool isVerboseETB : 1;
	/**
	 * If set to true, will use the specified duration for cranking dwell. If set to false, will use the specified dwell angle. Unless you have a really good reason to, leave this set to true to use duration mode.
	offset 1464 bit 4 */
	bool useConstantDwellDuringCranking : 1;
	/**
	 * This options enables data for 'engine sniffer' tab in console, which comes at some CPU price
	offset 1464 bit 5 */
	bool isEngineChartEnabled : 1;
	/**
	offset 1464 bit 6 */
	bool silentTriggerError : 1;
	/**
	offset 1464 bit 7 */
	bool useLinearCltSensor : 1;
	/**
	offset 1464 bit 8 */
	bool canReadEnabled : 1;
	/**
	offset 1464 bit 9 */
	bool canWriteEnabled : 1;
	/**
	offset 1464 bit 10 */
	bool useLinearIatSensor : 1;
	/**
	 * See fsioTimingAdjustment
	offset 1464 bit 11 */
	bool useFSIO16ForTimingAdjustment : 1;
	/**
	offset 1464 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1;
	/**
	offset 1464 bit 13 */
	bool isAlternatorControlEnabled : 1;
	/**
	offset 1464 bit 14 */
	bool invertPrimaryTriggerSignal : 1;
	/**
	offset 1464 bit 15 */
	bool invertSecondaryTriggerSignal : 1;
	/**
	offset 1464 bit 16 */
	bool cutFuelOnHardLimit : 1;
	/**
	offset 1464 bit 17 */
	bool cutSparkOnHardLimit : 1;
	/**
	offset 1464 bit 18 */
	bool step1fuelCutEnable : 1;
	/**
	offset 1464 bit 19 */
	bool step1SparkCutEnable : 1;
	/**
	offset 1464 bit 20 */
	bool hasFrequencyReportingMapSensor : 1;
	/**
	offset 1464 bit 21 */
	bool useFSIO8ForServo1 : 1;
	/**
	offset 1464 bit 22 */
	bool useFSIO9ForServo2 : 1;
	/**
	offset 1464 bit 23 */
	bool useFSIO10ForServo3 : 1;
	/**
	offset 1464 bit 24 */
	bool useFSIO11ForServo4 : 1;
	/**
	offset 1464 bit 25 */
	bool useFSIO12ForServo5 : 1;
	/**
	offset 1464 bit 26 */
	bool useFSIO15ForIdleRpmAdjustment : 1;
	/**
	 * Sometimes we just have to shut the engine down. Use carefully!
	offset 1464 bit 27 */
	bool useFSIO5ForCriticalIssueEngineStop : 1;
	/**
	 * Sometimes we have to miss injection on purpose to attract driver's attention
	offset 1464 bit 28 */
	bool useFSIO4ForSeriousEngineWarning : 1;
	/**
	offset 1464 bit 29 */
	bool unused_bit_1472_29 : 1;
	/**
	offset 1464 bit 30 */
	bool unused_bit_1472_30 : 1;
	/**
	 * offset 1468
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * offset 1469
	 */
	uint8_t unusedh[3];
	/**
	 * See also idleRpmPid
	 * offset 1472
	 */
	idle_mode_e idleMode;
	/**
	 * enable injection
	offset 1476 bit 0 */
	bool isInjectionEnabled : 1;
	/**
	 * enable ignition
	offset 1476 bit 1 */
	bool isIgnitionEnabled : 1;
	/**
	 * If TPS above 95% no fuel would be injected during cranking
	 * enable cylinder_cleanup
	offset 1476 bit 2 */
	bool isCylinderCleanupEnabled : 1;
	/**
	offset 1476 bit 3 */
	bool secondTriggerChannelEnabled : 1;
	/**
	offset 1476 bit 4 */
	bool needSecondTriggerInputDeprecated : 1;
	/**
	offset 1476 bit 5 */
	bool isMapAveragingEnabled : 1;
	/**
	offset 1476 bit 6 */
	bool overrideCrankingIacSetting : 1;
	/**
	offset 1476 bit 7 */
	bool useSeparateAdvanceForIdle : 1;
	/**
	offset 1476 bit 8 */
	bool isTunerStudioEnabled : 1;
	/**
	offset 1476 bit 9 */
	bool isWaveAnalyzerEnabled : 1;
	/**
	offset 1476 bit 10 */
	bool useSeparateVeForIdle : 1;
	/**
	 * enable trigger_details
	offset 1476 bit 11 */
	bool isPrintTriggerSynchDetails : 1;
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1476 bit 12 */
	bool isManualSpinningMode : 1;
	/**
	 * This is needed if batched injection and individual injector wiring
	 * enable two_wire_batch_injection
	offset 1476 bit 13 */
	bool twoWireBatchInjection : 1;
	/**
	 * VR sensors are only precise on rising front
	 * enable trigger_only_front
	offset 1476 bit 14 */
	bool useOnlyRisingEdgeForTrigger : 1;
	/**
	 * This is needed if batched igniton (waster spark) and individual coil wiring
	offset 1476 bit 15 */
	bool twoWireBatchIgnition : 1;
	/**
	offset 1476 bit 16 */
	bool useFixedBaroCorrFromMap : 1;
	/**
	offset 1476 bit 17 */
	bool useSeparateAdvanceForCranking : 1;
	/**
	offset 1476 bit 18 */
	bool useAdvanceCorrectionsForCranking : 1;
	/**
	 * This flag allows to use TPS for ignition lookup while in Speed Density Fuel Mode
	offset 1476 bit 19 */
	bool useTPSAdvanceTable : 1;
	/**
	offset 1476 bit 20 */
	bool unused_1484_bit_20 : 1;
	/**
	offset 1476 bit 21 */
	bool unused_1484_bit_21 : 1;
	/**
	 * offset 1480
	 */
	uint32_t engineChartSize;
	/**
	 * Relative to the target idle RPM
	 * offset 1484
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Prime injection pulse falloff temperature threshold, in Celsuis
	 * offset 1486
	 */
	int16_t primeInjFalloffTemperature;
	/**
	 * A/C button input handled as analog input
	 * offset 1488
	 */
	adc_channel_e acSwitchAdc;
	/**
	 * offset 1489
	 */
	uint8_t unusuedsw[3];
	/**
	 * At what trigger index should some ignition-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	 * offset 1492
	 */
	int ignMathCalculateAtIndex;
	/**
	 * offset 1496
	 */
	int16_t acCutoffLowRpm;
	/**
	 * offset 1498
	 */
	int16_t acCutoffHighRpm;
	/**
	 * offset 1500
	 */
	int16_t acIdleRpmBump;
	/**
	 * set warningPeriod X
	 * offset 1502
	 */
	int16_t warningPeriod;
	/**
	 * offset 1504
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * offset 1505
	 */
	uint8_t unusuedvref[3];
	/**
	 * offset 1508
	 */
	float knockDetectionWindowStart;
	/**
	 * offset 1512
	 */
	float knockDetectionWindowEnd;
	/**
	 * offset 1516
	 */
	float idleStepperReactionTime;
	/**
	 * offset 1520
	 */
	float knockVThreshold;
	/**
	 * offset 1524
	 */
	pin_input_mode_e fsioInputModes[FSIO_COMMAND_COUNT];
	/**
	 * offset 1540
	 */
	int alFIn[3];
	/**
	 * offset 1552
	 */
	int idleStepperTotalSteps;
	/**
	 * TODO: finish this #413
	 * offset 1556
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	 * offset 1560
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	 * offset 1564
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	 * offset 1580
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	 * offset 1596
	 */
	baro_corr_table_t baroCorrTable;
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * offset 1660
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	 * offset 1692
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * offset 1724
	 */
	float tachPulseDuractionMs;
	/**
	 * Trigger cycle index at which we start tach pulse (performance consideration)
	 * offset 1728
	 */
	int tachPulseTriggerIndex;
	/**
	 * offset 1732
	 */
	float wwaeTau;
	/**
	 * offset 1736
	 */
	pid_s alternatorControl;
	/**
	 * offset 1756
	 */
	pid_s etb;
	/**
	 * offset 1776
	 */
	pid_s warmupAfrPid;
	/**
	 * kPa value which is too low to be true
	 * offset 1796
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	 * offset 1800
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * RPMs prior to step1rpm point where ignition advance is retarded
	 * offset 1804
	 */
	int step1RpmWindow;
	/**
	 * See cltIdleRpmBins
	 * offset 1808
	 */
	pid_s idleRpmPid;
	/**
	 * Wall wetting/evaporation factor/Tau factor/fuel film
	 * offset 1828
	 */
	float wwaeBeta;
	/**
	 * blue LED on discovery by default
	 * offset 1832
	 */
	brain_pin_e communicationLedPin;
	/**
	 * green LED on discovery by default
	 * offset 1833
	 */
	brain_pin_e runningLedPin;
	/**
	 * offset 1834
	 */
	brain_pin_e binarySerialTxPin;
	/**
	 * offset 1835
	 */
	brain_pin_e binarySerialRxPin;
	/**
	 * offset 1836
	 */
	brain_pin_e auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 *  todo: finish pin migration from hard-coded to configurable?
	 * offset 1838
	 */
	brain_pin_e consoleSerialTxPin;
	/**
	 * todo: finish pin migration from hard-coded to configurable?
	 * offset 1839
	 */
	brain_pin_e consoleSerialRxPin;
	/**
	 * Knock sensor output knock detection threshold depending on current RPM
	 * offset 1840
	 */
	float knockNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * offset 1872
	 */
	float knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * offset 1904
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * offset 1908
	 */
	float throttlePedalWOTVoltage;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	 * offset 1912
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If RPM is close enough let's leave IAC alone, and maybe engage timing PID correction
	 * offset 1914
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * CLT-based target RPM for automatic idle controller
	 * offset 1916
	 */
	float cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 1980
	 */
	float cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * set targetvbatt VOLTS
	 * offset 2044
	 */
	float targetVBatt;
	/**
	 * Turn off alternator output above specified TPS
	 * offset 2048
	 */
	float alternatorOffAboveTps;
	/**
	 * Prime pulse for cold engine, duration in ms
	 * Linear interpolation between -40F/-40C and fallout temperature
	 * 
	 * See also isFasterEngineSpinUpEnabled
	 * set cranking_priming_pulse X
	 * offset 2052
	 */
	float startOfCrankingPrimingPulse;
	/**
	 * offset 2056
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	 * offset 2058
	 */
	int16_t iacByTpsTaper;
	/**
	 * RED led on Discovery by default.
	 * offset 2060
	 */
	brain_pin_e fatalErrorPin;
	/**
	 * offset 2061
	 */
	brain_pin_e warningLedPin;
	/**
	 * offset 2062
	 */
	brain_pin_e unused1234234;
	/**
	 * offset 2063
	 */
	brain_pin_e LIS302DLCsPin;
	/**
	 * How many engine cycles to consider?
	 * offset 2064
	 */
	int tpsAccelLength;
	/**
	 * Maximum delta in TPS percentage over the 'length'. Actual TPS change has to be above this value in order to TPS/TPS acceleration to kick in.
	 * offset 2068
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * angle between cam sensor and VVT zero position
	 * set vvt_offset X
	 * offset 2072
	 */
	float vvtOffset;
	/**
	 * offset 2076
	 */
	int engineLoadAccelLength;
	/**
	 * offset 2080
	 */
	float engineLoadDecelEnleanmentThreshold;
	/**
	 * offset 2084
	 */
	float engineLoadDecelEnleanmentMultiplier;
	/**
	 * offset 2088
	 */
	float engineLoadAccelEnrichmentThreshold;
	/**
	 * offset 2092
	 */
	float engineLoadAccelEnrichmentMultiplier;
	/**
	 * offset 2096
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * offset 2100
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * offset 2104
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	 * offset 2108
	 */
	float slowAdcAlpha;
	/**
	 * See http://rusefi.com/s/debugmode
	 * 
	 * set debug_mode X
	 * offset 2112
	 */
	debug_mode_e debugMode;
	/**
	 * CLT-based target AFR for PID-based control
	 * offset 2116
	 */
	float warmupTargetAfrBins[WARMUP_TARGET_AFR_SIZE];
	/**
	 * offset 2132
	 */
	float warmupTargetAfr[WARMUP_TARGET_AFR_SIZE];
	/**
	 * offset 2148
	 */
	float warmupAfrThreshold;
	/**
	 * kPa value at which we need to cut fuel and spark, 0 if not enabled
	 * offset 2152
	 */
	float boostCutPressure;
	/**
	 * offset 2156
	 */
	float mapAccelTaperBins[MAP_ACCEL_TAPER];
	/**
	 * offset 2188
	 */
	float mapAccelTaperMult[MAP_ACCEL_TAPER];
	/**
	 * todo: rename to fsioAnalogInputs
	 * offset 2220
	 */
	adc_channel_e fsioAdc[FSIO_ANALOG_INPUT_COUNT];
	/**
	 * Fixed timing, useful for TDC testing
	 * offset 2224
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * offset 2228
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * offset 2232
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * offset 2236
	 */
	float egoValueShift;
	/**
	 * offset 2240
	 */
	smart_pin_e auxPidPins[AUX_PID_COUNT];
	/**
	 * Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	 * TODO #660
	 * offset 2244
	 */
	brain_input_pin_e camInput;
	/**
	 * offset 2245
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * This implementation makes a pulse every time one of the coils is charged, using coil dwell for pulse width. See also tachOutputPin
	 * offset 2246
	 */
	brain_pin_e dizzySparkOutputPin;
	/**
	 * offset 2247
	 */
	pin_output_mode_e dizzySparkOutputPinMode;
	/**
	 * IAC position during cranking
	 * offset 2248
	 */
	int crankingIACposition;
	/**
	 * offset 2252
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 2256
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 2260
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 2264
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 2268
	 */
	fsio_pwm_freq_t auxPidFrequency[AUX_PID_COUNT];
	/**
	 * offset 2276
	 */
	int alternatorPwmFrequency;
	/**
	 * offset 2280
	 */
	mass_storage_e storageMode;
	/**
	 * offset 2284
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * offset 2285
	 */
	uint8_t unusedSpiPadding3[3];
	/**
	 * Narrow Band WBO Approximation
	 * offset 2288
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * offset 2320
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * set vvt_mode X
	 * offset 2352
	 */
	vvt_mode_e vvtMode;
	/**
	 * offset 2356
	 */
	bi_quard_s biQuad;
	/**
	 * CLT-based timing correction
	 * offset 2376
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2408
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2440
	 */
	int nbVvtIndex;
	/**
	 * offset 2444
	 */
	float autoTuneCltThreshold;
	/**
	 * offset 2448
	 */
	float autoTuneTpsRocThreshold;
	/**
	 * offset 2452
	 */
	float autoTuneTpsQuietPeriod;
	/**
	 * offset 2456
	 */
	float postCrankingTargetClt;
	/**
	 * Fuel multiplier taper, see also postCrankingDurationSec
	 * offset 2460
	 */
	float postCrankingFactor;
	/**
	 * See also postCrankingFactor
	 * offset 2464
	 */
	float postCrankingDurationSec;
	/**
	 * todo: finish implementation #332
	 * offset 2468
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * todo: finish implementation #332
	 * offset 2500
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * offset 2532
	 */
	int16_t fuelClosedLoopCltThreshold;
	/**
	 * offset 2534
	 */
	int16_t fuelClosedLoopTpsThreshold;
	/**
	 * offset 2536
	 */
	int16_t fuelClosedLoopRpmThreshold;
	/**
	 * offset 2538
	 */
	int16_t etbFreq;
	/**
	 * offset 2540
	 */
	pid_s fuelClosedLoopPid;
	/**
	 * offset 2560
	 */
	float fuelClosedLoopAfrHighThreshold;
	/**
	 * per-cylinder timing correction
	 * offset 2564
	 */
	cfg_float_t_1f timing_offset_cylinder[IGNITION_PIN_COUNT];
	/**
	 * offset 2612
	 */
	float idlePidActivationTime;
	/**
	 * offset 2616
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * offset 2617
	 */
	uint8_t unusedSpiPadding4[3];
	/**
	 * offset 2620
	 */
	pin_mode_e spi1SckMode;
	/**
	 * offset 2621
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 2622
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 2623
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 2624
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 2625
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * offset 2626
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 2627
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 2628
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 2629
	 */
	pin_output_mode_e stepperEnablePinMode;
	/**
	 * ResetB
	 * offset 2630
	 */
	brain_pin_e mc33816_rstb;
	/**
	 * offset 2631
	 */
	brain_pin_e mc33816_driven;
	/**
	 * offset 2632
	 */
	brain_pin_e brakePedalPin;
	/**
	 * lambda input
	 * offset 2633
	 */
	adc_channel_e cj125ua;
	/**
	 * heater input
	 * offset 2634
	 */
	adc_channel_e cj125ur;
	/**
	 * offset 2635
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 2636
	 */
	pid_s auxPid[AUX_PID_COUNT];
	/**
	 * offset 2716
	 */
	oil_pressure_config_s oilPressure;
	/**
	 * offset 2736
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * offset 2737
	 */
	uint8_t unusedSpiPadding5[3];
	/**
	 * offset 2740
	 */
	float fsioCurve1Bins[FSIO_CURVE_16];
	/**
	 * offset 2804
	 */
	float fsioCurve1[FSIO_CURVE_16];
	/**
	 * offset 2868
	 */
	float fsioCurve2Bins[FSIO_CURVE_16];
	/**
	 * offset 2932
	 */
	float fsioCurve2[FSIO_CURVE_16];
	/**
	 * offset 2996
	 */
	float fsioCurve3Bins[FSIO_CURVE_8];
	/**
	 * offset 3028
	 */
	float fsioCurve3[FSIO_CURVE_8];
	/**
	 * offset 3060
	 */
	float fsioCurve4Bins[FSIO_CURVE_8];
	/**
	 * offset 3092
	 */
	float fsioCurve4[FSIO_CURVE_8];
	/**
	 * offset 3124
	 */
	brain_input_pin_e flexFuelSensor;
	/**
	 * offset 3125
	 */
	brain_pin_e test557pin;
	/**
	 * offset 3126
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * offset 3127
	 */
	adc_channel_e externalKnockSenseAdc;
	/**
	 * offset 3128
	 */
	brain_pin_e stepperEnablePin;
	/**
	 * offset 3129
	 */
	brain_pin_e tle8888_cs;
	/**
	 * offset 3130
	 */
	pin_output_mode_e tle8888_csPinMode;
	/**
	 * offset 3131
	 */
	brain_pin_e mc33816_cs;
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * offset 3132
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * offset 3148
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * todo: more comments
	 * offset 3164
	 */
	brain_pin_e servoOutputPins[SERVO_COUNT];
	/**
	 * offset 3172
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * offset 3174
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * percent between 0 and 100
	 * offset 3176
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is deactivated if CLT<threshold
	 * offset 3178
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * offset 3180
	 */
	int16_t pidExtraForLowRpm;
	/**
	 *  maximum manifold pressure for fuel cut
	 * offset 3182
	 */
	int16_t coastingFuelCutMap;
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	 * offset 3184
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 *  CLT-based idle position for coasting (used in Auto-PID Idle mode)
	 * offset 3248
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 3312
	 */
	ignition_tps_table_t ignitionTpsTable;
	/**
	 * offset 3824
	 */
	float ignitionTpsBins[IGN_TPS_COUNT];
	/**
	 * offset 3888
	 */
	float tChargeAirCoefMin;
	/**
	 * offset 3892
	 */
	float tChargeAirCoefMax;
	/**
	 * offset 3896
	 */
	float tChargeAirFlowMax;
	/**
	 * offset 3900
	 */
	float tChargeAirIncrLimit;
	/**
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
	 * offset 3912
	 */
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	/**
	 * PWM bias, 0 to 100%
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
	 * offset 3984
	 */
	float etbDeadband;
	/**
	 * offset 3988
	 */
	etb_io etb2;
	/**
	 * See useIdleTimingPidControl
	 * offset 3992
	 */
	pid_s idleTimingPid;
	/**
	 * The timing correction works only if RPM is close enough, otherwise the IAC correction works
	 * offset 4012
	 */
	int16_t idleTimingPidWorkZone;
	/**
	 * If RPM is too perfect, let's leave the advance angle alone to avoid oscillation
	 * offset 4014
	 */
	int16_t idleTimingPidDeadZone;
	/**
	 * Added to the work zone for smooth correction falloff
	 * offset 4016
	 */
	int16_t idlePidFalloffDeltaRpm;
	/**
	 * A delay in cycles between fuel-enrich. portions
	 * offset 4018
	 */
	int16_t tpsAccelFractionPeriod;
	/**
	 * A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	 * offset 4020
	 */
	float tpsAccelFractionDivisor;
	/**
	 * offset 4024
	 */
	spi_device_e tle8888spiDevice;
	/**
	 * offset 4025
	 */
	spi_device_e mc33816spiDevice;
	/**
	 * offset 4026
	 */
	uint8_t unusedSpiPadding6[2];
	/**
	 * offset 4028
	 */
	spi_device_e tle6240spiDevice;
	/**
	 * offset 4029
	 */
	uint8_t unusedSpiPadding7[3];
	/**
	 * offset 4032
	 */
	spi_device_e mc33972spiDevice;
	/**
	 * offset 4033
	 */
	uint8_t unusedSpiPadding8[3];
	/**
	 * offset 4036
	 */
	int mainUnusedEnd[611];
	/** total size 6480*/
};

typedef struct engine_configuration_s engine_configuration_s;

// start of persistent_config_s
struct persistent_config_s {
	/**
	 * offset 0
	 */
	engine_configuration_s engineConfiguration;
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	 * offset 6480
	 */
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	/**
	 * CLT-based cranking position multiplier for simple manual idle controller
	 * offset 6512
	 */
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * offset 6544
	 */
	float idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	 * offset 6576
	 */
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	/**
	 * Optional VE table for Idle (see useSeparateVEForIdle)
	 * offset 6608
	 */
	float idleVeBins[IDLE_VE_CURVE_SIZE];
	/**
	 *  Optional VE table for Idle (see useSeparateVEForIdle)
	 * offset 6640
	 */
	float idleVe[IDLE_VE_CURVE_SIZE];
	/**
	 * offset 6672
	 */
	le_formula_t fsioFormulas[FSIO_COMMAND_COUNT];
	/**
	 * offset 9872
	 */
	le_formula_t timingMultiplier;
	/**
	 * offset 10072
	 */
	le_formula_t timingAdditive;
	/**
	 * offset 10272
	 */
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	 * offset 10336
	 */
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	 * offset 10400
	 */
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	 * offset 10464
	 */
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	 * offset 10528
	 */
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	/**
	 * offset 10560
	 */
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	/**
	 * offset 10592
	 */
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	/**
	 * offset 10624
	 */
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	/**
	 * CLT-based idle position multiplier for simple manual idle controller
	 * offset 10656
	 */
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	/**
	 *  CLT-based idle position multiplier for simple manual idle controller
	 * offset 10720
	 */
	float cltIdleCorr[CLT_CURVE_SIZE];
	/**
	 * kg/hour value.
	 * By the way 2.081989116 kg/h = 1 ft�/m
	 * offset 10784
	 */
	float mafDecoding[MAF_DECODING_COUNT];
	/**
	 * offset 11808
	 */
	float mafDecodingBins[MAF_DECODING_COUNT];
	/**
	 * offset 12832
	 */
	angle_table_t ignitionIatCorrTable;
	/**
	 * offset 13856
	 */
	float ignitionIatCorrLoadBins[IGN_LOAD_COUNT];
	/**
	 * offset 13920
	 */
	float ignitionIatCorrRpmBins[IGN_RPM_COUNT];
	/**
	 * offset 13984
	 */
	angle_table_t injectionPhase;
	/**
	 * offset 15008
	 */
	float injPhaseLoadBins[FUEL_LOAD_COUNT];
	/**
	 * offset 15072
	 */
	float injPhaseRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 15136
	 */
	fuel_table_t fuelTable;
	/**
	 * offset 16160
	 */
	float fuelLoadBins[FUEL_LOAD_COUNT];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 16224
	 */
	float fuelRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 16288
	 */
	ignition_table_t ignitionTable;
	/**
	 * offset 17312
	 */
	float ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	 * offset 17376
	 */
	float ignitionRpmBins[IGN_RPM_COUNT];
	/**
	 * offset 17440
	 */
	ve_table_t veTable;
	/**
	 * offset 18464
	 */
	float veLoadBins[FUEL_LOAD_COUNT];
	/**
	 * offset 18528
	 */
	float veRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 18592
	 */
	afr_table_t afrTable;
	/**
	 * offset 18848
	 */
	float afrLoadBins[FUEL_LOAD_COUNT];
	/**
	 * offset 18912
	 */
	float afrRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 18976
	 */
	tps_tps_table_t tpsTpsAccelTable;
	/**
	 * offset 19232
	 */
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19264
	 */
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	/**
	 * offset 19296
	 */
	fsio_table_8x8_f32t fsioTable1;
	/**
	 * offset 19552
	 */
	float fsioTable1LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19584
	 */
	float fsioTable1RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19616
	 */
	fsio_table_8x8_u8t fsioTable2;
	/**
	 * offset 19680
	 */
	float fsioTable2LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19712
	 */
	float fsioTable2RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19744
	 */
	fsio_table_8x8_u8t fsioTable3;
	/**
	 * offset 19808
	 */
	float fsioTable3LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19840
	 */
	float fsioTable3RpmBins[FSIO_TABLE_8];
	/**
	 * offset 19872
	 */
	fsio_table_8x8_u8t fsioTable4;
	/**
	 * offset 19936
	 */
	float fsioTable4LoadBins[FSIO_TABLE_8];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 19968
	 */
	float fsioTable4RpmBins[FSIO_TABLE_8];
	/** total size 20000*/
};

typedef struct persistent_config_s persistent_config_s;

#endif
// end
// this section was generated automatically by ConfigDefinition.jar based on integration\rusefi_config.txt Mon May 27 20:05:22 EDT 2019
