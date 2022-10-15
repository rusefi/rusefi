package com.rusefi.tools;

import com.rusefi.server.SessionDetails;
import com.rusefi.core.preferences.storage.Node;
import org.jetbrains.annotations.NotNull;

import static com.rusefi.core.preferences.storage.PersistentConfiguration.getConfig;

public class VehicleToken {
    private static final String VEHICLE_TOKEN = "VEHICLE_TOKEN";

    public static int getOrCreate() {
        String value = getConfig().getRoot().getProperty(VEHICLE_TOKEN, null);
        if (value == null || !Node.isNumeric(value)) {
            value = refresh();
        }
        return Integer.parseInt(value);
    }

    @NotNull
    public static String refresh() {
        String value = Integer.toString(SessionDetails.createOneTimeCode());
        getConfig().getRoot().setProperty(VEHICLE_TOKEN, value);
        return value;
    }
}
