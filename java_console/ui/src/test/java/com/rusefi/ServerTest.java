package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.server.Backend;
import com.rusefi.server.ClientConnectionState;
import com.rusefi.server.UserDetails;
import com.rusefi.tools.online.ProxyClient;
import org.junit.Test;

import java.io.IOException;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

/**
 * integration test of the rusEFI online backend process
 * At the moment this test is very loose with timing it must be unreliable?
 */
public class ServerTest {
    private final static Logger logger = Logger.CONSOLE;

    @Test
    public void testSessionTimeout() throws InterruptedException, IOException {
        int serverPort = 7000;

        CountDownLatch serverCreated = new CountDownLatch(1);


        Function<String, UserDetails> userDetailsResolver = authToken -> new UserDetails(authToken.substring(0, 5), authToken.charAt(6));

        CountDownLatch allClientsDisconnected = new CountDownLatch(1);
        CountDownLatch onConnected = new CountDownLatch(2);

        int httpPort = 8000;
        Backend backend = new Backend(userDetailsResolver, httpPort, logger) {
            @Override
            public void register(ClientConnectionState clientConnectionState) {
                super.register(clientConnectionState);
                onConnected.countDown();
            }

            @Override
            public void close(ClientConnectionState inactiveClient) {
                super.close(inactiveClient);
                if (getCount() == 0)
                    allClientsDisconnected.countDown();
            }
        };

        Backend.runProxy(serverPort, serverCreated, backend);

        assertTrue(serverCreated.await(30, TimeUnit.SECONDS));
        assertEquals(0, backend.getCount());


        new MockRusEfiDevice("00000000-1234-1234-1234-123456789012", "rusEFI 2020.07.06.frankenso_na6.2468827536", logger).connect(serverPort);
        new MockRusEfiDevice("12345678-1234-1234-1234-123456789012", "rusEFI 2020.07.11.proteus_f4.1986715563", logger).connect(serverPort);

        assertTrue(onConnected.await(30, TimeUnit.SECONDS));

        List<ClientConnectionState> clients = backend.getClients();
        assertEquals(2, clients.size());

        List<UserDetails> onlineUsers = ProxyClient.getOnlineUsers(httpPort);
        assertEquals(2, onlineUsers.size());

        assertTrue(allClientsDisconnected.await(30, TimeUnit.SECONDS));

    }

}
