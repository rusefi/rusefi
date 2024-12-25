/**
 * @file exp_average.h
 */

#pragma once

class ExpAverage {
public:
    float initOrAverage(float value) {
        if (current == 0) {
           current = value;
           return current;
        }
        return average(value);
    }

    float average(float value) {
        current = smoothingFactor * value + (1 - smoothingFactor) * current;
        return current;
    }

    void init(int length) {
        setSmoothingFactor(2 / (length + 1.0));
    }

    void setSmoothingFactor(float p_smoothingFactor) {
        smoothingFactor = p_smoothingFactor;
    }

    void reset() {
        current = 0;
        smoothingFactor = 0.5;
    }

private:
    float current = 0;
    float smoothingFactor = 0.5;
};
