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
#define setWholeTimingTable(x) setWholeTimingTable_d(x);
void setConstantDwell(floatms_t dwellMs DECLARE_CONFIG_PARAMETER_SUFFIX);

// needed by bootloader
void setDefaultBasePins();

void setDefaultSdCardParameters();

void onBurnRequest(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void incrementGlobalConfigurationVersion(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration);
void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration);

void emptyCallbackWithConfiguration(engine_configuration_s * engine);
void setDefaultFrankensoConfiguration();

typedef void (*configuration_callback_t)(engine_configuration_s*);

#ifdef __cplusplus
// because of 'Logging' class parameter these functions are visible only to C++ code but C code
void loadConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
/**
 * boardCallback is invoked after configuration reset but before specific engineType configuration
 */
void resetConfigurationExt(configuration_callback_t boardCallback, engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX);
void resetConfigurationExt(engine_type_e engineType DECLARE_ENGINE_PARAMETER_SUFFIX);

void rememberCurrentConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
#endif /* __cplusplus */

void setBoardDefaultConfiguration(void);
void setBoardConfigOverrides(void);

#if !EFI_UNIT_TEST
extern persistent_config_container_s persistentState;
#endif // EFI_UNIT_TEST
extern engine_configuration_s *engineConfiguration;
extern persistent_config_s *config;

/**
 * & is reference in C++ (not C)
 * Ref is a pointer that:
 *   you access with dot instead of arrow
 *   Cannot be null
 * This is about EFI_ACTIVE_CONFIGURATION_IN_FLASH
 */
extern engine_configuration_s & activeConfiguration;

#if ! EFI_ACTIVE_CONFIGURATION_IN_FLASH
// We store a special changeable copy of configuration is RAM, so we can just compare them
#define isConfigurationChanged(x) (engineConfiguration->x != activeConfiguration.x)
#else
// We cannot call prepareVoidConfiguration() for activeConfiguration if it's stored in flash,
// so we need to tell the firmware that it's "void" (i.e. zeroed, invalid) by setting a special flag variable,
// and then we consider 'x' as changed if it's just non-zero.
extern bool isActiveConfigurationVoid;
#define isConfigurationChanged(x) ((engineConfiguration->x != activeConfiguration.x) || (isActiveConfigurationVoid && engineConfiguration->x != 0))
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */

#define isPinOrModeChanged(pin, mode) (isConfigurationChanged(pin) || isConfigurationChanged(mode))
