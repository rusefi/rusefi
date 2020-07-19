package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.proxy.BaseBroadcastingThread;
import com.rusefi.server.ControllerInfo;
import com.rusefi.server.SessionDetails;
import com.rusefi.server.rusEFISSLContext;

import java.io.IOException;
import java.net.Socket;

import static com.rusefi.TestHelper.LOCALHOST;

public class MockRusEfiDevice {
    public static final String TEST_TOKEN_1 = "00000000-1234-1234-1234-123456789012";
    public static final String TEST_TOKEN_3 = "33333333-3333-1234-1234-123456789012";
    private SessionDetails sessionDetails;
    private final Logger logger;

    public MockRusEfiDevice(String authToken, String signature, Logger logger) {
        sessionDetails = createTestSession(authToken, signature);
        this.logger = logger;
    }

    public static SessionDetails createTestSession(String authToken, String signature) {
        ControllerInfo ci = new ControllerInfo("vehicle", "make", "code", signature);

        return new SessionDetails(ci, authToken, SessionDetails.createOneTimeCode());
    }

    public void connect(int serverPort) throws IOException {
        Socket socket = rusEFISSLContext.getSSLSocket(LOCALHOST, serverPort);
        BaseBroadcastingThread baseBroadcastingThread = new BaseBroadcastingThread(socket,
                sessionDetails,
                logger) {
            @Override
            protected void handleCommand(BinaryProtocolServer.Packet packet, TcpIoStream stream) throws IOException {
                super.handleCommand(packet, stream);

                if (packet.getPacket()[0] == Fields.TS_OUTPUT_COMMAND) {
                    byte[] response = new byte[1 + Fields.TS_OUTPUT_SIZE];
                    response[0] = (byte) BinaryProtocolServer.TS_OK.charAt(0);
                    stream.sendPacket(response, logger);
                }
            }
        };
        baseBroadcastingThread.start();
    }
}
