package com.rusefi.tools;

import com.devexperts.logging.Logging;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.TcpConnector;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class CANConnectorStartup {
    private static final Logging log = getLogging(CANConnectorStartup.class);

    public static void start(AbstractIoStream tsStream) throws IOException {
        if (tsStream == null)
            throw new IOException("Failed to initialise connector");

        String signature = BinaryProtocol.getSignature(tsStream);
        log.info("Got [" + signature + "] signature via " + tsStream);

        BinaryProtocolProxy.createProxy(tsStream, TcpConnector.DEFAULT_PORT, BinaryProtocolProxy.ClientApplicationActivityListener.VOID);

    }
}
