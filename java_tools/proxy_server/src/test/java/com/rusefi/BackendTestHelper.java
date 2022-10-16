package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.server.Backend;
import com.rusefi.server.UserDetails;
import com.rusefi.server.UserDetailsResolver;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.tools.online.HttpUtil;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.concurrent.CountDownLatch;

import static com.rusefi.TestHelper.assertLatch;

public class BackendTestHelper {
    public static void runApplicationConnectorBlocking(Backend backend, int serverPortForRemoteUsers) throws InterruptedException {
        CountDownLatch applicationServerCreated = new CountDownLatch(1);
        try {
            backend.runApplicationConnector(serverPortForRemoteUsers, parameter -> applicationServerCreated.countDown());
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
        assertLatch(applicationServerCreated);
    }

    public static void runControllerConnectorBlocking(Backend backend, int serverPortForControllers) throws InterruptedException {
        CountDownLatch controllerServerCreated = new CountDownLatch(1);
        try {
            backend.runControllerConnector(serverPortForControllers, parameter -> controllerServerCreated.countDown());
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
        assertLatch(controllerServerCreated);
    }

    @NotNull
    public static UserDetailsResolver createTestUserResolver() {
        return authToken -> new UserDetails(authToken.substring(0, 5), authToken.charAt(6));
    }

    public static void commonServerTest() throws MalformedURLException {
        HttpUtil.RUSEFI_PROXY_HOSTNAME = TcpConnector.LOCALHOST;
        BinaryProtocol.DISABLE_LOCAL_CONFIGURATION_CACHE = true;

        rusEFISSLContext.init("certificate/test_pkcs12.jks", "password");
    }
}
