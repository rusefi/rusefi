package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.proxy.BaseBroadcastingThread;
import com.rusefi.server.SessionDetails;
import com.rusefi.server.rusEFISSLContext;

import java.io.IOException;
import java.net.Socket;

import static com.rusefi.TestHelper.LOCALHOST;

public class MockRusEfiDevice {
    public static final String TEST_TOKEN_3 = "33333333-3333-1234-1234-123456789012";
    private final SessionDetails sessionDetails;

    public MockRusEfiDevice(String authToken, String signature) {
        sessionDetails = TestHelper.createTestSession(authToken, signature);
    }

    public void connect(int serverPort) throws IOException {
        Socket socket = rusEFISSLContext.getSSLSocket(LOCALHOST, serverPort);
        BaseBroadcastingThread baseBroadcastingThread = new BaseBroadcastingThread(socket,
                sessionDetails,
                TcpIoStream.DisconnectListener.VOID) {
            @Override
            protected void handleCommand(BinaryProtocolServer.Packet packet, TcpIoStream stream) throws IOException {
                super.handleCommand(packet, stream);

                if (packet.getPacket()[0] == Fields.TS_OUTPUT_COMMAND) {
                    GetOutputsCommand.sendOutput(stream);
                }
            }
        };
        baseBroadcastingThread.start();
    }

}
