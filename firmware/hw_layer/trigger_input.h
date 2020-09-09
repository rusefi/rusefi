/**
 * @file	trigger_input.h
 * @brief	Position sensor hardware layer
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#ifndef CRANK_INPUT_H_
#define CRANK_INPUT_H_

#include "engine.h"
#include "pin_repository.h"
#include "trigger_structure.h"
#include "trigger_central.h"

#define TRIGGER_SUPPORTED_CHANNELS 2

void turnOnTriggerInputPins(Logging *sharedLogger);
void applyNewTriggerInputPins(void);
void startTriggerInputPins(void);
void stopTriggerInputPins(void);

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

#endif /* CRANK_INPUT_H_ */
