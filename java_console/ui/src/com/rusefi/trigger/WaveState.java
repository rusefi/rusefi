package com.rusefi.trigger;

import com.rusefi.TriggerImage;
import com.rusefi.waves.EngineReport;

import java.util.ArrayList;
import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 8/29/2015.
 */
public class WaveState {
    double unusedDown = Double.NaN;
    double prevUp = Double.NaN;

    public List<EngineReport.UpDown> list = new ArrayList<>();

    public void handle(int signal, double angle) {
        if (signal == 1) {
            // down signal
            if (Double.isNaN(prevUp)) {
                // we have down before up, we would need to use it later
                unusedDown = angle;
            } else {
                EngineReport.UpDown ud = new EngineReport.UpDown(TriggerImage.angleToTime(prevUp), 0, TriggerImage.angleToTime(angle), 0);
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
            list.add(0, new EngineReport.UpDown(TriggerImage.angleToTime(prevUp), 0, TriggerImage.angleToTime(unusedDown + 720 * (3 + TriggerImage.EXTRA_COUNT)), 0));
        }
    }

    public int getMaxTime() {
        if (list.isEmpty())
            return 0;
        return list.get(list.size() - 1).downTime;
    }
}
