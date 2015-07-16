package com.rusefi.ui.engine;

import com.rusefi.core.Sensor;

import java.util.Comparator;
import java.util.Map;
import java.util.TreeMap;


// todo: merge with EngineChart
public class NameUtil {
    /**
     * See also 'CRANK1' in rusEfi firmware code
     */
    public static final String CRANK1 = "t1";
    protected static final Map<String, Sensor> name2sensor = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    static {
        name2sensor.put("inj1", Sensor.INJECTOR_1_DWELL);
        name2sensor.put("inj2", Sensor.INJECTOR_2_DWELL);
        name2sensor.put("inj3", Sensor.INJECTOR_3_DWELL);
        name2sensor.put("inj4", Sensor.INJECTOR_4_DWELL);
    }

    static String getUiName(String name) {
        if (name.isEmpty())
            return name;
        if (name.charAt(0) == 't')
            return "Trigger #" + name.substring(1);
        if (name.charAt(0) == 'c')
            return "Coil #" + name.substring(1);
        if (name.charAt(0) == 'i')
            return "Injector #" + name.substring(1);
        return name;
    }
}