package com.rusefi;

/**
 * Wiring of the process-wide production connectivity singleton: the real hardware probes (ui
 * module, they need the flashing tools) injected into the connectivity-module scan policy.
 * Referenced only at composition roots (ConsoleUI, MassUpdater.main, hardware sandboxes) —
 * everything below the roots receives {@link ConnectivityContext} as a parameter.
 * <p>
 * See docs/java-connectivity-context-review.md for the dependency-injection/testability plan.
 */
public final class ProductionConnectivity {
    public static final ConnectivityContext CONTEXT =
        new ConnectivityContext(new SerialPortScanner(new EcuHardwareProbes(), true));

    private ProductionConnectivity() {
    }
}
