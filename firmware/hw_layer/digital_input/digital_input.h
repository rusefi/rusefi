/*
 * @file digital_input.h
 * Universal input capture API hiding platform/hardware implementation details
 *
 * @date Nov 11, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "listener_array.h"

struct digital_input_s {
#if HAL_USE_ICU
	ICUDriver *driver;
#endif /* HAL_USE_ICU */

	volatile bool started = false;
	brain_pin_e brainPin = GPIO_UNASSIGNED;
	IntListenerArray<1> widthListeners;
	IntListenerArray<1> periodListeners;

	// Width/Period names are historically inherited from ICU implementation, todo: migrate to better names, high/low? rise/hall?
	void setWidthCallback(VoidInt handler, void *arg);
	void setPeriodCallback(VoidInt handler, void *arg);
};
