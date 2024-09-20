package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsUpdater;

import static com.rusefi.maintenance.JobType.UPDATE_CALIBRATIONS;

public class UpdateCalibrationsJob extends AsyncJobWithContext<SerialPortJobContext> {
    public UpdateCalibrationsJob(final SerialPortScanner.PortResult port) {
        super(UPDATE_CALIBRATIONS.jobName, new SerialPortJobContext(port));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        CalibrationsUpdater.INSTANCE.updateCalibrations(
            context.getPort().port,
            callbacks
        );
    }
}
