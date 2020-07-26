package com.rusefi.proxy;

import com.opensr5.ConfigurationImage;
import com.rusefi.BackendTestHelper;
import com.rusefi.TestHelper;
import com.rusefi.config.generated.Fields;
import com.rusefi.server.Backend;
import org.junit.Test;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.assertTrue;

public class NetworkConnectorTest {
    @Test
    public void testReconnect() throws InterruptedException {
        int serverPortForControllers = 7504;
        int httpPort = 7503;

        // create virtual controller to which "rusEFI network connector" connects to
        int controllerPort = 7502;
        ConfigurationImage controllerImage = new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE);
        TestHelper.createVirtualController(controllerPort, controllerImage);

        NetworkConnectorContext connectorContext = new NetworkConnectorContext() {
            @Override
            public int reconnectDelay() {
                return 3;
            }

            @Override
            public int serverPortForControllers() {
                return serverPortForControllers;
            }
        };

        CountDownLatch reconnectCounter = new CountDownLatch(1);

        // start "rusEFI network connector" to connect controller with backend since in real life controller has only local serial port it does not have network
        NetworkConnector.ReconnectListener reconnectListener = new NetworkConnector.ReconnectListener() {
            @Override
            public void onReconnect() {
                reconnectCounter.countDown();
            }
        };
        NetworkConnector.NetworkConnectorResult networkConnectorResult = NetworkConnector.runNetworkConnector(TestHelper.TEST_TOKEN_1, TestHelper.LOCALHOST + ":" + controllerPort, connectorContext, reconnectListener);

        assertTrue(reconnectCounter.await(30, TimeUnit.SECONDS));


        Backend backend = new Backend(BackendTestHelper.createTestUserResolver(), httpPort);
//        BackendTestHelper.runControllerConnectorBlocking(backend, serverPortForControllers);

        backend.close();
    }
}
