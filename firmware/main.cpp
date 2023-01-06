/**
 * @file	main.cpp
 * @brief C++ main entry point
 *
 * @date Nov 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2023
 *      http://rusefi.com/
 */

#include "pch.h"

#include "rusefi.h"
#include "mpu_util.h"

int main(void) {
	// Maybe your board needs to do something special before HAL init
	preHalInit();

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
