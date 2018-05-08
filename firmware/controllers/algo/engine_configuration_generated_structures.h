// this section was generated automatically by ConfigDefinition.jar based on rusefi_config.txt Thu Apr 26 02:12:36 EDT 2018
// begin
#ifndef ENGINE_CONFIGURATION_GENERATED_H_
#define ENGINE_CONFIGURATION_GENERATED_H_
#include "rusefi_types.h"
typedef struct {
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
	int16_t period;
	/**
	 * offset 16
	 */
	int16_t minValue;
	/**
	 * offset 18
	 */
	int16_t maxValue;
	/** total size 20*/
} pid_s;

typedef struct {
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
} cranking_parameters_s;

typedef struct {
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
	/** total size 16*/
} air_pressure_sensor_config_s;

/**
 * @brief MAP averaging configuration

*/
typedef struct {
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
} MAP_sensor_config_s;

/**
 * @brief Thermistor known values

*/
typedef struct {
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
} thermistor_conf_s;

/**
 * @brief Oil pressure sensor interpolation

*/
typedef struct {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
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
} oil_pressure_config_s;

/**
 * @brief Thermistor curve parameters

*/
typedef struct {
	/**
	 * offset 0
	 */
	thermistor_conf_s config;
	/**
	 * offset 28
	 */
	adc_channel_e adcChannel;
	/** total size 32*/
} ThermistorConf;

typedef struct {
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
} injector_s;

typedef struct {
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
} bi_quard_s;

typedef struct {
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
} specs_s;

/**
 * @brief Trigger wheel(s) configuration

*/
typedef struct {
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
} trigger_config_s;

typedef struct {
	/**
	 * offset 0
	 */
	adc_channel_e hwChannel;
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
} afr_sensor_s;

typedef struct {
	/**
	 * offset 0
	 */
	brain_pin_e solenoidPin;
	/**
	 * offset 4
	 */
	pin_output_mode_e solenoidPinMode;
	/**
	 * offset 8
	 */
	int solenoidFrequency;
	/**
	 * offset 12
	 */
	brain_pin_e stepperDirectionPin;
	/**
	 * offset 16
	 */
	brain_pin_e stepperStepPin;
	/** total size 20*/
} idle_hardware_s;

