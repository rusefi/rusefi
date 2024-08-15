package com.rusefi;

import com.devexperts.logging.Logging;

import java.time.Duration;
import java.time.Instant;
import java.util.HashMap;
import java.util.Map;
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
        final int cachedPortCount = cachedPorts.size();
        if (cachedPorts.keySet().retainAll(serialPortsToRetain)) {
            log.info(String.format("%d disappeared ports are removed", cachedPortCount - cachedPorts.size()));
        }
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
