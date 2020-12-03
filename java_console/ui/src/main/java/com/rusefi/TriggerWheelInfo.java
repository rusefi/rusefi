package com.rusefi;

import com.rusefi.trigger.WaveState;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.List;

class TriggerWheelInfo {
    final int id;
    final double tdcPosition;
    final String triggerName;
    final List<WaveState> waves;

    public TriggerWheelInfo(int id, double tdcPosition, String triggerName, List<WaveState> waves) {
        this.id = id;
        this.tdcPosition = tdcPosition;
        this.triggerName = triggerName;
        this.waves = waves;
    }

    static TriggerWheelInfo readTriggerWheelInfo(String line, BufferedReader reader) throws IOException {
        String[] tokens = line.split(" ");
        String idStr = tokens[1];
        int eventCount = Integer.parseInt(tokens[2]);
        String triggerName = tokens[3];
        System.out.println("Processing " + line + " " + idStr);

        int id = Integer.parseInt(idStr);
        double tdcPosition = Double.parseDouble(tokens[4]);

        System.out.println("id=" + id + ", count=" + eventCount + ", name=" + triggerName);

        List<WaveState> waves = TriggerImage.readTrigger(reader, eventCount);


        return new TriggerWheelInfo(id, tdcPosition, triggerName, waves);
    }
}
