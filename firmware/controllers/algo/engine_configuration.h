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

	Internal_ForceMyEnumIntSize_analog_chart_mode = ENUM_SIZE_HACK,
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

	Internal_ForceMyEnumIntSize_timing_mode = ENUM_SIZE_HACK,
} timing_mode_e;

typedef enum {
	CD_OFF = 0,
	CD_USE_CAN1 = 1,
	CD_USE_CAN2 = 2,

	Internal_ForceMyEnumIntSize_can_device_mode = ENUM_SIZE_HACK,
} can_device_mode_e;

typedef struct {
	adc_channel_e afrAdcChannel;
	float v1;
	float value1;
	float v2;
	float value2;
} afr_sensor_s;

#define DWELL_COUNT 8

#define CRANKING_CURVE_SIZE 8

/**
 * @brief Trigger wheel(s) configuration
 */
typedef struct {
	trigger_type_e triggerType;

	int customIsSynchronizationNeeded;

	int customTotalToothCount;
	int customSkippedToothCount;

	float customSyncRatioFrom;
	float customSyncRatioTo;

	int customUseRiseEdge;

} trigger_config_s;


#define HW_MAX_ADC_INDEX 16

typedef struct {
	// WARNING: by default, our small enums are ONE BYTE. this one is made 4-byte with the 'ENUM_SIZE_HACK' hack
	brain_pin_e idleValvePin;
	pin_output_mode_e idleValvePinMode;

	brain_pin_e fuelPumpPin;
	pin_output_mode_e fuelPumpPinMode;

	brain_pin_e injectionPins[INJECTION_PIN_COUNT];
	pin_output_mode_e injectionPinMode;

	brain_pin_e ignitionPins[IGNITION_PIN_COUNT];
	pin_output_mode_e ignitionPinMode;

	brain_pin_e malfunctionIndicatorPin;
	pin_output_mode_e malfunctionIndicatorPinMode;

	brain_pin_e fanPin;
	pin_output_mode_e fanPinMode;

	brain_pin_e electronicThrottlePin1;
	pin_output_mode_e electronicThrottlePin1Mode;

	/**
	 * some cars have a switch to indicate that clutch pedal is all the way down
	 */
	brain_pin_e clutchDownPin;
	/**
	 * some cars have a switch to indicate that clutch pedal is all the way down
	 */
	pin_input_mode_e clutchDownPinMode;

	brain_pin_e alternatorControlPin;
	pin_output_mode_e alternatorControlPinMode;

	brain_pin_e HD44780_rs;
	brain_pin_e HD44780_e;
	brain_pin_e HD44780_db4;
	brain_pin_e HD44780_db5;
	brain_pin_e HD44780_db6;
	brain_pin_e HD44780_db7;

	brain_pin_e gps_rx_pin;
	brain_pin_e gps_tx_pin;

	int idleSolenoidFrequency;

	int triggerSimulatorFrequency;


	/**
	 * Digital Potentiometer is used by stock ECU stimulation code
	 */
	spi_device_e digitalPotentiometerSpiDevice;
	brain_pin_e digitalPotentiometerChipSelect[DIGIPOT_COUNT];

	adc_channel_mode_e adcHwChannelEnabled[HW_MAX_ADC_INDEX];

	// offset 5760
	brain_pin_e triggerInputPins[3];
	// offset 5772
	brain_pin_e mainRelayPin;

	int idleThreadPeriod;
	int consoleLoopPeriod;
	int lcdThreadPeriod;
	int tunerStudioThreadPeriod;
	int generalPeriodicThreadPeriod;

	// offset 5796
	uint32_t tunerStudioSerialSpeed;

	brain_pin_e boardTestModeJumperPin;

	can_device_mode_e canDeviceMode;
	brain_pin_e canTxPin;
	brain_pin_e canRxPin;

	brain_pin_e triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	pin_output_mode_e triggerSimulatorPinModes[TRIGGER_SIMULATOR_PIN_COUNT];

	brain_pin_e o2heaterPin;
	pin_output_mode_e o2heaterPinModeTodO;

	unsigned int is_enabled_spi_1 : 1; // bit 0
	unsigned int is_enabled_spi_2 : 1; // bit 1
	unsigned int is_enabled_spi_3 : 1; // bit 2
	unsigned int isSdCardEnabled : 1; // bit 3
	unsigned int isFastAdcEnabled : 1; // bit 4
	unsigned int isEngineControlEnabled : 1; // bit 5
	unsigned int isHip9011Enabled : 1; // bit 6

	brain_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	/**
	 * default or inverted input
	 */
	uint8_t logicAnalyzerMode[LOGIC_ANALYZER_CHANNEL_COUNT];

	int unrealisticRpmThreashold;

	pin_output_mode_e mainRelayPinMode;

	egt_cs_array_t max31855_cs;

	spi_device_e max31855spiDevice;

	brain_pin_e fsioPins[LE_COMMAND_COUNT];
	pin_output_mode_e gpioPinModes[LE_COMMAND_COUNT];

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
	int unusedbs[7];

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
	int16_t analogChartMode;
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
	cranking_parameters_s crankingSettings;
	/**
	 * @see hasMapSensor
	 * @see isMapAveragingEnabled
	 * offset 356
	*/
	MAP_sensor_config_s map;

	// todo: merge with channel settings, use full-scale Thermistor here!
	ThermistorConf cltThermistorConf; // size 40 (10*4), offset 336
	ThermistorConf iatThermistorConf; // size 40, offset 376

	float sparkDwellBins[DWELL_COUNT]; // offset 580
	float sparkDwell[DWELL_COUNT];

	float ignitionLoadBins[IGN_LOAD_COUNT];
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
	 */
	float fixedModeTiming;

	float injectorLag;


	float fuelLoadBins[FUEL_LOAD_COUNT];
	// RPM is float and not integer in order to use unified methods for interpolation
	float fuelRpmBins[FUEL_RPM_COUNT]; //

	/**
	 * Engine displacement, in liters
	 * see also cylindersCount
	 */
	float displacement;
	int rpmHardLimit;

	injection_mode_e crankingInjectionMode;
	injection_mode_e injectionMode;

	/**
	 * Inside rusEfi all the angles are handled in relation to the trigger synchronization event
	 * which depends on the trigger shape and has nothing to do wit Top Dead Center (TDC)
	 *
	 * For engine configuration humans need angles from TDC.
	 *
	 * This field is the angle between Top Dead Center (TDC) and the first trigger event.
	 * Knowing this angle allows us to control timing and other angles in reference to TDC.
	 */
	float globalTriggerAngleOffset;
	/**
	 * We have 3.3V ADC and most of the analog input signals are 5V, this forces us to use
	 * voltage dividers on the input circuits. This parameter holds the coefficient of these dividers.
	 * see also vbattDividerCoeff
	 */
	float analogInputDividerCoefficient;

	/**
	 * This setting controls which algorithm is used for ENGINE LOAD
	 */
	engine_load_mode_e algorithm;

	/**
	 * see
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

	// offset 960
	brain_pin_e vehicleSpeedSensorInputPin;

	/**
	 * This coefficient translates vehicle speed input frequency (in Hz) into vehicle speed
	 * (in km/h)
	 * offset 964
	 */
	float vehicleSpeedCoef;
	// offset 968
	can_nbc_e canNbcType;
	// offset 972
	int canSleepPeriod;

	int cylindersCount;

	ignition_mode_e ignitionMode;
	firing_order_e firingOrder;

	/**
	 * This magic constant is about four-stroke engines with camshaft position sensors.
	 * On any four stroke engine, each revolution of the camshaft is two revolutions
	 * of the crankshaft. If camshaft position is our primary sensor, we use this multiplier
	 * to convert from camshaft angles to crankshaft angles. All angels across the system
	 * should be crankshaft angles.
	 */

	float rpmMultiplier;

	display_mode_e displayMode;

	log_format_e logFormat;

	int firmwareVersion;
	int HD44780width;
	int HD44780height;

	adc_channel_e tpsAdcChannel;
	int overrideCrankingIgnition;
	int analogChartFrequency;

	trigger_config_s triggerConfig;

	spi_device_e hip9011SpiDevice;
	adc_channel_e vbattAdcChannel;

	// offset 1060
	float globalFuelCorrection;

	// todo: merge with channel settings, use full-scale Thermistor!
	adc_channel_e cltAdcChannel;
	adc_channel_e iatAdcChannel;
	adc_channel_e mafAdcChannel;

	/**
	 * @see hasAfrSensor
	 */
	afr_sensor_s afrSensor;

	/**
	 * this is about deciding when the injector starts it's squirt
	 */
	float injectionAngle;

	float crankingTimingAngle;

	float diffLoadEnrichmentCoef;

	/**
	 * @see hasBaroSensor
	 */
	air_pressure_sensor_config_s baroSensor;

	float veLoadBins[FUEL_LOAD_COUNT];
	float veRpmBins[FUEL_RPM_COUNT];
	float afrLoadBins[FUEL_LOAD_COUNT];
	float afrRpmBins[FUEL_RPM_COUNT];

	// the large tables are always in the end - that's related to TunerStudio paging implementation
	fuel_table_t fuelTable; // size 1024
	ignition_table_t ignitionTable; // size 1024

	fuel_table_t veTable; // size 1024
	fuel_table_t afrTable; // size 1024

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
