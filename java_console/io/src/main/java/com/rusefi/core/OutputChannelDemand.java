package com.rusefi.core;

import java.util.Collections;
import java.util.LinkedHashSet;
import java.util.Locale;
import java.util.Objects;
import java.util.Set;

/** Immutable snapshot of output channels currently needed by consumers. */
public final class OutputChannelDemand {
    private final Set<String> channels;
    private final boolean full;
    private final long generation;

    public OutputChannelDemand(Set<String> channels, boolean full, long generation) {
        Objects.requireNonNull(channels, "channels");
        Set<String> normalized = new LinkedHashSet<>();
        for (String channel : channels) {
            normalized.add(Objects.requireNonNull(channel, "channel").toLowerCase(Locale.US));
        }
        this.channels = Collections.unmodifiableSet(normalized);
        this.full = full;
        this.generation = generation;
    }

    public static OutputChannelDemand full(long generation) {
        return new OutputChannelDemand(Collections.emptySet(), true, generation);
    }

    public static OutputChannelDemand selective(Set<String> channels, long generation) {
        return new OutputChannelDemand(channels, false, generation);
    }

    public Set<String> getChannels() {
        return channels;
    }

    public boolean isFull() {
        return full;
    }

    public long getGeneration() {
        return generation;
    }
}
