package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.concurrent.TimeUnit;

public class JobWithSuspendedSerialPortScanner extends AsyncJob {
    private final AsyncJob innerJob;

    public JobWithSuspendedSerialPortScanner(final AsyncJob innerJob) {
        super(innerJob.getName());
        this.innerJob = innerJob;
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks) {
        try {
            callbacks.logLine("Suspending port scanning...");
            try {
                SerialPortScanner.INSTANCE.suspend().await(1, TimeUnit.MINUTES);
                callbacks.logLine("Port scanning is suspended.");
            } catch (final InterruptedException e) {
                callbacks.logLine("Failed to  suspend port scanning in a minute.");
                callbacks.error();
                return;
            }

            innerJob.doJob(callbacks);
        } finally {
            callbacks.logLine("Resuming port scanning...");
            SerialPortScanner.INSTANCE.resume();
            callbacks.logLine("Port scanning is resumed.");
        }
    }
}
