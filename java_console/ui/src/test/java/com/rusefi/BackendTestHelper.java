package com.rusefi;

import com.rusefi.server.Backend;

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
}
