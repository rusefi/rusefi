package com.rusefi.proxy.client;

import com.rusefi.BackendTestHelper;
import com.rusefi.TestHelper;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.commands.GetOutputsCommand;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.ServerSocketReference;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.server.ApplicationRequest;
import com.rusefi.server.SessionDetails;
import com.rusefi.tools.online.ProxyClient;
import com.rusefi.ui.StatusConsumer;
import org.jetbrains.annotations.NotNull;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.BackendTestHelper.createTestUserResolver;
import static com.rusefi.TestHelper.*;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.binaryprotocol.BinaryProtocol.findCommand;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.config.generated.Fields.TS_PROTOCOL;
import static com.rusefi.io.tcp.BinaryProtocolServer.getPacketLength;
import static com.rusefi.io.tcp.TcpConnector.LOCALHOST;
import static com.rusefi.shared.FileUtil.close;
import static org.junit.Assert.*;

public class LocalApplicationProxyTest {
    private static final AtomicInteger portNumber = new AtomicInteger(4000);

    @Before
    public void setup() throws MalformedURLException {
        BackendTestHelper.commonServerTest();
    }

    @Test
    public void testDisconnectCallback() throws IOException, InterruptedException {
        LocalApplicationProxyContext context = createLocalApplicationProxy();

        CountDownLatch backendCreated = new CountDownLatch(1);
        ServerSocketReference mockBackend = BinaryProtocolServer.tcpServerSocket(context.serverPortForRemoteApplications(), "localAppTest", socket -> () -> {
            sleep(Timeouts.SECOND);
            close(socket);
        }, parameter -> backendCreated.countDown(), StatusConsumer.ANONYMOUS);
        assertLatch(backendCreated);

        SessionDetails sessionDetails = TestHelper.createTestSession(TEST_TOKEN_1, Fields.TS_SIGNATURE);
        ApplicationRequest applicationRequest = new ApplicationRequest(sessionDetails, createTestUserResolver().apply(TEST_TOKEN_1));

        CountDownLatch disconnected = new CountDownLatch(1);
        LocalApplicationProxy.startAndRun(context, applicationRequest, -1, (String message) -> disconnected.countDown(), LocalApplicationProxy.ConnectionListener.VOID);

        assertLatch(disconnected);
        mockBackend.close();
    }

    @Test
    public void testCommandProtocol() throws IOException, InterruptedException {
        LocalApplicationProxyContext context = createLocalApplicationProxy();
        CountDownLatch gaugePokes = new CountDownLatch(3);

        try (ServerSocketReference ignored1 = createMockBackend(context, gaugePokes)) {
            SessionDetails sessionDetails = TestHelper.createTestSession(TEST_TOKEN_1, Fields.TS_SIGNATURE);
            ApplicationRequest applicationRequest = new ApplicationRequest(sessionDetails, createTestUserResolver().apply(TEST_TOKEN_1));

            try (ServerSocketReference ignored2 = LocalApplicationProxy.startAndRun(context, applicationRequest, -1, TcpIoStream.DisconnectListener.VOID, LocalApplicationProxy.ConnectionListener.VOID)) {
                TcpIoStream applicationConnection = new TcpIoStream("mock application ", new Socket(LOCALHOST, context.authenticatorPort()));

                byte[] protocolResponse = new byte[TS_PROTOCOL.length()];
                // request
                applicationConnection.write(new byte[] {Fields.TS_COMMAND_F});
                applicationConnection.flush();
                // response
                applicationConnection.getDataBuffer().read(protocolResponse);
                assertArrayEquals(protocolResponse, TS_PROTOCOL.getBytes());

                // request again
                applicationConnection.write(new byte[] {Fields.TS_COMMAND_F});
                applicationConnection.flush();
                // response again
                applicationConnection.getDataBuffer().read(protocolResponse);
                assertArrayEquals(protocolResponse, TS_PROTOCOL.getBytes());

                // TODO: why is this logic duplicated from BinaryProtocol?
                byte[] commandPacket = new byte[5];
                commandPacket[0] = Fields.TS_OUTPUT_COMMAND;
                System.arraycopy(GetOutputsCommand.createRequest(), 0, commandPacket, 1, 4);

                applicationConnection.sendPacket(commandPacket);
                BinaryProtocolServer.Packet response = applicationConnection.readPacket();
                assertEquals(Fields.TS_TOTAL_OUTPUT_SIZE + 1, response.getPacket().length);
            }
        }
    }

