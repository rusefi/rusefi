#pragma once

#include <rusefi_hw_enums.h>

class PinRepository {
public:
	/**
	 * Class constructors are a great way to have simple initialization sequence
	 */
	PinRepository();
	const char* & getBrainUsedPin(size_t idx) {
    	/*if (idx >= getBrainPinTotalNum())
    		return NULL;*/
    	return PIN_USED[idx];
    }

	const char *PIN_USED[BRAIN_PIN_TOTAL_PINS];

};

/**
 * See also efiSetPadUnused
 */
void brain_pin_markUnused(Gpio brainPin);
int brainPin_to_index(Gpio brainPin);
