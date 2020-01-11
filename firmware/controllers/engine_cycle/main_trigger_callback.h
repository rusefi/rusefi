/**
 * @file    main_trigger_callback.h
 * @brief   Main logic header
 *
 *
 * @date Feb 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine.h"

#include "event_registry.h"

void initMainEventListener(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);

void startPrimeInjectionPulse(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void startSimultaniousInjection(Engine *engine);
void endSimultaniousInjection(InjectionEvent *event);
void turnInjectionPinHigh(InjectionEvent *event);
void turnInjectionPinLow(InjectionEvent *event);

// reset injection switch counter if the engine started spinning
void updatePrimeInjectionPulseState(DECLARE_ENGINE_PARAMETER_SIGNATURE);
