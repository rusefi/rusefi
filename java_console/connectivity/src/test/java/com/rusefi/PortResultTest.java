package com.rusefi;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
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
        assertFalse(new PortResult("p", SerialPortType.Unknown).isEcu());
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
    }
}
