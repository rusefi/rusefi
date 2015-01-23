/**
 * @file	engine_configuration.h
 * @brief	Main engine configuration data structure.
 *
 * @date Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef ENGINE_CONFIGURATION_H_
#define ENGINE_CONFIGURATION_H_

#include "efifeatures.h"
#include "crc.h"
#include "sensor_types.h"
#include "can_header.h"
#include "rusefi_enums.h"
#include "global.h"

#define MOCK_UNDEFINED -1

#define INJECTION_PIN_COUNT 12
#define IGNITION_PIN_COUNT 12

#define MAX31855_CS_COUNT 8

#define JOYSTICK_PIN_COUNT 5

#define FUEL_RPM_COUNT 16
#define FUEL_LOAD_COUNT 16

#define LE_COMMAND_LENGTH 200
#define LE_COMMAND_COUNT 16

typedef char le_formula_t[LE_COMMAND_LENGTH];

#define CLT_CURVE_SIZE 16
#define IAT_CURVE_SIZE 16
#define VBAT_INJECTOR_CURVE_SIZE 8

#define IGN_LOAD_COUNT 16
#define IGN_RPM_COUNT 16

// todo: merge these two types together?
typedef float fuel_table_t[FUEL_LOAD_COUNT][FUEL_RPM_COUNT];
typedef float ignition_table_t[IGN_LOAD_COUNT][IGN_RPM_COUNT];

// todo: rename this structure one all tables migrated
typedef struct {
	float loadBins[IGN_LOAD_COUNT];
	float rpmBins[IGN_RPM_COUNT];
	ignition_table_t map;
} full_i_table_s;

typedef brain_pin_e egt_cs_array_t[MAX31855_CS_COUNT];

#define DWELL_CURVE_SIZE 8

typedef enum {
	AC_OFF = 0,
	/**
	 * You would use this value if you want to see a detailed graph of your trigger events
	 */
	AC_TRIGGER = 1, AC_MAP = 2,

	Internal_ForceMyEnumIntSize_analog_chart = ENUM_32_BITS,
} analog_chart_e;

typedef enum {
	/**
	 * This is the default mode in which ECU controls timing dynamically
	 */
	TM_DYNAMIC = 0,
	/**
	 * Fixed timing is useful while you are playing with a timing gun - you need to have fixed
	 * timing if you want to install your distributor at some specific angle
	 */
	TM_FIXED = 1,

	Internal_ForceMyEnumIntSize_timing_mode = ENUM_32_BITS,
} timing_mode_e;

typedef enum {
	CD_OFF = 0,
	CD_USE_CAN1 = 1,
	CD_USE_CAN2 = 2,

	Internal_ForceMyEnumIntSize_can_device_mode = ENUM_32_BITS,
} can_device_mode_e;


#define DWELL_COUNT 8

#define CRANKING_CURVE_SIZE 8

#define HW_MAX_ADC_INDEX 16

// WARNING: by default, our small enums are ONE BYTE. this one is made 4-byte with the 'ENUM_32_BITS' hack

