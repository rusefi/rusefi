package com.rusefi;

import com.rusefi.core.io.UnsupportedEcuInfo;
import com.rusefi.updater.OpenbltDetectorStrategy.OpenbltInfo;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Tests for the PortResult identity contract: two results describing the same OS port with the
 * same detected type are the same logical device, regardless of the calibration/signature payload
 * captured during inspection. List diffing in the scanner and combo reselection in the UI rely on
 * this. [tag:better_ux_for_flashing]
 */
public class PortResultTest {

    @Test
    public void samePortAndTypeAreEqual() {
        assertEquals(new PortResult("COM1", SerialPortType.Ecu), new PortResult("COM1", SerialPortType.Ecu));
    }

    @Test
    public void differentPortOrTypeAreNotEqual() {
        PortResult ecu = new PortResult("COM1", SerialPortType.Ecu);
        assertNotEquals(ecu, new PortResult("COM2", SerialPortType.Ecu));
        assertNotEquals(ecu, new PortResult("COM1", SerialPortType.OpenBlt));
        assertNotEquals(ecu, null);
    }

    @Test
    public void hashCodeIsConsistentWithEquals() {
        // PortResult overrides equals(), so equal instances must hash equally or HashSet/HashMap
        // membership silently breaks
        assertEquals(
            new PortResult("COM1", SerialPortType.Ecu).hashCode(),
            new PortResult("COM1", SerialPortType.Ecu).hashCode());
    }

    @Test
    public void isEcuCoversBothEcuFlavorsOnly() {
        assertTrue(new PortResult("p", SerialPortType.Ecu).isEcu());
        assertTrue(new PortResult("p", SerialPortType.EcuWithOpenblt).isEcu());
        assertFalse(new PortResult("p", SerialPortType.OpenBlt).isEcu(), "a board in the bootloader is not a running ECU");
        assertFalse(new PortResult("p", SerialPortType.Dfu).isEcu());
        assertFalse(new PortResult("p", SerialPortType.CAN).isEcu());
        assertFalse(new PortResult("p", SerialPortType.UnsupportedEcu).isEcu());
        assertFalse(new PortResult("p", SerialPortType.Unknown).isEcu());
        assertFalse(new PortResult("p", SerialPortType.EcuUnknown).isEcu());
    }

    @Test
    public void unsupportedEcuCarriesDetectedIdentity() {
        UnsupportedEcuInfo info = new UnsupportedEcuInfo("hellen121nissan", "universal");
        PortResult unsupported = PortResult.unsupportedEcu("COM4", info);

        assertEquals(SerialPortType.UnsupportedEcu, unsupported.type);
        assertTrue(unsupported.isUnsupportedEcu());
        assertSame(info, unsupported.getUnsupportedEcuInfo());
        assertEquals("COM4 (Unsupported ECU: hellen121nissan)", unsupported.toString());
        assertEquals(unsupported,
            PortResult.unsupportedEcu("COM4", new UnsupportedEcuInfo("other", "universal")),
            "port identity remains port and type, not the scan metadata");
    }

    @Test
    public void firmwareHashIsEmptyWithoutCalibrations() {
        // [tag:offline_tune] pre-cached ports carry no calibration data — unknown hash, not an NPE
        assertFalse(new PortResult("COM1", SerialPortType.Ecu).getFirmwareHash().isPresent());
    }

    @Test
    public void toStringShowsPortWithFriendlyType() {
        assertEquals("COM1 (ECU)", new PortResult("COM1", SerialPortType.Ecu).toString());
        assertEquals("COM2 (OpenBLT Bootloader)", new PortResult("COM2", SerialPortType.OpenBlt).toString());
        assertEquals("COM3 (OpenBLT Bootloader: rusefi.uaefi)",
            new PortResult("COM3", SerialPortType.OpenBlt, null, new OpenbltInfo(true, "rusefi.uaefi")).toString());
    }

    @Test
    public void toStringShowsVersionAndFeatures() {
        assertEquals("COM1 (OpenBLT Bootloader v1.16.0[custom_led])",
            new PortResult("COM1", SerialPortType.OpenBlt, null,
                new OpenbltInfo(true, null, "1.16.0", List.of("custom_led"))).toString());
        assertEquals("COM1 (OpenBLT Bootloader v1.16.0)",
            new PortResult("COM1", SerialPortType.OpenBlt, null,
                new OpenbltInfo(true, null, "1.16.0", null)).toString(),
            "no brackets when the bootloader reports no features");
        assertEquals("COM1 (OpenBLT Bootloader v1.16.0[encrypted,custom_led])",
            new PortResult("COM1", SerialPortType.OpenBlt, null,
                new OpenbltInfo(true, null, "1.16.0", List.of("encrypted", "custom_led"))).toString());
    }

    @Test
    public void toStringShowsBoardString() {
        assertEquals("COM3 (OpenBLT Bootloader v1.16.0[custom_led]: rusefi.uaefi)",
            new PortResult("COM3", SerialPortType.OpenBlt, null,
                new OpenbltInfo(true, "rusefi.uaefi", "1.16.0", List.of("custom_led"))).toString());
    }

}
