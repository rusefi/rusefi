package com.rusefi.tools;

import com.rusefi.ts_plugin.ui.AuthTokenPanel;

/*
public class NetworkConnectorStartup {
    private final static Logging log = Logging.getLogging(NetworkConnectorStartup.class);

    public static void start() {
        String authToken = AuthTokenPanel.getAuthToken();
        if (!AuthTokenUtil.isToken(authToken)) {
            System.err.println("Please configure authentication token using 'set_auth_token' command");
            return;
        }

        String autoDetectedPort = PortDetector.autoDetectSerial(null).getSerialPort();
        if (autoDetectedPort == null) {
            System.err.println(ConsoleTools.RUS_EFI_NOT_DETECTED);
            return;
        }

        NetworkConnectorContext connectorContext = new NetworkConnectorContext();

        NetworkConnector.NetworkConnectorResult networkConnectorResult = new NetworkConnector().start(NetworkConnector.Implementation.SBC, authToken, autoDetectedPort, connectorContext, new NetworkConnector.ReconnectListener() {
            @Override
            public void onReconnect() {
                try {
                    String version = HttpUtil.executeGet(ProxyClient.getHttpAddress(HttpUtil.PROXY_JSON_API_HTTP_PORT) + ProxyClient.VERSION_PATH);
                    if (!version.contains(ProxyClient.BACKEND_VERSION)) {
                        String message = "Unexpected backend version " + version + " while we want " + ProxyClient.BACKEND_VERSION;
                        log.error(message);
                        System.out.println(message);
                        // let's give wrapper bash script a chance to update software
                        System.exit(NetworkConnectorContext.UPDATE_RELEASE_FIRMWARE_EXIT_CODE);
                    }
                } catch (IOException e) {
                    log.error("Ignoring", e);
                }
            }
        });
        log.info("Running with oneTimeToken=" + networkConnectorResult.getOneTimeToken());
    }
}
*/
