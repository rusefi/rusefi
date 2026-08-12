package com.rusefi;

import com.devexperts.logging.Logging;

import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

public class SerialPortCache {
    private final static Logging log = Logging.getLogging(SerialPortCache.class);

    private static final class Entry {
        final PortResult port;
        final long expiresAt;

        private Entry(PortResult port, long expiresAt) {
            this.port = port;
            this.expiresAt = expiresAt;
        }
    }

    private final Map<String, Entry> cachedPorts = new HashMap<>();

    synchronized Optional<PortResult> get(final String serialPort) {
        Entry entry = cachedPorts.get(serialPort);
        return Optional.ofNullable(entry == null ? null : entry.port);
    }

    synchronized Optional<PortResult> get(final String serialPort, long now) {
        Entry entry = cachedPorts.get(serialPort);
        if (entry != null && entry.expiresAt <= now) {
            cachedPorts.remove(serialPort);
            entry = null;
        }
        return Optional.ofNullable(entry == null ? null : entry.port);
    }

    synchronized void put(final PortResult port) {
        put(port, Long.MAX_VALUE);
    }

    synchronized void put(final PortResult port, long expiresAt) {
        cachedPorts.put(port.port, new Entry(port, expiresAt));
    }

    synchronized void retainAll(final Set<String> serialPortsToRetain) {
        final int cachedPortCount = cachedPorts.size();
        if (cachedPorts.keySet().retainAll(serialPortsToRetain)) {
            log.info(String.format("%d disappeared ports are removed", cachedPortCount - cachedPorts.size()));
        }
    }

    synchronized void invalidate(final String portName) {
        cachedPorts.remove(portName);
    }
}
