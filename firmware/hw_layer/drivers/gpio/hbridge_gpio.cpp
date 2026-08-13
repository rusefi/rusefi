/*
 * hbridge_gpio.cpp
 *
 * ETB-style H-bridge exposed as general-purpose on/off output pins, so functions like
 * fuel pump or main relay can be assigned to an otherwise unused H-bridge.
 * See https://github.com/rusefi/rusefi/issues/9673
 *
 * Limitations (per issue discussion):
 * - only ONE leg of each H-bridge is usable, the other leg must stay unconnected
 * - internal freewheeling diodes are always present
 * - "on" is full duty through TwoPinDcMotor, battery-voltage compensated like any DC actuator
 */

#include "pch.h"

#include "gpio/gpio_ext.h"
#include "gpio/hbridge_gpio.h"
#include "smart_gpio.h"

#if EFI_PROD_CODE && (BOARD_EXT_GPIOCHIPS > 0)

#include "dc_motors.h"

struct HbridgeGpio : public GpioChip {
	int init() override {
		return 0;
	}

	int writePad(size_t pin, int value) override {
		if (pin >= ETB_COUNT) {
			return -1;
		}

		// Refuse to fight a DC function (ETB/wastegate/...) that owns this slot
		if (engineConfiguration->etbFunctions[pin] != DC_Gpio) {
			return -1;
		}

		lastValue[pin] = value;
		// Safe before the DC hardware is started: an unconfigured TwoPinDcMotor ignores set()
		setDcMotorDuty(pin, value ? 1.0f : 0.0f);
		return 0;
	}

	int lastValue[ETB_COUNT] = {};
};

static HbridgeGpio hbridgeGpio;

void initHbridgeGpio() {
	bool anyGpioFunction = false;
	for (size_t i = 0; i < ETB_COUNT; i++) {
		anyGpioFunction |= engineConfiguration->etbFunctions[i] == DC_Gpio;
	}

	if (!anyGpioFunction) {
		return;
	}

	int ret = gpiochip_register(Gpio::HBRIDGE_1_OUT, "hbridge", hbridgeGpio, ETB_COUNT);
	criticalAssertVoid(ret == (int)Gpio::HBRIDGE_1_OUT, "hbridge gpio");
}

void hbridgeGpioReplayPinState(size_t dcIndex) {
	if (dcIndex >= ETB_COUNT) {
		return;
	}

	setDcMotorDuty(dcIndex, hbridgeGpio.lastValue[dcIndex] ? 1.0f : 0.0f);
}

#else // ! (EFI_PROD_CODE && BOARD_EXT_GPIOCHIPS > 0)

void initHbridgeGpio() {
}

void hbridgeGpioReplayPinState(size_t /*dcIndex*/) {
}

#endif // EFI_PROD_CODE && BOARD_EXT_GPIOCHIPS > 0
