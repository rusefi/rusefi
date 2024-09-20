package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;

class SerialPortJobContext {
    private final SerialPortScanner.PortResult port;

    SerialPortJobContext(final SerialPortScanner.PortResult port) {
        this.port = port;
    }

    SerialPortScanner.PortResult getPort() {
        return port;
    }
}
