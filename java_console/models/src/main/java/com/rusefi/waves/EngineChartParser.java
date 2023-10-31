package com.rusefi.waves;

import java.util.HashMap;
import java.util.Map;

/**
 * 1/26/14
 * Andrey Belomutskiy (c) 2012-2014
 */
public class EngineChartParser {
    public static final String DELI = "!";

    /**
     * This method unpacks a mixed-key message into a Map of messages by key
     */
    public static EngineChart unpackToMap(String value) {
        if (value == null)
            throw new NullPointerException("value");

        String[] array = value.split(DELI);

        Map<String, StringBuilder> map = new HashMap<>();

        int index = 0;
        while (index + 2 < array.length) {
            String name = array[index];

            StringBuilder sb = map.get(name);
            if (sb == null) {
                sb = new StringBuilder();
                map.put(name, sb);
            }

            String signal = array[index + 1];
            String val = array[index + 2];

            sb.append(signal).append(DELI).append(val).append(DELI);
            index += 3;
        }
        return new EngineChart(map);
    }
}
