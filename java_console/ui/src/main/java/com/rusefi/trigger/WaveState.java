package com.rusefi.trigger;

import com.rusefi.waves.EngineReport;

import java.util.ArrayList;
import java.util.List;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 8/29/2015.
 */
public class WaveState {
    double unusedDown = Double.NaN;
    double prevUp = Double.NaN;

    public List<EngineReport.UpDown> list = new ArrayList<>();

    private static int angleToTime(double prevUp) {
        return (int) (prevUp);
    }

    public enum trigger_value_e {
        TV_LOW,
        TV_HIGH
    }

    public void handle(WaveState.trigger_value_e signal, double angle) {
        if (signal == trigger_value_e.TV_LOW) {
            // down signal
            if (Double.isNaN(prevUp)) {
                // we have down before up, we would need to use it later
                unusedDown = angle;
            } else {
                EngineReport.UpDown ud = new EngineReport.UpDown(angleToTime(prevUp), 0, angleToTime(angle), 0);
                list.add(ud);
            }
            prevUp = Double.NaN;
        } else {
            // up signal handling
            prevUp = angle;
        }
    }

    public void wrap() {
        if (!Double.isNaN(unusedDown)) {
            list.add(0, new EngineReport.UpDown(angleToTime(prevUp), 0, angleToTime(unusedDown + 720 * (3 + TriggerImage.EXTRA_COUNT)), 0));
        }
    }
}
