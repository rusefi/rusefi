/**
 * @file digital_input_icu.h
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "digital_input.h"

#if HAL_USE_ICU

digital_input_s* startDigitalCapture(const char *msg, brain_pin_e brainPin);

void stopDigitalCapture(const char *msg, brain_pin_e brainPin);

//Nullable
ICUDriver * getInputCaptureDriver(const char *msg, brain_pin_e hwPin);
//Nullable
icuchannel_t getInputCaptureChannel(brain_pin_e hwPin);

#endif /* HAL_USE_ICU */

