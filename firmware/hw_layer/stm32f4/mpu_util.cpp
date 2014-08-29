/**
 * @file	mpu_util.cpp
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "mpu_util.h"
#include "error_handling.h"

void baseHardwareInit(void) {
	// looks like this holds a random value on start? Let's set a nice clean zero
	DWT_CYCCNT = 0;
}

void DebugMonitorVector(void) {

	chDbgPanic3("DebugMonitorVector", __FILE__, __LINE__);

	while (TRUE)
		;
}

void UsageFaultVector(void) {

	chDbgPanic3("UsageFaultVector", __FILE__, __LINE__);

	while (TRUE)
		;
}

void BusFaultVector(void) {

	chDbgPanic3("BusFaultVector", __FILE__, __LINE__);

	while (TRUE) {
        }
}

void HardFaultVector(void) {

	chDbgPanic3("HardFaultVector", __FILE__, __LINE__);

	while (TRUE) {
        }
}



