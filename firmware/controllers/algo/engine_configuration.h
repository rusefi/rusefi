/**
 * @file	engine_configuration.h
 * @brief	Main engine configuration data structure.
 *
 * @date Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ENGINE_CONFIGURATION_H_
#define ENGINE_CONFIGURATION_H_

#include "efifeatures.h"
#include "crc.h"
#include "rusefi_enums.h"
#include "global.h"
#include "rusefi_types.h"
#include "engine_configuration_generated_structures.h"

#define CLT_MANUAL_IDLE_CORRECTION config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE
#define WARMUP_CLT_EXTRA_FUEL_CURVE config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE
#define IAT_FUEL_CORRECTION_CURVE config->iatFuelCorrBins, config->iatFuelCorr, IAT_CURVE_SIZE
#define INJECTOR_LAG_CURVE engineConfiguration->injector.battLagCorrBins, engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE

#define MOCK_UNDEFINED -1

// WARNING: by default, our small enums are ONE BYTE. this one is made 4-byte with the 'ENUM_32_BITS' hack

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

float getRpmMultiplier(operation_mode_e mode);
void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode);

typedef struct {
	int version;
	int size;
	persistent_config_s persistentConfiguration;
	crc_t value;
} persistent_config_container_s;

void prepareVoidConfiguration(engine_configuration_s *activeConfiguration);
void setDefaultConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setTargetRpmCurve(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
void setAfrMap(afr_table_t table, float value);
/**
 * See also setLinearCurve()
 */
void setMap(fuel_table_t table, float value);
void setWholeFuelMap(float value DECLARE_ENGINE_PARAMETER_SUFFIX);
void setWholeIgnitionIatCorr(float value DECLARE_ENGINE_PARAMETER_SUFFIX);
void setFuelTablesLoadBin(float minValue, float maxValue DECLARE_ENGINE_PARAMETER_SUFFIX);
void setWholeIatCorrTimingTable(float value DECLARE_ENGINE_PARAMETER_SUFFIX);
void setWholeTimingTable(angle_t value DECLARE_ENGINE_PARAMETER_SUFFIX);
void setConstantDwell(floatms_t dwellMs DECLARE_ENGINE_PARAMETER_SUFFIX);
void printFloatArray(const char *prefix, float array[], int size);

// needed by bootloader
void setDefaultBasePins(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setDefaultSerialParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setDefaultSdCardParameters(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void rememberCurrentConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void incrementGlobalConfigurationVersion(DECLARE_ENGINE_PARAMETER_SIGNATURE);
int getGlobalConfigurationVersion(void);

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration);
void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration);

void copyTargetAfrTable(fuel_table_t const source, afr_table_t destination);
void copyFuelTable(fuel_table_t const source, fuel_table_t destination);
void copyTimingTable(ignition_table_t const source, ignition_table_t destination);

#endif /* ENGINE_CONFIGURATION_H_ */
