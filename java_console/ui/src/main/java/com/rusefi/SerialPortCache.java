package com.rusefi;

import com.devexperts.logging.Logging;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.Optional;
import java.util.Set;

public class SerialPortCache {
    private final static Logging log = Logging.getLogging(SerialPortCache.class);

    private final Map<String, SerialPortScanner.PortResult> cachedPorts = new HashMap<>();

    Optional<SerialPortScanner.PortResult> get(final String serialPort) {
        return Optional.ofNullable(cachedPorts.get(serialPort));
    }

    void put(final SerialPortScanner.PortResult port) {
        cachedPorts.put(port.port, port);
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
            log.info("Removing port " + p);
        });
    }
}
