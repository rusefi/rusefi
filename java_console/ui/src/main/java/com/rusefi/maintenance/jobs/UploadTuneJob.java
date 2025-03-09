package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.TuneUploader;

public class UploadTuneJob extends AsyncJobWithContext<UploadTuneJobContext> {
    public UploadTuneJob(final SerialPortScanner.PortResult port, final String panamaUrl) {
        super("Upload Tune", new UploadTuneJobContext(port, panamaUrl));
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
        JobHelper.doJob(() -> {
            if (TuneUploader.INSTANCE.uploadTune(context.getPort(), context.getPanamaUrl(), callbacks)) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        }, onJobFinished);

    }
}
