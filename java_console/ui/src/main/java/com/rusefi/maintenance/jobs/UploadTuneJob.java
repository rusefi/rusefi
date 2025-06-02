package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.TuneUploader;

public class UploadTuneJob extends AsyncJobWithContext<UploadTuneJobContext> {
    private final ConnectivityContext connectivityContext;

    public UploadTuneJob(ConnectivityContext connectivityContext, final PortResult port, final String panamaUrl) {
        super("Upload Tune", new UploadTuneJobContext(port, panamaUrl));
        this.connectivityContext = connectivityContext;
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
        JobHelper.doJob(() -> {
            if (TuneUploader.INSTANCE.uploadTune(context.getPort(), context.getPanamaUrl(), callbacks, connectivityContext)) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        }, onJobFinished);

    }
}
