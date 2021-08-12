/*
 * @file digital_input_exti.h
 *
 *  Created on: Dec 18, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "digital_input.h"

#if HAL_USE_PAL
void efiExtiInit();
void efiExtiEnablePin(const char *msg, brain_pin_e pin, uint32_t mode, palcallback_t cb, void *cb_data);
void efiExtiDisablePin(brain_pin_e brainPin);
#endif /* HAL_USE_PAL */
