package com.opensr5.ini;

/** Channel identifier and display label from an INI {@code [Datalog]} entry. */
public final class DatalogEntry {
    private final String channel;
    private final String label;

    public DatalogEntry(String channel, String label) {
        this.channel = channel;
        this.label = label;
    }

    public String getChannel() {
        return channel;
    }

    public String getLabel() {
        return label;
    }
}
