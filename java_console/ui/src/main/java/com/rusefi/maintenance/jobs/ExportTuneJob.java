package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.Optional;
import java.util.concurrent.ExecutionException;

public class ExportTuneJob extends AsyncJobWithContext<ExportTuneJobContext> {
    private final ConnectivityContext connectivityContext;
    private final @Nullable BinaryProtocol binaryProtocol;
    private final @Nullable LinkManager linkManager;

    public ExportTuneJob(PortResult port, String fileName, ConnectivityContext connectivityContext) {
        super("Save Tune", new ExportTuneJobContext(port, fileName));
        this.connectivityContext = connectivityContext;
        this.binaryProtocol = null;
        this.linkManager = null;
    }

    public ExportTuneJob(BinaryProtocol bp, LinkManager lm, String fileName, ConnectivityContext connectivityContext) {
        super("Save Tune", new ExportTuneJobContext(null, fileName));
        this.connectivityContext = connectivityContext;
        this.binaryProtocol = bp;
        this.linkManager = lm;
    }

    public static void exportTuneFromDevice(PortResult port, JComponent parent, ConnectivityContext connectivityContext, String tuneFileName, SingleAsyncJobExecutor singleAsyncJobExecutor) {
        singleAsyncJobExecutor.startJob(new ExportTuneJob(port, tuneFileName, connectivityContext), parent);
    }

    public static void exportTuneViaLiveConnection(BinaryProtocol bp, LinkManager lm, JComponent parent,
                                                    ConnectivityContext connectivityContext, String tuneFileName,
                                                    SingleAsyncJobExecutor singleAsyncJobExecutor) {
        singleAsyncJobExecutor.startJob(new ExportTuneJob(bp, lm, tuneFileName, connectivityContext), parent);
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
        if (binaryProtocol != null) {
            JobHelper.doJob(() -> {
                Optional<CalibrationsInfo>[] holder = new Optional[]{Optional.empty()};
                try {
                    linkManager.submit(() -> holder[0] = CalibrationsHelper.readCurrentCalibrations(binaryProtocol, callbacks)).get();
                } catch (ExecutionException | InterruptedException e) {
                    if (e instanceof InterruptedException) Thread.currentThread().interrupt();
                    callbacks.logLine("Failed to read calibrations: " + e.getMessage());
                    callbacks.error();
                    return;
                }
                if (holder[0].isPresent()) {
                    if (CalibrationsHelper.backUpCalibrationsInfo(holder[0].get(), context.getFileName(), callbacks)) {
                        callbacks.done();
                    } else {
                        callbacks.error();
                    }
                } else {
                    callbacks.error();
                }
            }, onJobFinished);
        } else {
            JobHelper.doJob(() -> {
                Optional<CalibrationsInfo> calibrationsInfo = CalibrationsHelper.readCurrentCalibrations(
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
}
