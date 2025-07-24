#pragma once

// Very generic signal debounce
// Unlike ButtonDebounce, works with any arbitrary signal (from ADC, CAN, etc)

#include "rusefi/timer.h"

class SignalDebounce {
public:
    SignalDebounce(float debounceSeconds, bool initialState) :
        period(debounceSeconds),
        state(initialState)
    { }
    SignalDebounce(float debounceSeconds) :
        period(debounceSeconds),
        state(false)
    { }

    // feed it with raw value periodically
    void set(bool newState)
    {
        if (state == newState) {
            timer.reset();
        }
        else if (timer.hasElapsedSec(period)) {
            state = newState;
            timer.reset();
        }
    }

    // get the 'smooth' value
    bool get() const {
        return state;
    }

private:
    Timer timer;
    const float period;
    bool state;
};
