package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.binaryprotocol.BinaryProtocolCommands;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.IOException;
import java.net.Socket;

import static com.rusefi.io.TcpCommunicationIntegrationTest.LOCALHOST;

public class MockRusEfiDevice {
    private final String signature;
    private final Logger logger;

    public MockRusEfiDevice(String signature, Logger logger) {
        this.signature = signature;
        this.logger = logger;
    }


    public void connect(int serverPort) throws IOException {
        TcpIoStream stream = new TcpIoStream(logger, new Socket(LOCALHOST, serverPort));



    }
}
