/**
 * @file	engine_configuration.h
 * @brief	Main engine configuration data structure.
 *
 * @date Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef ENGINE_CONFIGURATION_H_
#define ENGINE_CONFIGURATION_H_

#include "efifeatures.h"
#include "sensor_types.h"
#include "can_header.h"
#include "trigger_structure.h"
#include "event_registry.h"
#include "rusefi_enums.h"

typedef struct {
	float coolantTempMinC;
	float coolantTempMaxC;
	float fuelAtMinTempMs;
	float fuelAtMaxTempMs;

	/**
	 * This value controls what RPM values we consider 'cranking' (any RPM below 'crankingRpm')
	 * Anything above 'crankingRpm' would be 'running'
	 */
	short int crankingRpm;
} cranking_parameters_s;


/**
 * @brief Here we store information about which injector or spark should be fired when.
 */
typedef struct {
	ActuatorEventList crankingInjectionEvents;
	ActuatorEventList injectionEvents;
	ActuatorEventList ignitionEvents;
} EventHandlerConfiguration;

#define FUEL_RPM_COUNT 16
#define FUEL_LOAD_COUNT 16

#define CLT_CURVE_SIZE 16
#define IAT_CURVE_SIZE 16
#define VBAT_INJECTOR_CURVE_SIZE 8

#define IGN_LOAD_COUNT 16
#define IGN_RPM_COUNT 16

#define DWELL_CURVE_SIZE 8

typedef enum {
	AC_OFF = 0,
	/**
	 * You would use this value if you want to see a detailed graph of your trigger events
	 */
	AC_TRIGGER = 1,
	AC_MAP = 2,

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

typedef struct {
	int afrAdcChannel;
	float v1;
	float value1;
	float v2;
	float value2;
} afr_sensor_s;

#define DWELL_COUNT 8
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
	float injectorLag;	// size 4, offset 0
	float injectorFlow; // size 4, offset 4
	float battInjectorLagCorrBins[VBAT_INJECTOR_CURVE_SIZE]; // size 32, offset 8
	float battInjectorLagCorr[VBAT_INJECTOR_CURVE_SIZE]; // size 32, offset 40

	float cltFuelCorrBins[CLT_CURVE_SIZE]; // size 64, offset 72
	float cltFuelCorr[CLT_CURVE_SIZE]; // size 64, offset 136

	float iatFuelCorrBins[IAT_CURVE_SIZE]; // size 64, offset 200
	float iatFuelCorr[IAT_CURVE_SIZE]; // size 64, offset 264

	short int rpmHardLimit; // size 2, offset 328

	// todo: extract these two fields into a structure
	// todo: we need two sets of TPS parameters - modern ETBs have to sensors
	short int tpsMin; // size 2, offset 330
	// tpsMax value as 10 bit ADC value. Not Voltage!
	short int tpsMax; // size 2, offset 332
	short int analogChartMode;

	cranking_parameters_s crankingSettings;

	map_s map;

	// todo: merge with channel settings, use full-scale Thermistor here!
	ThermistorConf cltThermistorConf; // size 40 (10*4), offset 336
	ThermistorConf iatThermistorConf; // size 40, offset 376

	float sparkDwellBins[DWELL_COUNT]; // offset 580
	float sparkDwell[DWELL_COUNT];

	float ignitionTable[IGN_LOAD_COUNT][IGN_RPM_COUNT];
	float ignitionLoadBins[IGN_LOAD_COUNT]; // offset 3450
	float ignitionRpmBins[IGN_RPM_COUNT]; // offset 3542

	/**
	 * this value could be used to offset the whole ignition timing table by a constant
	 */
	float ignitionOffset;

	/**
	 * While cranking (which causes battery voltage to drop) we can calculate dwell time in shaft
	 * degrees, not in absolute time as in running mode.
	 */
	float crankingChargeAngle;

	timing_mode_e timingMode;
	/**
	 * This value is used in 'fixed timing' mode, i.e. constant timing
	 * This mode is useful for instance while adjusting distributor location
	 */
	float fixedModeTiming;

	// WARNING: by default, our small enums are ONE BYTE. but if the are surrounded by non-enums - alignments do the trick
	engine_type_e engineType;


	float fuelTable[FUEL_LOAD_COUNT][FUEL_RPM_COUNT]; // size 1024, offset 1816
	float fuelLoadBins[FUEL_LOAD_COUNT]; // offset 2840
	// RPM is float and not integer in order to use unified methods for interpolation
	float fuelRpmBins[FUEL_RPM_COUNT]; // offset 3542

	// WARNING: by default, our small enums are ONE BYTE. this one is made 4-byte with the 'ENUM_SIZE_HACK' hack
	pin_output_mode_e injectionPinMode;
	pin_output_mode_e ignitionPinMode;
	pin_output_mode_e idlePinMode;
	pin_output_mode_e fuelPumpPinMode;
	pin_output_mode_e malfunctionIndicatorPinMode;

	/**
	 * This is the angle between Top Dead Center (TDC) and the first trigger event.
	 * Knows this angle allows us to control timing and other angles in reference to TDC.
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
	engine_load_mode_e engineLoadMode;

	/**
	 * see
	 */
	float vbattDividerCoeff;
	/**
	 * Cooling fan turn-on temperature threshold, in Celsuis
	 */
	float fanOnTemperature;
	/**
	 * Cooling fan turn-off temperature threshold, in Celsuis
	 */
	float fanOffTemperature;

	int canReadEnabled;
	int canWriteEnabled;
	can_nbc_e can_nbc_type;
	int can_sleep_period;

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

	int tpsAdcChannel;
	brain_pin_e ignitionPins[12];

	trigger_config_s triggerConfig;

	int needSecondTriggerInput;
	brain_pin_e injectionPins[12];
	brain_pin_e fuelPumpPin;
	brain_pin_e idleValvePin;
	int vBattAdcChannel;

	float globalFuelCorrection;

	// todo: merge with channel settings, use full-scale Thermistor!
	int cltAdcChannel;
	int iatAdcChannel;
	int mafAdcChannel;

	afr_sensor_s afrSensor;

	float injectionOffset;

} engine_configuration_s;

/**
 * this part of the structure is separate just because so far
 * these fields are not integrated with Tuner Studio. Step by step :)
 */
typedef struct {
	int hasMapSensor;
	int hasCltSensor;

	Thermistor iat;
	Thermistor clt;


	int crankAngleRange;


	trigger_shape_s triggerShape;

	cranking_ignition_mode_e crankingIgnitionMode;

	EventHandlerConfiguration engineEventConfiguration;
} engine_configuration2_s;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

char* getConfigurationName(engine_configuration_s *engineConfiguration);
void setDefaultConfiguration(engine_configuration_s *engineConfiguration);
void setConstantDwell(engine_configuration_s *engineConfiguration, float dwellMs);
void setDefaultNonPersistentConfiguration(engine_configuration2_s *engineConfiguration2);
void printConfiguration(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2);
void printFloatArray(char *prefix, float array[], int size);

void resetConfigurationExt(engine_type_e engineType, engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2);
void applyNonPersistentConfiguration(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2, engine_type_e engineType);

void incrementGlobalConfigurationVersion(void);
int getGlobalConfigurationVersion(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* ENGINE_CONFIGURATION_H_ */
