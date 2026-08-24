package com.rusefi;

import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.EnumSet;
import java.util.List;

import static java.util.Arrays.asList;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the immutable hardware snapshot the scanner publishes to listeners.
 * Consumers (DevicePane combo, ProgramSelector, ConsoleUI reboot-follow) drive off the
 * type-filtered views, so the filtering semantics are load-bearing. [tag:better_ux_for_flashing]
 */
public class AvailableHardwareTest {
    private static final PortResult ECU = new PortResult("COM1", SerialPortType.Ecu);
    private static final PortResult ECU_WITH_BLT = new PortResult("COM2", SerialPortType.EcuWithOpenblt);
    private static final PortResult BLT = new PortResult("COM3", SerialPortType.OpenBlt);
    private static final PortResult UNKNOWN = new PortResult("COM4", SerialPortType.Unknown);

    private static AvailableHardware hardwareWith(PortResult... ports) {
        return new AvailableHardware(asList(ports), false, false, false);
    }

    @Test
    public void getKnownPortsReturnsDefensiveCopy() {
        AvailableHardware hardware = hardwareWith(ECU);
        List<PortResult> ports = hardware.getKnownPorts();
        ports.clear();
        assertEquals(1, hardware.getKnownPorts().size(), "mutating the returned list must not affect the snapshot");
    }

    @Test
    public void filterByTypeReturnsOnlyMatchingPorts() {
        AvailableHardware hardware = hardwareWith(ECU, ECU_WITH_BLT, BLT, UNKNOWN);
        List<PortResult> bootloaders = hardware.getKnownPorts(SerialPortType.OpenBlt);
        assertEquals(Collections.singletonList(BLT), bootloaders);
    }

    @Test
    public void filterByTypeSetReturnsAllMatchingTypes() {
        AvailableHardware hardware = hardwareWith(ECU, ECU_WITH_BLT, BLT, UNKNOWN);
        // The set used by ConsoleUI's reboot-follow logic: connectable ECU flavors only
        List<PortResult> ecus = hardware.getKnownPorts(EnumSet.of(SerialPortType.Ecu, SerialPortType.EcuWithOpenblt));
        assertEquals(asList(ECU, ECU_WITH_BLT), ecus);
    }

    @Test
    public void isPortAvailableMatchesByPortNameAndType() {
        AvailableHardware hardware = hardwareWith(ECU);
        // a different instance describing the same port+type counts as available
        assertTrue(hardware.isPortAvailable(new PortResult("COM1", SerialPortType.Ecu)));
        assertFalse(hardware.isPortAvailable(new PortResult("COM1", SerialPortType.OpenBlt)),
            "same OS port re-detected as a different type is a different logical device");
        assertFalse(hardware.isPortAvailable(new PortResult("COM9", SerialPortType.Ecu)));
    }

    @Test
    public void isEmptyRequiresNoPortsAndNoDeviceFlags() {
        assertTrue(new AvailableHardware(Collections.emptyList(), false, false, false).isEmpty());
        assertFalse(hardwareWith(UNKNOWN).isEmpty());
        assertFalse(new AvailableHardware(Collections.emptyList(), true, false, false).isEmpty(), "DFU counts as hardware");
        assertFalse(new AvailableHardware(Collections.emptyList(), false, true, false).isEmpty(), "ST-Link counts as hardware");
        assertFalse(new AvailableHardware(Collections.emptyList(), false, false, true).isEmpty(), "PCAN counts as hardware");
    }

    @Test
    public void equalsComparesPortsAndAllDeviceFlags() {
        AvailableHardware base = new AvailableHardware(Collections.singletonList(ECU), false, false, false);
        // the scanner's notify-only-on-change diff relies on this equality
        assertEquals(base, new AvailableHardware(Collections.singletonList(ECU), false, false, false));
        assertNotEquals(base, new AvailableHardware(Collections.emptyList(), false, false, false));
        assertNotEquals(base, new AvailableHardware(Collections.singletonList(ECU), true, false, false));
        assertNotEquals(base, new AvailableHardware(Collections.singletonList(ECU), false, true, false));
        assertNotEquals(base, new AvailableHardware(Collections.singletonList(ECU), false, false, true));
    }
}
