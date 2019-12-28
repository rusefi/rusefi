/*
 * @file	trigger_input.cpp
 *
 * @date Nov 11, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"
#include "trigger_input.h"

#if (EFI_PROD_CODE && EFI_SHAFT_POSITION_INPUT) || defined(__DOXYGEN__)

EXTERN_ENGINE;

#if (HAL_USE_ICU == TRUE) || (HAL_TRIGGER_USE_PAL == TRUE)
void stopTriggerInputPins(void) {
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (isConfigurationChanged(triggerInputPins[i])) {
			turnOffTriggerInputPin(activeConfiguration.triggerInputPins[i]);
		}
	}
	for (int i = 0; i < CAM_INPUTS_COUNT; i++) {
		if (isConfigurationChanged(camInputs[i])) {
			turnOffTriggerInputPin(activeConfiguration.camInputs[i]);
		}
	}
}

void startTriggerInputPins(void) {
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (isConfigurationChanged(triggerInputPins[i])) {
			const char * msg = (i == 0 ? "trigger#1" : (i == 1 ? "trigger#2" : "trigger#3"));
			turnOnTriggerInputPin(msg, i, true);
		}
	}

	for (int i = 0; i < CAM_INPUTS_COUNT; i++) {
		if (isConfigurationChanged(camInputs[i])) {
			turnOnTriggerInputPin("cam", i, false);
		}
	}

	setPrimaryChannel(CONFIG(triggerInputPins)[0]);
}
#endif


void applyNewTriggerInputPins(void) {
	// first we will turn off all the changed pins
	stopTriggerInputPins();
	// then we will enable all the changed pins
	startTriggerInputPins();
}

#endif /* EFI_SHAFT_POSITION_INPUT */
