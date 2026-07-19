/*
 * @file board_overrides.h
 * @brief Board-specific override mechanism
 *
 * This header provides a centralized way to define and manage board-specific overrides.
 * Custom boards can implement their own specialized behavior by overriding these function pointers.
 *
 * Usage Example:
 *
 * 1. Define your custom function:
 *    void myBoardCustomHello() {
 *        // Your custom implementation
 *    }
 *
 * 2. Set up the override in your board_configuration.cpp:
 *    void setup_custom_board_overrides() {
 *        custom_board_boardSayHello = myBoardCustomHello;
 *    }
 *
 *
 * @date: jul 09, 2025
 * @author FDSoftware
 */

#pragma once
#include <functional>
#include <optional>
#include "rusefi_types.h"
#include "engine_configuration.h"

// function with no parameters and returning void
using setup_custom_board_overrides_type = void (*)();
using setup_custom_board_config_type = void (*)(const engine_configuration_s * /*previousConfiguration*/);
using setup_custom_board_output_type = int (*)();
using setup_custom_board_engine_type_type = void (*)(engine_type_e);
using setup_custom_get_float_type = float (*)();
using setup_custom_board_adjust_voltage_type = float (*)(float /*voltage*/, adc_channel_e /*hwChannel*/);
using setup_custom_on_board_standby_type = void (*)();
using setup_custom_hack_hellen_board_id_type = int (*)(int /*detectedId*/);
using setup_custom_get_cylinder_ignition_trim_type = angle_t (*)(size_t /*cylinderNumber*/, float /*rpm*/, float /*ignitionLoad*/);
using setup_custom_get_cylinder_fuel_trim_type = float (*)(size_t /*cylinderNumber*/, float /*rpm*/, float /*fuelLoad*/);
using setup_custom_bool_type = bool (*)();

using custom_validate_config_type = bool (*)();
extern std::optional<custom_validate_config_type> custom_board_validateConfig;

using setup_custom_board_ts_command_override_type = void (*)(uint16_t /*subsystem*/, uint16_t /*index*/);
extern std::optional<setup_custom_board_ts_command_override_type> custom_board_ts_command;

// Board-specific TunerStudio binary command handler. Unlike custom_board_ts_command
// (which rides on TS_EXECUTE and is fire-and-forget), this hook receives the channel and
// the raw request payload so a board can implement a full binary request/response. It is invoked for opcodes the
// base firmware does not recognize (see TS_BOARD_COMMAND / handleCrcCommand). `data`
// points at the payload (the command byte already consumed) and `size` is its length in
// bytes. Return true if the command was handled; false lets the base firmware NAK it.
class TsChannelBase;
using board_ts_binary_command_type = bool (*)(TsChannelBase* /*tsChannel*/, char /*command*/, uint8_t* /*data*/, uint16_t /*size*/);
extern std::optional<board_ts_binary_command_type> custom_board_ts_binary_command;

#if !defined(EFI_BOOTLOADER) && EFI_FILE_LOGGING && EFI_PROD_CODE
#include "ff.h"
using setup_custom_board_write_error_file_type = void (*)(FIL * /*fd*/);
extern std::optional<setup_custom_board_write_error_file_type> custom_board_onBoardWriteErrorFile;
#endif // EFI_FILE_LOGGING

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can_msg_tx.h"
using board_can_rx_type = void (*)(const size_t, const CANRxFrame &, efitick_t);
extern std::optional<board_can_rx_type> custom_board_can_rx;

using board_can_update_dash_type = void (*)(CanCycle cycle);
extern std::optional<board_can_update_dash_type> custom_board_update_dash;
#endif // EFI_CAN_SUPPORT

/**
 * @brief Pre-HAL initialization override point
 * Allows boards to perform custom initialization before HAL is initialized
 */
extern std::optional<setup_custom_board_overrides_type> custom_board_preHalInit;

extern std::optional<setup_custom_board_overrides_type> custom_board_boardSayHello;

// Called before configuration is loaded
extern std::optional<setup_custom_board_overrides_type> custom_board_InitHardwareEarly;
extern std::optional<setup_custom_board_overrides_type> custom_board_InitHardware;
extern std::optional<setup_custom_board_overrides_type> custom_board_InitHardwareExtra;
extern std::optional<setup_custom_board_config_type> custom_board_OnConfigurationChange;

