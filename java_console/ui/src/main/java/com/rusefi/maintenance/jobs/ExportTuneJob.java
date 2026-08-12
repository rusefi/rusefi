package com.rusefi.maintenance.jobs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;

import javax.swing.*;
import java.util.Optional;
import java.util.concurrent.ExecutionException;

public class ExportTuneJob extends AsyncJobWithContext<ExportTuneJobContext> {
    private final BinaryProtocol binaryProtocol;
    private final LinkManager linkManager;

    public ExportTuneJob(BinaryProtocol bp, LinkManager lm, String fileName) {
        super("Save Tune", new ExportTuneJobContext(fileName));
        this.binaryProtocol = bp;
        this.linkManager = lm;
    }

    public static void exportTuneViaLiveConnection(BinaryProtocol bp, LinkManager lm, JComponent parent,
                                                    String tuneFileName,
                                                    SingleAsyncJobExecutor singleAsyncJobExecutor) {
        singleAsyncJobExecutor.startJob(new ExportTuneJob(bp, lm, tuneFileName), parent);
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
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
    }
}
