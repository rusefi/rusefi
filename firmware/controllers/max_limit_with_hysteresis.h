//
// Created by kifir on 8/20/24.
//

#pragma once

#include "hysteresis.h"

class MaxLimitWithHysteresis {
public:
	bool checkIfLimitIsExceeded(const float value, const float maxLimit, const float hysteresis);
private:
    Hysteresis m_hysteresis;
};