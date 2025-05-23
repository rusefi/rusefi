package com.rusefi.maintenance.jobs;

class SerialPortJobContext<PortClass> {
    private final PortClass port;

    SerialPortJobContext(final PortClass port) {
        this.port = port;
    }

    PortClass getPort() {
        return port;
    }
}
