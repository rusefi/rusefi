/*
 * @file digital_input.cpp
 *
 * @date Nov 11, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "digital_input.h"

void digital_input_s::setWidthCallback(VoidInt handler, void *arg) {
	widthListeners.registerCallback(handler, arg);
}

void digital_input_s::setPeriodCallback(VoidInt handler, void *arg) {
	periodListeners.registerCallback(handler, arg);
}
