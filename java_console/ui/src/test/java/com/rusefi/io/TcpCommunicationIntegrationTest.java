package com.rusefi.io;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.Listener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.config.Field;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.tune.xml.Constant;
import org.jetbrains.annotations.NotNull;
import org.junit.Test;

import java.io.*;
import java.net.Socket;
import java.util.Objects;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class TcpCommunicationIntegrationTest {
    private static final Logger LOGGER = Logger.CONSOLE;
    public static final String LOCALHOST = "localhost";

    // todo: implement & test TCP connector restart!
    @Test
    public void testConnectionFailed() throws InterruptedException {
        int port = 6101;

        CountDownLatch failedCountDownLatch = new CountDownLatch(1);

        LinkManager clientManager = new LinkManager(LOGGER);
        clientManager.startAndConnect(Integer.toString(port), new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                System.out.println("Established");
            }

            @Override
            public void onConnectionFailed() {
                System.out.println("onConnectionFailed");
                failedCountDownLatch.countDown();
            }
        });

        assertTrue(failedCountDownLatch.await(30, TimeUnit.SECONDS));
    }

    @Test
    public void testConnectAndTransmitImageOverTcpIp() throws InterruptedException {
        ScalarIniField iniField = createIniField(Fields.CYLINDERSCOUNT);
        int value = 239;
        ConfigurationImage serverImage = prepareImage(value, iniField);
        int port = 6100;

        CountDownLatch serverCreated = new CountDownLatch(1);
        BinaryProtocolServer server = createVirtualController(serverImage, port, parameter -> serverCreated.countDown());
        assertTrue(serverCreated.await(30, TimeUnit.SECONDS));

        CountDownLatch connectionEstablishedCountDownLatch = new CountDownLatch(1);

        // todo: remove CONFIGURATION_RUSEFI_BINARY or nicer API to disable local file load

        LinkManager clientManager = new LinkManager(LOGGER);
        clientManager.startAndConnect(LOCALHOST + ":" + port, new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                connectionEstablishedCountDownLatch.countDown();
            }

            @Override
            public void onConnectionFailed() {
                System.out.println("Failed");
            }
        });
        assertTrue("Connection established", connectionEstablishedCountDownLatch.await(30, TimeUnit.SECONDS));

        assertEquals(0, server.unknownCommands.get());

        BinaryProtocol clientStreamState = clientManager.getCurrentStreamState();
        Objects.requireNonNull(clientStreamState, "clientStreamState");
        ConfigurationImage clientImage = clientStreamState.getControllerConfiguration();
        String clientValue = iniField.getValue(clientImage);
        assertEquals(Double.toString(value), clientValue);

        clientManager.stop();
    }

    @Test
    public void testProxy() throws InterruptedException {
        ConfigurationImage serverImage = prepareImage(239, createIniField(Fields.CYLINDERSCOUNT));
        int controllerPort = 6102;

        CountDownLatch serverCreated = new CountDownLatch(1);
        BinaryProtocolServer server = createVirtualController(serverImage, controllerPort, parameter -> serverCreated.countDown());
        assertTrue(serverCreated.await(30, TimeUnit.SECONDS));
        int proxyPort = 6103;


        IoStream targetEcuSocket;
        try {
            targetEcuSocket = new TcpIoStream(LOGGER, new Socket(LOCALHOST, controllerPort));
        } catch (IOException e) {
            throw new IllegalStateException("Failed to connect to controller " + LOCALHOST + ":" + controllerPort);
        }
        BinaryProtocolProxy.createProxy(targetEcuSocket, proxyPort);

        CountDownLatch connectionEstablishedCountDownLatch = new CountDownLatch(1);

        LinkManager clientManager = new LinkManager(LOGGER);
        clientManager.startAndConnect(LOCALHOST + ":" + proxyPort, new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                connectionEstablishedCountDownLatch.countDown();
            }

            @Override
            public void onConnectionFailed() {
                System.out.println("Failed");
            }
        });
        assertTrue("Connection established", connectionEstablishedCountDownLatch.await(30, TimeUnit.SECONDS));

        clientManager.stop();
    }

    @NotNull
    private static ScalarIniField createIniField(Field field) {
        return new ScalarIniField(field.getName(), field.getOffset(), "", field.getType(), 1);
    }

    @NotNull
    private BinaryProtocolServer createVirtualController(ConfigurationImage ci, int port, Listener serverSocketCreationCallback) {
        BinaryProtocolState state = new BinaryProtocolState();
        state.setController(ci);
        state.setCurrentOutputs(new byte[1 + Fields.TS_OUTPUT_SIZE]);

        LinkManager linkManager = new LinkManager(LOGGER);
        linkManager.setConnector(LinkConnector.getDetachedConnector(state));
        BinaryProtocolServer server = new BinaryProtocolServer(LOGGER);
        server.start(linkManager, port, serverSocketCreationCallback);
        return server;
    }

    @NotNull
    private ConfigurationImage prepareImage(int input, ScalarIniField scalarIniField) {
        ConfigurationImage ci = new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE);

        scalarIniField.setValue(ci, new Constant(scalarIniField.getName(), "", Integer.toString(input)));
        return ci;
    }
}