typedef struct {
	/**
	 * offset 0
	 */
	idle_hardware_s idle;
	/**
	 * value between 0 and 100 used in Manual mode
	 * offset 20
	 */
	float manIdlePosition;
	/**
	 * offset 24
	 */
	brain_pin_e fuelPumpPin;
	/**
	 * offset 28
	 */
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * offset 32
	 */
	brain_pin_e injectionPins[INJECTION_PIN_COUNT];
	/**
	 * offset 80
	 */
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 84
	 */
	brain_pin_e ignitionPins[IGNITION_PIN_COUNT];
	/**
	 * offset 132
	 */
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 136
	 */
	brain_pin_e malfunctionIndicatorPin;
	/**
	 * offset 140
	 */
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 144
	 */
	brain_pin_e fanPin;
	/**
	 * offset 148
	 */
	pin_output_mode_e fanPinMode;
	/**
	 * offset 152
	 */
	float mapFrequency0Kpa;
	/**
	 * offset 156
	 */
	float mapFrequency100Kpa;
	/**
	 * some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 160
	 */
	brain_pin_e clutchDownPin;
	/**
	 * offset 164
	 */
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 168
	 */
	brain_pin_e alternatorControlPin;
	/**
	 * offset 172
	 */
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 176
	 */
	brain_pin_e HD44780_rs;
	/**
	 * offset 180
	 */
	brain_pin_e HD44780_e;
	/**
	 * offset 184
	 */
	brain_pin_e HD44780_db4;
	/**
	 * offset 188
	 */
	brain_pin_e HD44780_db5;
	/**
	 * offset 192
	 */
	brain_pin_e HD44780_db6;
	/**
	 * offset 196
	 */
	brain_pin_e HD44780_db7;
	/**
	 * offset 200
	 */
	brain_pin_e gps_rx_pin;
	/**
	 * offset 204
	 */
	brain_pin_e gps_tx_pin;
	/**
	 * rpm X
	 * offset 208
	 */
	int triggerSimulatorFrequency;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 212
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 216
	 */
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 232
	 */
	brain_pin_e etbDirectionPin1;
	/**
	 * offset 236
	 */
	brain_pin_e etbDirectionPin2;
	/**
	 * offset 240
	 */
	brain_pin_e etbControlPin1;
	/**
	 * offset 244
	 */
	brain_pin_e etbControlPin2;
	/**
	 * offset 248
	 */
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * offset 252
	 */
	brain_pin_e wboHeaterPin;
	/**
	 * offset 256
	 */
	brain_pin_e cj125CsPin;
	/**
	 * offset 260
	 */
	float fuelLevelEmptyTankVoltage;
	/**
	 * offset 264
	 */
	float fuelLevelFullTankVoltage;
	/**
	 * AFR, WBO, EGO - whatever you like to call it
	 * offset 268
	 */
	ego_sensor_e afr_type;
	/**
	 * offset 272
	 */
	float fuelClosedLoopAfrLowThreshold;
	/**
	 * offset 276
	 */
	pin_output_mode_e hip9011CsPinMode;
	/**
	 * This implementation produces one pulse per engine cycle. See also dizzySparkOutputPin.
	 * offset 280
	 */
	brain_pin_e tachOutputPin;
	/**
	 * offset 284
	 */
	pin_output_mode_e tachOutputPinMode;
	/**
	 * offset 288
	 */
	brain_input_pin_e triggerInputPins[3];
	/**
	 * offset 300
	 */
	brain_pin_e mainRelayPin;
	/**
	 * offset 304
	 */
	int idleThreadPeriod;
	/**
	 * offset 308
	 */
	int consoleLoopPeriod;
	/**
	 * offset 312
	 */
	int lcdThreadPeriod;
	/**
	 * offset 316
	 */
	brain_pin_e sdCardCsPin;
	/**
	 * offset 320
	 */
	int generalPeriodicThreadPeriod;
	/**
	 * offset 324
	 */
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 328
	 */
	brain_pin_e boardTestModeJumperPin;
	/**
	 * offset 332
	 */
	can_device_mode_e canDeviceMode;
	/**
	 * offset 336
	 */
	brain_pin_e canTxPin;
	/**
	 * offset 340
	 */
	brain_pin_e canRxPin;
	/**
	 * offset 344
	 */
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 356
	 */
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 368
	 */
	brain_pin_e o2heaterPin;
	/**
	 * offset 372
	 */
	pin_output_mode_e o2heaterPinModeTodO;
	/**
	offset 376 bit 0 */
	bool is_enabled_spi_1 : 1;
	/**
	offset 376 bit 1 */
	bool is_enabled_spi_2 : 1;
	/**
	offset 376 bit 2 */
	bool is_enabled_spi_3 : 1;
	/**
	offset 376 bit 3 */
	bool isSdCardEnabled : 1;
	/**
	offset 376 bit 4 */
	bool isFastAdcEnabled : 1;
	/**
	offset 376 bit 5 */
	bool isEngineControlEnabled : 1;
	/**
	offset 376 bit 6 */
	bool isHip9011Enabled : 1;
	/**
	offset 376 bit 7 */
	bool isVerboseAlternator : 1;
	/**
	offset 376 bit 8 */
	bool useSerialPort : 1;
	/**
	offset 376 bit 9 */
	bool useStepperIdle : 1;
	/**
	offset 376 bit 10 */
	bool enabledStep1Limiter : 1;
	/**
	offset 376 bit 11 */
	bool useTpicAdvancedMode : 1;
	/**
	offset 376 bit 12 */
	bool useLcdScreen : 1;
	/**
	offset 376 bit 13 */
	bool startConsoleInBinaryMode : 1;
	/**
	offset 376 bit 14 */
	bool useWarmupPidAfr : 1;
	/**
	offset 376 bit 15 */
	bool onOffAlternatorLogic : 1;
	/**
	offset 376 bit 16 */
	bool isCJ125Enabled : 1;
	/**
	 * Use rise or fall signal front
	offset 376 bit 17 */
	bool vvtCamSensorUseRise : 1;
	/**
	 * Useful for individual intakes
	offset 376 bit 18 */
	bool measureMapOnlyInOneCylinder : 1;
	/**
	offset 376 bit 19 */
	bool stepperForceParkingEveryRestart : 1;
	/**
	offset 376 bit 20 */
	bool isFasterEngineSpinUpEnabled : 1;
	/**
	offset 376 bit 21 */
	bool coastingFuelCutEnabled : 1;
	/**
	offset 376 bit 22 */
	bool useIacTableForCoasting : 1;
	/**
	offset 376 bit 23 */
	bool useNoiselessTriggerDecoder : 1;
	/**
	offset 376 bit 24 */
	bool unused_board_984_24 : 1;
	/**
	offset 376 bit 25 */
	bool unused_board_984_25 : 1;
	/**
	offset 376 bit 26 */
	bool unused_board_984_26 : 1;
	/**
	offset 376 bit 27 */
	bool unused_board_984_27 : 1;
	/**
	offset 376 bit 28 */
	bool unused_board_984_28 : 1;
	/**
	offset 376 bit 29 */
	bool unused_board_984_29 : 1;
	/**
	offset 376 bit 30 */
	bool unused_board_984_30 : 1;
	/**
	 * offset 380
	 */
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * default or inverted input
	 * offset 396
	 */
	uint8_t logicAnalyzerMode[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 400
	 */
	int unrealisticRpmThreashold;
	/**
	 * offset 404
	 */
	pin_output_mode_e mainRelayPinMode;
	/**
	 * offset 408
	 */
	brain_pin_e max31855_cs[EGT_CHANNEL_COUNT];
	/**
	 * offset 440
	 */
	spi_device_e max31855spiDevice;
	/**
	 * todo: more comments
	 * offset 444
	 */
	brain_pin_e fsioOutputPins[FSIO_COMMAND_COUNT];
	/**
	 * offset 508
	 */
	pin_output_mode_e gpioPinModes[FSIO_COMMAND_COUNT];
	/**
	 * offset 572
	 */
	uart_device_e consoleUartDevice;
	/**
	 * offset 576
	 */
	int mapMinBufferLength;
	/**
	 * offset 580
	 */
	int16_t idlePidDeactivationTpsThreshold;
	/**
	 * offset 582
	 */
	int16_t stepperParkingExtraSteps;
	/**
	 * offset 584
	 */
	float nb2ratioFrom;
	/**
	 * offset 588
	 */
	float nb2ratioTo;
	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 * offset 592
	 */
	brain_pin_e triggerErrorPin;
	/**
	 * offset 596
	 */
	pin_output_mode_e triggerErrorPinMode;
	/**
	 * offset 600
	 */
	brain_pin_e acRelayPin;
	/**
	 * offset 604
	 */
	pin_output_mode_e acRelayPinMode;
	/**
	 * offset 608
	 */
	fsio_pwm_freq_t fsioFrequency[FSIO_COMMAND_COUNT];
	/**
	 * offset 640
	 */
	brain_pin_e hip9011CsPin;
	/**
	 * offset 644
	 */
	brain_pin_e hip9011IntHoldPin;
	/**
	 * offset 648
	 */
	pin_output_mode_e hip9011IntHoldPinMode;
	/**
	 * offset 652
	 */
	fsio_setting_t fsio_setting[FSIO_COMMAND_COUNT];
	/**
	 * offset 716
	 */
	brain_pin_e spi1mosiPin;
	/**
	 * offset 720
	 */
	brain_pin_e spi1misoPin;
	/**
	 * offset 724
	 */
	brain_pin_e spi1sckPin;
	/**
	 * offset 728
	 */
	brain_pin_e spi2mosiPin;
	/**
	 * offset 732
	 */
	brain_pin_e spi2misoPin;
	/**
	 * offset 736
	 */
	brain_pin_e spi2sckPin;
	/**
	 * offset 740
	 */
	brain_pin_e spi3mosiPin;
	/**
	 * offset 744
	 */
	brain_pin_e spi3misoPin;
	/**
	 * offset 748
	 */
	brain_pin_e spi3sckPin;
	/**
	 * offset 752
	 */
	float hip9011Gain;
	/**
	 * offset 756
	 */
	brain_pin_e joystickCenterPin;
	/**
	 * offset 760
	 */
	brain_pin_e joystickAPin;
	/**
	 * offset 764
	 */
	brain_pin_e joystickBPin;
	/**
	 * offset 768
	 */
	brain_pin_e joystickCPin;
	/**
	 * offset 772
	 */
	brain_pin_e joystickDPin;
	/**
	 * rusEfi console Sensor Sniffer mode
	 * offset 776
	 */
	sensor_chart_e sensorChartMode;
	/**
	 * offset 780
	 */
	maf_sensor_type_e mafSensorType;
	/**
	 * offset 784
	 */
	brain_input_pin_e vehicleSpeedSensorInputPin;
	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 * offset 788
	 */
	brain_pin_e clutchUpPin;
	/**
	 * offset 792
	 */
	pin_input_mode_e clutchUpPinMode;
	/**
	 * todo:not finshed
	 * These input pins allow us to pull toggle buttons state
	 * offset 796
	 */
	brain_pin_e fsioDigitalInputs[FSIO_COMMAND_COUNT];
	/**
	 * offset 860
	 */
	brain_input_pin_e frequencyReportingMapInputPin;
	/** total size 864*/
} board_configuration_s;

typedef struct {
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
	bool issue_294_10 : 1;
	/**
	offset 76 bit 11 */
	bool issue_294_11 : 1;
	/**
	offset 76 bit 12 */
	bool issue_294_12 : 1;
	/**
	offset 76 bit 13 */
	bool issue_294_13 : 1;
	/**
	offset 76 bit 14 */
	bool issue_294_14 : 1;
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
	 * Closed throttle. todo: extract these two fields into a structure
	 * todo: we need two sets of TPS parameters - modern ETBs have two sensors
	 * offset 80
	 */
	int16_t tpsMin;
	/**
	 * Full throtle. tpsMax value as 10 bit ADC value. Not Voltage!
	 * offset 82
	 */
	int16_t tpsMax;
	/**
	 * TPS error detection, what TPS % value is unrealistically low
	 * offset 84
	 */
	int16_t tpsErrorDetectionTooLow;
	/**
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
	 * 6=8MHz todo solit two bit fields
	 * offset 324
	 */
	int hip9011PrescalerAndSDO;
	/**
	 * kHz knock band override
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
	 * offset 468
	 */
	adc_channel_e vbattAdcChannel;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsuis
	 * offset 472
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsuis
	 * offset 476
	 */
	float fanOffTemperature;
	/**
	 * offset 480
	 */
	adc_channel_e fuelLevelSensor;
	/**
	 * This coefficient translates vehicle speed input frequency (in Hz) into vehicle speed, km/h
	 * offset 484
	 */
	float vehicleSpeedCoef;
	/**
	 * set can_mode X
	 * offset 488
	 */
	can_nbc_e canNbcType;
	/**
	 * CANbus thread period, ms
	 * offset 492
	 */
	int canSleepPeriod;
	/**
	 * See engineCycle
	 * set operation_mode X
	 * offset 496
	 */
	operation_mode_e operationMode;
	/**
	 * offset 500
	 */
	display_mode_e displayMode;
	/**
	 * offset 504
	 */
	log_format_e logFormat;
	/**
	 * offset 508
	 */
	int byFirmwareVersion;
	/**
	 * offset 512
	 */
	int HD44780width;
	/**
	 * offset 516
	 */
	int HD44780height;
	/**
	 * offset 520
	 */
	adc_channel_e tpsAdcChannel;
	/**
	 * offset 524
	 */
	int overrideCrankingIgnition;
	/**
	 * offset 528
	 */
	int sensorChartFrequency;
	/**
	 * offset 532
	 */
	trigger_config_s trigger;
	/**
	 * offset 548
	 */
	spi_device_e hip9011SpiDevice;
	/**
	 * set global_fuel_correction X
	 * offset 552
	 */
	float globalFuelCorrection;
	/**
	 * offset 556
	 */
	float adcVcc;
	/**
	 * maximum total number of degrees to subtract from ignition advance
	 * when knocking
	 * offset 560
	 */
	float maxKnockSubDeg;
	/**
	 * See hasMafSensor
	 * offset 564
	 */
	adc_channel_e mafAdcChannel;
	/**
	 * offset 568
	 */
	afr_sensor_s afr;
	/**
	 * offset 588
	 */
	adc_channel_e pedalPositionChannel;
	/**
	 * @see hasBaroSensor
	 * offset 592
	 */
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 608
	 */
	board_configuration_s bc;
	/**
	offset 1472 bit 0 */
	bool vvtDisplayInverted : 1;
	/**
	offset 1472 bit 1 */
	bool fuelClosedLoopCorrectionEnabled : 1;
	/**
	offset 1472 bit 2 */
	bool isVerboseIAC : 1;
	/**
	 * enable verbose_etb
	offset 1472 bit 3 */
	bool isVerboseETB : 1;
	/**
	offset 1472 bit 4 */
	bool useConstantDwellDuringCranking : 1;
	/**
	 * This options enables data for 'engine sniffer' tab in console, which comes at some CPU price
	offset 1472 bit 5 */
	bool isEngineChartEnabled : 1;
	/**
	offset 1472 bit 6 */
	bool silentTriggerError : 1;
	/**
	offset 1472 bit 7 */
	bool useLinearCltSensor : 1;
	/**
	offset 1472 bit 8 */
	bool canReadEnabled : 1;
	/**
	offset 1472 bit 9 */
	bool canWriteEnabled : 1;
	/**
	offset 1472 bit 10 */
	bool useLinearIatSensor : 1;
	/**
	 * See fsioTimingAdjustment
	offset 1472 bit 11 */
	bool useFSIO16ForTimingAdjustment : 1;
	/**
	offset 1472 bit 12 */
	bool tachPulseDurationAsDutyCycle : 1;
	/**
	offset 1472 bit 13 */
	bool isAlternatorControlEnabled : 1;
	/**
	offset 1472 bit 14 */
	bool invertPrimaryTriggerSignal : 1;
	/**
	offset 1472 bit 15 */
	bool invertSecondaryTriggerSignal : 1;
	/**
	offset 1472 bit 16 */
	bool cutFuelOnHardLimit : 1;
	/**
	offset 1472 bit 17 */
	bool cutSparkOnHardLimit : 1;
	/**
	offset 1472 bit 18 */
	bool step1fuelCutEnable : 1;
	/**
	offset 1472 bit 19 */
	bool step1SparkCutEnable : 1;
	/**
	offset 1472 bit 20 */
	bool hasFrequencyReportingMapSensor : 1;
	/**
	offset 1472 bit 21 */
	bool useFSIO8ForServo1 : 1;
	/**
	offset 1472 bit 22 */
	bool useFSIO9ForServo2 : 1;
	/**
	offset 1472 bit 23 */
	bool useFSIO10ForServo3 : 1;
	/**
	offset 1472 bit 24 */
	bool useFSIO11ForServo4 : 1;
	/**
	offset 1472 bit 25 */
	bool useFSIO12ForServo5 : 1;
	/**
	offset 1472 bit 26 */
	bool unused_bit_1472_26 : 1;
	/**
	offset 1472 bit 27 */
	bool unused_bit_1472_27 : 1;
	/**
	 * offset 1476
	 */
	adc_channel_e hipOutputChannel;
	/**
	 * offset 1480
	 */
	idle_mode_e idleMode;
	/**
	 * enable injection
	offset 1484 bit 0 */
	bool isInjectionEnabled : 1;
	/**
	 * enable ignition
	offset 1484 bit 1 */
	bool isIgnitionEnabled : 1;
	/**
	 * If TPS above 95% no fuel would be injected during cranking
	 * enable cylinder_cleanup
	offset 1484 bit 2 */
	bool isCylinderCleanupEnabled : 1;
	/**
	offset 1484 bit 3 */
	bool secondTriggerChannelEnabled : 1;
	/**
	offset 1484 bit 4 */
	bool needSecondTriggerInputDeprecated : 1;
	/**
	offset 1484 bit 5 */
	bool isMapAveragingEnabled : 1;
	/**
	offset 1484 bit 6 */
	bool overrideCrankingIacSetting : 1;
	/**
	offset 1484 bit 7 */
	bool useSeparateAdvanceForIdle : 1;
	/**
	offset 1484 bit 8 */
	bool isTunerStudioEnabled : 1;
	/**
	offset 1484 bit 9 */
	bool isWaveAnalyzerEnabled : 1;
	/**
	offset 1484 bit 10 */
	bool useSeparateVeForIdle : 1;
	/**
	 * enable trigger_details
	offset 1484 bit 11 */
	bool isPrintTriggerSynchDetails : 1;
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	offset 1484 bit 12 */
	bool isManualSpinningMode : 1;
	/**
	 * This is needed if batched injection and individual injector wiring
	 * enable two_wire_batch_injection
	offset 1484 bit 13 */
	bool twoWireBatchInjection : 1;
	/**
	 * VR sensors are only precise on rising front
	 * enable trigger_only_front
	offset 1484 bit 14 */
	bool useOnlyRisingEdgeForTrigger : 1;
	/**
	 * This is needed if batched igniton (waster spark) and individual coil wiring
	offset 1484 bit 15 */
	bool twoWireBatchIgnition : 1;
	/**
	offset 1484 bit 16 */
	bool useFixedBaroCorrFromMap : 1;
	/**
	offset 1484 bit 17 */
	bool useSeparateAdvanceForCranking : 1;
	/**
	offset 1484 bit 18 */
	bool useAdvanceCorrectionsForCranking : 1;
	/**
	offset 1484 bit 19 */
	bool unused_1484_bit_19 : 1;
	/**
	offset 1484 bit 20 */
	bool unused_1484_bit_20 : 1;
	/**
	offset 1484 bit 21 */
	bool unused_1484_bit_21 : 1;
	/**
	 * offset 1488
	 */
	uint32_t engineChartSize;
	/**
	 * Relative to the target idle RPM
	 * offset 1492
	 */
	int16_t idlePidRpmUpperLimit;
	/**
	 * Prime injection pulse falloff temperature threshold, in Celsuis
	 * offset 1494
	 */
	int16_t primeInjFalloffTemperature;
	/**
	 * A/C button input handled as analog input
	 * offset 1496
	 */
	adc_channel_e acSwitchAdc;
	/**
	 * At what trigger index should some ignition-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	 * offset 1500
	 */
	int ignMathCalculateAtIndex;
	/**
	 * offset 1504
	 */
	int16_t acCutoffLowRpm;
	/**
	 * offset 1506
	 */
	int16_t acCutoffHighRpm;
	/**
	 * offset 1508
	 */
	int16_t acIdleRpmBump;
	/**
	 * set warningPeriod X
	 * offset 1510
	 */
	int16_t warningPeriod;
	/**
	 * offset 1512
	 */
	adc_channel_e vRefAdcChannel;
	/**
	 * offset 1516
	 */
	float knockDetectionWindowStart;
	/**
	 * offset 1520
	 */
	float knockDetectionWindowEnd;
	/**
	 * offset 1524
	 */
	float idleStepperReactionTime;
	/**
	 * offset 1528
	 */
	float knockVThreshold;
	/**
	 * offset 1532
	 */
	pin_input_mode_e fsioInputModes[FSIO_COMMAND_COUNT];
	/**
	 * offset 1596
	 */
	adc_channel_e externalKnockSenseAdc;
	/**
	 * offset 1600
	 */
	int unsed2342342[3];
	/**
	 * offset 1612
	 */
	int idleStepperTotalSteps;
	/**
	 * offset 1616
	 */
	brain_pin_e stepperEnablePin;
	/**
	 * TODO: finish this #413
	 * offset 1620
	 */
	float noAccelAfterHardLimitPeriodSecs;
	/**
	 * At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	 * offset 1624
	 */
	int mapAveragingSchedulingAtIndex;
	/**
	 * offset 1628
	 */
	float baroCorrPressureBins[BARO_CORR_SIZE];
	/**
	 * offset 1644
	 */
	float baroCorrRpmBins[BARO_CORR_SIZE];
	/**
	 * offset 1660
	 */
	baro_corr_table_t baroCorrTable;
	/**
	 * offset 1724
	 */
	pin_mode_e spi2SckMode;
	/**
	 * offset 1728
	 */
	pin_mode_e spi2MosiMode;
	/**
	 * offset 1732
	 */
	pin_mode_e spi2MisoMode;
	/**
	 * Cranking fuel correction coefficient based on TPS
	 * offset 1736
	 */
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	/**
	 * offset 1768
	 */
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	/**
	 * offset 1800
	 */
	float tachPulseDuractionMs;
	/**
	 * Trigger cycle index at which we start tach pulse (performance consideration)
	 * offset 1804
	 */
	int tachPulseTriggerIndex;
	/**
	 * offset 1808
	 */
	float suckedOffCoef;
	/**
	 * offset 1812
	 */
	pid_s alternatorControl;
	/**
	 * offset 1832
	 */
	pid_s etb;
	/**
	 * offset 1852
	 */
	pid_s warmupAfrPid;
	/**
	 * kPa value which is too low to be true
	 * offset 1872
	 */
	float mapErrorDetectionTooLow;
	/**
	 * kPa value which is too high to be true
	 * offset 1876
	 */
	float mapErrorDetectionTooHigh;
	/**
	 * RPMs prior to step1rpm point where ignition advance is retarded
	 * offset 1880
	 */
	int step1RpmWindow;
	/**
	 * See cltIdleRpmBins
	 * offset 1884
	 */
	pid_s idleRpmPid;
	/**
	 * offset 1904
	 */
	float addedToWallCoef;
	/**
	 * blue LED on discovery by default
	 * offset 1908
	 */
	brain_pin_e communicationPin;
	/**
	 * green LED on discovery by default
	 * offset 1912
	 */
	brain_pin_e runningPin;
	/**
	 * offset 1916
	 */
	brain_pin_e binarySerialTxPin;
	/**
	 * offset 1920
	 */
	brain_pin_e binarySerialRxPin;
	/**
	 *  todo: finish pin migration from hard-coded to configurable?
	 * offset 1924
	 */
	brain_pin_e consoleSerialTxPin;
	/**
	 * todo: finish pin migration from hard-coded to configurable?
	 * offset 1928
	 */
	brain_pin_e consoleSerialRxPin;
	/**
	 * Knock sensor output knock detection threshold depending on current RPM
	 * offset 1932
	 */
	float knockNoise[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * offset 1964
	 */
	float knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	/**
	 * offset 1996
	 */
	float throttlePedalUpVoltage;
	/**
	 * Pedal in the floor
	 * offset 2000
	 */
	float throttlePedalWOTVoltage;
	/**
	 * offset 2004
	 */
	pin_output_mode_e stepperDirectionPinMode;
	/**
	 * on ECU start turn fuel pump on to build fuel pressure
	 * offset 2008
	 */
	int16_t startUpFuelPumpDuration;
	/**
	 * If RPM is close enough let's leave IAC alone
	 * offset 2010
	 */
	int16_t idlePidRpmDeadZone;
	/**
	 * CLT-based target RPM for automatic idle controller
	 * offset 2012
	 */
	float cltIdleRpmBins[CLT_CURVE_SIZE];
	/**
	 * See idleRpmPid
	 * offset 2076
	 */
	float cltIdleRpm[CLT_CURVE_SIZE];
	/**
	 * set targetvbatt VOLTS
	 * offset 2140
	 */
	float targetVBatt;
	/**
	 * Turn off alternator output above specified TPS
	 * offset 2144
	 */
	float alternatorOffAboveTps;
	/**
	 * Prime pulse duration in ms
	 * set cranking_priming_pulse X
	 * offset 2148
	 */
	float startOfCrankingPrimingPulse;
	/**
	 * offset 2152
	 */
	int16_t afterCrankingIACtaperDuration;
	/**
	 * Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	 * offset 2154
	 */
	int16_t iacByTpsTaper;
	/**
	 * offset 2156
	 */
	brain_pin_e LIS302DLCsPin;
	/**
	 * offset 2160
	 */
	int tpsAccelLength;
	/**
	 * offset 2164
	 */
	float tpsAccelEnrichmentThreshold;
	/**
	 * set vvt_offset X
	 * offset 2168
	 */
	float vvtOffset;
	/**
	 * offset 2172
	 */
	int engineLoadAccelLength;
	/**
	 * offset 2176
	 */
	float engineLoadDecelEnleanmentThreshold;
	/**
	 * offset 2180
	 */
	float engineLoadDecelEnleanmentMultiplier;
	/**
	 * offset 2184
	 */
	float engineLoadAccelEnrichmentThreshold;
	/**
	 * offset 2188
	 */
	float engineLoadAccelEnrichmentMultiplier;
	/**
	 * RED led on Discovery by default. TODO: start using this property
	 * offset 2192
	 */
	brain_pin_e fatalErrorPin;
	/**
	 * TODO: start using this property
	 * offset 2196
	 */
	brain_pin_e warninigPin;
	/**
	 * offset 2200
	 */
	brain_pin_e configResetPin;
	/**
	 * offset 2204
	 */
	uint32_t uartConsoleSerialSpeed;
	/**
	 * offset 2208
	 */
	float tpsDecelEnleanmentThreshold;
	/**
	 * offset 2212
	 */
	float tpsDecelEnleanmentMultiplier;
	/**
	 * ExpAverage alpha coefficient
	 * offset 2216
	 */
	float slowAdcAlpha;
	/**
	 * set debug_mode X
	 * offset 2220
	 */
	debug_mode_e debugMode;
	/**
	 * offset 2224
	 */
	brain_pin_e auxValves[AUX_DIGITAL_VALVE_COUNT];
	/**
	 * CLT-based target AFR for PID-based control
	 * offset 2232
	 */
	float warmupTargetAfrBins[WARMUP_TARGET_AFR_SIZE];
	/**
	 * offset 2248
	 */
	float warmupTargetAfr[WARMUP_TARGET_AFR_SIZE];
	/**
	 * offset 2264
	 */
	float warmupAfrThreshold;
	/**
	 * kPa value at which we need to cut fuel and spark, 0 if not enabled
	 * offset 2268
	 */
	float boostCutPressure;
	/**
	 * offset 2272
	 */
	float mapAccelTaperBins[MAP_ACCEL_TAPER];
	/**
	 * offset 2304
	 */
	float mapAccelTaperMult[MAP_ACCEL_TAPER];
	/**
	 * This implementation makes a pulse every time one of the coils is charged, using coil dwell for pulse width. See also tachOutputPin
	 * offset 2336
	 */
	brain_pin_e dizzySparkOutputPin;
	/**
	 * offset 2340
	 */
	pin_output_mode_e dizzySparkOutputPinMode;
	/**
	 * todo: rename to fsioAnalogInputs
	 * offset 2344
	 */
	adc_channel_e fsioAdc[FSIO_ANALOG_INPUT_COUNT];
	/**
	 * Fixed timing, useful for TDC testing
	 * offset 2360
	 */
	float fixedTiming;
	/**
	 * MAP voltage for low point
	 * offset 2364
	 */
	float mapLowValueVoltage;
	/**
	 * MAP voltage for low point
	 * offset 2368
	 */
	float mapHighValueVoltage;
	/**
	 * EGO value correction
	 * offset 2372
	 */
	float egoValueShift;
	/**
	 * offset 2376
	 */
	brain_input_pin_e camInput;
	/**
	 * IAC position during cranking
	 * offset 2380
	 */
	int crankingIACposition;
	/**
	 * offset 2384
	 */
	float tChargeMinRpmMinTps;
	/**
	 * offset 2388
	 */
	float tChargeMinRpmMaxTps;
	/**
	 * offset 2392
	 */
	float tChargeMaxRpmMinTps;
	/**
	 * offset 2396
	 */
	float tChargeMaxRpmMaxTps;
	/**
	 * offset 2400
	 */
	brain_pin_e auxPidPins[AUX_PID_COUNT];
	/**
	 * offset 2416
	 */
	fsio_pwm_freq_t auxPidFrequency[AUX_PID_COUNT];
	/**
	 * offset 2424
	 */
	int alternatorPwmFrequency;
	/**
	 * offset 2428
	 */
	mass_storage_e storageMode;
	/**
	 * offset 2432
	 */
	spi_device_e cj125SpiDevice;
	/**
	 * offset 2436
	 */
	pin_output_mode_e cj125CsPinMode;
	/**
	 * Narrow Band WBO Approximation
	 * offset 2440
	 */
	float narrowToWideOxygenBins[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * offset 2472
	 */
	float narrowToWideOxygen[NARROW_BAND_WIDE_BAND_CONVERSION_SIZE];
	/**
	 * set vvt_mode X
	 * offset 2504
	 */
	vvt_mode_e vvtMode;
	/**
	 * offset 2508
	 */
	bi_quard_s biQuad;
	/**
	 * CLT-based timing correction
	 * offset 2528
	 */
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2560
	 */
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	/**
	 * offset 2592
	 */
	int nbVvtIndex;
	/**
	 * offset 2596
	 */
	float autoTuneCltThreshold;
	/**
	 * offset 2600
	 */
	float autoTuneTpsRocThreshold;
	/**
	 * offset 2604
	 */
	float autoTuneTpsQuietPeriod;
	/**
	 * offset 2608
	 */
	float postCrankingTargetClt;
	/**
	 * Fuel multiplier taper, see also postCrankingDurationSec
	 * offset 2612
	 */
	float postCrankingFactor;
	/**
	 * See also postCrankingFactor
	 * offset 2616
	 */
	float postCrankingDurationSec;
	/**
	 * todo: finish implementation #332
	 * offset 2620
	 */
	ThermistorConf auxTempSensor1;
	/**
	 * todo: finish implementation #332
	 * offset 2652
	 */
	ThermistorConf auxTempSensor2;
	/**
	 * offset 2684
	 */
	int16_t fuelClosedLoopCltThreshold;
	/**
	 * offset 2686
	 */
	int16_t fuelClosedLoopTpsThreshold;
	/**
	 * offset 2688
	 */
	int16_t fuelClosedLoopRpmThreshold;
	/**
	 * offset 2690
	 */
	int16_t etbFreq;
	/**
	 * offset 2692
	 */
	pid_s fuelClosedLoopPid;
	/**
	 * offset 2712
	 */
	float fuelClosedLoopAfrHighThreshold;
	/**
	 * offset 2716
	 */
	pin_mode_e stepperEnablePinMode;
	/**
	 * lampda input
	 * offset 2720
	 */
	adc_channel_e cj125ua;
	/**
	 * heater input
	 * offset 2724
	 */
	adc_channel_e cj125ur;
	/**
	 * offset 2728
	 */
	cfg_float_t_1f timing_offset_cylinder[IGNITION_PIN_COUNT];
	/**
	 * offset 2776
	 */
	pin_input_mode_e brakePedalPinMode;
	/**
	 * offset 2780
	 */
	float idlePidActivationTime;
	/**
	 * offset 2784
	 */
	spi_device_e sdCardSpiDevice;
	/**
	 * offset 2788
	 */
	pin_mode_e spi3SckMode;
	/**
	 * offset 2792
	 */
	pin_mode_e spi3MosiMode;
	/**
	 * offset 2796
	 */
	pin_mode_e spi3MisoMode;
	/**
	 * offset 2800
	 */
	pin_mode_e spi1SckMode;
	/**
	 * offset 2804
	 */
	pin_mode_e spi1MosiMode;
	/**
	 * offset 2808
	 */
	pin_mode_e spi1MisoMode;
	/**
	 * offset 2812
	 */
	brain_pin_e brakePedalPin;
	/**
	 * offset 2816
	 */
	pid_s auxPid[AUX_PID_COUNT];
	/**
	 * offset 2896
	 */
	oil_pressure_config_s oilPressure;
	/**
	 * offset 2916
	 */
	spi_device_e accelerometerSpiDevice;
	/**
	 * offset 2920
	 */
	float fsioCurve1Bins[FSIO_CURVE_16];
	/**
	 * offset 2984
	 */
	float fsioCurve1[FSIO_CURVE_16];
	/**
	 * offset 3048
	 */
	float fsioCurve2Bins[FSIO_CURVE_16];
	/**
	 * offset 3112
	 */
	float fsioCurve2[FSIO_CURVE_16];
	/**
	 * offset 3176
	 */
	float fsioCurve3Bins[FSIO_CURVE_8];
	/**
	 * offset 3208
	 */
	float fsioCurve3[FSIO_CURVE_8];
	/**
	 * offset 3240
	 */
	float fsioCurve4Bins[FSIO_CURVE_8];
	/**
	 * offset 3272
	 */
	float fsioCurve4[FSIO_CURVE_8];
	/**
	 * offset 3304
	 */
	brain_input_pin_e flexFuelSensor;
	/**
	 * offset 3308
	 */
	brain_pin_e test557pin;
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * offset 3312
	 */
	float crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	 * offset 3328
	 */
	float crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	/**
	 * todo: more comments
	 * offset 3344
	 */
	brain_pin_e servoOutputPins[SERVO_COUNT];
	/**
	 * offset 3376
	 */
	int16_t coastingFuelCutRpmHigh;
	/**
	 * offset 3378
	 */
	int16_t coastingFuelCutRpmLow;
	/**
	 * percent between 0 and 100
	 * offset 3380
	 */
	int16_t coastingFuelCutTps;
	/**
	 * Fuel cutoff is deactivated if CLT<threshold
	 * offset 3382
	 */
	int16_t coastingFuelCutClt;
	/**
	 * Increases PID reaction for RPM<target by adding extra percent to PID-error
	 * offset 3384
	 */
	int16_t pidExtraForLowRpm;
	/**
	 * offset 3386
	 */
	int16_t unusedInt16;
	/**
	 * CLT-based idle position for coasting (used in Auto-PID Idle mode)
	 * offset 3388
	 */
	float iacCoastingBins[CLT_CURVE_SIZE];
	/**
	 *  CLT-based idle position for coasting (used in Auto-PID Idle mode)
	 * offset 3452
	 */
	float iacCoasting[CLT_CURVE_SIZE];
	/**
	 * offset 3516
	 */
	int unusedEnd[741];
	/** total size 6480*/
} engine_configuration_s;

typedef struct {
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
	 * By the way 2.081989116 kg/h = 1 ft³/m
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
} persistent_config_s;

#endif
// end
// this section was generated automatically by ConfigDefinition.jar based on rusefi_config.txt Thu Apr 26 02:12:36 EDT 2018
