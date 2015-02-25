/**
 * @file wave_analyzer_hw.h
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef WAVE_ANALYZER_HW_H_
#define WAVE_ANALYZER_HW_H_

#include "main.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

#include "listener_array.h"

typedef struct {
	ICUDriver *driver;
	GPIO_TypeDef *port;
	int pin;
	bool_t isActiveHigh; // false for ICU_INPUT_ACTIVE_LOW, true for ICU_INPUT_ACTIVE_HIGH
	volatile int started;

	// todo: make this a template & reduce number of listeners?
	// todo: would one listener be enough?
	IntListenerArray widthListeners;
	IntListenerArray periodListeners;
} WaveReaderHw;

void initWaveAnalyzerDriver(WaveReaderHw *hw, brain_pin_e brainPin);
void startInputDriver(WaveReaderHw *hw, bool isActiveHigh);
ICUDriver * getInputCaptureDriver(brain_pin_e hwPin);

#endif

#endif /* WAVE_ANALYZER_HW_H_ */
