#pragma once

#include "gppwm.h"

class OutputPin;
class SimplePwm;
class ValueProvider3D;

class GppwmChannel {
public:
	DECLARE_ENGINE_PTR;

	void init(bool usePwm, SimplePwm* pwm, OutputPin* outputPin, const ValueProvider3D* table, const gppwm_channel* config);
	float update();
	percent_t getOutput() const;
	void setOutput(float result);

private:

	// Store the current state so we can apply hysteresis
	bool m_state = false;

	// Configuration fields
	const gppwm_channel* m_config = nullptr;
	bool m_usePwm = false;
	SimplePwm* m_pwm = nullptr;
	OutputPin* m_output = nullptr;
	const ValueProvider3D* m_table = nullptr;
};
