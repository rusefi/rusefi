/**
 * @file	adc_inputs.h
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_hw_enums.h"

static inline bool isAdcChannelValid(adc_channel_e hwChannel) {
	if (hwChannel <= EFI_ADC_NONE) {
		return false;
	} else if (hwChannel >= EFI_ADC_LAST_CHANNEL) {
		/* this should not happen!
		 * if we have enum out of range somewhere in settings
		 * that means something goes terribly wrong
		 * TODO: should we say something?
		 */
		return false;
	} else {
		return true;
	}
}

#include "boards.h"
