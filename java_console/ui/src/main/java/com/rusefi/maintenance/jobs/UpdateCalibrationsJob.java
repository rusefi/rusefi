package com.rusefi.maintenance.jobs;

import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsUpdater;

public class UpdateCalibrationsJob extends AsyncJobWithContext<UpdateCalibrationsJobContext> {
    public UpdateCalibrationsJob(final PortResult port, final ConfigurationImageWithMeta calibrations) {
        super("Update calibrations", new UpdateCalibrationsJobContext(port, calibrations));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        CalibrationsUpdater.INSTANCE.updateCalibrations(
            context.getPort().port,
            context.getCalibrations().getConfigurationImage(),
            callbacks,
            onJobFinished
        );
    }
}
