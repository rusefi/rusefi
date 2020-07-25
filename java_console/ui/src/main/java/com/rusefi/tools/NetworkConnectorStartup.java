package com.rusefi.tools;

import com.rusefi.auth.AutoTokenUtil;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.server.Backend;
import com.rusefi.ui.AuthTokenPanel;

public class NetworkConnectorStartup {
    public static void start() {
        String authToken = AuthTokenPanel.getAuthToken();
        if (!AutoTokenUtil.isToken(authToken)) {
            System.err.println("Please configure authentication token using 'set_auth_token' command");
            return;
        }

        String autoDetectedPort = PortDetector.autoDetectSerial(null);
        if (autoDetectedPort == null) {
            System.err.println(ConsoleTools.RUS_EFI_NOT_DETECTED);
            return;
        }

        NetworkConnector.NetworkConnectorResult networkConnectorResult = NetworkConnector.runNetworkConnector(authToken, autoDetectedPort, Backend.SERVER_PORT_FOR_CONTROLLERS);
        System.out.println("Running with " + networkConnectorResult.getOneTimeToken());
    }
}
