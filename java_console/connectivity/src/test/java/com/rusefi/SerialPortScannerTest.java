package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.CalibrationsInfo;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.TreeSet;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Unit tests for the scan *policy* of {@link SerialPortScanner} — caching, Unknown-retry,
 * cache eviction, listener-on-change, device-probe throttling — driven with scripted
 * {@link SerialPortScanner.HardwareProbes}, no hardware and no scan thread.
 * [tag:better_ux_for_flashing]
 */
public class SerialPortScannerTest {

    static class FakeProbes implements SerialPortScanner.HardwareProbes {
        Set<String> serialPorts = new TreeSet<>();
        /** Scripted per-port inspection results; a missing entry means a dead port (null result). */
        final Map<String, PortResult> inspectResults = new HashMap<>();
        final Map<String, Integer> inspectCalls = new HashMap<>();
        Collection<String> tcpPorts = new ArrayList<>();
        int tcpCalibrationsCalls;
        boolean liveEcuConnected;
        boolean dfuConnected;
        int deviceProbeCalls;
        long time = 1_000_000;

        @Override
        public Set<String> listSerialPorts() {
            return new TreeSet<>(serialPorts);
        }

        @Override
        public PortResult inspectPort(String serialPort) {
            inspectCalls.merge(serialPort, 1, Integer::sum);
            return inspectResults.get(serialPort);
        }

        @Override
        public Collection<String> listTcpPorts() {
            return new ArrayList<>(tcpPorts);
        }

        @Override
        public Optional<CalibrationsInfo> getEcuCalibrations(String tcpPort) {
            tcpCalibrationsCalls++;
            return Optional.empty();
        }

        @Override
        public boolean isLiveEcuConnected() {
            return liveEcuConnected;
        }

        @Override
        public boolean isDfuDeviceConnected() {
            deviceProbeCalls++;
            return dfuConnected;
        }

        @Override
        public boolean isStLinkConnected() {
            return false;
        }

        @Override
        public boolean isPcanConnected() {
            return false;
        }

        @Override
        public long now() {
            return time;
        }
    }

    private FakeProbes probes;
    private SerialPortScanner scanner;

    @BeforeEach
    public void createScanner() {
        probes = new FakeProbes();
        // No background scan thread: tests drive scan cycles directly.
        scanner = new SerialPortScanner(probes, false);
    }

    /**
     * Run one scan cycle. inspectPorts signals scan completion by interrupting the calling
     * (= this test) thread out of its timeout sleep; clear any stray flag so it cannot leak
     * into later waits inside the test framework.
     */
    private void scan(boolean includeSlowLookup) {
        scanner.findAllAvailablePorts(includeSlowLookup);
        Thread.interrupted();
    }

    private void addPort(String name, SerialPortType type) {
        probes.serialPorts.add(name);
        probes.inspectResults.put(name, new PortResult(name, type));
    }

    private List<PortResult> knownPorts() {
        return scanner.getCurrentHardware().getKnownPorts();
    }

    @Test
    public void legacyMotherboardUartsAreNeverInspected() {
        probes.serialPorts.add("ttyS0");
        addPort("ttyUSB0", SerialPortType.Ecu);

        scan(false);

        assertFalse(probes.inspectCalls.containsKey("ttyS0"), "ttyS* must be dropped before the scan pipeline");
        assertEquals(1, knownPorts().size());
        assertEquals("ttyUSB0", knownPorts().get(0).port);
    }

    @Test
    public void detectedEcuIsCachedAndNotReinspectedNextCycle() {
        addPort("COM5", SerialPortType.Ecu);

        scan(false);
        scan(false);

        assertEquals(1, (int) probes.inspectCalls.get("COM5"), "cached port must not be re-opened");
        assertEquals(1, knownPorts().size(), "cached port must still be reported");
    }

    @Test
    public void unknownPortIsNotCachedSoDetectionRetriesEveryCycle() {
        addPort("COM5", SerialPortType.Unknown);

        scan(false);
        scan(false);

        assertEquals(2, (int) probes.inspectCalls.get("COM5"),
            "Unknown must be retried without waiting for the port to disappear and reappear");
    }

    @Test
    public void deadPortIsDroppedEntirely() {
        probes.serialPorts.add("COM9");
        // no scripted inspect result -> inspectPort returns null, the stale-OS-node case

        scan(false);

        assertTrue(knownPorts().isEmpty(), "stale OS nodes must not surface as ports at all");
    }

