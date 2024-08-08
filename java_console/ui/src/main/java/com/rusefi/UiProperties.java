package com.rusefi;

import com.rusefi.core.net.ConnectionAndMeta;

import java.util.Properties;

public class UiProperties {
    private static Properties properties;

    public static boolean usePCAN() {
        return ConnectionAndMeta.getBoolean("show_pcan", getProperties());
    }

    public static boolean useSimulator() {
        return ConnectionAndMeta.getBoolean("show_simulator", getProperties());
    }

    private static Properties getProperties() {
        if (properties == null) {
            properties = ConnectionAndMeta.getProperties();
        }
        return properties;
    }

    // https://github.com/rusefi/rusefi/issues/6777 sad hack: Autoupdate invokes via reflection
    public static void setProperties(Properties properties) {
        UiProperties.properties = properties;
    }

    public static String getWhiteLabel() {
        return ConnectionAndMeta.getWhiteLabel(getProperties());
    }

    public static boolean isDefaultWhitelabel() {
        return ConnectionAndMeta.DEFAULT_WHITE_LABEL.equals(getWhiteLabel());
    }
}
