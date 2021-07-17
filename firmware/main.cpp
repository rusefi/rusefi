/**
 * @file	main.cpp
 * @brief C++ main entry point
 *
 * @date Nov 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 *      http://rusefi.com/
 */

#include "os_access.h"
#include "rusefi.h"
#include "mpu_util.h"
#include "version_check.h"

int main(void) {
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

