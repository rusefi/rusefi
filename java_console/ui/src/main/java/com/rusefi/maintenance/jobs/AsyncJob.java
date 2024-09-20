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

    public abstract void doJob(final UpdateOperationCallbacks callbacks);
}
