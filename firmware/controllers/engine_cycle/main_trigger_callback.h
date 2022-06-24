/**
 * @file    main_trigger_callback.h
 * @brief   Main logic header
 *
 *
 * @date Feb 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "event_registry.h"

void initMainEventListener();

void mainTriggerCallback(uint32_t trgEventIndex, efitick_t edgeTimestamp, float currentPhase, float nextPhase);
bool noFiringUntilVvtSync(vvt_mode_e vvtMode);

void startSimultaniousInjection(void* = nullptr);
void endSimultaniousInjection(InjectionEvent *event);
void turnInjectionPinHigh(InjectionEvent *event);
void turnInjectionPinLow(InjectionEvent *event);

// reset injection switch counter if the engine started spinning
void updatePrimeInjectionPulseState();

// Internal use only - exposed for tests
void handleFuelInjectionEvent(int injEventIndex, InjectionEvent *event,
		int rpm, efitick_t nowNt);
