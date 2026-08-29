package com.rusefi.candump;

import org.junit.jupiter.api.Test;

import java.time.LocalDateTime;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

class CanDumpFormatTest {

    @Test
    void messageRowStandard() {
        byte[] data = new byte[]{0x03, (byte) 0xFF, 0x00, 0x00, 0x00, 0x00, (byte) 0xFF, (byte) 0xFD};
        String row = CanDumpFormat.messageRow(1, 1.6, "Rx", 0x0646, false, 8, data);
        assertEquals("     1)        1.600  Rx             0646  8  03 FF 00 00 00 00 FF FD", row);
    }

    @Test
    void messageRowExtended() {
        byte[] data = new byte[]{0x32, 0x03};
        String row = CanDumpFormat.messageRow(2, 2.5, "Rx", 0x10667, true, 2, data);
        assertEquals("     2)        2.500  Rx         00010667  2  32 03", row);
    }

    @Test
    void messageRowRtrNoData() {
        String row = CanDumpFormat.messageRow(3, 0.001, "Rx", 0x123, false, 4, new byte[0]);
        assertEquals("     3)        0.001  Rx             0123  4  ", row);
    }

    @Test
    void statusRow() {
        String row = CanDumpFormat.statusRow(12, 421.537, "BUSHEAVY", 0x0008);
        assertEquals("    12)      421.537  Status      BUSHEAVY  0x0008", row);
    }

    @Test
    void hexFormat() {
        assertEquals("03 FF 00", CanDumpFormat.hex(new byte[]{0x03, (byte) 0xFF, 0x00}, 3));
        assertEquals("", CanDumpFormat.hex(new byte[]{1, 2, 3}, 0));
    }

    @Test
    void startTimeSerialMatchesPcanView() {
        // Verified against orig_1.trc: 12.08.2026 18:21:08.298 (MSK, local-as-UTC) -> 46246.764679375
        LocalDateTime start = LocalDateTime.of(2026, 8, 12, 18, 21, 8, 298_000_000);
        assertEquals(46246.764679375, CanDumpFormat.startTimeSerial(start), 1e-6);
    }

    @Test
    void startTimeText() {
        LocalDateTime start = LocalDateTime.of(2026, 8, 12, 18, 21, 8, 298_000_000);
        assertEquals("12.08.2026 18:21:08.298.0", CanDumpFormat.startTimeText(start));
    }

    @Test
    void statusNameKnown() {
        assertEquals("BUSOFF", CanDumpFormat.statusName(0x00010));
        assertEquals("QOVERRUN", CanDumpFormat.statusName(0x00040));
        assertNull(CanDumpFormat.statusName(0x1234));
    }
}
