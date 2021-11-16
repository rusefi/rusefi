#pragma once

#include "gppwm.h"

#include "rusefi_types.h"

struct gppwm_channel;
class OutputPin;
struct IPwm;
class ValueProvider3D;

class GppwmChannel {
public:
	void init(bool usePwm, IPwm* pwm, OutputPin* outputPin, const ValueProvider3D* table, const gppwm_channel* config);
	float update();
	percent_t getOutput() const;
	void setOutput(float result);

private:
	// Store the current state so we can apply hysteresis
	bool m_state = false;

	// Configuration fields
	const gppwm_channel* m_config = nullptr;
	bool m_usePwm = false;
	IPwm* m_pwm = nullptr;
	OutputPin* m_output = nullptr;
	const ValueProvider3D* m_table = nullptr;
};
