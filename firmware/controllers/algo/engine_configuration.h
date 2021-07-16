/**
 * @file	engine_configuration.h
 * @brief	Main engine configuration data structure.
 *
 * @date Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "persistent_configuration.h"

#ifndef DEFAULT_ENGINE_TYPE
#define DEFAULT_ENGINE_TYPE DEFAULT_FRANKENSO
#endif

#define CLT_MANUAL_IDLE_CORRECTION config->cltIdleCorrBins, config->cltIdleCorr, CLT_CURVE_SIZE
#define WARMUP_CLT_EXTRA_FUEL_CURVE config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE
#define IAT_FUEL_CORRECTION_CURVE config->iatFuelCorrBins, config->iatFuelCorr, IAT_CURVE_SIZE
#define INJECTOR_LAG_CURVE engineConfiguration->injector.battLagCorrBins, engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE

#define MOCK_UNDEFINED -1

void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode);

void prepareVoidConfiguration(engine_configuration_s *activeConfiguration);
void setTargetRpmCurve(int rpm DECLARE_CONFIG_PARAMETER_SUFFIX);
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
void incrementGlobalConfigurationVersion(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration);
void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration);

void emptyCallbackWithConfiguration(engine_configuration_s * engine);
void setDefaultFrankensoConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);

typedef void (*configuration_callback_t)(engine_configuration_s*);

#ifdef __cplusplus
// because of 'Logging' class parameter these functions are visible only to C++ code but C code
void loadConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void resetConfigurationExt(configuration_callback_t boardCallback, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX);
void resetConfigurationExt(engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX);

void rememberCurrentConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
#endif /* __cplusplus */

void setBoardDefaultConfiguration(void);
void setBoardConfigOverrides(void);

EXTERN_CONFIG;