typedef struct {
	/**
	 * offset 0
	*/
	brain_pin_e idleValvePin;
	/**
	 * offset 4
	*/
	pin_output_mode_e idleValvePinMode;
	/**
	 * offset 8
	*/
	brain_pin_e fuelPumpPin;
	/**
	 * offset 12
	*/
	pin_output_mode_e fuelPumpPinMode;
	/**
	 * offset 16
	*/
	brain_pin_e injectionPins[INJECTION_PIN_COUNT];
	/**
	 * offset 64
	*/
	pin_output_mode_e injectionPinMode;
	/**
	 * offset 68
	*/
	brain_pin_e ignitionPins[IGNITION_PIN_COUNT];
	/**
	 * offset 116
	*/
	pin_output_mode_e ignitionPinMode;
	/**
	 * offset 120
	*/
	brain_pin_e malfunctionIndicatorPin;
	/**
	 * offset 124
	*/
	pin_output_mode_e malfunctionIndicatorPinMode;
	/**
	 * offset 128
	*/
	brain_pin_e fanPin;
	/**
	 * offset 132
	*/
	pin_output_mode_e fanPinMode;
	/**
	 * offset 136
	*/
	brain_pin_e electronicThrottlePin1;
	/**
	 * offset 140
	*/
	pin_output_mode_e electronicThrottlePin1Mode;
	/**
	 * some cars have a switch to indicate that clutch pedal is all the way down
	 * offset 144
	*/
	brain_pin_e clutchDownPin;
	/**
	 * offset 148
	*/
	pin_input_mode_e clutchDownPinMode;
	/**
	 * offset 152
	*/
	brain_pin_e alternatorControlPin;
	/**
	 * offset 156
	*/
	pin_output_mode_e alternatorControlPinMode;
	/**
	 * offset 160
	*/
	brain_pin_e HD44780_rs;
	/**
	 * offset 164
	*/
	brain_pin_e HD44780_e;
	/**
	 * offset 168
	*/
	brain_pin_e HD44780_db4;
	/**
	 * offset 172
	*/
	brain_pin_e HD44780_db5;
	/**
	 * offset 176
	*/
	brain_pin_e HD44780_db6;
	/**
	 * offset 180
	*/
	brain_pin_e HD44780_db7;
	/**
	 * offset 184
	*/
	brain_pin_e gps_rx_pin;
	/**
	 * offset 188
	*/
	brain_pin_e gps_tx_pin;
	/**
	 * offset 192
	*/
	int idleSolenoidFrequency;
	/**
	 * offset 196
	*/
	int triggerSimulatorFrequency;
	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 * offset 200
	*/
	spi_device_e digitalPotentiometerSpiDevice;
	/**
	 * offset 204
	*/
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];
	/**
	 * offset 220
	*/
	adc_channel_mode_e adcHwChannelEnabled[HW_MAX_ADC_INDEX];
	/**
	 * offset 284
	*/
	brain_pin_e triggerInputPins[3];
	/**
	 * offset 296
	*/
	brain_pin_e mainRelayPin;
	/**
	 * offset 300
	*/
	int idleThreadPeriod;
	/**
	 * offset 304
	*/
	int consoleLoopPeriod;
	/**
	 * offset 308
	*/
	int lcdThreadPeriod;
	/**
	 * offset 312
	*/
	int tunerStudioThreadPeriod;
	/**
	 * offset 316
	*/
	int generalPeriodicThreadPeriod;
	/**
	 * offset 320
	*/
	uint32_t tunerStudioSerialSpeed;
	/**
	 * offset 324
	*/
	brain_pin_e boardTestModeJumperPin;
	/**
	 * offset 328
	*/
	can_device_mode_e canDeviceMode;
	/**
	 * offset 332
	*/
	brain_pin_e canTxPin;
	/**
	 * offset 336
	*/
	brain_pin_e canRxPin;
	/**
	 * offset 340
	*/
	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 352
	*/
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];
	/**
	 * offset 364
	*/
	brain_pin_e o2heaterPin;
	/**
	 * offset 368
	*/
	pin_output_mode_e o2heaterPinModeTodO;

	/** offset 372 bit 0 */
	uint32_t is_enabled_spi_1 : 1;
	/** offset 372 bit 1 */
	uint32_t is_enabled_spi_2 : 1;
	/** offset 372 bit 2 */
	uint32_t is_enabled_spi_3 : 1;
	/** offset 372 bit 3 */
	uint32_t isSdCardEnabled : 1;
	/** offset 372 bit 4 */
	uint32_t isFastAdcEnabled : 1;
	/** offset 372 bit 5 */
	uint32_t isEngineControlEnabled : 1;
	/** offset 372 bit 6 */
	uint32_t isHip9011Enabled : 1;
	/**
	 * offset 376
	*/
	brain_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * default or inverted input
	 * offset 392
	*/
	uint8_t logicAnalyzerMode[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * offset 396
	*/
	int unrealisticRpmThreashold;
	/**
	 * offset 400
	*/
	pin_output_mode_e mainRelayPinMode;
	/**
	 * offset 404
	*/
	egt_cs_array_t max31855_cs;
	/**
	 * offset 408
	*/
	spi_device_e max31855spiDevice;
	/**
	 * offset 412
	*/
	brain_pin_e fsioPins[LE_COMMAND_COUNT];
	/**
	 * offset 476
	*/
	pin_output_mode_e gpioPinModes[LE_COMMAND_COUNT];
	/**
	 * offset 540
	*/
	brain_pin_e joystickPins[JOYSTICK_PIN_COUNT];

	/**
	 * This pin is used for debugging - snap a logic analyzer on it and see if it's ever high
	 */
	brain_pin_e triggerErrorPin;
	pin_output_mode_e triggerErrorPinMode;

	/**
	 * value between 0 and 1
	 */
	float idleSolenoidPwm;

	brain_pin_e acRelayPin;
	pin_output_mode_e acRelayPinMode;

	int16_t fsioFrequency[LE_COMMAND_COUNT];

	brain_pin_e hip9011CsPin;
	brain_pin_e hip9011IntHoldPin;
	brain_pin_e hip9011OutPin;

	float fsio_setting[LE_COMMAND_COUNT];
	brain_pin_e spi1mosiPin;
	brain_pin_e spi1misoPin;
	brain_pin_e spi1sckPin;
	brain_pin_e spi2mosiPin;
	brain_pin_e spi2misoPin;
	brain_pin_e spi2sckPin;
	brain_pin_e spi3mosiPin;
	brain_pin_e spi3misoPin;
	brain_pin_e spi3sckPin;

	// offset 6228
	float hip9011Gain;
	// offset 6232
	brain_pin_e joystickCenterPin;
	// offset 6236
	brain_pin_e joystickAPin;
	// offset 6240
	brain_pin_e joystickBPin;
	// offset 6244
	brain_pin_e joystickCPin;
	// offset 6248
	brain_pin_e joystickDPin;
	// offset 6252
	analog_chart_e analogChartMode;
	int unusedbs[6];

	le_formula_t le_formulas[LE_COMMAND_COUNT];

} board_configuration_s;

