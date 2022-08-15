package com.rusefi.tools;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.ui.StatusConsumer;

import java.io.IOException;

public class CANConnectorStartup {
    public static void start(AbstractIoStream tsStream, StatusConsumer statusListener) throws IOException {
        if (tsStream == null)
            throw new IOException("Failed to initialise connector");

        String signature = BinaryProtocol.getSignature(tsStream);
        if (signature == null) {
            statusListener.append("Error: no ECU signature from " + tsStream);
        } else {
            statusListener.append("Got [" + signature + "] ECU signature via " + tsStream);
        }
        BinaryProtocolProxy.createProxy(tsStream, TcpConnector.DEFAULT_PORT, BinaryProtocolProxy.ClientApplicationActivityListener.VOID, statusListener);

    }
}
