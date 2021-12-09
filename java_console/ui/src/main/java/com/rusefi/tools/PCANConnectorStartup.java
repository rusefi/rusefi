package com.rusefi.tools;

import com.rusefi.io.stream.PCanIoStream;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.TcpConnector;

import java.io.IOException;

public class PCANConnectorStartup {
    public static void start() throws IOException {
        PCanIoStream tsStream = PCanIoStream.getPCANIoStream();
        if (tsStream == null)
            throw new IOException("No PCAN");

        BinaryProtocolProxy.createProxy(tsStream, TcpConnector.DEFAULT_PORT, BinaryProtocolProxy.ClientApplicationActivityListener.VOID);

    }
}
