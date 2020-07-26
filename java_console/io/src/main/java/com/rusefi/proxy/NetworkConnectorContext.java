package com.rusefi.proxy;

import com.rusefi.Timeouts;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.tools.online.ProxyClient;

public class NetworkConnectorContext {
    public int reconnectDelay() {
        return 15; // this one is seconds
    }

    public int firstPacketTimeout() {
        return Timeouts.CMD_TIMEOUT;
    }

    public int consecutivePacketTimeout() {
        return BinaryProtocolProxy.USER_IO_TIMEOUT;
    }

    public int serverPortForControllers() {
        return ProxyClient.SERVER_PORT_FOR_CONTROLLERS;
    }
}
