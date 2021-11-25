/**
 * @file	trigger_input.h
 * @brief	Position sensor hardware layer
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "trigger_structure.h"
#include "trigger_central.h"

#define TRIGGER_BAIL_IF_DISABLED          \
    if (!engine->hwTriggerInputEnabled) { \
		return;                           \
	}

#define TRIGGER_BAIL_IF_SELF_STIM                                 \
    if (engine->directSelfStimulation) {                          \
		/* sensor noise + self-stim = loss of trigger sync */     \
		return;                                                   \
	}


#define TRIGGER_SUPPORTED_CHANNELS 2

void turnOnTriggerInputPins();
void applyNewTriggerInputPins();
void startTriggerInputPins();
void stopTriggerInputPins();

void stopTriggerDebugPins();
void startTriggerDebugPins();

#if HAL_TRIGGER_USE_ADC && HAL_USE_ADC
// This detector has 2 modes for low-RPM (ADC) and fast-RPM (EXTI)
enum triggerAdcMode_t {
	TRIGGER_NONE = 0,
	TRIGGER_ADC,
	TRIGGER_EXTI,
};

adc_channel_e getAdcChannelForTrigger(void);
void addAdcChannelForTrigger(void);
void triggerAdcCallback(adcsample_t value);
#endif /* HAL_USE_ADC */

