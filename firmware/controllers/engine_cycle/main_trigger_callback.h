/**
 * @file    main_trigger_callback.h
 * @brief   Main logic header
 *
 *
 * @date Feb 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#pragma once

#include "engine.h"

#include "event_registry.h"

void initMainEventListener(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);
void mainTriggerCallback(trigger_event_e ckpSignalType, uint32_t eventIndex DECLARE_ENGINE_PARAMETER_SUFFIX);

int isIgnitionTimingError(void);
void startPrimeInjectionPulse(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void showMainHistogram(void);

void startSimultaniousInjection(Engine *engine);
void endSimultaniousInjection(InjectionEvent *event);
void seTurnPinHigh(InjectionEvent *event);
void seTurnPinLow(InjectionEvent *event);

// reset injection switch counter if the engine started spinning
void updatePrimeInjectionPulseState(DECLARE_ENGINE_PARAMETER_SIGNATURE);
