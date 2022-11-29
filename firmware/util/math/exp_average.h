/**
 * @file exp_average.h
 */

#pragma once

class ExpAverage {
public:
    float average(float value) {
        current = smoothingFactor * value + (1 - smoothingFactor) * current;
        return current;
    }

    void init(int length) {
        smoothingFactor = 2 / (length + 1.0);
    }

    void reset() {
        current = 0;
        smoothingFactor = 0.5;
    }

private:
    float current = 0;
    float smoothingFactor = 0.5;
};
