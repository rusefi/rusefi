package com.irnems.ui;

import com.irnems.core.Sensor;
import com.irnems.core.SensorCentral;

import java.util.ArrayList;
import java.util.List;

/**
 * Model for RPM reading with a feature of smoothing the displayed value: new value is not displayed if updated
 * value is within 5% range around currently displayed value. Here we rely on the fact that RPM values are coming in
 * constantly
 * <p/>
 * Date: 12/27/12
 * (c) Andrey Belomutskiy
 */
public class RpmModel {
    private static final RpmModel INSTANCE = new RpmModel();
    private static final double SMOOTHING_RATIO = 0.05;
    private int displayedValue;
    private int value;
    private final List<RpmListener> listeners = new ArrayList<RpmListener>();

    public static RpmModel getInstance() {
        return INSTANCE;
    }

    private RpmModel() {
        SensorCentral.getInstance().addListener(Sensor.RPM, new SensorCentral.AdcListener() {
            @Override
            public void onAdcUpdate(SensorCentral model, double value) {
                setValue((int) value);
            }
        });
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
        if (diff > value * SMOOTHING_RATIO)
            displayedValue = value;
        return displayedValue;
    }

    public void addListener(RpmListener listener) {
        listeners.add(listener);
    }

    interface RpmListener {
        void onRpmChange(RpmModel rpm);
    }
}
