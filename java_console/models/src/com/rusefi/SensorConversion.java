package com.rusefi;

public interface SensorConversion {
    SensorConversion C_TO_F = new SensorConversion() {
        @Override
        public double convertValue(double c) {
            return c * 9 / 5 + 32;
        }
    };

    double convertValue(double value);
}