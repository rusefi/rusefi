package com.rusefi.tools;

import com.rusefi.auth.AutoTokenUtil;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.server.Backend;
import com.rusefi.server.SessionDetails;
import com.rusefi.ui.AuthTokenPanel;

import java.io.IOException;

public class NetworkConnectorStartup {
    public static void start() throws IOException, InterruptedException {
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

        SessionDetails sessionDetails = NetworkConnector.runNetworkConnector(authToken, autoDetectedPort, Backend.SERVER_PORT_FOR_CONTROLLERS, new TcpIoStream.DisconnectListener() {
            @Override
            public void onDisconnect() {
                System.err.println("Disconnect detected");
                System.exit(-1);
            }
        });
        System.out.println("Running with " + sessionDetails.getOneTimeToken());
    }
}
