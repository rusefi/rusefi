package com.rusefi.maintenance.jobs;

public abstract class AsyncJobWithContext<JobContextType> extends AsyncJob {
    protected final JobContextType context;

    protected AsyncJobWithContext(final String name, final JobContextType context) {
        super(name);
        this.context = context;
    }
}
