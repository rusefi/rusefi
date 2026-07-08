package com.rusefi;

/**
 * Hardware-discovery context: owns the reference to the port scanner watching the OS device list.
 * Process-wide by nature (one scan thread serves all windows/sessions) — the production singleton
 * is wired in the ui module ({@code ProductionConnectivity.CONTEXT}, next to the real hardware
 * probes) and consumers receive this object via constructor/method parameters so tests can
 * substitute a fake scanner. Per-session state belongs in the ui module's {@code UIContext},
 * not here.
 * <p>
 * See docs/java-connectivity-context-review.md for the dependency-injection/testability plan.
 */
public class ConnectivityContext {
    private final PortScanner portScanner;

    public ConnectivityContext(PortScanner portScanner) {
        this.portScanner = portScanner;
    }

    public PortScanner getPortScanner() {
        return portScanner;
    }

    public AvailableHardware getCurrentHardware() {
        return getPortScanner().getCurrentHardware();
    }
}
