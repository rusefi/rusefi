/*
 * @file mc33816.cpp
 *
 * The NXP MC33816 is a programmable gate driver IC for precision solenoid control applications.
 *
 * @date May 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "efifeatures.h"
#include "mc33816.h"

#if EFI_MC33816

static void showStats() {

}

static void sentByte(int param) {

}

void initMc33816() {
	if (engineConfiguration->mc33816_cs == GPIO_UNASSIGNED)
		return;

	addConsoleAction("mc33_stats", showStats);

	addConsoleActionI("mc33_send", sentByte);

}

#endif /* EFI_MC33816 */
