package com.rusefi.composite;

public class CompositeEvent {
    private final int timestamp;
    private final boolean primaryTrigger;
    private final boolean secondaryTrigger;
    private final boolean trg;
    private final boolean sync;

    public CompositeEvent(int timestamp, boolean primaryTrigger, boolean secondaryTrigger, boolean trg, boolean sync) {
        this.timestamp = timestamp;
        this.primaryTrigger = primaryTrigger;
        this.secondaryTrigger = secondaryTrigger;
        this.trg = trg;
        this.sync = sync;
    }

    public int getTimestamp() {
        return timestamp;
    }

    public int isPrimaryTriggerAsInt() {
        return primaryTrigger ? 1 : 0;
    }

    public int isSecondaryTriggerAsInt() {
        return secondaryTrigger ? 1 : 0;
    }

    public int isSyncAsInt() {
        return sync ? 1 : 0;
    }

    public int isTrgAsInt() {
        return trg ? 1 : 0;
    }
}
