package com.rusefi.file;

import com.rusefi.FileLog;
import com.rusefi.models.XYData;

/**
 * 7/18/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class BaseMap {
    public static void main(String[] args) {
        loadData("a.csv", "maf", "af");
    }

    public static XYData loadData(String filename, final String key, final String value) {
        FileLog.MAIN.logLine("Loading from " + filename);
        FileLog.MAIN.logLine("Loading " + key + ">" + value);
        final XYData data = new XYData();

        /*
        EngineState.EngineStateListener listener = new EngineState.EngineStateListenerImpl() {
            Map<String, String> values = new HashMap<String, String>();

            @Override
            public void onKeyValue(String key, String value) {
                values.put(key, value);
            }

            @Override
            public void afterLine(String fullLine) {
                if (values.containsKey("rpm") && values.containsKey(key)) {
                    process(values);
                }
                values.clear();
            }

            private void process(Map<String, String> values) {
                int rpm = (int) Double.parseDouble(values.get("rpm"));
                double k = Double.parseDouble(values.get(key));
                String valueString = values.get(value);
                if (valueString == null)
                    throw new NullPointerException("No value for: " + value);
                float v = Float.parseFloat(valueString);

                data.addPoint(new Point3D(rpm, k, v));
            }
        };
        EngineState engineState = new EngineState(listener);
        engineState.registerStringValueAction(EngineReport.ENGINE_CHART, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
            }
        });
        FileUtils.readFile2(filename, engineState);
        //return AverageData.average(data, 8);
         */
        return data;
    }
}
