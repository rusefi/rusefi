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
    // Identity of the board this process talks to: written at connect time (via the LinkManager sharing
    // this instance, see ProductionConnectivity), read by flashing/hardware-kind decisions. Owned here
    // because those decisions run in offline flows where no live connection exists to ask.
    private final com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget;

    public ConnectivityContext(PortScanner portScanner) {
        this(portScanner, new com.rusefi.core.io.ConnectedEcuTarget());
    }

    public ConnectivityContext(PortScanner portScanner, com.rusefi.core.io.ConnectedEcuTarget connectedEcuTarget) {
        this.portScanner = portScanner;
        this.connectedEcuTarget = connectedEcuTarget;
    }

    public PortScanner getPortScanner() {
        return portScanner;
    }

    public com.rusefi.core.io.ConnectedEcuTarget getConnectedEcuTarget() {
        return connectedEcuTarget;
    }

    public AvailableHardware getCurrentHardware() {
        return getPortScanner().getCurrentHardware();
    }
}
