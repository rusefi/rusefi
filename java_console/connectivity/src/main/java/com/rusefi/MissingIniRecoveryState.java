package com.rusefi;

import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/** Exact serial-port/signature pairing retained across temporary scan connections. */
public final class MissingIniRecoveryState {
    public static final class Failure {
        public final String port;
        public final String signature;

        private Failure(String port, String signature) {
            this.port = port;
            this.signature = signature;
        }
    }

    private volatile Failure failure;
    private final Set<String> resolvingPorts = ConcurrentHashMap.newKeySet();

    public void record(String port, String signature) {
        if (port != null && signature != null) {
            failure = new Failure(port, signature);
        }
    }

    public Failure get() {
        return failure;
    }

    public void resolutionStarted(String port) {
        if (port != null) {
            resolvingPorts.add(port);
        }
    }

    public void resolutionFinished(String port) {
        if (port != null) {
            resolvingPorts.remove(port);
        }
    }

    public boolean shouldWaitForResolution(String port) {
        return port != null && resolvingPorts.contains(port);
    }

    public void clearForPort(String port) {
        Failure current = failure;
        if (current != null && current.port.equals(port)) {
            failure = null;
        }
        resolutionFinished(port);
    }
}
