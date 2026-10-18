#pragma once

#include "gppwm.h"

#include "rusefi_types.h"
#include <rusefi/timer.h>

struct gppwm_channel;
class OutputPin;
struct IPwm;
class ValueProvider3D;

struct GppwmResult {
	percent_t Result;
	float X;
	float Y;
};

class GppwmChannel {
public:
	void init(bool usePwm, IPwm* pwm, OutputPin* outputPin, const ValueProvider3D* table, const gppwm_channel* config);
	GppwmResult update(size_t index);
	GppwmResult getOutput(size_t index) const;

	// Returns actual output duty, with hysteresis applied
	float setOutput(float result);

private:
	// Store the current state so we can apply hysteresis
	bool m_state = false;

	// #9386 - how long the output has been in its current state, for minimumStateTime.
	// A default constructed Timer reads as "a very long time ago", so the first switch is free.
	Timer m_stateChangeTimer;

	// Configuration fields
	const gppwm_channel* m_config = nullptr;
	bool m_usePwm = false;
	IPwm* m_pwm = nullptr;
	OutputPin* m_output = nullptr;
	const ValueProvider3D* m_table = nullptr;
};
