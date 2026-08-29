package com.rusefi.candump;

import java.time.LocalDateTime;
import java.time.ZoneOffset;
import java.util.Locale;

/**
 * Pure formatting helpers for the PCAN-View .trc dump format (file version 1.1).
 * Kept free of hardware dependencies so the layout is unit-testable.
 *
 * <p>Row layout mirrors PCAN-View 6.x:
 * <pre>
 *      1)         1.6  Rx         0646  8  03 FF 00 00 00 00 FF FD
 * </pre>
 * Standard IDs are 4 hex digits, extended IDs 8 hex digits, both right-aligned
 * in a 13-character field. The time offset is written with millisecond
 * precision (3 decimals - the PCAN-USB timestamp is 1 us).
 */
public final class CanDumpFormat {

    private CanDumpFormat() {
    }

    	/** One message row in the PCAN-View TRC layout. */
    	public static String messageRow(long number, double offsetMs, String type,
    	                                int id, boolean extended, int dlc, byte[] data) {
    		String idText = String.format(Locale.ROOT, extended ? "%08X" : "%04X", id & (extended ? 0x1FFFFFFF : 0x7FF));
    		// RTR frames carry a DLC but no data bytes - clamp to the buffer.
    		String dataText = hex(data, Math.min(dlc, data.length));
        return String.format(Locale.ROOT, "%6d) %12.3f  %-5s %13s  %d  %s",
                number, offsetMs, type, idText, dlc, dataText);
    }

    /** A bus-status / error row: the status text lands in the ID column. */
    public static String statusRow(long number, double offsetMs, String statusText, int code) {
        return String.format(Locale.ROOT, "%6d) %12.3f  %-5s %13s  0x%04X",
                number, offsetMs, "Status", statusText, code);
    }

    /** Space-separated uppercase hex bytes, e.g. "03 FF 00". */
    public static String hex(byte[] data, int length) {
        StringBuilder sb = new StringBuilder(length * 3);
        for (int i = 0; i < length; i++) {
            if (i > 0) {
                sb.append(' ');
            }
            sb.append(String.format(Locale.ROOT, "%02X", data[i] & 0xFF));
        }
        return sb.toString();
    }

    /**
     * PCAN-View's $STARTTIME serial: the LOCAL wall-clock date/time serialized
     * as if it were UTC (PCAN-View does exactly this - verified against
     * orig_1.trc: 2026-08-12 18:21:08.298 MSK -> 46246.7647).
     */
    public static double startTimeSerial(LocalDateTime start) {
        long localAsUtcMillis = start.toInstant(ZoneOffset.UTC).toEpochMilli();
        return localAsUtcMillis / 86_400_000.0 + 25_569.0;
    }

    /** "dd.MM.yyyy HH:mm:ss.SSS.0" like the PCAN-View "Start time" header line. */
    public static String startTimeText(LocalDateTime start) {
        return String.format(Locale.ROOT, "%02d.%02d.%04d %02d:%02d:%02d.%03d.0",
                start.getDayOfMonth(), start.getMonthValue(), start.getYear(),
                start.getHour(), start.getMinute(), start.getSecond(),
                start.getNano() / 1_000_000);
    }

    /** Human-readable name for the common PCAN status codes. */
    public static String statusName(int code) {
        switch (code) {
            case 0x00004: return "BUSLIGHT";
            case 0x00008: return "BUSHEAVY";
            case 0x00010: return "BUSOFF";
            case 0x00020: return "QRCVEMPTY";
            case 0x00040: return "QOVERRUN";
            case 0x00080: return "QXMTFULL";
            case 0x40000: return "BUSPASSIVE";
            default: return null;
        }
    }
}
