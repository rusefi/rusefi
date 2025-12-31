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
#include "engine_configuration.h"

// function with no parameters and returning void
using setup_custom_board_overrides_type = void (*)();
using setup_custom_board_config_type = void (*)(engine_configuration_s * /*previousConfiguration*/);
using setup_custom_board_output_type = int (*)();
using setup_custom_board_engine_type_type = void (*)(engine_type_e);

// todo: migrate 'validateBoardConfig'
using custom_validate_config_type = bool (*)();

using setup_custom_board_ts_command_override_type = void (*)(uint16_t /*subsystem*/, uint16_t /*index*/);
extern std::optional<setup_custom_board_ts_command_override_type> custom_board_ts_command;

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

extern std::optional<setup_custom_board_overrides_type> custom_board_BeforeTuneDefaults;
extern std::optional<setup_custom_board_engine_type_type> custom_board_AfterTuneDefaults;
extern std::optional<setup_custom_board_engine_type_type> custom_board_applyUnknownType;

extern std::optional<setup_custom_board_overrides_type> custom_board_periodicSlowCallback;
extern std::optional<setup_custom_board_overrides_type> custom_board_periodicFastCallback;

// Board hardware related:
extern std::optional<setup_custom_board_output_type> custom_board_getMetaOutputsCount;
extern std::optional<setup_custom_board_output_type> custom_board_getMetaLowSideOutputs;

// LTFT to VE table custom apply algo
extern std::optional<setup_custom_board_overrides_type> custom_board_LtftTrimToVeApply;

// specific firmware builds are meant for specific hardware. In order to provide best user experience on well-known boards sometimes we reduce user flexibility.
extern std::optional<setup_custom_board_overrides_type> custom_board_DefaultConfiguration;
extern std::optional<setup_custom_board_overrides_type> custom_board_ConfigOverrides;

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
