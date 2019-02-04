/*
 * digital_input_exti.cpp
 *
 *  Created on: Dec 18, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"

#if HAL_USE_PAL && EFI_PROD_CODE || defined(__DOXYGEN__)
#include "digital_input_exti.h"
#include "efiGpio.h"

/**
 * EXTI is a funny thing: you can only use same pin on one port. For example, you can use
 * PA0 PB5 PE2 PD7
 * but you cannot use
 * PA0 PB0 PE2 PD7
 * because pin '0' would be used on two different ports
 */

// EXT is not able to give you the front direction but you could read the pin in the callback.
void enableExti(brain_pin_e pin, uint32_t mode, palcallback_t cb) {
	if (pin == GPIO_UNASSIGNED)
		return;

	int index = getHwPin("joy", pin);
	ioportid_t port = getHwPort("joy", pin);

	ioline_t line = PAL_LINE(port, index);
	palEnableLineEvent(line, mode);
    palSetLineCallback(line, cb, (void*)index);
}

#endif /* HAL_USE_PAL && EFI_PROD_CODE */
