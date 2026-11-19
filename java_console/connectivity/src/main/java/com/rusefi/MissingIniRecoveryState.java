package com.rusefi;

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

    public void record(String port, String signature) {
        if (port != null && signature != null) {
            failure = new Failure(port, signature);
        }
    }

    public Failure get() {
        return failure;
    }

    public void clearForPort(String port) {
        Failure current = failure;
        if (current != null && current.port.equals(port)) {
            failure = null;
        }
    }
}
