package com.rusefi;

/**
 * Hardware-discovery context: owns the reference to the port scanner watching the OS device list.
 * Process-wide by nature (one scan thread serves all windows/sessions), hence the {@link #INSTANCE}
 * used at composition roots — but consumers receive this object via constructor/method parameters
 * so tests can substitute a fake scanner. Per-session state belongs in {@link com.rusefi.ui.UIContext},
 * not here.
 * <p>
 * See docs/java-connectivity-context-review.md for the dependency-injection/testability plan.
 */
public class ConnectivityContext {
    public static final ConnectivityContext INSTANCE = new ConnectivityContext(SerialPortScanner.INSTANCE);

    private final SerialPortScanner serialPortScanner;

    public ConnectivityContext(SerialPortScanner serialPortScanner) {
        this.serialPortScanner = serialPortScanner;
    }

    public SerialPortScanner getSerialPortScanner() {
        return serialPortScanner;
    }

    public AvailableHardware getCurrentHardware() {
        return getSerialPortScanner().getCurrentHardware();
    }
}
