/*
 * @file efi_output.h
 *
 */

#include "io_pins.h"
#include "smart_gpio.h"

#pragma once

// Used if you want a function to be virtual only for unit testing purposes
#if EFI_UNIT_TEST
#define TEST_VIRTUAL virtual
#else
#define TEST_VIRTUAL
#endif

/**
 * @brief   Single output pin reference and state
 */
class OutputPin {
public:
	OutputPin();
	/**
	 * initializes pin & registers it in pin repository
	 * outputMode being a pointer allow us to change configuration (for example invert logical pin) in configuration and get resuts applied
	 * away, or at least I hope that's why
	 */
	void initPin(const char *msg, brain_pin_e brainPin, const pin_output_mode_e *outputMode, bool forceInitWithFatalError = false);
	/**
	 * same as above, with DEFAULT_OUTPUT mode
	 */
	void initPin(const char *msg, brain_pin_e brainPin);

	/**
	 * dissociates pin from this output and un-registers it in pin repository
	 */
	void deInit();

	bool isInitialized();

	bool getAndSet(int logicValue);
	TEST_VIRTUAL void setValue(int logicValue);
	void toggle();
	bool getLogicValue() const;

	brain_pin_diag_e getDiag() const;

#if EFI_GPIO_HARDWARE
	ioportid_t port = 0;
	uint8_t pin = 0;
#endif /* EFI_GPIO_HARDWARE */

#if EFI_UNIT_TEST
	int unitTestTurnedOnCounter = 0;
#endif

	brain_pin_e brainPin = Gpio::Unassigned;

#if (EFI_GPIO_HARDWARE && (BOARD_EXT_GPIOCHIPS > 0))
	/* used for external pins */
	bool ext = false;
#endif /* EFI_GPIO_HARDWARE */

	int8_t currentLogicValue = INITIAL_PIN_STATE;
	/**
	 * we track current pin status so that we do not touch the actual hardware if we want to write new pin bit
	 * which is same as current pin value. This maybe helps in case of status leds, but maybe it's a total over-engineering
	 */
private:
	// todo: inline this method?
	void setDefaultPinState(const pin_output_mode_e *defaultState);
	void setOnchipValue(int electricalValue);

	// 4 byte pointer is a bit of a memory waste here
	const pin_output_mode_e *modePtr = nullptr;
};

/**
 * OutputPin which is reported on Engine Sniffer
 */
class NamedOutputPin : public virtual OutputPin {
public:
	NamedOutputPin();
	explicit NamedOutputPin(const char *name);
	virtual void setHigh();
	virtual void setLow();
	const char *getName() const;
	const char *getShortName() const;
	/**
	 * @return true if pin was stopped
	 */
	bool stop();
	// todo: char pointer is a bit of a memory waste here, we can reduce RAM usage by software-based getName() method
	const char *name = nullptr;
	/**
	 * rusEfi Engine Sniffer protocol uses these short names to reduce bytes usage
	 */
	const char *shortName = nullptr;
};
