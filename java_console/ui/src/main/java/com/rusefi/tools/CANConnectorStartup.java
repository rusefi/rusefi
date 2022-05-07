package com.rusefi.tools;

import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.TcpConnector;

import java.io.IOException;

public class CANConnectorStartup {
    public static void start(AbstractIoStream tsStream) throws IOException {
        if (tsStream == null)
            throw new IOException("Failed to initialise connector");

        BinaryProtocolProxy.createProxy(tsStream, TcpConnector.DEFAULT_PORT, BinaryProtocolProxy.ClientApplicationActivityListener.VOID);

    }
}
