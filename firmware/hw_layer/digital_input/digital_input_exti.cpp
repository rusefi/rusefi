/*
 * digital_input_exti.cpp
 *
 *  Created on: Dec 18, 2018
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#if HAL_USE_PAL && EFI_PROD_CODE
#include "digital_input_exti.h"

/**
 * EXTI is a funny thing: you can only use same pin on one port. For example, you can use
 * PA0 PB5 PE2 PD7
 * but you cannot use
 * PA0 PB0 PE2 PD7
 * because pin '0' would be used on two different ports
 */

static ioportmask_t ext_used = 0;
static const char *EXT_USED[16];

void efiExtiInit() {
	memset(EXT_USED, 0, sizeof(EXT_USED));
}

// EXT is not able to give you the front direction but you could read the pin in the callback.
void efiExtiEnablePin(const char *msg, brain_pin_e brainPin, uint32_t mode, palcallback_t cb, void *cb_data) {
	firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_2, "Enter en");
	/* paranoid check, in case of GPIO_UNASSIGNED getHwPort will return NULL
	 * and we will fail on next check */
	if (!isBrainPinValid(brainPin)) {
		return;
	}

	ioportid_t port = getHwPort(msg, brainPin);
	if (port == NULL) {
		return;
	}

	bool wasUsed = brain_pin_markUsed(brainPin, msg);
	if (wasUsed) {
		// error condition we shall bail
		return;
	}

	int index = getHwPin(msg, brainPin);

	/* is this index already used? */
	if (ext_used & PAL_PORT_BIT(index)) {
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_2, "%s: pin %d: exti index already used", msg, brainPin);
		return;
	}

	ioline_t line = PAL_LINE(port, index);
	palEnableLineEvent(line, mode);
	palSetLineCallback(line, cb, cb_data);

	/* mark used */
	ext_used |= PAL_PORT_BIT(index);
	EXT_USED[index] = msg;
}

void efiExtiDisablePin(brain_pin_e brainPin)
{
	firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_2, "Enter dis");
	/* paranoid check, in case of GPIO_UNASSIGNED getHwPort will return NULL
	 * and we will fail on next check */
	if (!isBrainPinValid(brainPin))
		return;

	ioportid_t port = getHwPort("exti", brainPin);
	if (port == NULL)
		return;
	brain_pin_markUnused(brainPin);

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

digital_input_s* startDigitalCaptureExti(const char *msg, brain_pin_e brainPin) {
	return nullptr;
}

#if ! EFI_ICU_INPUTS
digital_input_s* startDigitalCapture(const char *msg, brain_pin_e brainPin) {
	return startDigitalCaptureExti(msg, brainPin);
}
#endif // EFI_ICU_INPUTS

#endif /* HAL_USE_PAL && EFI_PROD_CODE */





