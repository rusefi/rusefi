package com.rusefi.proxy;

import com.devexperts.logging.Logging;
import com.rusefi.Timeouts;
import com.rusefi.proxy.client.LocalApplicationProxyContext;
import com.rusefi.tools.online.ProxyClient;

import static com.devexperts.logging.Logging.getLogging;

public class NetworkConnectorContext {
    private static final Logging log = getLogging(NetworkConnectorContext.class);
    private static final int UPDATE_LATEST_SBC_EXIT_CODE = 15;
    public static final int UPDATE_LATEST_FIRMWARE_EXIT_CODE = 16;
    private static final int UPDATE_RELEASE_SBC_EXIT_CODE = 17;
    public static final int UPDATE_RELEASE_FIRMWARE_EXIT_CODE = 18;

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

    /*
    public void onConnectorSoftwareUpdateToLatestRequest() {
        log.info("onConnectorSoftwareUpdateToLatestRequest");
        System.exit(UPDATE_LATEST_SBC_EXIT_CODE);
    }

    public void onConnectorSoftwareUpdateToReleaseRequest() {
        log.info("onConnectorSoftwareUpdateToReleaseRequest");
        System.exit(UPDATE_RELEASE_SBC_EXIT_CODE);
    }

    public void onFirmwareUpdateToLatestRequest() {
        log.info("onFirmwareUpdateRequest");
        System.exit(UPDATE_LATEST_FIRMWARE_EXIT_CODE);
    }

    public void onFirmwareUpdateToReleaseRequest() {
        log.info("onFirmwareUpdateRequest");
        System.exit(UPDATE_RELEASE_FIRMWARE_EXIT_CODE);
    }
     */
}
