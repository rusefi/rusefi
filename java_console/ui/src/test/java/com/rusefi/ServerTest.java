package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.server.Backend;
import com.rusefi.server.ClientConnectionState;
import org.junit.Test;

import java.io.IOException;
import java.net.Socket;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
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


        Backend backend = new Backend(5 * Timeouts.SECOND);


        BinaryProtocolServer.tcpServerSocket(serverPort, "Server", new Function<Socket, Runnable>() {
            @Override
            public Runnable apply(Socket clientSocket) {
                return new Runnable() {
                    @Override
                    public void run() {
                        ClientConnectionState clientConnectionState = new ClientConnectionState(clientSocket, logger);
                        clientConnectionState.sayHello();
                        backend.register(clientConnectionState);

                        while(true) {

                        }

                    }
                };
            }
        }, logger, parameter -> serverCreated.countDown());

        assertTrue(serverCreated.await(30, TimeUnit.SECONDS));
        assertEquals(0, backend.getCount());


        new MockRusEfiDevice("00000000-1234-1234-1234-123456789012", "rusEFI 2020.07.06.frankenso_na6.2468827536", logger).connect(serverPort);
        new MockRusEfiDevice("12345678-1234-1234-1234-123456789012", "rusEFI 2020.07.11.proteus_f4.1986715563", logger).connect(serverPort);


        // todo: technically we should have callbacks for 'connect', will make this better if this would be failing
        sleep(Timeouts.SECOND);

        List<ClientConnectionState> clients = backend.getClients();
        assertEquals(2, clients.size());


    }
}
