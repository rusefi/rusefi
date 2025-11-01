package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

import javax.swing.*;

public class DfuAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final ConnectivityContext connectivityContext;

    public DfuAutoJob(final PortResult port, final JComponent parent, ConnectivityContext connectivityContext) {
        super("DFU update", new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (DfuFlasher.doAutoDfu(context.getParent(), context.getPort(), callbacks, connectivityContext)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            },
            onJobFinished
        );
    }
}
