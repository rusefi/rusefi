package com.rusefi.io;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.TestHelper;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.BinaryProtocolServer;
import org.junit.Test;

import java.util.Objects;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class TcpCommunicationIntegrationTest {
    private static final Logger LOGGER = Logger.CONSOLE;

    // todo: implement & test TCP connector restart!
    @Test
    public void testConnectionFailed() throws InterruptedException {
        int port = 6101;

        CountDownLatch failedCountDownLatch = new CountDownLatch(1);

        LinkManager clientManager = new LinkManager();
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
        ScalarIniField iniField = TestHelper.createIniField(Fields.CYLINDERSCOUNT);
        int value = 239;
        ConfigurationImage serverImage = TestHelper.prepareImage(value, iniField);
        int port = 6100;

        BinaryProtocolServer server = TestHelper.createVirtualController(port, serverImage);

        CountDownLatch connectionEstablishedCountDownLatch = new CountDownLatch(1);

        // todo: remove CONFIGURATION_RUSEFI_BINARY or nicer API to disable local file load

        LinkManager clientManager = new LinkManager();
        clientManager.startAndConnect(TestHelper.LOCALHOST + ":" + port, new ConnectionStateListener() {
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

        clientManager.close();
    }

    @Test
    public void testProxy() throws InterruptedException {
        ConfigurationImage serverImage = TestHelper.prepareImage(239, TestHelper.createIniField(Fields.CYLINDERSCOUNT));
        int controllerPort = 6102;

        // create virtual controller
        TestHelper.createVirtualController(controllerPort, serverImage);
        int proxyPort = 6103;


        // connect proxy to virtual controller
        IoStream targetEcuSocket = TestHelper.connectToLocalhost(controllerPort, LOGGER);
        BinaryProtocolProxy.createProxy(targetEcuSocket, proxyPort);

        CountDownLatch connectionEstablishedCountDownLatch = new CountDownLatch(1);

        // connect to proxy and read virtual controller through it
        LinkManager clientManager = new LinkManager();
        clientManager.startAndConnect(TestHelper.LOCALHOST + ":" + proxyPort, new ConnectionStateListener() {
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

        clientManager.close();
    }

}
