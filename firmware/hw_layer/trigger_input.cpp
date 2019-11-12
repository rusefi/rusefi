/*
 * @file	trigger_input.cpp
 *
 * @date Nov 11, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"
#include "trigger_input.h"

#if (EFI_PROD_CODE && EFI_SHAFT_POSITION_INPUT) || defined(__DOXYGEN__)

void applyNewTriggerInputPins(void) {
	// first we will turn off all the changed pins
	stopTriggerInputPins();
	// then we will enable all the changed pins
	startTriggerInputPins();
}

#endif /* EFI_SHAFT_POSITION_INPUT */
