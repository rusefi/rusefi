package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final ConnectivityContext connectivityContext;

    public OpenBltAutoJob(final PortResult port, final JComponent parent, ConnectivityContext connectivityContext) {
        super("OpenBLT via Serial", new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (ProgramSelector.flashOpenbltSerialAutomatic(context.getParent(), context.getPort(), callbacks, connectivityContext)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            },
            onJobFinished
        );
    }
}
