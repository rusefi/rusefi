package com.rusefi.io.can.slcan;

import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import java.util.function.Function;

import static java.util.Arrays.asList;
import static java.util.Collections.singletonList;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Scan-policy tests with scripted probes (no real serial ports): caching of classified ports,
 * UNKNOWN retry, forgetting disappeared ports, change-only listener notification, and the
 * fan-out classification rules (dead port dropped, crashing probe degrades to UNKNOWN).
 */
public class SlcanPortScannerTest {

    private static class AdapterStream extends com.rusefi.io.serial.AbstractIoStream {
        final com.rusefi.binaryprotocol.IncomingDataBuffer buffer =
            new com.rusefi.binaryprotocol.IncomingDataBuffer("adapter-test", getStreamStats());
        boolean failHello;
        boolean console;
        String version = "V1220";
        public com.rusefi.binaryprotocol.IncomingDataBuffer getDataBuffer() { return buffer; }
        public void setInputListener(com.opensr5.io.DataListener listener) { }
        public void write(byte[] bytes) throws java.io.IOException {
            if (bytes[0] == 0) {
                if (console) {
                    buffer.addData(com.rusefi.binaryprotocol.IoHelper.makeCrc32Packet(
                        "\u0000rusEFI test".getBytes(java.nio.charset.StandardCharsets.US_ASCII)));
                }
                if (failHello) { throw new java.io.IOException("No TS console"); }
                return;
            }
            String command = new String(bytes, java.nio.charset.StandardCharsets.US_ASCII);
            if (command.endsWith("V\r")) {
                buffer.addData(("\u0007\r" + version + "\r").getBytes(java.nio.charset.StandardCharsets.US_ASCII));
            }
        }
    }

    @Test
    public void silentHelloLeavesTimeForSlcanProbe() {
        AdapterStream stream = new AdapterStream();
        List<SlcanPortScanner.Result> results = SlcanPortScanner.inspectPorts(
            singletonList("COM116"), new HashMap<>(),
            port -> SlcanPortScanner.inspectStream(port, stream));
        assertEquals(SlcanPortScanner.Type.SLCAN, results.get(0).type);
        assertEquals("V1220", results.get(0).detail);
    }

    @Test
    public void canableVersionRecognized() {
        AdapterStream stream = new AdapterStream();
        stream.failHello = true;
        stream.version = "16e7497-dirty github.com/normaldotcom/canable2.git";
        assertEquals(SlcanPortScanner.Type.SLCAN,
            SlcanPortScanner.inspectStream("COM116", stream).type);
    }

    @Test
    public void tunerStudioConsoleIsNotProbedAsSlcan() {
        AdapterStream stream = new AdapterStream() {
            public void write(byte[] bytes) throws java.io.IOException {
                assertEquals(0, bytes[0]);
                super.write(bytes);
            }
        };
        stream.console = true;
        assertEquals(SlcanPortScanner.Type.TS_CONSOLE,
            SlcanPortScanner.inspectStream("console", stream).type);
        assertTrue(stream.isClosed());
    }

    @Test
    public void unrelatedVersionTextIsRejected() {
        AdapterStream stream = new AdapterStream();
        stream.failHello = true;
        stream.version = "Version of an unrelated serial device";
        assertEquals(SlcanPortScanner.Type.NOT_SLCAN,
            SlcanPortScanner.inspectStream("other", stream).type);
        assertTrue(stream.isClosed());
    }

    private static class FakeProbes implements SlcanPortScanner.Probes {
        final Set<String> ports = new TreeSet<>();
        final Map<String, SlcanPortScanner.Result> results = new HashMap<>();
        final Map<String, Integer> inspectCount = new HashMap<>();

        @Override
        public Set<String> listSerialPorts() {
            return new TreeSet<>(ports);
        }

        @Override
        public SlcanPortScanner.Result inspectPort(String serialPort) {
            inspectCount.merge(serialPort, 1, Integer::sum);
            return results.get(serialPort);
        }

        void addPort(String port, SlcanPortScanner.Type type, String detail) {
            ports.add(port);
            results.put(port, new SlcanPortScanner.Result(port, type, detail));
        }
    }

    @Test
    public void classifiedPortsAreCachedAndNotReinspected() {
        FakeProbes probes = new FakeProbes();
        probes.addPort("COM7", SlcanPortScanner.Type.SLCAN, "V1220");
        probes.addPort("COM8", SlcanPortScanner.Type.TS_CONSOLE, "rusEFI test");
        probes.addPort("COM9", SlcanPortScanner.Type.NOT_SLCAN, "'ATZ'");
        SlcanPortScanner scanner = new SlcanPortScanner(probes, ports -> {});

        scanner.scanCycle();
        scanner.scanCycle();

        assertEquals(Integer.valueOf(1), probes.inspectCount.get("COM7"));
        assertEquals(Integer.valueOf(1), probes.inspectCount.get("COM8"));
        assertEquals(Integer.valueOf(1), probes.inspectCount.get("COM9"));
        assertEquals(asList(
            new SlcanPortScanner.Result("COM7", SlcanPortScanner.Type.SLCAN, "V1220"),
            new SlcanPortScanner.Result("COM8", SlcanPortScanner.Type.TS_CONSOLE, "rusEFI test"),
            new SlcanPortScanner.Result("COM9", SlcanPortScanner.Type.NOT_SLCAN, "'ATZ'")
        ), scanner.getKnownPorts());
    }

