package com.rusefi.proxy;

import com.rusefi.Timeouts;
import com.rusefi.proxy.client.LocalApplicationProxyContext;
import com.rusefi.tools.online.ProxyClient;

public class NetworkConnectorContext {
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
}
