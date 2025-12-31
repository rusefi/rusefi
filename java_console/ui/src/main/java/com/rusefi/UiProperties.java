package com.rusefi;

import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.net.PropertiesHolder;

public class UiProperties {
    public static final String SKIP_ECU_TYPE_DETECTION = "skip_ecu_type_detection";

    public static boolean usePCAN() {
        return ConnectionAndMeta.getBoolean("show_pcan", PropertiesHolder.INSTANCE.getProperties());
    }

    public static boolean useSimulator() {
        return ConnectionAndMeta.getBoolean("show_simulator", PropertiesHolder.INSTANCE.getProperties());
    }

    public static String getWhiteLabel() {
        return ConnectionAndMeta.getWhiteLabel(PropertiesHolder.INSTANCE.getProperties());
    }

    public static String getUpdateHelpUrl() {
        return PropertiesHolder.getProperty("UPDATE_FW_HELP_URL", "https://wiki.rusefi.com/HOWTO-Update-Firmware");
    }

    public static boolean skipEcuTypeDetection() {
        return ConnectionAndMeta.getBoolean(SKIP_ECU_TYPE_DETECTION);
    }
}
