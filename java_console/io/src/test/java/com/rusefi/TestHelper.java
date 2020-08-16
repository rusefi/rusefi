package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.config.Field;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.server.ControllerInfo;
import com.rusefi.server.SessionDetails;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.tune.xml.Constant;
import org.jetbrains.annotations.NotNull;
import org.mockito.stubbing.Answer;

import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.rusefi.Timeouts.READ_IMAGE_TIMEOUT;
import static org.junit.Assert.assertTrue;

public class TestHelper {
    public static final String LOCALHOST = "localhost";
    public static final String TEST_SIGNATURE_1 = "rusEFI 2020.07.06.frankenso_na6.2468827536";
    public static final String TEST_SIGNATURE_2 = "rusEFI 2020.07.11.proteus_f4.1986715563";
    public static final ControllerInfo CONTROLLER_INFO = new ControllerInfo("name", "make", "code", Fields.TS_SIGNATURE);
    public static final String TEST_TOKEN_1 = "00000000-1234-1234-1234-123456789012";
    public static final String TEST_TOKEN_3 = "33333333-3333-1234-1234-123456789012";
    public static final Answer<?> NEGATIVE_ANSWER = invocation -> {
        throw new UnsupportedOperationException("Not mocked " + invocation);
    };

    @NotNull
    public static ScalarIniField createIniField(Field field) {
        return new ScalarIniField(field.getName(), field.getOffset(), "", field.getType(), 1, "0");
    }

    @NotNull
    public static ConfigurationImage prepareImage(int input, ScalarIniField scalarIniField) {
        ConfigurationImage ci = new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE);

        scalarIniField.setValue(ci, new Constant(scalarIniField.getName(), "", Integer.toString(input), scalarIniField.getDigits()));
        return ci;
    }

    @NotNull
    public static BinaryProtocolServer createVirtualController(ConfigurationImage ci, int port, Listener serverSocketCreationCallback, BinaryProtocolServer.Context context) throws IOException {
        BinaryProtocolState state = new BinaryProtocolState();
        state.setController(ci);
        state.setCurrentOutputs(new byte[1 + Fields.TS_OUTPUT_SIZE]);

        LinkManager linkManager = new LinkManager();
        linkManager.setConnector(LinkConnector.getDetachedConnector(state));
        BinaryProtocolServer server = new BinaryProtocolServer();
        server.start(linkManager, port, serverSocketCreationCallback, context);
        return server;
    }

    @NotNull
    public static IoStream secureConnectToLocalhost(int controllerPort) {
        IoStream targetEcuSocket;
        try {
            targetEcuSocket = new TcpIoStream("[local]", rusEFISSLContext.getSSLSocket(LOCALHOST, controllerPort));
        } catch (IOException e) {
            throw new IllegalStateException("Failed to connect to controller " + LOCALHOST + ":" + controllerPort);
        }
        return targetEcuSocket;
    }

    @NotNull
    public static IoStream connectToLocalhost(int controllerPort, Logger logger) {
        IoStream targetEcuSocket;
        try {
            targetEcuSocket = new TcpIoStream("[local]", new Socket(LOCALHOST, controllerPort));
        } catch (IOException e) {
            throw new IllegalStateException("Failed to connect to controller " + LOCALHOST + ":" + controllerPort);
        }
        return targetEcuSocket;
    }

    public static BinaryProtocolServer createVirtualController(int controllerPort, ConfigurationImage controllerImage, BinaryProtocolServer.Context context) throws InterruptedException {
        CountDownLatch controllerCreated = new CountDownLatch(1);
        try {
            BinaryProtocolServer server = createVirtualController(controllerImage, controllerPort, parameter -> controllerCreated.countDown(), context);
            assertLatch(controllerCreated);
            return server;
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public static SessionDetails createTestSession(String authToken, String signature) {
        ControllerInfo ci = new ControllerInfo("vehicle", "make", "code", signature);

        return new SessionDetails(NetworkConnector.Implementation.Unknown, ci, authToken, SessionDetails.createOneTimeCode(), rusEFIVersion.CONSOLE_VERSION);
    }

    public static void assertLatch(String message, CountDownLatch reconnectCounter) throws InterruptedException {
        assertTrue(message, reconnectCounter.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));
    }

    public static void assertLatch(CountDownLatch reconnectCounter) throws InterruptedException {
        assertTrue(reconnectCounter.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));
    }
}
