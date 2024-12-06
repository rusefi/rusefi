//
// Created by kifir on 8/20/24.
//

#pragma once

#include "hysteresis.h"

template <typename RisingChecker = StrictChecker, typename FallingChecker = StrictChecker>
class MaxLimitWithHysteresis {
public:
	bool checkIfLimitIsExceeded(const float value, const float maxLimit, const float hysteresis);
private:
    Hysteresis m_hysteresis;
};

template <typename RisingChecker, typename FallingChecker>
bool MaxLimitWithHysteresis<RisingChecker, FallingChecker>::checkIfLimitIsExceeded(
    const float value,
    const float maxLimit,
    const float hysteresis
) {
    return m_hysteresis.test<RisingChecker, FallingChecker>(value, maxLimit, maxLimit - hysteresis);
}