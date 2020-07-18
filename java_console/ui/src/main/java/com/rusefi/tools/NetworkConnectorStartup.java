package com.rusefi.tools;

import com.opensr5.Logger;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkManager;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.server.Backend;
import com.rusefi.ui.AuthTokenPanel;

import java.io.IOException;

public class NetworkConnectorStartup {
    public static void start(String[] strings) throws IOException {
        String autoDetectedPort = PortDetector.autoDetectSerial(null);
        if (autoDetectedPort == null) {
            System.err.println(ConsoleTools.RUS_EFI_NOT_DETECTED);
            return;
        }


        LinkManager linkManager = new LinkManager(Logger.CONSOLE)
                .setCompositeLogicEnabled(false)
                .setNeedPullData(false);
        linkManager.startAndConnect(autoDetectedPort, ConnectionStateListener.VOID);

        String authToken = AuthTokenPanel.getAuthToken();
        NetworkConnector.runNetworkConnector(Backend.SERVER_PORT_FOR_CONTROLLERS, linkManager.getConnector().getBinaryProtocol().getStream(), Logger.CONSOLE,
                authToken);
    }
}
