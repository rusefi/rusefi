/**
 * @file digital_input_hw.h
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef WAVE_ANALYZER_HW_H_
#define WAVE_ANALYZER_HW_H_

#include "main.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

#include "listener_array.h"

typedef struct {
	ICUDriver *driver;
	bool isActiveHigh; // false for ICU_INPUT_ACTIVE_LOW, true for ICU_INPUT_ACTIVE_HIGH
	volatile bool started;

	// todo: make this a template & reduce number of listeners?
	// todo: would one listener be enough?
	IntListenerArray widthListeners;
	IntListenerArray periodListeners;
} digital_input_s;

void turnOnCapturePin(const char *msg, brain_pin_e brainPin);
digital_input_s *initWaveAnalyzerDriver(const char *msg, brain_pin_e brainPin);
void startInputDriver(digital_input_s *hw, bool isActiveHigh);
ICUDriver * getInputCaptureDriver(brain_pin_e hwPin);

#endif

#endif /* WAVE_ANALYZER_HW_H_ */
