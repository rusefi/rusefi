package com.rusefi;

import com.rusefi.core.preferences.storage.PersistentConfiguration;

public class AutoupdateProperty {
    public static final String AUTO_UPDATE_BUNDLE_PROPERTY = "AUTO_UPDATE_BUNDLE";

    public static boolean get() {
        return PersistentConfiguration.getBoolProperty(AUTO_UPDATE_BUNDLE_PROPERTY, true);
    }
}
