package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.server.ControllerInfo;
import com.rusefi.server.SessionDetails;

import javax.net.ssl.SSLSocketFactory;
import java.io.IOException;
import java.net.Socket;

import static com.rusefi.tools.online.ProxyClient.LOCALHOST;

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
        Socket socket = SSLSocketFactory.getDefault().createSocket(LOCALHOST, serverPort);
        BaseBroadcastingThread baseBroadcastingThread = new BaseBroadcastingThread(socket,
                sessionDetails,
                logger);
        baseBroadcastingThread.start();
    }
}
