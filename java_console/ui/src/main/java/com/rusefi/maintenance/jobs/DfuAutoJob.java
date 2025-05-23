package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

import javax.swing.*;

public class DfuAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public DfuAutoJob(final PortResult port, final JComponent parent) {
        super("DFU update", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (DfuFlasher.doAutoDfu(context.getParent(), context.getPort(), callbacks)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            },
            onJobFinished
        );
    }
}
