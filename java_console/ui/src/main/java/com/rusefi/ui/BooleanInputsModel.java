package com.rusefi.ui;

import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

/**
 * Date: 1/14/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class BooleanInputsModel {
    private static final Map<Integer, String> inputTitle = new HashMap<Integer, String>();
    private static final Map<Integer, String> trueValueText = new HashMap<Integer, String>();
    private static final Map<Integer, String> falseValueText = new HashMap<Integer, String>();
    private final Map<Integer, Boolean> values = new TreeMap<Integer, Boolean>();

    static {
        inputTitle.put(0, "cranking");
        int idle = 1;
        inputTitle.put(idle, "idle switch");
        trueValueText.put(idle, "idle");
        falseValueText.put(idle, "not idle");
    }

    static String getTitle(int channel) {
        String title = inputTitle.containsKey(channel) ? (": " + inputTitle.get(channel)) : "";
        return "adc " + channel + title;
    }

    public void setValue(int channel, boolean value) {
        values.put(channel, value);
    }

    public static String getValueLabelText(int channel, boolean value) {
        if (value && trueValueText.containsKey(channel))
            return trueValueText.get(channel);
        if (!value && falseValueText.containsKey(channel))
            return falseValueText.get(channel);
        return Boolean.toString(value);
    }
}
