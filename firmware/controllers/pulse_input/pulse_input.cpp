#include "pulse_input.h"
#include "tunerstudio_outputs.h"

void PulseInput::configure(bool activeHigh_, uint32_t windowUs_) {
    activeHigh = activeHigh_;
    windowUs = windowUs_;

    lastEdgeTime = 0;
    windowStartTime = 0;
    activeAccumUs = 0;
    pulseCount = 0;
    currentlyActive = false;
}

void PulseInput::onEdge(bool pinState, efitick_t nowUs) {
    bool isActive = activeHigh ? pinState : !pinState;

    // Accumulate ON time while active
    if (lastEdgeTime != 0 && currentlyActive) {
        activeAccumUs += (nowUs - lastEdgeTime);
    }

    // Count rising edge of active state
    if (!currentlyActive && isActive) {
        pulseCount++;
    }

    currentlyActive = isActive;
    lastEdgeTime = nowUs;
}

void PulseInput::update(efitick_t nowUs) {
    if (windowStartTime == 0) {
        windowStartTime = nowUs;
        return;
    }

    // Close partial pulse at window boundary
    if (currentlyActive && lastEdgeTime != 0) {
        activeAccumUs += (nowUs - lastEdgeTime);
        lastEdgeTime = nowUs;
    }

    efitick_t elapsed = nowUs - windowStartTime;
    if (elapsed < windowUs) {
        return;
    }

    float windowSeconds = windowUs * 1e-6f;

    if (pulseCount > 0) {
        avgPulseWidthUs = (float)activeAccumUs / (float)pulseCount;
        frequencyHz = pulseCount / windowSeconds;
    } else {
        avgPulseWidthUs = 0;
        frequencyHz = 0;
    }

    dutyCycle = (float)activeAccumUs / (float)windowUs;

    // Publish to TunerStudio outputs
    auto* out = getTunerStudioOutputChannels();
    out->pulseInputAvgOnTimeMs = avgPulseWidthUs * 0.001f;
    out->pulseInputDutyCycle = dutyCycle * 100.0f;
    out->pulseInputFrequencyHz = frequencyHz;

    // Reset window
    windowStartTime = nowUs;
    activeAccumUs = 0;
    pulseCount = 0;
}

float PulseInput::getAveragePulseWidthUs() const {
    return avgPulseWidthUs;
}

float PulseInput::getDutyCycle() const {
    return dutyCycle;
}

float PulseInput::getFrequencyHz() const {
    return frequencyHz;
}