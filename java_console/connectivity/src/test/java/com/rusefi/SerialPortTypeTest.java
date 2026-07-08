package com.rusefi;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Pins the sort ordering the scanner's port list relies on ({@code Comparator.comparingInt(sortOrder)}):
 * bootloaders first (they are what a flash-recovery user is looking for), then running ECUs, then
 * everything else, Unknown last. [tag:better_ux_for_flashing]
 */
public class SerialPortTypeTest {

    @Test
    public void bootloadersSortFirstAndUnknownSortsLast() {
        assertTrue(SerialPortType.OpenBlt.sortOrder < SerialPortType.Dfu.sortOrder);
        assertTrue(SerialPortType.Dfu.sortOrder < SerialPortType.Ecu.sortOrder);
        assertTrue(SerialPortType.Ecu.sortOrder < SerialPortType.CAN.sortOrder);
        assertTrue(SerialPortType.CAN.sortOrder < SerialPortType.Unknown.sortOrder);
    }

    @Test
    public void bothEcuFlavorsShareTheSameRank() {
        assertEquals(SerialPortType.Ecu.sortOrder, SerialPortType.EcuWithOpenblt.sortOrder,
            "a running ECU sorts the same whether or not it has OpenBLT support");
    }
}
