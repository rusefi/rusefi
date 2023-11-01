
#include "pch.h"

#include "protected_gpio.h"
#include "gpio/gpio_ext.h"

#define PROTECTED_CHANNEL_COUNT 8

class ProtectedGpio {
public:
	int setPadMode(iomode_t mode);
	int set(bool value);
	int get() const;
	brain_pin_diag_e getDiag() const;

	void configure(const ProtectedGpioConfig& config);
	void check(efitick_t nowNt);

private:
	OutputPin m_output;

	const ProtectedGpioConfig* m_config;
};

void ProtectedGpio::configure(const ProtectedGpioConfig& cfg) {
	m_config = &cfg;
}

int ProtectedGpio::setPadMode(iomode_t mode) {
	if (mode == PAL_MODE_OUTPUT_PUSHPULL) {
		m_output.initPin("protected", m_config->Pin);
	} else {
		m_output.deInit();
	}

	return 0;
}

int ProtectedGpio::set(bool value) {
	if (!m_config) {
		return -1;
	}

	// TODO: operate state machine to handle overcurrent
	m_output.setValue(value);

	return 0;
}

int ProtectedGpio::get() const {
	return m_output.getLogicValue();
}

void ProtectedGpio::check(efitick_t /*nowNt*/) {
	if (!m_config) {
		return;
	}

	float senseVolts = getVoltage("protected", m_config->SenseChannel);
	float amps = senseVolts * m_config->AmpsPerVolt;

	// TODO: smarter state machine
	if (amps > m_config->MaximumAllowedCurrent) {
		m_output.setValue(false);
	}
}

brain_pin_diag_e ProtectedGpio::getDiag() const {
	// TODO: return PIN_OVERLOAD if there's a problem
	return PIN_OK;
}

class ProtectedGpios : public GpioChip {
public:
	int init() override { return 0; }
	int setPadMode(size_t pin, iomode_t mode) override;
	int writePad(size_t pin, int value) override;
	int readPad(size_t pin) override;

	brain_pin_diag_e getDiag(size_t pin) override;

	void configure(const ProtectedGpioConfig* const configs);
	void check(efitick_t nowNt);

private:
	ProtectedGpio m_channels[PROTECTED_CHANNEL_COUNT];
};

int ProtectedGpios::setPadMode(size_t pin, iomode_t mode) {
	if (pin >= PROTECTED_CHANNEL_COUNT) {
		return -1;
	}

	return m_channels[pin].setPadMode(mode);
}

int ProtectedGpios::writePad(size_t pin, int value) {
	if (pin >= PROTECTED_CHANNEL_COUNT) {
		return -1;
	}

	return m_channels[pin].set(value);
}

int ProtectedGpios::readPad(size_t pin) {
	if (pin >= PROTECTED_CHANNEL_COUNT) {
		return -1;
	}

	return m_channels[pin].get();
}

brain_pin_diag_e ProtectedGpios::getDiag(size_t pin) {
	if (pin >= PROTECTED_CHANNEL_COUNT) {
		return PIN_INVALID;
	}

	return m_channels[pin].getDiag();
}

void ProtectedGpios::configure(const ProtectedGpioConfig* const configs) {
	for (size_t i = 0; i < efi::size(m_channels); i++) {
		m_channels[i].configure(configs[i]);
	}
}

void ProtectedGpios::check(efitick_t nowNt) {
	for (size_t i = 0; i < efi::size(m_channels); i++) {
		m_channels[i].check(nowNt);
	}
}

static ProtectedGpios protectedGpios;
static bool didInit = false;

int protectedGpio_add(brain_pin_e base, const ProtectedGpioConfig* const configs) {
	protectedGpios.configure(configs);

	int result = gpiochip_register(base, "protected", protectedGpios, PROTECTED_CHANNEL_COUNT);

	if (result == static_cast<int>(base)) {
		didInit = true;
	}

	return result;
}

void protectedGpio_check(efitick_t nowNt) {
	if (didInit) {
		protectedGpios.check(nowNt);
	}
}
