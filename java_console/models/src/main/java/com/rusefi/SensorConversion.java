package com.rusefi;

public interface SensorConversion {
    SensorConversion celsius_to_fahrenheit = celsius -> celsius * 9 / 5 + 32;

    double convertValue(double value);
}