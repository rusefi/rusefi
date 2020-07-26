package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.server.Backend;
import com.rusefi.server.UserDetails;
import com.rusefi.server.UserDetailsResolver;
import com.rusefi.server.rusEFISSLContext;
import com.rusefi.tools.online.HttpUtil;
import org.jetbrains.annotations.NotNull;

import java.net.MalformedURLException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.rusefi.Timeouts.READ_IMAGE_TIMEOUT;
import static org.junit.Assert.assertTrue;

public class BackendTestHelper {
    public static void runApplicationConnectorBlocking(Backend backend, int serverPortForRemoteUsers) throws InterruptedException {
        CountDownLatch applicationServerCreated = new CountDownLatch(1);
        backend.runApplicationConnector(serverPortForRemoteUsers, parameter -> applicationServerCreated.countDown());
        assertTrue(applicationServerCreated.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));
    }

    public static void runControllerConnectorBlocking(Backend backend, int serverPortForControllers) throws InterruptedException {
        CountDownLatch controllerServerCreated = new CountDownLatch(1);
        backend.runControllerConnector(serverPortForControllers, parameter -> controllerServerCreated.countDown());
        assertTrue(controllerServerCreated.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));
    }

    @NotNull
    public static UserDetailsResolver createTestUserResolver() {
        return authToken -> new UserDetails(authToken.substring(0, 5), authToken.charAt(6));
    }

    public static void commonServerTest() throws MalformedURLException {
        HttpUtil.RUSEFI_PROXY_HOSTNAME = TestHelper.LOCALHOST;
        BinaryProtocol.DISABLE_LOCAL_CACHE = true;

        rusEFISSLContext.init("certificate/test_pkcs12.jks", "password");
    }
}
