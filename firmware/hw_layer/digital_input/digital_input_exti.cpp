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

#ifdef EFI_IRQ_EXTI_HANDOFF_PRIORITY
void efiExtiInit() {
	nvicEnableVector(I2C1_EV_IRQn, EFI_IRQ_EXTI_HANDOFF_PRIORITY);
}

struct ExtiChannel
{
	ExtiCallback Callback = nullptr;
	void* CallbackData;

	// Name is also used as an enable bit
	const char* Name = nullptr;
};

static ExtiChannel channels[16];

// EXT is not able to give you the front direction but you could read the pin in the callback.
int efiExtiEnablePin(const char *msg, brain_pin_e brainPin, uint32_t mode, ExtiCallback cb, void *cb_data) {
	/* paranoid check, in case of Gpio::Unassigned getHwPort will return NULL
	 * and we will fail on next check */
	if (!isBrainPinValid(brainPin)) {
		return -1;
	}

	criticalAssert(msg, "efiExtiEnablePin msg must not be null", -1);

	ioportid_t port = getHwPort(msg, brainPin);
	if (port == NULL) {
		return -1;
	}

	efiSetPadMode(msg, brainPin, PAL_MODE_INPUT);

	int index = getHwPin(msg, brainPin);

	auto& channel = channels[index];

	/* is this index already used? */
	if (channel.Callback) {
		firmwareError(ObdCode::CUSTOM_ERR_PIN_ALREADY_USED_2, "%s: pin %s/index %d: exti index already used by %s (stm32 limitation, cannot use those two pins as event inputs simultaneously)",
			msg,
			hwPortname(brainPin),
			index,
			channel.Name);
		return -1;
	}

	channel.Callback = cb;
	channel.CallbackData = cb_data;
	channel.Name = msg;

	ioline_t line = PAL_LINE(port, index);
	palEnableLineEvent(line, mode);

	return 0;
}

void efiExtiDisablePin(brain_pin_e brainPin)
{
	/* paranoid check, in case of Gpio::Unassigned getHwPort will return NULL
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

static inline void triggerInterrupt() {
	// Manually fire the I2C1_EV interrupt, it will be queued after this interrupt returns
	NVIC->STIR = I2C1_EV_IRQn;
}

struct ExtiQueueEntry {
	efitick_t Timestamp;
	uint8_t Channel;
};

template <typename T, size_t TSize>
class ExtiQueue {
public:
	void push(const T& val) {
		if ((m_write == m_read - 1) || (m_write == TSize - 1 && m_read == 0)) {
			// queue full, drop
			return;
		}

		arr[m_write] = val;
		m_write++;

		// wrap end of list
		if (m_write == TSize) {
			m_write = 0;
		}
	}

	expected<T> pop() {
		if (m_read == m_write) {
			// Queue empty
			return unexpected;
		}

		T value = arr[m_read];
		m_read++;

		// wrap end of list
		if (m_read == TSize) {
			m_read = 0;
		}

		return value;
	}

private:
	T arr[TSize];

	uint8_t m_read = 0;
	uint8_t m_write = 0;
};

static ExtiQueue<ExtiQueueEntry, 32> queue;

static uint8_t overflowCounter = 0;

CH_IRQ_HANDLER(STM32_I2C1_EVENT_HANDLER) {
	OSAL_IRQ_PROLOGUE();

	while (true) {
		// get the timestamp out under lock
		// todo: lock freeeeee!
		__disable_irq();
		auto result = queue.pop();
		__enable_irq();

		// Queue empty, we're done here.
		if (!result) {
			break;
		}

		auto& entry = result.Value;
		auto& timestamp = entry.Timestamp;

		if (timestamp != 0) {
			auto& channel = channels[entry.Channel];

			if (channel.Callback) {
				channel.Callback(channel.CallbackData, timestamp);
			}
		} else {
			overflowCounter++;
		}
	}

	assertInterruptPriority(__func__, EFI_IRQ_EXTI_HANDOFF_PRIORITY);

	OSAL_IRQ_EPILOGUE();
}

uint8_t getExtiOverflowCounter() {
	return overflowCounter;
}

void handleExtiIsr(uint8_t index) {
	// No need to lock anything, we're already the highest-pri interrupt!

	uint32_t pr;
	extiGetAndClearGroup1(1U << index, pr);

	if (pr & (1 << index)) {
		queue.push({getTimeNowNt(), index});

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

#else // not STM32

// TODO: non-stm32 exti
void efiExtiInit() {
	criticalError("exti not supported");
}

int efiExtiEnablePin(const char *, brain_pin_e, uint32_t, ExtiCallback, void *)
{
	return 0;
}
void efiExtiDisablePin(brain_pin_e) { }

uint8_t getExtiOverflowCounter() {
	return 0;
}

#endif

#endif /* HAL_USE_PAL && EFI_PROD_CODE */
