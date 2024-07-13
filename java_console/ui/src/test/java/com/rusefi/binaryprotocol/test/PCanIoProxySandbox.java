package com.rusefi.binaryprotocol.test;

import com.rusefi.io.can.PCanIoStream;
import com.rusefi.tools.CANConnectorStartup;

import java.io.IOException;

public class PCanIoProxySandbox {
    public static void main(String[] args) throws IOException {
        PCanIoStream stream = PCanIoStream.createStream();
        CANConnectorStartup.start(stream, (status, breakLineOnTextArea, sendToLogger) -> System.out.println("Status: " + status));
    }
}
