package com.rusefi.ui.widgets;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;

import javax.swing.*;

/**
 * 8/2/13
 * (c) Andrey Belomutskiy
 */
public class IdleLabel extends JLabel {
    public IdleLabel() {
//        SensorCentral.getInstance().addListener(Sensor.IDLE_SWITCH, new SensorCentral.SensorListener() {
//            @Override
//            public void onSensorUpdate(double value) {
//                IdleLabel.this.setText("Idle: " + (value == 0));
//            }
//        });
    }
}
