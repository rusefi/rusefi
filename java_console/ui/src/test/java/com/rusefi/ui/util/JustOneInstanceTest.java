package com.rusefi.ui.util;

import org.junit.jupiter.api.Test;

import java.io.File;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

class JustOneInstanceTest {
    @Test
    void forwardsAUnicodeLogPathOverLoopback() throws Exception {
        File expected = new File("logs/été drive.mlg").getAbsoluteFile();
        AtomicReference<File> received = new AtomicReference<>();
        AtomicReference<Throwable> serverFailure = new AtomicReference<>();

        try (ServerSocket server = new ServerSocket(0, 1, InetAddress.getLoopbackAddress())) {
            Thread receiver = new Thread(() -> {
                try (Socket socket = server.accept()) {
                    JustOneInstance.handleConnection(socket, received::set);
                } catch (Throwable failure) {
                    serverFailure.set(failure);
                }
            });
            receiver.start();

            assertTrue(JustOneInstance.sendRequest(server.getLocalPort(),
                JustOneInstance.openLogRequest(expected)));
            receiver.join(2_000);

            assertFalse(receiver.isAlive());
            assertNull(serverFailure.get());
            assertEquals(expected, received.get());
        }
    }

    @Test
    void pingDoesNotSubmitAFile() throws Exception {
        AtomicReference<File> received = new AtomicReference<>();
        AtomicReference<Throwable> serverFailure = new AtomicReference<>();

        try (ServerSocket server = new ServerSocket(0, 1, InetAddress.getLoopbackAddress())) {
            Thread receiver = new Thread(() -> {
                try (Socket socket = server.accept()) {
                    JustOneInstance.handleConnection(socket, received::set);
                } catch (Throwable failure) {
                    serverFailure.set(failure);
                }
            });
            receiver.start();

            assertTrue(JustOneInstance.sendRequest(server.getLocalPort(), JustOneInstance.PING));
            receiver.join(2_000);

            assertFalse(receiver.isAlive());
            assertNull(serverFailure.get());
            assertNull(received.get());
        }
    }
}
