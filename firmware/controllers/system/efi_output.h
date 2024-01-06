/*
 * @file efi_output.h
 *
 */

#pragma once

#include "io_pins.h"
#include "smart_gpio.h"
#include <rusefi/timer.h>

// This class acts as a boolean, but has a switch counter inside
class SwitchedState {
public:
    SwitchedState(int8_t *p_state) {
        state = p_state;
    }

    // returns true if the state has been changed
    bool update(bool newState);
    uint16_t getCounter();

    operator bool() const {
        return (bool)*state;
    }

private:
    int8_t *state;
    uint16_t counter = 0;
};

class SimpleSwitchedState {
public:
  SimpleSwitchedState() : state(&value) {
  }
  int8_t value = 0;
  SwitchedState state;
};

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

	bool isInitialized();

	bool getAndSet(int logicValue);
	void setValue(const char *msg, int logicValue, bool isForce = false);
	TEST_VIRTUAL void setValue(int logicValue, bool isForce = false);
	void toggle();
	bool getLogicValue() const;

	brain_pin_diag_e getDiag() const;

#if EFI_GPIO_HARDWARE
	ioportid_t port = 0;
	uint8_t pin = 0;
#endif /* EFI_GPIO_HARDWARE */

#if EFI_UNIT_TEST || EFI_SIMULATOR
	int pinToggleCounter = 0;
#endif

#if EFI_SIMULATOR
	Timer pinToggleTimer;
	uint32_t durationsInStateMs[2];

	void resetToggleStats();
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
	void setDefaultPinState(pin_output_mode_e mode);
	void setOnchipValue(int electricalValue);

	// 4 byte pointer is a bit of a memory waste here
	pin_output_mode_e mode = OM_DEFAULT;
};

/**
 * OutputPin which is reported on Engine Sniffer
 */
class NamedOutputPin : public virtual OutputPin {
public:
	NamedOutputPin();
	explicit NamedOutputPin(const char *name);
	virtual void setHigh(const char *msg);
	virtual void setLow(const char *msg);
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
