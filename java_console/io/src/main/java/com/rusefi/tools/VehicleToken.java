package com.rusefi.tools;

import com.rusefi.server.SessionDetails;
import com.rusefi.ui.storage.Node;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class VehicleToken {
    private static final String VEHICLE_TOKEN = "VEHICLE_TOKEN";

    public static int getOrCreate() {
        String value = getConfig().getRoot().getProperty(VEHICLE_TOKEN, null);
        if (value == null || !Node.isNumeric(value)) {
            value = Integer.toString(SessionDetails.createOneTimeCode());
            getConfig().getRoot().setProperty(VEHICLE_TOKEN, value);
        }
        return Integer.parseInt(value);
    }
}