    @Test
    public void testGaugePoking() throws IOException, InterruptedException {
        LocalApplicationProxyContext context = createLocalApplicationProxy();

        CountDownLatch gaugePokes = new CountDownLatch(3);

        try (ServerSocketReference ignored1 = createMockBackend(context, gaugePokes)) {

            SessionDetails sessionDetails = TestHelper.createTestSession(TEST_TOKEN_1, Fields.TS_SIGNATURE);
            ApplicationRequest applicationRequest = new ApplicationRequest(sessionDetails, createTestUserResolver().apply(TEST_TOKEN_1));

            CountDownLatch disconnected = new CountDownLatch(1);

            try (ServerSocketReference ignored2 = LocalApplicationProxy.startAndRun(context, applicationRequest, -1, (String message) -> disconnected.countDown(), LocalApplicationProxy.ConnectionListener.VOID)) {

                // wait for three output requests to take place
                assertLatch("gaugePokes", gaugePokes);

                // but there must be a disconnect after some time
                assertLatch("disconnected", disconnected);
            }
        }
    }

    @NotNull
    private static ServerSocketReference createMockBackend(LocalApplicationProxyContext context, CountDownLatch gaugePokes) throws IOException, InterruptedException {
        CountDownLatch backendCreated = new CountDownLatch(1);
        ServerSocketReference mockBackend = BinaryProtocolServer.tcpServerSocket(context.serverPortForRemoteApplications(), "localAppTest", socket -> () -> {
            try {
                IoStream applicationClientStream = new TcpIoStream("gauge", socket);

                HelloCommand.getHelloResponse(applicationClientStream.getDataBuffer());

                while (!socket.isClosed()) {
                    AtomicBoolean handled = new AtomicBoolean();

                    int length = getPacketLength(applicationClientStream.getDataBuffer(), new BinaryProtocolServer.Handler() {
                        @Override
                        public void handle() throws IOException {
                            applicationClientStream.write(TS_PROTOCOL.getBytes());
                            handled.set(true);
                        }
                    });
                    if (handled.get())
                        continue;

                    BinaryProtocolServer.Packet packet = BinaryProtocolServer.readPromisedBytes(applicationClientStream.getDataBuffer(), length);
                    System.out.println("Got packet " + findCommand(packet.getPacket()[0]));

                    if (packet.getPacket().length != 5)
                        throw new IllegalStateException("Unexpected length " + packet.getPacket().length);

                    GetOutputsCommand.sendOutput(applicationClientStream);
                    gaugePokes.countDown();
                }
            } catch (IOException e) {
                throw new IllegalStateException(e);
            }

        }, parameter -> backendCreated.countDown(), StatusConsumer.ANONYMOUS);

        assertLatch(backendCreated);
        return mockBackend;
    }

    @NotNull
    private LocalApplicationProxyContext createLocalApplicationProxy() {
        return new LocalApplicationProxyContext() {
            private final int remotePort = portNumber.incrementAndGet();
            private final int authenticatorPort = portNumber.incrementAndGet();

            @Override
            public String executeGet(String url) {
                if (url.endsWith(ProxyClient.VERSION_PATH))
                    return ProxyClient.BACKEND_VERSION;
                throw new UnsupportedOperationException(url);
            }

            @Override
            public int serverPortForRemoteApplications() {
                return remotePort;
            }

            @Override
            public int authenticatorPort() {
                return authenticatorPort;
            }

            @Override
            public int startUpIdle() {
                return 7 * Timeouts.SECOND;
            }

            @Override
            public int gaugePokingPeriod() {
                return SECOND;
            }
        };
    }
}
