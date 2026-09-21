package com.rusefi.io.can;

import java.util.Arrays;
import java.util.Objects;

/** Immutable Classic CAN frame with at most eight payload bytes. */
public final class ClassicCanFrame {
    private static final int MAX_PAYLOAD_BYTES = 8;

    private final CanAddress address;
    private final byte[] payload;

    public ClassicCanFrame(CanAddress address, byte[] payload) {
        this.address = Objects.requireNonNull(address, "address");
        Objects.requireNonNull(payload, "payload");
        if (payload.length > MAX_PAYLOAD_BYTES) {
            throw new IllegalArgumentException("Classic CAN payload exceeds " + MAX_PAYLOAD_BYTES + " bytes: " + payload.length);
        }
        this.payload = Arrays.copyOf(payload, payload.length);
    }

    public CanAddress getAddress() {
        return address;
    }

    public byte[] getPayload() {
        return Arrays.copyOf(payload, payload.length);
    }

    @Override
    public boolean equals(Object other) {
        if (this == other) {
            return true;
        }
        if (!(other instanceof ClassicCanFrame)) {
            return false;
        }
        ClassicCanFrame that = (ClassicCanFrame) other;
        return address.equals(that.address) && Arrays.equals(payload, that.payload);
    }

    @Override
    public int hashCode() {
        return 31 * address.hashCode() + Arrays.hashCode(payload);
    }
}
