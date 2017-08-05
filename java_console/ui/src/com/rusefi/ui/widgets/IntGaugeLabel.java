package com.rusefi.ui.widgets;

import com.rusefi.config.FieldType;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;

import javax.swing.*;

/**
 * Read-only text representation of an int {@link Sensor}
 * <p>
 * (c) Andrey Belomutskiy
 * 8/5/2017
 */
public class IntGaugeLabel extends JLabel {
    public IntGaugeLabel(final String shortName, Sensor sensor) {
        if (sensor.getType() != FieldType.INT)
            throw new IllegalArgumentException(sensor.name());
        SensorCentral.getInstance().addListener(sensor, new SensorCentral.SensorListener() {
            @Override
            public void onSensorUpdate(double value) {
                IntGaugeLabel.this.setText(shortName + ": " + (int)value);
            }
        });
    }
}
