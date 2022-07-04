package com.rusefi.composite;

public class CompositeEvent {
    private final long timestamp;
    private final boolean primaryTrigger;
    private final boolean secondaryTrigger;
    private final boolean trg;
    private final boolean sync;
    private final boolean coil;
    private final boolean injector;

    public CompositeEvent(long timestamp, boolean primaryTrigger, boolean secondaryTrigger, boolean trg, boolean sync, boolean coil, boolean injector) {
        this.timestamp = timestamp;
        this.primaryTrigger = primaryTrigger;
        this.secondaryTrigger = secondaryTrigger;
        this.trg = trg;
        this.sync = sync;
        this.coil = coil;
        this.injector = injector;
    }

    public long getTimestamp() {
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

    public int isCoil() {
        return coil ? 1 : 0;
    }

    public int isInjector() {
        return injector ? 1 : 0;
    }
}
