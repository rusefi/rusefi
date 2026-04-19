package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;

import javax.swing.*;
import java.util.Optional;

public class ExportTuneJob extends AsyncJobWithContext<ExportTuneJobContext> {
    private final ConnectivityContext connectivityContext;

    public ExportTuneJob(PortResult port, String fileName, ConnectivityContext connectivityContext) {
        super("Save Tune", new ExportTuneJobContext(port, fileName));
        this.connectivityContext = connectivityContext;
    }

    public static void exportTuneFromDevice(PortResult port, JComponent parent, ConnectivityContext connectivityContext, String tuneFileName, SingleAsyncJobExecutor singleAsyncJobExecutor) {
        singleAsyncJobExecutor.startJob(new ExportTuneJob(port, tuneFileName, connectivityContext), parent);
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
        JobHelper.doJob(() -> {
            Optional<com.rusefi.maintenance.CalibrationsInfo> calibrationsInfo = CalibrationsHelper.readCurrentCalibrations(
                    context.getPort().port,
                    callbacks,
                    connectivityContext
            );

            if (calibrationsInfo.isPresent()) {
                if (CalibrationsHelper.backUpCalibrationsInfo(calibrationsInfo.get(), context.getFileName(), callbacks)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            } else {
                callbacks.error();
            }
        }, onJobFinished);
    }
}
