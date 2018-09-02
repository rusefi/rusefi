package com.irnems;

import com.rusefi.core.EngineState;
import com.rusefi.file.FileUtils;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

/**
 * @author Andrey Belomutskiy
 *         8/5/13
 */
public class AdcFilter {
    public static void main(String[] args) {
        String filename = "a.csv";

        final Map<String, String> map = new TreeMap<String, String>();

        final List<Double> values = new ArrayList<Double>();

        EngineState.EngineStateListener listener = new EngineState.EngineStateListenerImpl() {
            @Override
            public void onKeyValue(String key, String value) {
                map.put(key, value);

//                boolean isOurValue = key.equals("clt");
                boolean isOurValue = key.equals("mat");
                if (isOurValue) {
                    double newValue = Double.valueOf(value);
                    if (values.isEmpty() || values.get(values.size() - 1) != newValue)
                        values.add(newValue);
                }
            }

            @Override
            public void afterLine(String fullLine) {
                map.clear();
            }
        };

        EngineState engineState = new EngineState(listener);

        FileUtils.readFile2(filename, engineState);

        System.out.println("Got " + values.size() + " values");

        FileUtils.saveList("v2.csv", values);


        final List<Double> medianValues = MedianFilter.filter(values, 5);
        FileUtils.saveList("v_med.csv", medianValues);


        final List<Double> filtered = SimpleFilter.filter(medianValues, 100);
        FileUtils.saveList("v_fil.csv", filtered);

    }
}