    @Test
    public void unknownPortIsRetriedEveryCycle() {
        FakeProbes probes = new FakeProbes();
        probes.addPort("COM7", SlcanPortScanner.Type.UNKNOWN, "no response to V probe");
        SlcanPortScanner scanner = new SlcanPortScanner(probes, ports -> {});

        scanner.scanCycle();
        scanner.scanCycle();

        assertEquals(Integer.valueOf(2), probes.inspectCount.get("COM7"));
    }

    @Test
    public void disappearedPortIsForgottenAndReinspectedOnReturn() {
        FakeProbes probes = new FakeProbes();
        probes.addPort("COM7", SlcanPortScanner.Type.SLCAN, "V1220");
        SlcanPortScanner scanner = new SlcanPortScanner(probes, ports -> {});

        scanner.scanCycle();
        probes.ports.remove("COM7");
        scanner.scanCycle();
        assertTrue(scanner.getKnownPorts().isEmpty());

        probes.ports.add("COM7");
        scanner.scanCycle();
        assertEquals(Integer.valueOf(2), probes.inspectCount.get("COM7"));
        assertEquals(singletonList(new SlcanPortScanner.Result("COM7", SlcanPortScanner.Type.SLCAN, "V1220")),
            scanner.getKnownPorts());
    }

    @Test
    public void listenerFiresOnlyWhenPortListChanges() {
        FakeProbes probes = new FakeProbes();
        List<List<SlcanPortScanner.Result>> notifications = new ArrayList<>();
        SlcanPortScanner scanner = new SlcanPortScanner(probes, notifications::add);

        scanner.scanCycle();
        assertTrue(notifications.isEmpty(), "no ports and no change: no notification");

        probes.addPort("COM7", SlcanPortScanner.Type.SLCAN, "V1220");
        scanner.scanCycle();
        assertEquals(1, notifications.size());
        scanner.scanCycle();
        assertEquals(1, notifications.size(), "unchanged list must not re-notify");

        probes.ports.remove("COM7");
        scanner.scanCycle();
        assertEquals(2, notifications.size());
        assertTrue(notifications.get(1).isEmpty());
    }

    @Test
    public void legacyMotherboardUartsAreNeverInspected() {
        FakeProbes probes = new FakeProbes();
        probes.addPort("ttyS0", SlcanPortScanner.Type.SLCAN, "V1220");
        SlcanPortScanner scanner = new SlcanPortScanner(probes, ports -> {});

        scanner.scanCycle();

        assertTrue(probes.inspectCount.isEmpty());
        assertTrue(scanner.getKnownPorts().isEmpty());
    }

    private static List<SlcanPortScanner.Result> inspect(List<String> ports,
                                                         Function<String, SlcanPortScanner.Result> inspector) {
        try {
            return SlcanPortScanner.inspectPorts(ports, new HashMap<>(), inspector);
        } finally {
            // clear a possible self-interrupt from the fan-out timeout path
            Thread.interrupted();
        }
    }

    @Test
    public void deadPortIsDroppedFromResults() {
        List<SlcanPortScanner.Result> results = inspect(asList("COM1", "COM_DEAD"),
            port -> "COM_DEAD".equals(port) ? null
                : new SlcanPortScanner.Result(port, SlcanPortScanner.Type.SLCAN, "V1220"));
        assertEquals(singletonList(new SlcanPortScanner.Result("COM1", SlcanPortScanner.Type.SLCAN, "V1220")),
            results);
    }

    @Test
    public void crashingInspectorYieldsUnknownInsteadOfKillingTheScan() {
        List<SlcanPortScanner.Result> results = inspect(asList("COM1", "COM_BOOM"), port -> {
            if ("COM_BOOM".equals(port)) {
                throw new RuntimeException("probe blew up");
            }
            return new SlcanPortScanner.Result(port, SlcanPortScanner.Type.SLCAN, "V1220");
        });
        assertEquals(2, results.size());
        assertTrue(results.contains(new SlcanPortScanner.Result("COM1", SlcanPortScanner.Type.SLCAN, "V1220")));
        assertEquals(1, results.stream()
            .filter(r -> "COM_BOOM".equals(r.port) && r.type == SlcanPortScanner.Type.UNKNOWN)
            .count(), "a probe exception must degrade to UNKNOWN, not lose the port");
    }
}
