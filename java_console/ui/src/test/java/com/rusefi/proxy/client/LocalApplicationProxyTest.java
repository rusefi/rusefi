package com.rusefi.proxy.client;

import com.rusefi.BackendTestHelper;
import com.rusefi.TestHelper;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.ServerSocketReference;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.server.ApplicationRequest;
import com.rusefi.server.SessionDetails;
import com.rusefi.tools.online.ProxyClient;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

import static com.rusefi.BackendTestHelper.createTestUserResolver;
import static com.rusefi.TestHelper.TEST_TOKEN_1;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class LocalApplicationProxyTest {
    @Before
    public void setup() throws MalformedURLException {
        BackendTestHelper.commonServerTest();
    }

    @Test
    public void testLocalApplication() throws IOException, InterruptedException {
        LocalApplicationProxyContext context = new LocalApplicationProxyContext() {
            @Override
            public String executeGet(String url) {
                if (url.endsWith(ProxyClient.VERSION_PATH))
                    return ProxyClient.BACKEND_VERSION;
                throw new UnsupportedOperationException(url);
            }

            @Override
            public int serverPortForRemoteApplications() {
                return 5999;
            }

            @Override
            public int authenticatorPort() {
                return 5998;
            }
        };

        CountDownLatch backendCreated = new CountDownLatch(1);
        ServerSocketReference mockBackend = BinaryProtocolServer.tcpServerSocket(context.serverPortForRemoteApplications(), "localAppTest", new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket socket) {
                return new Runnable() {
                    @Override
                    public void run() {

                    }
                };
            }
        }, parameter -> backendCreated.countDown());
        assertTrue(backendCreated.await(30, TimeUnit.SECONDS));


        SessionDetails sessionDetails = TestHelper.createTestSession(TEST_TOKEN_1, Fields.TS_SIGNATURE);
        ApplicationRequest applicationRequest = new ApplicationRequest(sessionDetails, createTestUserResolver().apply(TEST_TOKEN_1));


        LocalApplicationProxy.startAndRun(context, applicationRequest, -1, TcpIoStream.DisconnectListener.VOID, LocalApplicationProxy.ConnectionListener.VOID);


        mockBackend.close();
    }
}
