/**
 * @file    main_trigger_callback.h
 * @brief   Main logic header
 *
 *
 * @date Feb 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"

#include "event_registry.h"

void initMainEventListener(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void mainTriggerCallback(uint32_t trgEventIndex, efitick_t edgeTimestamp DECLARE_ENGINE_PARAMETER_SUFFIX);

void startPrimeInjectionPulse(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void startSimultaniousInjection(Engine *engine);
void endSimultaniousInjection(InjectionEvent *event);
void turnInjectionPinHigh(InjectionEvent *event);
void turnInjectionPinLow(InjectionEvent *event);

// reset injection switch counter if the engine started spinning
void updatePrimeInjectionPulseState(DECLARE_ENGINE_PARAMETER_SIGNATURE);

// Internal use only - exposed for tests
void handleFuelInjectionEvent(int injEventIndex, InjectionEvent *event,
		int rpm, efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX);
