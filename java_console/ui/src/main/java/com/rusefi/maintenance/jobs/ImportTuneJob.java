package com.rusefi.maintenance.jobs;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;

import javax.swing.*;
import javax.xml.bind.JAXBException;

import static com.devexperts.logging.Logging.getLogging;

public class ImportTuneJob extends AsyncJobWithContext<ImportTuneJobContext> {
    private static final Logging log = getLogging(ImportTuneJob.class);
    private final ConnectivityContext connectivityContext;

    public ImportTuneJob(final PortResult port, final Msq tuneToImport, final ConnectivityContext connectivityContext) {
        super("Import tune", new ImportTuneJobContext(port, tuneToImport));
        this.connectivityContext = connectivityContext;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
    }
}
