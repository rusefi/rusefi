package com.rusefi.maintenance.jobs;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;

import javax.swing.*;
import jakarta.xml.bind.JAXBException;

import static com.devexperts.logging.Logging.getLogging;

public class ImportTuneJob extends AsyncJobWithContext<ImportTuneJobContext> {
    private static final Logging log = getLogging(ImportTuneJob.class);
    private final ConnectivityContext connectivityContext;

    public ImportTuneJob(final PortResult port, final Msq tuneToImport, final ConnectivityContext connectivityContext) {
        super("Load Tune", new ImportTuneJobContext(port, tuneToImport));
        this.connectivityContext = connectivityContext;
    }

    public static void importTuneIntoDevice(PortResult port, JComponent parent, ConnectivityContext connectivityContext, String tuneFileName, SingleAsyncJobExecutor singleAsyncJobExecutor) {
        try {
            final Msq tuneToImport = Msq.readTune(tuneFileName);
            singleAsyncJobExecutor.startJob(new ImportTuneJob(port, tuneToImport, connectivityContext), parent);
        } catch (JAXBException e) {
            final String errorMsg = String.format(
                "Failed to load tune to import from file %s",
                tuneFileName
            );
            log.error(errorMsg, e);
            JOptionPane.showMessageDialog(
                parent,
                errorMsg,
                "Error",
                JOptionPane.ERROR_MESSAGE
            );
        }
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(() -> {
            if (CalibrationsHelper.importTune(
                context.getPort().port,
                context.getTuneToImport(),
                callbacks,
                connectivityContext
            )) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        }, onJobFinished);
    }
}