// Dynamic hardware re-init participation (see docs/hardware-reinit-and-power-cycle.md).
// custom_board_OnConfigurationChange fires only AFTER applyNewHardwareSettings() has finished
// starting hardware, so it cannot release pins - a stale board claim makes the pin repository
// reject the new owner with a criticalError. Boards that claim user-configurable pins must
// instead use this pair, which follows the all-stops-before-any-starts invariant:
// - custom_board_StopHardware: called during the stop phase of applyNewHardwareSettings(),
//   while activeConfiguration still describes the previously claimed pins - release them here.
// - custom_board_StartHardware: called from startHardware() on BOTH ECU start and configuration
//   change - claim pins here from engineConfiguration.
// Boards claiming only fixed pins (not user-configurable) can keep using custom_board_InitHardware.
extern std::optional<setup_custom_board_overrides_type> custom_board_StopHardware;
extern std::optional<setup_custom_board_overrides_type> custom_board_StartHardware;

extern std::optional<setup_custom_board_overrides_type> custom_board_TriggerResetState;

extern std::optional<setup_custom_board_overrides_type> custom_board_BeforeTuneDefaults;
extern std::optional<setup_custom_board_engine_type_type> custom_board_AfterTuneDefaults;
extern std::optional<setup_custom_board_engine_type_type> custom_board_applyUnknownType;

extern std::optional<setup_custom_board_overrides_type> custom_board_periodicSlowCallback;
extern std::optional<setup_custom_board_overrides_type> custom_board_periodicFastCallback;

// Called from Engine::onEngineStopped() so boards can reset their own per-run state
// (e.g. board-specific trigger/IO hardware) when the engine stops.
extern std::optional<setup_custom_board_overrides_type> custom_board_onEngineStopped;

// Board hardware related:
extern std::optional<setup_custom_board_output_type> custom_board_getMetaOutputsCount;
extern std::optional<setup_custom_board_output_type> custom_board_getMetaLowSideOutputs;

// LTFT to VE table custom apply algo
extern std::optional<setup_custom_board_overrides_type> custom_board_LtftTrimToVeApply;

// specific firmware builds are meant for specific hardware. In order to provide best user experience on well-known boards sometimes we reduce user flexibility.
extern std::optional<setup_custom_board_overrides_type> custom_board_DefaultConfiguration;
extern std::optional<setup_custom_board_overrides_type> custom_board_ConfigOverrides;

extern std::optional<setup_custom_get_float_type> custom_board_getFuncPairAllowedSplit;
extern std::optional<setup_custom_board_adjust_voltage_type> custom_board_boardAdjustVoltage;
extern std::optional<setup_custom_on_board_standby_type> custom_board_onBoardStandBy;
extern std::optional<setup_custom_hack_hellen_board_id_type> custom_board_hackHellenBoardId;
extern std::optional<setup_custom_get_cylinder_ignition_trim_type> custom_board_getCylinderIgnitionTrim;
extern std::optional<setup_custom_get_cylinder_fuel_trim_type> custom_board_getCylinderFuelTrim;

extern std::optional<setup_custom_bool_type> custom_board_isBoardWithPowerManagement;
extern std::optional<setup_custom_bool_type> custom_board_boardAllowTriggerActions;

extern std::optional<setup_custom_bool_type> custom_board_getAcrState;
extern std::optional<setup_custom_bool_type> custom_board_allowFlashNow;

/**
 * This function checks if an override is present and calls it if available.
 * Return true if override is present and was called
 */
template<typename FuncType, typename... Args>
static inline bool call_board_override(std::optional<FuncType> board_override, Args&&... args){
    if (board_override.has_value()) {
        std::invoke(board_override.value(), std::forward<Args>(args)...);
        return true;
    }
    return false;
}

template<typename FuncType, typename... Args>
static inline auto get_board_override_result(std::optional<FuncType> board_override, auto defaultValue, Args&&... args){
    if (board_override.has_value()) {
        return std::invoke(board_override.value(), std::forward<Args>(args)...);
    }
    return defaultValue;
}
