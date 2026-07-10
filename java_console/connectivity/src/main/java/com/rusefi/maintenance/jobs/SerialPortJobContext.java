package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;

class SerialPortJobContext {
    private final PortResult port;

    SerialPortJobContext(final PortResult port) {
        this.port = port;
    }

    PortResult getPort() {
        return port;
    }
}
