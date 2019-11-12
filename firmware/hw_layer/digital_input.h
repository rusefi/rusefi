/*
 * @file digital_input.h
 *
 * @date Nov 11, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#pragma once

#include "global.h"
#include "listener_array.h"

typedef struct {
#if HAL_USE_ICU
	ICUDriver *driver;
#endif /* HAL_USE_ICU */

	bool isActiveHigh = false;
	volatile bool started = false;
	brain_pin_e brainPin = GPIO_UNASSIGNED;
	IntListenerArray<1> widthListeners;
	IntListenerArray<1> periodListeners;
} digital_input_s;
