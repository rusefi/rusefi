package com.rusefi.tools;

import com.devexperts.logging.Logging;
import com.rusefi.auth.AutoTokenUtil;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.proxy.NetworkConnectorContext;
import com.rusefi.tools.online.ProxyClient;
import com.rusefi.ui.AuthTokenPanel;

public class NetworkConnectorStartup {
    private final static Logging log = Logging.getLogging(NetworkConnectorStartup.class);
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

        NetworkConnectorContext connectorContext = new NetworkConnectorContext();

        NetworkConnector.NetworkConnectorResult networkConnectorResult = new NetworkConnector().runNetworkConnector(authToken, autoDetectedPort, connectorContext, NetworkConnector.ReconnectListener.VOID);
        log.info("Running with oneTimeToken=" + networkConnectorResult.getOneTimeToken());
    }
}
