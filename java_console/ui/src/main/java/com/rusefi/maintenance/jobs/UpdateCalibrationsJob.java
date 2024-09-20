package com.rusefi.maintenance.jobs;

import com.opensr5.ConfigurationImage;
import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsUpdater;

public class UpdateCalibrationsJob extends AsyncJobWithContext<UpdateCalibrationsJobContext> {
    public UpdateCalibrationsJob(final SerialPortScanner.PortResult port, final ConfigurationImage calibrations) {
        super("Update calibrations", new UpdateCalibrationsJobContext(port, calibrations));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        CalibrationsUpdater.INSTANCE.updateCalibrations(
            context.getPort().port,
            context.getCalibrations(),
            callbacks
        );
    }
}
