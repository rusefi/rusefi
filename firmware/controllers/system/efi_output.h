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
	// initializes pin & registers it in pin repository
	void initPin(const char *msg, brain_pin_e brainPin, pin_output_mode_e outputMode, bool forceInitWithFatalError = false);

	// same as above, with OM_DEFAULT mode
	void initPin(const char *msg, brain_pin_e brainPin);

	// dissociates pin from this output and un-registers it in pin repository
	void deInit();

	bool isInitialized() const;

	bool getAndSet(int logicValue);
	TEST_VIRTUAL void setValue(int logicValue);
	void toggle();
	bool getLogicValue() const;

	brain_pin_diag_e getDiag() const;

#if EFI_GPIO_HARDWARE
	ioportid_t m_port = 0;
	uint8_t m_pin = 0;
#endif /* EFI_GPIO_HARDWARE */

#if EFI_UNIT_TEST
	int unitTestTurnedOnCounter = 0;
#endif

	brain_pin_e m_brainPin = Gpio::Unassigned;

#if (EFI_GPIO_HARDWARE && (BOARD_EXT_GPIOCHIPS > 0))
	/* used for external pins */
	bool ext = false;
#endif /* EFI_GPIO_HARDWARE */

	int8_t m_currentLogicValue = INITIAL_PIN_STATE;
	/**
	 * we track current pin status so that we do not touch the actual hardware if we want to write new pin bit
	 * which is same as current pin value. This maybe helps in case of status leds, but maybe it's a total over-engineering
	 */
private:
	// todo: inline this method?
	void setDefaultPinState(pin_output_mode_e mode);
	void setOnchipValue(int electricalValue);

	pin_output_mode_e m_mode = OM_DEFAULT;
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
	void setName(const char*);
	const char *getShortName() const;
	/**
	 * @return true if pin was stopped
	 */
	bool stop();

	/**
	 * rusEfi Engine Sniffer protocol uses these short names to reduce bytes usage
	 */
	const char* shortName = nullptr;

private:
	const char* m_name = nullptr;
};
