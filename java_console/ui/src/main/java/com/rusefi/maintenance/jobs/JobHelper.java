package com.rusefi.maintenance.jobs;

public class JobHelper {
    public static void doJob(final Runnable job, final Runnable onJobFinished) {
        try {
            job.run();
        } finally {
            onJobFinished.run();
        }
    }
}
