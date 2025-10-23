/**
 * @file	main.cpp
 * @brief C++ main entry point
 *
 * See rusefi.cpp for implementation details notes.
 *
 * @date Nov 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2023
 *      http://rusefi.com/
 */

#include "pch.h"

#include "rusefi.h"
#include "mpu_util.h"

#include "fw_configuration.h"
#include "board_overrides.h"

// this function is used to link all the possibles overrides of the bord, is one of the first func call, before any hw init!
// use ONLY for the setup of the overrides!!
void setup_custom_board_overrides();

std::optional<setup_custom_board_overrides_type> custom_board_preHalInit;

int main(void) {
	setup_custom_fw_overrides();
	setup_custom_board_overrides();
	// Maybe your board needs to do something special before HAL init
	preHalInit();
	call_board_override(custom_board_preHalInit);

	/*
	 * ChibiOS/RT initialization
	 */
	halInit();
	chSysInit();

	/**
	 * most basic MCU initialization - no configuration access, no external hardware access
	 */
	baseMCUInit();

	runRusEfi();
	return 0;
}

// Weak linked default implementation (not necessarily required for all boards)
__attribute__((weak)) void preHalInit() { }