    @Test
    public void disappearedPortIsEvictedFromCacheAndReinspectedOnReturn() {
        addPort("COM5", SerialPortType.Ecu);
        scan(false);
        assertEquals(1, (int) probes.inspectCalls.get("COM5"));

        probes.serialPorts.remove("COM5");
        scan(false);
        assertTrue(knownPorts().isEmpty());

        probes.serialPorts.add("COM5");
        scan(false);
        assertEquals(2, (int) probes.inspectCalls.get("COM5"), "unplug/replug must trigger re-inspection");
    }

    @Test
    public void listenersFireOnlyWhenTheHardwareListActuallyChanges() {
        List<AvailableHardware> notifications = new ArrayList<>();
        scanner.addListener(notifications::add);

        addPort("COM5", SerialPortType.Ecu);
        scan(false);
        scan(false);

        assertEquals(1, notifications.size(), "an unchanged scan result must not re-notify");

        probes.serialPorts.remove("COM5");
        scan(false);
        assertEquals(2, notifications.size());
    }

    @Test
    public void dfuDeviceSurfacesAsSyntheticPortAndFlag() {
        probes.dfuConnected = true;

        scan(true);

        assertTrue(scanner.getCurrentHardware().isDfuFound());
        List<PortResult> dfuPorts = scanner.getCurrentHardware().getKnownPorts(SerialPortType.Dfu);
        assertEquals(1, dfuPorts.size());
        assertEquals(LinkManager.DFU, dfuPorts.get(0).port);
    }

    @Test
    public void fastScanCycleNeverRunsExpensiveDeviceProbes() {
        probes.dfuConnected = true;

        scan(false);

        assertEquals(0, probes.deviceProbeCalls);
        assertFalse(scanner.getCurrentHardware().isDfuFound());
    }

    @Test
    public void deviceProbesAreThrottledAndLastKnownValuesReusedBetweenProbes() {
        probes.dfuConnected = true;

        scan(true);
        assertEquals(1, probes.deviceProbeCalls);

        probes.time += 1000; // within the throttle interval
        scan(true);
        assertEquals(1, probes.deviceProbeCalls, "device probes must not run every scan cycle");
        assertTrue(scanner.getCurrentHardware().isDfuFound(), "last-known result must be reused, not dropped");

        probes.time += 3000; // past the throttle interval
        scan(true);
        assertEquals(2, probes.deviceProbeCalls);
    }

    @Test
    public void deviceProbesAreSkippedWhileLiveEcuIsConnected() {
        probes.liveEcuConnected = true;
        probes.dfuConnected = true;

        scan(true);

        assertEquals(0, probes.deviceProbeCalls,
            "a connected board cannot also be a DFU device; the scan thread must stay responsive");
    }

    @Test
    public void tcpPortWithoutEcuIsReportedUnknownAndNotCached() {
        probes.tcpPorts.add("29001");

        scan(true);
        scan(true);

        assertEquals(2, probes.tcpCalibrationsCalls, "a non-ECU TCP port must be re-checked, not cached");
        List<PortResult> ports = knownPorts();
        assertEquals(1, ports.size());
        assertEquals(SerialPortType.Unknown, ports.get(0).type);
    }

    @Test
    public void preCachedPortIsNeverProbedUntilInvalidated() {
        PortResult live = new PortResult("COM7", SerialPortType.EcuWithOpenblt);
        probes.serialPorts.add("COM7");
        probes.inspectResults.put("COM7", new PortResult("COM7", SerialPortType.EcuWithOpenblt));

        scanner.cachePort(live);
        scan(false);
        assertFalse(probes.inspectCalls.containsKey("COM7"),
            "cachePort must prevent the scanner from opening a port a live connection is using");

        scanner.invalidatePort("COM7");
        scan(false);
        assertEquals(1, (int) probes.inspectCalls.get("COM7"),
            "invalidatePort must force re-inspection on the next cycle");
    }

    @Test
    public void ecuSortsAboveUnknownPorts() {
        addPort("COM1", SerialPortType.Unknown);
        addPort("COM2", SerialPortType.Ecu);

        scan(false);

        List<PortResult> ports = knownPorts();
        assertEquals(2, ports.size());
        assertEquals(SerialPortType.Ecu, ports.get(0).type, "your ECU belongs at the top of the list");
    }
}
