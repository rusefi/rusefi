package com.rusefi;

import com.devexperts.logging.Logging;

import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.Optional;
import java.util.Set;

public class SerialPortCache {
    private final static Logging log = Logging.getLogging(SerialPortCache.class);
    private final static Duration lifeTime = Duration.ofSeconds(2);

    private final Map<String, CachedPort> cachedPorts = new HashMap<>();

    Optional<SerialPortScanner.PortResult> get(final String serialPort) {
        final CachedPort cachedPort = cachedPorts.get(serialPort);
        if (cachedPort != null) {
            if (cachedPort.isExpired()) {
                cachedPorts.remove(serialPort);
                log.info("Expired port is removed: " + serialPort);

            } else {
                return Optional.of(cachedPort.port);
            }
        }
        return Optional.empty();
    }

    void put(final SerialPortScanner.PortResult port) {
        cachedPorts.put(port.port, new CachedPort(port));
    }

    void retainAll(final Set<String> serialPortsToRetain) {
        // Clean the port cache of any entries that no longer exist
        // If the same port appears later, we want to re-probe it at that time
        // In any other scenario, auto could have unexpected behavior for the user
        List<String> toRemove = new ArrayList<>();
        for (String x : cachedPorts.keySet()) {
            if (!serialPortsToRetain.contains(x)) {
                toRemove.add(x);
            }
        }

        // two steps to avoid ConcurrentModificationException
        toRemove.forEach(p -> {
            cachedPorts.remove(p);
            log.info("Disappeared port is removed: " + p);
        });
    }

    private static class CachedPort {
        private final SerialPortScanner.PortResult port;
        private final Instant expirationTimestamp;

        CachedPort(final SerialPortScanner.PortResult portToCache) {
            this.port = portToCache;
            this.expirationTimestamp = Instant.now().plus(lifeTime);
        }

        boolean isExpired() {
            return expirationTimestamp.isBefore(Instant.now());
        }
    }
}
