package com.rusefi.openblt;

import java.util.Arrays;

/**
 * A single CAN frame: identifier (11 or 29 bit, the MSB flag stripped), the
 * extended-flag and the payload. Immutable with value equality so tests can
 * compare what actually went out on the wire.
 */
public final class CanFrame {
    private final int id;
    private final boolean extended;
    private final byte[] data;

    public CanFrame(int id, boolean extended, byte[] data) {
        this.id = id;
        this.extended = extended;
        this.data = data.clone();
    }

    public int id() {
        return id;
    }

    public boolean extended() {
        return extended;
    }

    public byte[] data() {
        return data.clone();
    }

    public int length() {
        return data.length;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (!(o instanceof CanFrame other)) {
            return false;
        }
        return id == other.id && extended == other.extended && Arrays.equals(data, other.data);
    }

    @Override
    public int hashCode() {
        int result = 31 * id + (extended ? 1 : 0);
        result = 31 * result + Arrays.hashCode(data);
        return result;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(String.format("%s 0x%03X [%d]", extended ? "EXT" : "STD", id, data.length));
        for (byte b : data) {
            sb.append(String.format(" %02X", b));
        }
        return sb.toString();
    }
}
