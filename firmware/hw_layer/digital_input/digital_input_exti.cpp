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

void efiExtiInit() {
	nvicEnableVector(I2C1_EV_IRQn, STM32_I2C_I2C1_IRQ_PRIORITY);
}

struct ExtiChannel
{
	const char* Name;

	ExtiCallback Callback;
	void* CallbackData;

	efitick_t Timestamp = 0;
};

static ExtiChannel channels[16];

// EXT is not able to give you the front direction but you could read the pin in the callback.
void efiExtiEnablePin(const char *msg, brain_pin_e brainPin, uint32_t mode, ExtiCallback cb, void *cb_data) {
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

	auto& channel = channels[index];

	/* is this index already used? */
	if (channel.Callback) {
		firmwareError(CUSTOM_ERR_PIN_ALREADY_USED_2, "%s: pin %s/index %d: exti index already used by %s",
		msg,
		hwPortname(brainPin),
		index,
		channel.Name);
		return;
	}

	ioline_t line = PAL_LINE(port, index);
	palEnableLineEvent(line, mode);

	channel.Name = msg;
	channel.Callback = cb;
	channel.CallbackData = cb_data;
}

void efiExtiDisablePin(brain_pin_e brainPin)
{
	/* paranoid check, in case of GPIO_UNASSIGNED getHwPort will return NULL
	 * and we will fail on next check */
	if (!isBrainPinValid(brainPin))
		return;

	ioportid_t port = getHwPort("exti", brainPin);
	if (port == NULL)
		return;
	brain_pin_markUnused(brainPin);

	int index = getHwPin("exti", brainPin);

	auto& channel = channels[index];

	/* is this index was used? */
	if (!channel.Callback) {
		return;
	}

	ioline_t line = PAL_LINE(port, index);
	palDisableLineEvent(line);

	/* mark unused */
	channel.Name = nullptr;
	channel.Callback = nullptr;
	channel.CallbackData = nullptr;
}

digital_input_s* startDigitalCaptureExti(const char *msg, brain_pin_e brainPin) {
	return nullptr;
}

#if ! EFI_ICU_INPUTS
digital_input_s* startDigitalCapture(const char *msg, brain_pin_e brainPin) {
	return startDigitalCaptureExti(msg, brainPin);
}
#endif // EFI_ICU_INPUTS

static inline void triggerInterrupt() {
	// Manually fire the I2C1_EV interrupt, it will be queued after this interrupt returns
	NVIC->STIR = I2C1_EV_IRQn;
}

CH_IRQ_HANDLER(STM32_I2C1_EVENT_HANDLER) {
	OSAL_IRQ_PROLOGUE();

	for (size_t i = 0; i < 16; i++) {
		auto& channel = channels[i];

		if (channel.Timestamp != 0 && channel.Callback) {
			channel.Callback(channel.CallbackData, channel.Timestamp);
			channel.Timestamp = 0;
		}
	}

	OSAL_IRQ_EPILOGUE();
}

void handleExtiIsr(uint8_t index) {
	// No need to lock anything, we're already the highest-pri interrupt!

	uint32_t pr;
	extiGetAndClearGroup1(1U << index, pr);

	if (pr & (1 << index)) {
		const auto& timestamp = channels[index].Timestamp;

		if (timestamp == 0) {
			channels[index].Timestamp = getTimeNowNt();
		}

		triggerInterrupt();
	}
}

CH_FAST_IRQ_HANDLER(Vector58) {
	handleExtiIsr(0);
}

CH_FAST_IRQ_HANDLER(Vector5C) {
	handleExtiIsr(1);
}

CH_FAST_IRQ_HANDLER(Vector60) {
	handleExtiIsr(2);
}

CH_FAST_IRQ_HANDLER(Vector64) {
	handleExtiIsr(3);
}

CH_FAST_IRQ_HANDLER(Vector68) {
	handleExtiIsr(4);
}

CH_FAST_IRQ_HANDLER(Vector9C) {
	handleExtiIsr(5);
	handleExtiIsr(6);
	handleExtiIsr(7);
	handleExtiIsr(8);
	handleExtiIsr(9);
}

CH_FAST_IRQ_HANDLER(VectorE0) {
	handleExtiIsr(10);
	handleExtiIsr(11);
	handleExtiIsr(12);
	handleExtiIsr(13);
	handleExtiIsr(14);
	handleExtiIsr(15);
}

#endif /* HAL_USE_PAL && EFI_PROD_CODE */
