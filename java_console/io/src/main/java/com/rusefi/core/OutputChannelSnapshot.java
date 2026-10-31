package com.rusefi.core;

import java.util.BitSet;
import java.util.Collections;
import java.util.LinkedHashSet;
import java.util.Objects;
import java.util.Set;

/**
 * One completed host poll of ECU output channels.
 * Output-channel offsets exclude the response-code byte at response[0].
 */
public final class OutputChannelSnapshot {
    private final byte[] response;
    private final BitSet validBytes;
    private final Set<String> requestedChannels;
    private final long generation;
    private final boolean full;

    public OutputChannelSnapshot(byte[] response, BitSet validBytes, Set<String> requestedChannels,
                                 long generation, boolean full) {
        this.response = Objects.requireNonNull(response, "response").clone();
        this.validBytes = (BitSet) Objects.requireNonNull(validBytes, "validBytes").clone();
        this.requestedChannels = Collections.unmodifiableSet(
            new LinkedHashSet<>(Objects.requireNonNull(requestedChannels, "requestedChannels")));
        this.generation = generation;
        this.full = full;
    }

    public static OutputChannelSnapshot full(byte[] response) {
        BitSet validBytes = new BitSet(Math.max(0, response.length - 1));
        validBytes.set(0, Math.max(0, response.length - 1));
        return new OutputChannelSnapshot(response, validBytes, Collections.emptySet(), 0, true);
    }

    public byte[] getResponse() {
        return response.clone();
    }

    public boolean isRangeValid(int offset, int size) {
        if (offset < 0 || size < 0 || offset + size < offset || offset + size > response.length - 1) {
            return false;
        }
        return size == 0 || validBytes.nextClearBit(offset) >= offset + size;
    }

    public Set<String> getRequestedChannels() {
        return requestedChannels;
    }

    public long getGeneration() {
        return generation;
    }

    public boolean isFull() {
        return full;
    }
}
