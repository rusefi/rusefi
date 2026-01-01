#pragma once

#include "engine.h"

/**
 * Windowed pulse measurement helper.
 *
 * Usage model:
 *  - onEdge() called from GPIO/EXTI ISR
 *  - update() called from fast callback (~1 kHz)
 */
 
class PulseInput {
public:
    void configure(bool activeHigh, uint32_t windowUs);
    void onEdge(bool pinState, efitick_t nowUs);
    void update(efitick_t nowUs);

    float getAveragePulseWidthUs() const;
    float getDutyCycle() const;
    float getFrequencyHz() const;

private:
    bool activeHigh = false;
    bool currentlyActive = false;

    efitick_t lastEdgeTime = 0;
    efitick_t windowStartTime = 0;

    uint32_t windowUs = 100000;

    efitick_t activeAccumUs = 0;
    uint32_t pulseCount = 0;
};