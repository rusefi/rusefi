package com.rusefi.proxy;

import com.devexperts.logging.Logging;
import com.rusefi.Timeouts;
import com.rusefi.proxy.client.LocalApplicationProxyContext;
import com.rusefi.tools.online.ProxyClient;

import static com.devexperts.logging.Logging.getLogging;

public class NetworkConnectorContext {
    private static final Logging log = getLogging(NetworkConnectorContext.class);
    private static final int UPDATE_SBC_EXIT_CODE = 15;
    public static final int UPDATE_FIRMWARE_EXIT_CODE = 16;

    public int reconnectDelay() {
        return 15; // this one is seconds
    }

    public int firstPacketTimeout() {
        return Timeouts.CMD_TIMEOUT;
    }

    /**
     * Application is supposed to be constantly poking the controller one way or another
     *
     * @see LocalApplicationProxyContext#gaugePokingPeriod()
     */
    public int consecutivePacketTimeout() {
        return 30 * Timeouts.SECOND;
    }

    public int serverPortForControllers() {
        return ProxyClient.SERVER_PORT_FOR_CONTROLLERS;
    }

    public void onConnectorSoftwareUpdateRequest() {
        log.info("onConnectorSoftwareUpdateRequest");
        System.exit(UPDATE_SBC_EXIT_CODE);
    }

    public void onFirmwareUpdateRequest() {
        log.info("onFirmwareUpdateRequest");
        System.exit(UPDATE_FIRMWARE_EXIT_CODE);
    }
}
