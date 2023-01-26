package com.rusefi.composite;

public class CompositeEvent {
    private final long timestamp;
    private final boolean primaryTrigger;
    private final boolean secondaryTrigger;
    private final boolean trg;
    private final boolean sync;

    private static final int numCoils = 6;
    private final boolean coils[] = new boolean[numCoils];
    private static final int numInjectors = 6;
    private final boolean injectors[] = new boolean[numInjectors];

    public CompositeEvent(long timestamp, boolean primaryTrigger, boolean secondaryTrigger, boolean trg, boolean sync, boolean coil, boolean injector) {
        this.timestamp = timestamp;
        this.primaryTrigger = primaryTrigger;
        this.secondaryTrigger = secondaryTrigger;
        this.trg = trg;
        this.sync = sync;

        this.coils[0] = coil;
        for (int i = 1; i < numCoils; i++) {
            this.coils[i] = false;
        }

        this.injectors[0] = injector;
        for (int i = 1; i < numInjectors; i++) {
            this.injectors[i] = false;
        }
    }

    public CompositeEvent(long timestamp, boolean primaryTrigger, boolean secondaryTrigger, boolean trg, boolean sync, boolean coils[], boolean injectors[]) {
        this.timestamp = timestamp;
        this.primaryTrigger = primaryTrigger;
        this.secondaryTrigger = secondaryTrigger;
        this.trg = trg;
        this.sync = sync;
        System.arraycopy(coils, 0, this.coils, 0, numCoils);
        System.arraycopy(injectors, 0, this.injectors, 0, numInjectors);
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
        return isCoil(0);
    }

    public int isCoil(int idx) {
        return coils[idx] ? 1 : 0;
    }

    public int isInjector() {
        return isInjector(0);
    }

    public int isInjector(int idx) {
        return injectors[idx] ? 1 : 0;
    }
}
