package com.rusefi.io.can;

import java.util.Objects;

/** Immutable Classic CAN arbitration ID. */
public final class CanAddress {
    private static final int STANDARD_MAX_ID = 0x7FF;
    private static final int EXTENDED_MAX_ID = 0x1FFFFFFF;

    private final int id;
    private final boolean extended;

    public CanAddress(int id, boolean extended) {
        int maxId = extended ? EXTENDED_MAX_ID : STANDARD_MAX_ID;
        if (id < 0 || id > maxId) {
            throw new IllegalArgumentException("CAN " + (extended ? "extended" : "standard") + " ID out of range: " + id);
        }
        this.id = id;
        this.extended = extended;
    }

    public int getId() {
        return id;
    }

    public boolean isExtended() {
        return extended;
    }

    @Override
    public boolean equals(Object other) {
        if (this == other) {
            return true;
        }
        if (!(other instanceof CanAddress)) {
            return false;
        }
        CanAddress that = (CanAddress) other;
        return id == that.id && extended == that.extended;
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, extended);
    }

    @Override
    public String toString() {
        return "CanAddress{" + Integer.toHexString(id) + ", extended=" + extended + '}';
    }
}
