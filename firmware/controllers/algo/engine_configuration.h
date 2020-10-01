/**
 * @file	engine_configuration.h
 * @brief	Main engine configuration data structure.
 *
 * @date Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "persistent_configuration.h"
#include "config_override_helper.h"

// From config/boards/<board name>
#include "config_overrides.h"

#ifndef DEFAULT_ENGINE_TYPE
#define DEFAULT_ENGINE_TYPE DEFAULT_FRANKENSO
#endif

#define CLT_MANUAL_IDLE_CORRECTION config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE
#define WARMUP_CLT_EXTRA_FUEL_CURVE config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE
#define IAT_FUEL_CORRECTION_CURVE config->iatFuelCorrBins, config->iatFuelCorr, IAT_CURVE_SIZE
#define INJECTOR_LAG_CURVE engineConfiguration->injector.battLagCorrBins, engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE

#define MOCK_UNDEFINED -1

float getRpmMultiplier(operation_mode_e mode);
void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode);

void prepareVoidConfiguration(engine_configuration_s *activeConfiguration);
void setTargetRpmCurve(int rpm DECLARE_CONFIG_PARAMETER_SUFFIX);
int getTargetRpmForIdleCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setAfrMap(afr_table_t table, float value);
/**
 * See also setLinearCurve()
 */
void setMap(fuel_table_t table, float value);
void setWholeIgnitionIatCorr(float value DECLARE_CONFIG_PARAMETER_SUFFIX);
void setFuelTablesLoadBin(float minValue, float maxValue DECLARE_CONFIG_PARAMETER_SUFFIX);
void setWholeIatCorrTimingTable(float value DECLARE_CONFIG_PARAMETER_SUFFIX);
void setWholeTimingTable_d(angle_t value DECLARE_CONFIG_PARAMETER_SUFFIX);
#define setWholeTimingTable(x) setWholeTimingTable_d(x PASS_CONFIG_PARAMETER_SUFFIX);
void setConstantDwell(floatms_t dwellMs DECLARE_CONFIG_PARAMETER_SUFFIX);

// needed by bootloader
void setDefaultBasePins(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setDefaultSdCardParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void onBurnRequest(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void wipeStrings(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void rememberCurrentConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void incrementGlobalConfigurationVersion(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration);
void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration);

#define MEMCPY(dest, src) {memcpy(dest, src, sizeof(src));}

void emptyCallbackWithConfiguration(engine_configuration_s * engine);
void setDefaultFrankensoConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);

typedef void (*configuration_callback_t)(engine_configuration_s*);

#ifdef __cplusplus
// because of 'Logging' class parameter these functions are visible only to C++ code but C code
void resetConfigurationExt(Logging * logger, configuration_callback_t boardCallback, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX);
void resetConfigurationExt(Logging * logger, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX);
#endif /* __cplusplus */


const ConfigOverrides& getConfigOverrides();

// This macro is for reading config values that may be overridden
// for a particular board, and their values from the flash config
// ignored.  For example, a user shouldn't be able to pick which SPI device
// the SD card is wired to on a board with hard wired SD card.
//
// To override a value
//  - Add it to the list below (allow_override_for)
//  - Add it to the ConfigOverrides struct for your board
//  - Add the override value to the function getConfigOverrides in board_configuration.cpp
#define CONFIG_OVERRIDE(__x__) (			\
	(CONFIG_HAS_OVERRIDE(__x__)) \
	? (CONFIG( __x__ )) : ( getConfigOverrides().__x__ ))

// This declares possible config fields we want to be able to override
namespace efi::overrides {
	// List every config entry we want to be able to override
	allow_override_for(canTxPin);
	allow_override_for(canRxPin);
}
