package com.rusefi.server;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class Backend {

    // guarded by own monitor
    private final Set<ClientConnectionState> clients = new HashSet<>();
    private final int clientTimeout;

    public Backend(int clientTimeout) {
        this.clientTimeout = clientTimeout;

        new Thread(() -> {
            while (true) {
                runCleanup();
                sleep(clientTimeout);
            }
        }, "rusEFI Server Cleanup").start();


    }

    private void runCleanup() {
        List<ClientConnectionState> inactiveClients = new ArrayList<>();

        synchronized (clients) {
            long now = System.currentTimeMillis();
            for (ClientConnectionState client : clients) {
                if (now - client.getLastActivityTimestamp() > clientTimeout)
                    inactiveClients.add(client);
            }
        }

        for (ClientConnectionState inactiveClient : inactiveClients) {
            close(inactiveClient);
        }

    }

    private void close(ClientConnectionState inactiveClient) {
        inactiveClient.close();
        synchronized (clients) {
            clients.remove(inactiveClient);
        }
    }

    public void register(ClientConnectionState clientConnectionState) {
        synchronized (clients) {
            clients.add(clientConnectionState);
        }
    }

    public int getCount() {
        synchronized (clients) {
            return clients.size();
        }
    }
}
