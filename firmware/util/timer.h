#pragma once

#include "efitime.h"

class Timer {
public:
	void reset();
	bool hasElapsedSec(float seconds) const;
	bool hasElapsedMs(float ms) const;
	bool hasElapsedUs(float us) const;

private:
	efitick_t m_lastReset = INT64_MIN;
};
