package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsUpdater;

public class UpdateCalibrationsJob extends AsyncJobWithContext<SerialPortJobContext> {
    public UpdateCalibrationsJob(final SerialPortScanner.PortResult port) {
        super("Update calibrations", new SerialPortJobContext(port));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        CalibrationsUpdater.INSTANCE.updateCalibrations(
            context.getPort().port,
            callbacks
        );
    }
}
