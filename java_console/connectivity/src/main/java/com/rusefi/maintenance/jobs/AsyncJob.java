package com.rusefi.maintenance.jobs;

import com.rusefi.io.UpdateOperationCallbacks;

public abstract class AsyncJob {
    private final String name;

    protected AsyncJob(final String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    /** Worker-side preflight while the executor remains reserved, before update/handoff hooks or status windows. */
    public boolean prepare(UpdateOperationCallbacks callbacks) {
        return true;
    }

    public abstract void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished);
}
