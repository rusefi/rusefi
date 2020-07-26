package com.rusefi.proxy;

import com.rusefi.tools.online.ProxyClient;

public class NetworkConnectorContext {
    public int reconnectDelay() {
        return 15; // this one is seconds
    }

    public int serverPortForControllers() {
        return ProxyClient.SERVER_PORT_FOR_CONTROLLERS;
    }
}
