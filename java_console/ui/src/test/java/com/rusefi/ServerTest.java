package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.server.*;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.rusefi.Timeouts.READ_IMAGE_TIMEOUT;
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
        HttpUtil.RUSEFI_PROXY_HOSTNAME = ProxyClient.LOCALHOST;

        rusEFISSLContext.init("certificate/test_pkcs12.jks", "password");
    }

    @Test
    public void testControllerSessionTimeout() throws InterruptedException, IOException {
        int serverPortForControllers = 7000;
        int httpPort = 8000;
        UserDetailsResolver userDetailsResolver = authToken -> new UserDetails(authToken.substring(0, 5), authToken.charAt(6));

        CountDownLatch serverCreated = new CountDownLatch(1);
        CountDownLatch allClientsDisconnected = new CountDownLatch(1);
        CountDownLatch onConnected = new CountDownLatch(2);

        try (Backend backend = new Backend(userDetailsResolver, httpPort, logger) {
            @Override
            public void register(ControllerConnectionState clientConnectionState) {
                super.register(clientConnectionState);
                onConnected.countDown();
            }

            @Override
            public void close(ControllerConnectionState inactiveClient) {
                super.close(inactiveClient);
                if (getCount() == 0)
                    allClientsDisconnected.countDown();
            }
        }) {

            backend.runControllerConnector(serverPortForControllers, parameter -> serverCreated.countDown());
            assertTrue(serverCreated.await(30, TimeUnit.SECONDS));
            assertEquals(0, backend.getCount());


            new MockRusEfiDevice(MockRusEfiDevice.TEST_TOKEN_1, "rusEFI 2020.07.06.frankenso_na6.2468827536", logger).connect(serverPortForControllers);
            new MockRusEfiDevice("12345678-1234-1234-1234-123456789012", "rusEFI 2020.07.11.proteus_f4.1986715563", logger).connect(serverPortForControllers);

            assertTrue("onConnected", onConnected.await(30, TimeUnit.SECONDS));

            List<ControllerConnectionState> clients = backend.getClients();
            assertEquals(2, clients.size());

            List<UserDetails> onlineUsers = ProxyClient.getOnlineUsers(httpPort);
            assertEquals(2, onlineUsers.size());

            assertTrue("allClientsDisconnected", allClientsDisconnected.await(30, TimeUnit.SECONDS));
        }
    }

    @Test
    public void testInvalidApplicationRequest() throws InterruptedException, IOException {
        UserDetailsResolver userDetailsResolver = authToken -> new UserDetails(authToken.substring(0, 5), authToken.charAt(6));
        int httpPort = 8001;
        int serverPortForRemoteUsers = 6801;
        CountDownLatch disconnectedCountDownLatch = new CountDownLatch(1);
        try (Backend backend = new Backend(userDetailsResolver, httpPort, logger) {
            @Override
            protected void onDisconnectApplication() {
                super.onDisconnectApplication();
                disconnectedCountDownLatch.countDown();
            }
        }) {

            CountDownLatch applicationServerCreated = new CountDownLatch(1);
            backend.runApplicationConnector(serverPortForRemoteUsers, parameter -> applicationServerCreated.countDown());
            assertTrue(applicationServerCreated.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));

            // start authenticator
            IoStream authenticatorToProxyStream = TestHelper.secureConnectToLocalhost(serverPortForRemoteUsers, logger);
            new HelloCommand(logger, "hello").handle(authenticatorToProxyStream);

            assertTrue(disconnectedCountDownLatch.await(30, TimeUnit.SECONDS));
        }
    }

    @Test
    public void testAuthenticatorRequestUnknownSession() throws InterruptedException, IOException {
        int serverPortForRemoteUsers = 6800;

        UserDetailsResolver userDetailsResolver = authToken -> new UserDetails(authToken.substring(0, 5), authToken.charAt(6));
        int httpPort = 8002;

        CountDownLatch disconnectedCountDownLatch = new CountDownLatch(1);

        try (Backend backend = new Backend(userDetailsResolver, httpPort, logger) {
            @Override
            protected void onDisconnectApplication() {
                super.onDisconnectApplication();
                disconnectedCountDownLatch.countDown();
            }
        }) {

            CountDownLatch applicationServerCreated = new CountDownLatch(1);
            backend.runApplicationConnector(serverPortForRemoteUsers, parameter -> applicationServerCreated.countDown());
            assertTrue(applicationServerCreated.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));

            SessionDetails sessionDetails = MockRusEfiDevice.createTestSession(MockRusEfiDevice.TEST_TOKEN_1, Fields.TS_SIGNATURE);
            ApplicationRequest applicationRequest = new ApplicationRequest(sessionDetails, 123);

            // start authenticator
            IoStream authenticatorToProxyStream = TestHelper.secureConnectToLocalhost(serverPortForRemoteUsers, logger);
            LocalApplicationProxy localApplicationProxy = new LocalApplicationProxy(logger, applicationRequest);
            localApplicationProxy.run(authenticatorToProxyStream);

            assertTrue(disconnectedCountDownLatch.await(30, TimeUnit.SECONDS));
        }
    }
}
