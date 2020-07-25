package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.server.*;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;
import com.rusefi.tools.online.PublicSession;
import org.jetbrains.annotations.NotNull;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

/**
 * integration test of the rusEFI online backend process
 * At the moment this test is very loose with timing it must be unreliable?
 * <p>
 * https://github.com/rusefi/web_backend/blob/master/documentation/rusEFI%20remote.png
 */
public class ServerTest {
    private final static Logger logger = Logger.CONSOLE;

    @Before
    public void setTestCertificate() throws MalformedURLException {
        commonServerTest();
    }

    static void commonServerTest() throws MalformedURLException {
        HttpUtil.RUSEFI_PROXY_HOSTNAME = TestHelper.LOCALHOST;

        rusEFISSLContext.init("certificate/test_pkcs12.jks", "password");
    }

    @Test
    public void testControllerSessionStartupTimeTimeout() throws InterruptedException, IOException {
        int serverPortForControllers = 7000;
        int httpPort = 8000;

        CountDownLatch serverCreated = new CountDownLatch(1);
        CountDownLatch allClientsDisconnected = new CountDownLatch(1);
        CountDownLatch onConnected = new CountDownLatch(2);

        CountDownLatch allConnected = new CountDownLatch(1);


        try (Backend backend = new Backend(createTestUserResolver(), httpPort) {
            @Override
            public void register(ControllerConnectionState clientConnectionState) {
                super.register(clientConnectionState);
                onConnected.countDown();
                try {
                    allConnected.await();
                } catch (InterruptedException e) {
                    throw new IllegalStateException(e);
                }
                throw new IllegalStateException();
            }

            @Override
            public void close(ControllerConnectionState inactiveClient) {
                super.close(inactiveClient);
                if (getControllersCount() == 0)
                    allClientsDisconnected.countDown();
            }
        }) {

            backend.runControllerConnector(serverPortForControllers, parameter -> serverCreated.countDown());
            assertTrue(serverCreated.await(30, TimeUnit.SECONDS));
            assertEquals(0, backend.getControllersCount());


            new MockRusEfiDevice(MockRusEfiDevice.TEST_TOKEN_1, "rusEFI 2020.07.06.frankenso_na6.2468827536", logger).connect(serverPortForControllers);
            new MockRusEfiDevice("12345678-1234-1234-1234-123456789012", "rusEFI 2020.07.11.proteus_f4.1986715563", logger).connect(serverPortForControllers);

            assertTrue("onConnected", onConnected.await(30, TimeUnit.SECONDS));

            List<ControllerConnectionState> clients = backend.getControllers();
            assertEquals(2, clients.size());

            List<PublicSession> onlineUsers = ProxyClient.getOnlineApplications(HttpUtil.RUSEFI_PROXY_JSON_PROTOCOL + TestHelper.LOCALHOST + ":" + httpPort + ProxyClient.LIST_CONTROLLERS_PATH);
            assertEquals(2, onlineUsers.size());

            allConnected.countDown();

            assertTrue("allClientsDisconnected", allClientsDisconnected.await(30, TimeUnit.SECONDS));
        }
    }

/*
covered by FullServerTest
    @Test
    public void testApplicationTimeout() throws InterruptedException, IOException {
        int serverPortForRemoteUsers = 6999;
        int httpPort = 6998;
        int serverPortForControllers = 6997;
        int controllerPort = 6996;
        int userId = 7;


        UserDetailsResolver userDetailsResolver = authToken -> new UserDetails(authToken.substring(0, 5), userId);

        CountDownLatch controllerRegistered = new CountDownLatch(1);
        try (Backend backend = new Backend(userDetailsResolver, httpPort, logger) {
            @Override
            protected void onRegister(ControllerConnectionState controllerConnectionState) {
                super.onRegister(controllerConnectionState);
                controllerRegistered.countDown();
            }
        }) {

            TestHelper.runApplicationConnectorBlocking(backend, serverPortForRemoteUsers);

            TestHelper.runControllerConnectorBlocking(backend, serverPortForControllers);

            // create virtual controller to which "rusEFI network connector" connects to
            TestHelper.createVirtualController(controllerPort, new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE), logger);

            // start "rusEFI network connector" to connect controller with backend since in real life controller has only local serial port it does not have network
            SessionDetails deviceSessionDetails = NetworkConnector.runNetworkConnector(MockRusEfiDevice.TEST_TOKEN_1, TestHelper.LOCALHOST + ":" + controllerPort, serverPortForControllers);

            assertTrue(controllerRegistered.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));

            SessionDetails authenticatorSessionDetails = new SessionDetails(deviceSessionDetails.getControllerInfo(), MockRusEfiDevice.TEST_TOKEN_3, deviceSessionDetails.getOneTimeToken());
            ApplicationRequest applicationRequest = new ApplicationRequest(authenticatorSessionDetails, userId);


        }
    }
 */

    @Test
    public void testInvalidApplicationRequest() throws InterruptedException, IOException {
        int httpPort = 8001;
        int serverPortForRemoteUsers = 6801;
        CountDownLatch disconnectedCountDownLatch = new CountDownLatch(1);
        try (Backend backend = new Backend(createTestUserResolver(), httpPort) {
            @Override
            protected void onDisconnectApplication(ApplicationConnectionState applicationConnectionState) {
                super.onDisconnectApplication(applicationConnectionState);
                disconnectedCountDownLatch.countDown();
            }
        }) {

            TestHelper.runApplicationConnectorBlocking(backend, serverPortForRemoteUsers);

            // start authenticator
            IoStream authenticatorToProxyStream = TestHelper.secureConnectToLocalhost(serverPortForRemoteUsers, logger);
            new HelloCommand("hello").handle(authenticatorToProxyStream);

            assertTrue(disconnectedCountDownLatch.await(30, TimeUnit.SECONDS));
        }
    }

    @NotNull
    private static UserDetailsResolver createTestUserResolver() {
        return authToken -> new UserDetails(authToken.substring(0, 5), authToken.charAt(6));
    }

    @Test
    public void testAuthenticatorRequestUnknownSession() throws InterruptedException, IOException {
        int serverPortForRemoteUsers = 6800;

        int httpPort = 8002;

        CountDownLatch disconnectedCountDownLatch = new CountDownLatch(1);

        try (Backend backend = new Backend(createTestUserResolver(), httpPort) {
            @Override
            protected void onDisconnectApplication(ApplicationConnectionState applicationConnectionState) {
                super.onDisconnectApplication(applicationConnectionState);
                disconnectedCountDownLatch.countDown();
            }
        }) {

            TestHelper.runApplicationConnectorBlocking(backend, serverPortForRemoteUsers);

            SessionDetails sessionDetails = MockRusEfiDevice.createTestSession(MockRusEfiDevice.TEST_TOKEN_1, Fields.TS_SIGNATURE);
            ApplicationRequest applicationRequest = new ApplicationRequest(sessionDetails, createTestUserResolver().apply(MockRusEfiDevice.TEST_TOKEN_1));

            // start authenticator
            IoStream authenticatorToProxyStream = TestHelper.secureConnectToLocalhost(serverPortForRemoteUsers, logger);
            LocalApplicationProxy.sendHello(authenticatorToProxyStream, applicationRequest);

            assertTrue(disconnectedCountDownLatch.await(30, TimeUnit.SECONDS));
        }
    }
}
