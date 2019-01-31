/**
 * @file	main.cpp
 * @brief C++ main entry point
 *
 * @date Nov 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
 *      http://rusefi.com/
 */

#include "global.h"
#include "rusefi.h"
#include "mpu_util.h"

extern "C" {
extern void mainLED(void);
}

int main(void) {

	mainLED();
#if 0
	/*
	 * ChibiOS/RT initialization
	 */
	halInit();
	chSysInit();

	baseHardwareInit();

	runRusEfi();
#endif
	return 0;
}

