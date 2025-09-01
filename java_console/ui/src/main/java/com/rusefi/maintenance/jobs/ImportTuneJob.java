package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.tune.xml.Msq;

public class ImportTuneJob extends AsyncJobWithContext<ImportTuneJobContext> {
    private final ConnectivityContext connectivityContext;

    public ImportTuneJob(final PortResult port, final Msq tuneToImport, final ConnectivityContext connectivityContext) {
        super("Import tune", new ImportTuneJobContext(port, tuneToImport));
        this.connectivityContext = connectivityContext;
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
