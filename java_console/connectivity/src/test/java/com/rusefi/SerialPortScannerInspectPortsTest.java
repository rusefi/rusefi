package com.rusefi;

import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.List;
import java.util.function.Function;

import static java.util.Arrays.asList;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the parallel port-probe fan-out with an injected inspector — no real serial ports.
 * Covers the result-classification rules: dead ports (null) are dropped, a crashing probe is
 * reported as Unknown rather than killing the scan. The deliberate-timeout path is NOT tested here:
 * it costs the hardwired 5s sleep and needs a clock seam first (see
 * docs/java-connectivity-ui-unit-testing.md). [tag:better_ux_for_flashing]
 */
public class SerialPortScannerInspectPortsTest {

    /**
     * All scripted inspectors below complete, which makes the fan-out interrupt the calling thread
     * to cancel its timeout sleep — clear the flag so it never leaks into the next test.
     */
    private static List<PortResult> inspect(List<String> ports, Function<String, PortResult> inspector) {
        try {
            return SerialPortScanner.inspectPorts(ports, null, inspector);
        } finally {
            Thread.interrupted();
        }
    }

    @Test
    public void emptyPortListReturnsImmediately() {
        long before = System.currentTimeMillis();
        assertTrue(inspect(Collections.emptyList(), port -> new PortResult(port, SerialPortType.Ecu)).isEmpty());
        assertTrue(System.currentTimeMillis() - before < 4000, "empty input must not wait out the probe timeout");
    }

    @Test
    public void allInspectedPortsAreCollected() {
        List<PortResult> results = inspect(asList("COM1", "COM2"),
            port -> new PortResult(port, "COM1".equals(port) ? SerialPortType.Ecu : SerialPortType.Unknown));
        assertEquals(2, results.size());
        assertTrue(results.contains(new PortResult("COM1", SerialPortType.Ecu)));
        assertTrue(results.contains(new PortResult("COM2", SerialPortType.Unknown)));
    }

    @Test
    public void deadPortIsDroppedFromResults() {
        // the inspector returns null for a stale OS node (device disappeared mid-probe)
        List<PortResult> results = inspect(asList("COM1", "COM_DEAD"),
            port -> "COM_DEAD".equals(port) ? null : new PortResult(port, SerialPortType.Ecu));
        assertEquals(Collections.singletonList(new PortResult("COM1", SerialPortType.Ecu)), results);
    }

    @Test
    public void crashingInspectorYieldsUnknownInsteadOfKillingTheScan() {
        List<PortResult> results = inspect(asList("COM1", "COM_BOOM"), port -> {
            if ("COM_BOOM".equals(port)) {
                throw new RuntimeException("probe blew up");
            }
            return new PortResult(port, SerialPortType.Ecu);
        });
        assertEquals(2, results.size());
        assertTrue(results.contains(new PortResult("COM_BOOM", SerialPortType.Unknown)),
            "a probe exception must degrade to Unknown, not lose the port");
        assertTrue(results.contains(new PortResult("COM1", SerialPortType.Ecu)));
    }
}
