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
#define DEFAULT_ENGINE_TYPE engine_type_e::MINIMAL_PINS
#endif

#define WARMUP_CLT_EXTRA_FUEL_CURVE config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE
#define IAT_FUEL_CORRECTION_CURVE config->iatFuelCorrBins, config->iatFuelCorr, IAT_CURVE_SIZE

#define MOCK_UNDEFINED -1

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90826 Weak symbol does not work reliably on windows
// https://sourceware.org/bugzilla/show_bug.cgi?id=9687 Weak symbols not working on mingw32
// DEPRECATED see PUBLIC_API_WEAK
#if !defined(IS_WINDOWS_COMPILER) || !IS_WINDOWS_COMPILER
#define PUBLIC_API_WEAK_SOMETHING_WEIRD __attribute__((weak))
#else
#define PUBLIC_API_WEAK_SOMETHING_WEIRD
#endif

void setCrankOperationMode();
void setCamOperationMode();

void prepareVoidConfiguration(engine_configuration_s *activeConfiguration);
void setTargetRpmCurve(float rpm);
void setFuelTablesLoadBin(float minValue, float maxValue);
void setWholeIatCorrTimingTable(float value);
void setWholeTimingTable(angle_t value);
void setConstantDwell(floatms_t dwellMs);

// needed by bootloader
void setDefaultBasePins();

void setDefaultSdCardParameters();

void onBurnRequest();
void incrementGlobalConfigurationVersion(const char * msg = "undef");

void commonFrankensoAnalogInputs();

void emptyCallbackWithConfiguration(engine_configuration_s * engine);

typedef void (*configuration_callback_t)(engine_configuration_s*);

#ifdef __cplusplus
// because of 'Logging' class parameter these functions are visible only to C++ code but C code
void loadConfiguration();
/**
 * boardCallback is invoked after configuration reset but before specific engineType configuration
 */
void resetConfigurationExt(configuration_callback_t boardCallback, engine_type_e engineType);
void resetConfigurationExt(engine_type_e engineType);

void rememberCurrentConfiguration();
#endif /* __cplusplus */

void setBoardDefaultConfiguration();
void setBoardConfigOverrides();
void onBoardStandBy();
Gpio getCommsLedPin();
// fun fact: the red LED for critical error is defined via LED_CRITICAL_ERROR_BRAIN_PIN
Gpio getWarningLedPin();
// technical debt: only used to blink from bootloader?
Gpio getRunningLedPin();

int hackHellenBoardId(int detectedId);
void applyEngineType(engine_type_e engineType);

#if !EFI_UNIT_TEST
extern persistent_config_container_s persistentState;
static constexpr engine_configuration_s * engineConfiguration = &persistentState.persistentConfiguration.engineConfiguration;
static constexpr persistent_config_s * config = &persistentState.persistentConfiguration;
#else // EFI_UNIT_TEST
extern engine_configuration_s *engineConfiguration;
extern persistent_config_s *config;
#endif // EFI_UNIT_TEST

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
#define isConfigurationChanged(x) ((engineConfiguration->x != activeConfiguration.x) || (isActiveConfigurationVoid && (int)(engineConfiguration->x) != 0))
#endif /* EFI_ACTIVE_CONFIGURATION_IN_FLASH */

#define isPinOrModeChanged(pin, mode) (isConfigurationChanged(pin) || isConfigurationChanged(mode))

// total number of outputs: low side + high side
int getBoardMetaOutputsCount();
int getBoardMetaLowSideOutputsCount();
Gpio* getBoardMetaOutputs();
int getBoardMetaDcOutputsCount();

struct engine_configuration_custom : public engine_configuration_s {
    // Pulse Input Settings
    brain_pin_e pulseInputPin = GPIO_UNASSIGNED;        // User-selected pin
    uint16_t pulseInputAveragingMs = 100;              // Averaging window in milliseconds
};
