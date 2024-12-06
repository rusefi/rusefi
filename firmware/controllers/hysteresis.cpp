//
// Created by kifir on 8/23/24.
//

#include "hysteresis.h"

bool Hysteresis::test(const bool risingCondition, const bool fallingCondition) {
    if (risingCondition) {
        m_state = true;
    } else if (fallingCondition) {
        m_state = false;
    }

    return m_state;
}