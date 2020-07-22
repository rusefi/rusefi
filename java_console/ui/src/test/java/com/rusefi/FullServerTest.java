package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.opensr5.Logger;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkManager;
import com.rusefi.proxy.NetworkConnector;
import com.rusefi.server.*;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.Objects;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static com.rusefi.TestHelper.createIniField;
import static com.rusefi.TestHelper.prepareImage;
import static com.rusefi.Timeouts.READ_IMAGE_TIMEOUT;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class FullServerTest {
    private final static Logger logger = Logger.CONSOLE;

    @Before
    public void setTestCertificate() throws MalformedURLException {
        ServerTest.commonServerTest();
        BinaryProtocol.DISABLE_LOCAL_CACHE = true;
    }

    @Test
    public void testRelayWorkflow() throws InterruptedException, IOException {
        ScalarIniField iniField = TestHelper.createIniField(Fields.CYLINDERSCOUNT);
        int value = 241;
        int userId = 7;

        CountDownLatch controllerRegistered = new CountDownLatch(1);

        UserDetailsResolver userDetailsResolver = authToken -> new UserDetails(authToken.substring(0, 5), userId);
        int httpPort = 8003;
        try (Backend backend = new Backend(userDetailsResolver, httpPort, logger) {
            @Override
            protected void onRegister(ControllerConnectionState controllerConnectionState) {
                super.onRegister(controllerConnectionState);
                controllerRegistered.countDown();
            }
        }; LinkManager clientManager = new LinkManager(logger)) {
            int serverPortForControllers = 7001;
            int serverPortForRemoteUsers = 7003;


            // first start backend server
            CountDownLatch controllerServerCreated = new CountDownLatch(1);
            CountDownLatch applicationServerCreated = new CountDownLatch(1);

            backend.runControllerConnector(serverPortForControllers, parameter -> controllerServerCreated.countDown());

            backend.runApplicationConnector(serverPortForRemoteUsers, parameter -> applicationServerCreated.countDown());

            assertTrue(controllerServerCreated.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));
            assertTrue(applicationServerCreated.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));


            // create virtual controller to which "rusEFI network connector" connects to
            int controllerPort = 7002;
            ConfigurationImage controllerImage = prepareImage(value, createIniField(Fields.CYLINDERSCOUNT));
            TestHelper.createVirtualController(controllerPort, controllerImage, logger);


            // start "rusEFI network connector" to connect controller with backend since in real life controller has only local serial port it does not have network
            SessionDetails deviceSessionDetails = NetworkConnector.runNetworkConnector(MockRusEfiDevice.TEST_TOKEN_1, TestHelper.LOCALHOST + ":" + controllerPort, serverPortForControllers);

            assertTrue(controllerRegistered.await(READ_IMAGE_TIMEOUT, TimeUnit.MILLISECONDS));

            SessionDetails authenticatorSessionDetails = new SessionDetails(deviceSessionDetails.getControllerInfo(), MockRusEfiDevice.TEST_TOKEN_3, deviceSessionDetails.getOneTimeToken());
            ApplicationRequest applicationRequest = new ApplicationRequest(authenticatorSessionDetails, userId);

            // start authenticator

            int authenticatorPort = 7004; // local port on which authenticator accepts connections from Tuner Studio
            LocalApplicationProxy.startAndRun(logger, serverPortForRemoteUsers, applicationRequest, authenticatorPort);


            CountDownLatch connectionEstablishedCountDownLatch = new CountDownLatch(1);

            // connect to proxy and read virtual controller through it
            clientManager.startAndConnect(TestHelper.LOCALHOST + ":" + authenticatorPort, new ConnectionStateListener() {
                @Override
                public void onConnectionEstablished() {
                    connectionEstablishedCountDownLatch.countDown();
                }

                @Override
                public void onConnectionFailed() {
                    System.out.println("Failed");
                }
            });
            assertTrue("Proxied ECU Connection established", connectionEstablishedCountDownLatch.await(30, TimeUnit.SECONDS));

            BinaryProtocol clientStreamState = clientManager.getCurrentStreamState();
            Objects.requireNonNull(clientStreamState, "clientStreamState");
            ConfigurationImage clientImage = clientStreamState.getControllerConfiguration();
            String clientValue = iniField.getValue(clientImage);
            assertEquals(Double.toString(value), clientValue);
        }
    }

}
