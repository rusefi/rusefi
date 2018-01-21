/**
 * @file	main.cpp
 * @brief C++ main entry point
 *
 * @date Nov 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
 *      http://rusefi.com/
 */

#include "main.h"
#include "rusefi.h"
#include "mpu_util.h"

int main(void) {
	/*
	 * ChibiOS/RT initialization
	 */
	halInit();
	chSysInit();

	baseHardwareInit();

	runRusEfi();
	return 0;
}