// 19010105 decimal
#define HEADER_MAGIC_NUMBER 0x1221239

/**
 * @brief	Engine configuration.
 * 		Values in this data structure are adjustable and persisted in on-board flash RAM.
 *
 *  The offsets are tracked using
 *  https://docs.google.com/spreadsheet/ccc?key=0AiAmAn6tn3L_dGJXZDZOcVVhaG9SaHZKU1dyMjhEV0E
 *
 *  todo: currently the fields here are simply in the order in which they were implemented
 *  todo: re-arrange this structure one we have a stable code version
 */
typedef struct {
	// this section was generated by config_definition.jar on Thu Jan 15 15:59:00 EST 2015
	// begin
	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
	 * offset 0
	*/
	engine_type_e engineType;
	/**
	 * this magic number is used to make sure that what we read from Flash is in fact some configuration
	 * offset 4
	*/
	int headerMagicValue;
	/**
	 * offset 8
	*/
	float battInjectorLagCorrBins[VBAT_INJECTOR_CURVE_SIZE];
	/**
	 * offset 40
	*/
	float battInjectorLagCorr[VBAT_INJECTOR_CURVE_SIZE];
	/**
	 * offset 72
	*/
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	/**
	 * offset 136
	*/
	float cltFuelCorr[CLT_CURVE_SIZE];
	/**
	 * offset 200
	*/
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	/**
	 * offset 264
	*/
	float iatFuelCorr[IAT_CURVE_SIZE];
	/**
	 * Should the trigger emulator push data right into trigger input, eliminating the need for physical jumper wires?
	 * PS: Funny name, right? :)
	 * todo: make this a bit on some bit field
	 * offset 328
	*/
	int16_t directSelfStimulation;
	/**
	 * todo: extract these two fields into a structure
	 * todo: we need two sets of TPS parameters - modern ETBs have to sensors
	 * offset 330
	*/
	int16_t tpsMin;
	/**
	 * tpsMax value as 10 bit ADC value. Not Voltage!
	 * offset 332
	*/
	int16_t tpsMax;
	/**
	 * offset 334
	*/
	uint16_t unused334;

	/**
	 * todo: finish implementation. These values are used for TPS disconnect detection
	 * offset 336
	*/
	int16_t tpsErrorLowValue;
	/**
	 * offset 338
	*/
	int16_t tpsErrorHighValue;
	/**
	 * offset 340
	*/
	float primingSquirtDurationMs;
	/**
	 * 360 for two-stroke
	 * 720 for four-stroke
	 * offset 344
	*/
	int engineCycle;

	/**
	 * offset 348
	*/
	cranking_parameters_s cranking;
	/**
	 * @see hasMapSensor
	 * @see isMapAveragingEnabled
	 * offset 356
	*/
	MAP_sensor_config_s map;

	/**
	 * todo: merge with channel settings, use full-scale Thermistor here!
	 * offset 500
	*/
	ThermistorConf clt;
	/**
	 * offset 540
	*/
	ThermistorConf iat;
	/**
	 * offset 580
	*/
	float sparkDwellBins[DWELL_COUNT];
	/**
	 * offset 612
	*/
	float sparkDwell[DWELL_COUNT];
	/**
	 * offset 644
	*/
	float ignitionLoadBins[IGN_LOAD_COUNT];
	/**
	 * offset 708
	*/
	float ignitionRpmBins[IGN_RPM_COUNT];
	/**
	 * this value could be used to offset the whole ignition timing table by a constant
	 * offset 772
	*/
	float ignitionBaseAngle;
	/**
	 * While cranking (which causes battery voltage to drop) we can calculate dwell time in shaft
	 * degrees, not in absolute time as in running mode.
	 * offset 776
	*/
	float crankingChargeAngle;
	/**
	 * offset 780
	*/
	timing_mode_e timingMode;
	/**
	 * This value is used in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful for instance while adjusting distributor location
	 * offset 784
	*/
	float fixedModeTiming;
	/**
	 * offset 788
	*/
	float injectorLag;
	/**
	 * offset 792
	*/
	float fuelLoadBins[FUEL_LOAD_COUNT];
	/**
	 * RPM is float and not integer in order to use unified methods for interpolation
	 * offset 856
	*/
	float fuelRpmBins[FUEL_RPM_COUNT];
	/**
	 * Engine displacement, in liters
	 * see also cylindersCount
	 * offset 920
	*/
	float displacement;
	/**
	 * offset 924
	*/
	int rpmHardLimit;
	/**
	 * offset 928
	*/
	injection_mode_e crankingInjectionMode;
	/**
	 * offset 932
	*/
	injection_mode_e injectionMode;
	/**
	 * This field is the angle between Top Dead Center (TDC) and the first trigger event.
	 * Knowing this angle allows us to control timing and other angles in reference to TDC.
	 * offset 936
	*/
	float globalTriggerAngleOffset;
	/**
	 * This parameter holds the coefficient of input voltage dividers
	 * offset 940
	*/
	float analogInputDividerCoefficient;
	/**
	 * This setting controls which algorithm is used for ENGINE LOAD
	 * offset 944
	*/
	engine_load_mode_e algorithm;
	/**
	 * offset 948
	*/
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsuis
	 * offset 952
	*/
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsuis
	 * offset 956
	*/
	float fanOffTemperature;
	/**
	 * offset 960
	*/
	brain_pin_e vehicleSpeedSensorInputPin;
	/**
	 * This coefficient translates vehicle speed input frequency (in Hz) into vehicle speed, km/h
	 * offset 964
	*/
	float vehicleSpeedCoef;
	/**
	 * offset 968
	*/
	can_nbc_e canNbcType;
	/**
	 * offset 972
	*/
	int canSleepPeriod;
	/**
	 * offset 976
	*/
	int cylindersCount;
	/**
	 * offset 980
	*/
	ignition_mode_e ignitionMode;
	/**
	 * offset 984
	*/
	firing_order_e firingOrder;
	/**
	 * todo: refactor this, see operation_mode_e
	 * offset 988
	*/
	float rpmMultiplier;
	/**
	 * offset 992
	*/
	display_mode_e displayMode;
	/**
	 * offset 996
	*/
	log_format_e logFormat;
	/**
	 * offset 1000
	*/
	int firmwareVersion;
	/**
	 * offset 1004
	*/
	int HD44780width;
	/**
	 * offset 1008
	*/
	int HD44780height;
	/**
	 * offset 1012
	*/
	adc_channel_e tpsAdcChannel;
	/**
	 * offset 1016
	*/
	int overrideCrankingIgnition;
	/**
	 * offset 1020
	*/
	int analogChartFrequency;
	/**
	 * offset 1024
	*/
	trigger_config_s trigger;
	/**
	 * offset 1052
	*/
	spi_device_e hip9011SpiDevice;
	/**
	 * offset 1056
	*/
	adc_channel_e vbattAdcChannel;
	/**
	 * offset 1060
	*/
	float globalFuelCorrection;
	/**
	 * todo: merge with channel settings, use full-scale Thermistor!
	 * offset 1064
	*/
	adc_channel_e cltAdcChannel;
	/**
	 * offset 1068
	*/
	adc_channel_e iatAdcChannel;
	/**
	 * offset 1072
	*/
	adc_channel_e mafAdcChannel;
	/**
	 * @see hasAfrSensor
	 * offset 1076
	*/
	afr_sensor_s afrSensor;
	/**
	 * this is about deciding when the injector starts it's squirt
	 * offset 1096
	*/
	float injectionAngle;
	/**
	 * offset 1100
	*/
	float crankingTimingAngle;
	/**
	 * offset 1104
	*/
	float diffLoadEnrichmentCoef;
	/**
	 * @see hasBaroSensor
	 * offset 1108
	*/
	air_pressure_sensor_config_s baroSensor;
	/**
	 * offset 1124
	*/
	float veLoadBins[FUEL_LOAD_COUNT];
	/**
	 * offset 1188
	*/
	float veRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 1252
	*/
	float afrLoadBins[FUEL_LOAD_COUNT];
	/**
	 * offset 1316
	*/
	float afrRpmBins[FUEL_RPM_COUNT];
	/**
	 * offset 1380
	*/
	fuel_table_t fuelTable;
	/**
	 * offset 2404
	*/
	ignition_table_t ignitionTable;
	/**
	 * offset 3428
	*/
	fuel_table_t veTable;
	/**
	 * offset 4452
	*/
	fuel_table_t afrTable;

	board_configuration_s bc;

	// offset 6280
	/**
	 * @see isMapAveragingEnabled
	 */
	bool_t hasMapSensor : 1; // bit 0
	bool_t hasIatSensor : 1; // bit 1
	bool_t hasBaroSensor : 1; // bit 1
	bool_t hasAfrSensor : 1; // bit 2
	bool_t useConstantDwellDuringCranking : 1; // bit 3
	bool_t isDigitalChartEnabled : 1; // bit 4
	bool_t isCanEnabled : 1; // bit 5
	bool_t hasCltSensor : 1; // bit 6
	bool_t canReadEnabled : 1; // bit 7
	bool_t canWriteEnabled : 1; // bit 8
	bool_t hasVehicleSpeedSensor : 1; // bit 9
	bool_t isJoystickEnabled : 1; // bit 10
	bool_t isGpsEnabled : 1; // bit 11
	bool_t hasMafSensor : 1; // bit 12
	bool_t hasTpsSensor : 1; // bit 13

	/**
	 * see also
	 * offset 6284
	 */
	adc_channel_e hipOutputChannel;

	idle_mode_e idleMode;

	bool isInjectionEnabled : 1; // bit 0
	bool isIgnitionEnabled : 1; // bit 1
	bool isCylinderCleanupEnabled : 1; // bit 2
	bool secondTriggerChannelEnabled : 1; // bit 3
	bool needSecondTriggerInput : 1; // bit 4
	bool isMapAveragingEnabled : 1; // bit 5
	bool isMilEnabled : 1; // bit 6
	bool isFuelPumpEnabled : 1; // bit 7
	bool isTunerStudioEnabled : 1; // bit 8
	bool isWaveAnalyzerEnabled : 1; // bit 9
	bool isIdleThreadEnabled : 1; // bit 10
	bool isPrintTriggerSynchDetails : 1; // bit 11
	/**
	 * Usually if we have no trigger events that means engine is stopped
	 * Unless we are troubleshooting and spinning the engine by hand - this case a longer
	 * delay is needed
	 */
	bool isManualSpinningMode : 1; // bit 11

	uint32_t digitalChartSize;
	/**
	 * cc/min, cubic centimeter per minute
	 *
	 * By the way, g/s = 0.125997881 * (lb/hr)
	 * g/s = 0.125997881 * (cc/min)/10.5
	 * g/s = 0.0119997981 * cc/min
	 *
	 */
	float injectorFlow; // size 4

	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	float crankingFuelBins[CRANKING_CURVE_SIZE];

	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	float crankingCycleBins[CRANKING_CURVE_SIZE];

	float ignitionDwellForCrankingMs;

	int targetIdleRpm;

	/**
	 * A/C button input handled as analog input
	 */
	adc_channel_e acSwitchAdc;

	int ignMathCalculateAtIndex;

	int16_t acCutoffLowRpm;
	int16_t acCutoffHighRpm;

	int16_t acIdleRpmBump;
	int16_t unusedShort;

	adc_channel_e vRefAdcChannel;

	float knockDetectionWindowStart;
	float knockDetectionWindowEnd;

	/**
	 * Cylinder diameter, in mm.
	 */
	float cylinderBore;

	/**
	 * Some vehicles have a switch to indicate that clutch pedal is all the way up
	 */
	brain_pin_e clutchUpPin;
	pin_input_mode_e clutchUpPinMode;
	float hipThreshold;
	brain_pin_e fsioInputs[LE_COMMAND_COUNT];
	pin_input_mode_e fsioInputModes[LE_COMMAND_COUNT];
	int unused3[149];

	le_formula_t timingMultiplier;
	le_formula_t timingAdditive;

	fuel_table_t injectionPhase;


} engine_configuration_s;

void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode);
operation_mode_e getOperationMode(engine_configuration_s const *engineConfiguration);

typedef struct {
	engine_configuration_s engineConfiguration;
} persistent_config_s;

typedef struct {
	int version;
	int size;
	persistent_config_s persistentConfiguration;
	crc_t value;
} persistent_config_container_s;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void setDefaultConfiguration(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration);
void setMap(fuel_table_t table, float value);
void setWholeFuelMap(engine_configuration_s *engineConfiguration, float value);
void setWholeTimingTable(engine_configuration_s *engineConfiguration, float value);
void setConstantDwell(engine_configuration_s *engineConfiguration, float dwellMs);
void printFloatArray(const char *prefix, float array[], int size);

void incrementGlobalConfigurationVersion(void);
int getGlobalConfigurationVersion(void);

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration);
void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration);

#ifdef __cplusplus
}
#endif /* __cplusplus */

void copyFuelTable(fuel_table_t const source, fuel_table_t destination);
void copyTimingTable(ignition_table_t const source, ignition_table_t destination);

#endif /* ENGINE_CONFIGURATION_H_ */
