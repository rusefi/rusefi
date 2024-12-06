//
// Created by kifir on 8/23/24.
//

#pragma once

#include <cmath>

class StrictChecker {
public:
    static bool isBefore(float x, float y) { return std::isless(x, y); }
};

class UnstrictChecker {
public:
    static bool isBefore(float x, float y) { return std::islessequal(x, y); }
};

class Hysteresis {
public:
    // returns true if value > rising, false if value < falling, previous if falling < value < rising.
    template <typename RisingChecker = StrictChecker, typename FallingChecker = StrictChecker>
    bool test(float value, float rising, float falling);

    bool test(bool risingCondition, bool fallingCondition);
private:
    bool m_state = false;
};

template <typename RisingChecker, typename FallingChecker>
bool Hysteresis::test(const float value, const float rising, const float falling) {
    return test(RisingChecker::isBefore(rising, value), FallingChecker::isBefore(value, falling));
}