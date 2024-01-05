package com.rusefi.ui.engine;

import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;

import java.util.Map;
import java.util.TreeMap;


// todo: merge with EngineChart
public class NameUtil {
    protected static final Map<String, Sensor> name2sensor = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    static String getUiName(String name) {
        if (name.isEmpty())
            return name;
        if (name.charAt(0) == Fields.PROTOCOL_CRANK1.charAt(0))
            return "Trigger #" + name.substring(1);
        if (name.charAt(0) == Fields.PROTOCOL_COIL1_SHORT_NAME.charAt(0))
            return "Coil #" + name.substring(1);
        if (name.charAt(0) == Fields.PROTOCOL_INJ1_SHORT_NAME.charAt(0))
            return "Injector #" + name.substring(1);
        if (name.charAt(0) == Fields.PROTOCOL_INJ1_STAGE2_SHORT_NAME.charAt(0))
            return "Injector Second Stage #" + name.substring(1);
        return name;
    }
}