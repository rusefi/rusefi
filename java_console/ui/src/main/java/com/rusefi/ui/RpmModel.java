package com.rusefi.ui;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;

import java.util.ArrayList;
import java.util.List;

/**
 * Model for RPM reading with a feature of smoothing the displayed value: new value is not displayed if updated
 * value is within 5% range around currently displayed value. Here we rely on the fact that RPM values are coming in
 * constantly
 * <p/>
 * Date: 12/27/12
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class RpmModel {
    private static final RpmModel INSTANCE = new RpmModel();
    private static final double SMOOTHING_RATIO = 0.05;
    private int displayedValue;
    private int value;
    private final List<RpmListener> listeners = new ArrayList<>();
    private long timeAtLastUpdate;

    public static RpmModel getInstance() {
        return INSTANCE;
    }

    private RpmModel() {
        SensorCentral.getInstance().addListener(Sensor.RPMGauge, value -> setValue((int) value));
    }

    public void setValue(int rpm) {
        value = rpm;

        for (RpmListener listener : listeners)
            listener.onRpmChange(this);
    }

    public int getValue() {
        return value;
    }

    public int getSmoothedValue() {
        int diff = Math.abs(displayedValue - value);
        boolean isOlderThenOneSecond = System.currentTimeMillis() - timeAtLastUpdate > 1000;
        if (diff > value * SMOOTHING_RATIO || isOlderThenOneSecond) {
            timeAtLastUpdate = System.currentTimeMillis();
            displayedValue = value;
        }
        return displayedValue;
    }

    public void addListener(RpmListener listener) {
        listeners.add(listener);
    }

    interface RpmListener {
        void onRpmChange(RpmModel rpm);
    }
}
