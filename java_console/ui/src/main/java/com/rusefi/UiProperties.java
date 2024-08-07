package com.rusefi;

import com.rusefi.core.net.ConnectionAndMeta;

public class UiProperties {
    public static boolean usePCAN() {
        return ConnectionAndMeta.getBoolean("show_pcan");
    }

    public static boolean useSimulator() {
        return ConnectionAndMeta.getBoolean("show_simulator");
    }
}
