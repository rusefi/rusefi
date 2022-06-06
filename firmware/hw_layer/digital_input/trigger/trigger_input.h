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

void turnOnTriggerInputPins();
void applyNewTriggerInputPins();
void startTriggerInputPins();
void stopTriggerInputPins();

void stopTriggerDebugPins();
void startTriggerDebugPins();

#if HAL_USE_ADC
typedef adcsample_t triggerAdcSample_t;
#else
typedef uint16_t triggerAdcSample_t;
#endif /* HAL_USE_ADC */

// This detector has 2 modes for low-RPM (ADC) and fast-RPM (EXTI)
enum triggerAdcMode_t {
	TRIGGER_ADC_NONE = 0,
	TRIGGER_ADC_ADC,
	TRIGGER_ADC_EXTI,
};

adc_channel_e getAdcChannelForTrigger(void);
void addAdcChannelForTrigger(void);
void triggerAdcCallback(triggerAdcSample_t value);

void setTriggerAdcMode(triggerAdcMode_t adcMode);
void onTriggerChanged(efitick_t stamp, bool isPrimary, bool isRising);
