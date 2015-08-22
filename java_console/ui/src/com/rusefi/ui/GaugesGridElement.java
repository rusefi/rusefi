package com.rusefi.ui;

import com.rusefi.core.Sensor;
import com.rusefi.ui.widgets.SensorGauge;

import java.awt.*;

/**
 * (c) Andrey Belomutskiy
 * 8/21/2015.
 */
public class GaugesGridElement {
    public static Component createGauge(Sensor sensor, SensorGauge.GaugeChangeListener gaugeChangeListener) {
        return SensorGauge.createGauge(sensor, gaugeChangeListener);
    }
}
