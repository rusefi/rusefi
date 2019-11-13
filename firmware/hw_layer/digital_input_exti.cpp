/*
 * digital_input_exti.cpp
 *
 *  Created on: Dec 18, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"

#if HAL_USE_PAL && EFI_PROD_CODE
#include "digital_input_exti.h"
#include "efi_gpio.h"
#include "error_handling.h"

/**
 * EXTI is a funny thing: you can only use same pin on one port. For example, you can use
 * PA0 PB5 PE2 PD7
 * but you cannot use
 * PA0 PB0 PE2 PD7
 * because pin '0' would be used on two different ports
 */

static ioportmask_t ext_used = 0;

// EXT is not able to give you the front direction but you could read the pin in the callback.
int efiExtiEnablePin(const char *msg, brain_pin_e brainPin, uint32_t mode, palcallback_t cb, void *cb_data) {

	/* paranoid check, in case of GPIO_UNASSIGNED getHwPort will return NULL
	 * and we will fail on next check */
	if (brainPin == GPIO_UNASSIGNED)
		return -1;

	ioportid_t port = getHwPort(msg, brainPin);
	if (port == NULL)
		return -1;

	int index = getHwPin(msg, brainPin);

	/* is this index already used? */
	if (ext_used & PAL_PORT_BIT(index)) {
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_2, "%s: pin %d: exti index already used", msg, brainPin);
		return -1;
	}

	ioline_t line = PAL_LINE(port, index);
	palEnableLineEvent(line, mode);
	palSetLineCallback(line, cb, cb_data);

	/* mark used */
	ext_used |= PAL_PORT_BIT(index);

	return 0;
}

void efiExtiDisablePin(brain_pin_e brainPin)
{
	/* paranoid check, in case of GPIO_UNASSIGNED getHwPort will return NULL
	 * and we will fail on next check */
	if (brainPin == GPIO_UNASSIGNED)
		return;

	ioportid_t port = getHwPort("exti", brainPin);
	if (port == NULL)
		return;

	int index = getHwPin("exti", brainPin);

	/* is this index was used? */
	if (!(ext_used & PAL_PORT_BIT(index))) {
		return;
	}

	ioline_t line = PAL_LINE(port, index);
	palDisableLineEvent(line);

	/* mark unused */
	ext_used &= ~PAL_PORT_BIT(index);
}

#endif /* HAL_USE_PAL && EFI_PROD_CODE */
