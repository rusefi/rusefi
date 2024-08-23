//
// Created by kifir on 8/23/24.
//

#pragma once

class Hysteresis {
public:
    // returns true if value > rising, false if value < falling, previous if falling < value < rising.
    bool test(float value, float rising, float falling);

    bool test(bool risingCondition, bool fallingCondition);
private:
    bool m_state = false;
};