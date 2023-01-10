package com.rusefi.trigger;

import org.jetbrains.annotations.NotNull;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class TriggerSignalReader {
    @NotNull
    public static List<TriggerSignal> readSignals(BufferedReader reader, int count) throws IOException {
        String line;
        String[] tokens;
        List<TriggerSignal> signals = new ArrayList<>();

        int index = 0;
        while (index < count) {
            line = reader.readLine();
            if (line.trim().startsWith("#"))
                continue;
            tokens = line.split(" ");
            if (tokens.length < 4)
                throw new IllegalStateException("Unexpected [" + line + "]");
            int signalIndex = Integer.parseInt(tokens[2]);
            int signalState = Integer.parseInt(tokens[3]);
            double angle = Double.parseDouble(tokens[4]);
            double gap = parseDoubleMaybeNaN(tokens[5]);

            TriggerSignal s = new TriggerSignal(signalIndex, signalState, angle, gap);
//            System.out.println(s);
            signals.add(s);
            index++;
        }
        return signals;
    }

    private static double parseDoubleMaybeNaN(String doubleString) {
        if ("nan".equalsIgnoreCase(doubleString))
            return Double.NaN;
        return Double.parseDouble(doubleString);
    }
}
