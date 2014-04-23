/**
 * @file	main.cpp
 * @brief C++ main entry point
 *
 * @date Nov 29, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 *      http://rusefi.com/
 */

extern "C"
{
#include "global.h"
}
#include "main.h"

extern "C"
{
#include "rusefi.h"
}
int main(void) {
	/*
	 * ChibiOS/RT initialization
	 */
	halInit();
	chSysInit();

	runRusEfi();
	return 0;
}

