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

// function with no parameters and returning void
using setup_custom_board_overrides_type = void (*)();

#if EFI_CAN_SUPPORT
using board_can_rx_type = void (*)(const size_t, const CANRxFrame &, efitick_t);
extern std::optional<board_can_rx_type> custom_board_can_rx;
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

/**
 * This function checks if an override is present and calls it if available.
 */
static inline void call_board_override(std::optional<setup_custom_board_overrides_type> board_override){
    if (board_override.has_value()) {
        std::invoke(board_override.value());
    }
}
