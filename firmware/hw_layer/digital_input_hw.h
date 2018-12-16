/**
 * @file digital_input_hw.h
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef DIGITAL_INPUT_HW_H_
#define DIGITAL_INPUT_HW_H_

#include "global.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

#include "listener_array.h"

typedef struct {
	ICUDriver *driver;
	bool isActiveHigh; // false for ICU_INPUT_ACTIVE_LOW, true for ICU_INPUT_ACTIVE_HIGH
	volatile bool started;
	brain_pin_e brainPin;
	IntListenerArray<1> widthListeners;
	IntListenerArray<1> periodListeners;
} digital_input_s;

void turnOnCapturePin(const char *msg, brain_pin_e brainPin);

digital_input_s *addWaveAnalyzerDriver(const char *msg, brain_pin_e brainPin);
void startInputDriver(digital_input_s *hw, bool isActiveHigh);
void removeWaveAnalyzerDriver(const char *msg, brain_pin_e brainPin);

ICUDriver * getInputCaptureDriver(const char *msg, brain_pin_e hwPin);
icuchannel_t getInputCaptureChannel(brain_pin_e hwPin);

#endif /* HAL_USE_ICU */

#endif /* DIGITAL_INPUT_HW_H_ */
