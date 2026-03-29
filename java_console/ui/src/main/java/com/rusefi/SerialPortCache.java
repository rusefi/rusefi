package com.rusefi;

import com.devexperts.logging.Logging;

import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

public class SerialPortCache {
    private final static Logging log = Logging.getLogging(SerialPortCache.class);

    private final Map<String, PortResult> cachedPorts = new HashMap<>();

    Optional<PortResult> get(final String serialPort) {
        return Optional.ofNullable(cachedPorts.get(serialPort));
    }

    void put(final PortResult port) {
        cachedPorts.put(port.port, port);
    }

    void retainAll(final Set<String> serialPortsToRetain) {
        final int cachedPortCount = cachedPorts.size();
        if (cachedPorts.keySet().retainAll(serialPortsToRetain)) {
            log.info(String.format("%d disappeared ports are removed", cachedPortCount - cachedPorts.size()));
        }
    }

    void invalidate(final String portName) {
        cachedPorts.remove(portName);
    }
}
