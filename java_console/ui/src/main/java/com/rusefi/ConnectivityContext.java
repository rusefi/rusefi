package com.rusefi;

public enum ConnectivityContext {
    INSTANCE;

    public SerialPortScanner getSerialPortScanner() {
        return SerialPortScanner.INSTANCE;
    }
}
