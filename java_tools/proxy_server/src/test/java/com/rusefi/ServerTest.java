package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.proxy.BaseBroadcastingThread;
import com.rusefi.proxy.NetworkConnectorContext;
import com.rusefi.proxy.client.LocalApplicationProxy;
import com.rusefi.server.*;
import com.rusefi.tools.online.HttpUtil;
import com.rusefi.tools.online.ProxyClient;
import com.rusefi.tools.online.PublicSession;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.Socket;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import static com.rusefi.TestHelper.assertLatch;
import static org.junit.Assert.assertEquals;

/**
 * integration test of the rusEFI online backend process
 * At the moment this test is very loose with timing it must be unreliable?
 * <p>
 * https://github.com/rusefi/web_backend/blob/master/documentation/rusEFI%20remote.png
 */
public class ServerTest {
    @Before
    public void setup() throws MalformedURLException {
        BackendTestHelper.commonServerTest();
    }

    @Test
    public void testControllerSessionStartupTimeTimeout() throws InterruptedException, IOException {
        int serverPortForControllers = 7000;
        int httpPort = 8000;

        CountDownLatch serverCreated = new CountDownLatch(1);
        CountDownLatch allClientsDisconnected = new CountDownLatch(1);
        CountDownLatch onConnected = new CountDownLatch(2);

        CountDownLatch allConnected = new CountDownLatch(1);


        try (Backend backend = new Backend(BackendTestHelper.createTestUserResolver(), httpPort) {
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
            assertLatch(serverCreated);
            assertEquals(0, backend.getControllersCount());


            new MockNetworkConnector(TestHelper.TEST_TOKEN_1, TestHelper.TEST_SIGNATURE_1).connect(serverPortForControllers);
            new MockNetworkConnector("12345678-1234-1234-1234-123456789012", TestHelper.TEST_SIGNATURE_2).connect(serverPortForControllers);

            assertLatch("onConnected", onConnected);

            List<ControllerConnectionState> clients = backend.getControllers();
            assertEquals(2, clients.size());

            List<PublicSession> onlineUsers = ProxyClient.getOnlineApplications(HttpUtil.RUSEFI_PROXY_JSON_PROTOCOL + TcpConnector.LOCALHOST + ":" + httpPort + ProxyClient.LIST_CONTROLLERS_PATH);
            assertEquals(2, onlineUsers.size());

            allConnected.countDown();

            assertLatch("allClientsDisconnected", allClientsDisconnected);
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
            SessionDetails deviceSessionDetails = NetworkConnector.start(MockRusEfiDevice.TEST_TOKEN_1, TestHelper.LOCALHOST + ":" + controllerPort, serverPortForControllers);

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
        try (Backend backend = new Backend(BackendTestHelper.createTestUserResolver(), httpPort) {
            @Override
            protected void onDisconnectApplication(ApplicationConnectionState applicationConnectionState) {
                super.onDisconnectApplication(applicationConnectionState);
                disconnectedCountDownLatch.countDown();
            }
        }) {

            BackendTestHelper.runApplicationConnectorBlocking(backend, serverPortForRemoteUsers);

            // start authenticator
            IoStream authenticatorToProxyStream = TestHelper.secureConnectToLocalhost(serverPortForRemoteUsers);
            new HelloCommand("hello").handle(authenticatorToProxyStream);

            assertLatch(disconnectedCountDownLatch);
        }
    }

    @Test
    public void testAuthenticatorRequestUnknownSession() throws InterruptedException, IOException {
        int serverPortForRemoteUsers = 6800;

        int httpPort = 8002;

        CountDownLatch disconnectedCountDownLatch = new CountDownLatch(1);

        try (Backend backend = new Backend(BackendTestHelper.createTestUserResolver(), httpPort) {
            @Override
            protected void onDisconnectApplication(ApplicationConnectionState applicationConnectionState) {
                super.onDisconnectApplication(applicationConnectionState);
                disconnectedCountDownLatch.countDown();
            }
        }) {

            BackendTestHelper.runApplicationConnectorBlocking(backend, serverPortForRemoteUsers);

            SessionDetails sessionDetails = TestHelper.createTestSession(TestHelper.TEST_TOKEN_1, Fields.TS_SIGNATURE);
            ApplicationRequest applicationRequest = new ApplicationRequest(sessionDetails, BackendTestHelper.createTestUserResolver().apply(TestHelper.TEST_TOKEN_1));

            // start authenticator
            IoStream authenticatorToProxyStream = TestHelper.secureConnectToLocalhost(serverPortForRemoteUsers);
            LocalApplicationProxy.sendHello(authenticatorToProxyStream, applicationRequest);

            assertLatch(disconnectedCountDownLatch);
        }
    }

    private static class MockNetworkConnector {
        private final SessionDetails sessionDetails;

        private MockNetworkConnector(String authToken, String signature) {
            sessionDetails = TestHelper.createTestSession(authToken, signature);
        }

        public void connect(int serverPort) throws IOException {
            Socket socket = rusEFISSLContext.getSSLSocket(TcpConnector.LOCALHOST, serverPort);
            BaseBroadcastingThread baseBroadcastingThread = new BaseBroadcastingThread(socket,
                    sessionDetails,
                    TcpIoStream.DisconnectListener.VOID, new NetworkConnectorContext()) {
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
}
