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

    private static synchronized Properties getProperties() {
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

    public static String getUpdateHelpUrl() {
        return properties.getProperty("UPDATE_FW_HELP_URL", "https://github.com/rusefi/rusefi/wiki/HOWTO-Update-Firmware");
    }
}
