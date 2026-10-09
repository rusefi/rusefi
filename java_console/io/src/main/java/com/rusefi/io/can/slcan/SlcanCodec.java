package com.rusefi.io.can.slcan;

import com.rusefi.io.can.CanAddress;
import com.rusefi.io.can.ClassicCanFrame;
import org.jetbrains.annotations.Nullable;

/** Strict classic SLCAN frame syntax, independent of serial I/O and receive filtering. */
public final class SlcanCodec {
    private SlcanCodec() {
    }

    /** One valid t/T/r/R line. Bus indices are zero-based; untagged traffic is bus 0. */
    public static final class Frame {
        public final int busIndex;
        public final CanAddress address;
        public final boolean rtr;
        public final int dlc;
        public final byte[] data;
        @Nullable
        public final String timestamp;

        private Frame(int busIndex, CanAddress address, boolean rtr, int dlc,
                      byte[] data, String timestamp) {
            this.busIndex = busIndex;
            this.address = address;
            this.rtr = rtr;
            this.dlc = dlc;
            this.data = data;
            this.timestamp = timestamp;
        }
    }

    /** Encode a data frame without its terminating CR. */
    public static String encode(ClassicCanFrame frame, int busIndex) {
        if (busIndex < 0 || busIndex > 2) {
            throw new IllegalArgumentException("SLCAN bus index must be 0..2");
        }
        CanAddress address = frame.getAddress();
        byte[] payload = frame.getPayload();
        StringBuilder result = new StringBuilder(busIndex == 0 ? "" : busIndex == 1 ? "&" : "$");
        result.append(String.format(address.isExtended() ? "T%08X" : "t%03X", address.getId()));
        result.append(payload.length);
        for (byte value : payload) {
            result.append(String.format("%02X", value & 0xff));
        }
        return result.toString();
    }

    /** Return null for non-frame lines, invalid IDs, non-hex data or incorrect lengths. */
    @Nullable
    public static Frame decode(String line) {
        if (line == null || line.isEmpty()) {
            return null;
        }
        int busIndex = 0;
        if (line.charAt(0) == '&' || line.charAt(0) == '$') {
            busIndex = line.charAt(0) == '&' ? 1 : 2;
            line = line.substring(1);
        }
        if (!line.matches("([tr][0-9A-Fa-f]{3}|[TR][0-9A-Fa-f]{8})[0-8][0-9A-Fa-f]*")) {
            return null;
        }
        char type = line.charAt(0);
        boolean extended = type == 'T' || type == 'R';
        boolean rtr = type == 'r' || type == 'R';
        int idEnd = extended ? 9 : 4;
        int dlc = Character.digit(line.charAt(idEnd), 16);
        int expected = idEnd + 1 + (rtr ? 0 : 2 * dlc);
        if (line.length() != expected && line.length() != expected + 4) {
            return null;
        }
        try {
            CanAddress address = new CanAddress(Integer.parseInt(line.substring(1, idEnd), 16), extended);
            byte[] payload = new byte[rtr ? 0 : dlc];
            for (int i = 0; i < payload.length; i++) {
                int offset = idEnd + 1 + 2 * i;
                payload[i] = (byte) Integer.parseInt(line.substring(offset, offset + 2), 16);
            }
            String timestamp = line.length() == expected + 4 ? line.substring(expected) : null;
            return new Frame(busIndex, address, rtr, dlc, payload, timestamp);
        } catch (IllegalArgumentException e) {
            return null;
        }
    }
}
