#include "max_limit_with_hysteresis.h"

bool MaxLimitWithHysteresis::checkIfLimitIsExceeded(const float value, const float maxLimit, const float hysteresis) {
	return m_hysteresis.test(value, maxLimit, maxLimit - hysteresis);
}