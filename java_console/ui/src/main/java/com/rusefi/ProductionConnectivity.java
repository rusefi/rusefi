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
    // One board identity for the whole process: shared between the context (readers: flashing /
    // hardware-kind decisions), the probes (DFU H7-vs-F4/F7 driver query) and — via UIContext — the
    // console's LinkManager (writer: BinaryProtocol records the target at connect time).
    private static final com.rusefi.core.io.ConnectedEcuTarget CONNECTED_ECU_TARGET =
        new com.rusefi.core.io.ConnectedEcuTarget();

    public static final ConnectivityContext CONTEXT = new ConnectivityContext(
        new SerialPortScanner(new EcuHardwareProbes(CONNECTED_ECU_TARGET), true), CONNECTED_ECU_TARGET);

    private ProductionConnectivity() {
    }
}
