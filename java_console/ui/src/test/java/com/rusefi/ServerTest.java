package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.server.Backend;
import com.rusefi.server.ClientConnectionState;
import org.junit.Test;

import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

/**
 * integration test of the rusEFI online backend process
 */
public class ServerTest {
    private final static Logger logger = Logger.CONSOLE;

    @Test
    public void testSessionTimeout() throws InterruptedException, IOException {
        int serverPort = 7000;


        CountDownLatch serverCreated = new CountDownLatch(1);


        Backend backend = new Backend(3 * Timeouts.SECOND);


        BinaryProtocolServer.tcpServerSocket(serverPort, "Server", new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket clientSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        ClientConnectionState clientConnectionState = new ClientConnectionState(clientSocket, logger);
                        clientConnectionState.sayHello();
                        backend.register(clientConnectionState);

                    }
                };
            }
        }, logger, parameter -> serverCreated.countDown());

        assertTrue(serverCreated.await(30, TimeUnit.SECONDS));
        assertEquals(0, backend.getCount());


        new MockRusEfiDevice("rusEFI 2020.07.06.frankenso_na6.2468827536", logger).connect(serverPort);
        new MockRusEfiDevice("rusEFI 2020.07.11.proteus_f4.1986715563", logger).connect(serverPort);
    }
}
