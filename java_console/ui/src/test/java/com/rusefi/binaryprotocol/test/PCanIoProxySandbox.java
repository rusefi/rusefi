package com.rusefi.binaryprotocol.test;

import com.rusefi.io.can.PCanIoStream;
import com.rusefi.tools.CANConnectorStartup;

import java.io.IOException;

/**
 * Manual harness that bridges a PCAN stream through the CAN connector startup.
 */
public class PCanIoProxySandbox {
    public static void main(String[] args) throws IOException {
        PCanIoStream stream = PCanIoStream.createStream();
        CANConnectorStartup.start(stream, (status) -> System.out.println("Status: " + status));
    }
}
