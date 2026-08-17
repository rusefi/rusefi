#include "pch.h"

#include "hbridge_gpio.h"

#include "dc_motors.h"

#if (EFI_PROD_CODE && BOARD_HBRIDGE_GPIO_COUNT > 0) || EFI_UNIT_TEST

class HbridgeGpio final : public GpioChip {
public:
	int init() override {
		for (size_t i = 0; i < ETB_COUNT; i++) {
			m_motors[i] = nullptr;

			if (engineConfiguration->etbFunctions[i] == DC_Gpio) {
				m_motors[i] = initDcMotor("H-bridge GPIO inactive", engineConfiguration->etbIo[i], i,
					engineConfiguration->etb_use_two_wires);
				m_motors[i]->disable("H-bridge GPIO inactive");
			}
		}

		return 0;
	}

	int setPadMode(size_t pin, iomode_t mode) override {
		auto motor = getMotor(pin);
		if (!motor) {
			if (mode == PAL_MODE_OUTPUT_PUSHPULL) {
				configError("H-bridge GPIO %d requires DC function GPIO", static_cast<int>(pin + 1));
			}

			return -1;
		}

		if (mode == PAL_MODE_OUTPUT_PUSHPULL) {
			motor->enable();
		} else {
			motor->disable("H-bridge GPIO inactive");
		}

		return 0;
	}

	int writePad(size_t pin, int value) override {
		auto motor = getMotor(pin);
		if (!motor) {
			return -1;
		}

		motor->set(value ? 1.0f : 0.0f);
		return 0;
	}

	int readPad(size_t pin) override {
		auto motor = getMotor(pin);
		return motor ? motor->get() > 0.5f : -1;
	}

private:
	DcMotor* getMotor(size_t pin) const {
		return pin < ETB_COUNT ? m_motors[pin] : nullptr;
	}

	DcMotor* m_motors[ETB_COUNT] = {};
};

static HbridgeGpio hbridgeGpio;

#endif

int hbridgeGpio_add() {
#if EFI_PROD_CODE && BOARD_HBRIDGE_GPIO_COUNT > 0
	return gpiochip_register(Gpio::HBRIDGE_1_OUT, "H-bridge", hbridgeGpio, ETB_COUNT);
#else
	return 0;
#endif
}

#if EFI_UNIT_TEST
GpioChip& getHbridgeGpioForUnitTest() {
	return hbridgeGpio;
}
#endif
