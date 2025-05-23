package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public OpenBltAutoJob(final PortResult port, final JComponent parent) {
        super("OpenBLT via Serial", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (ProgramSelector.flashOpenbltSerialAutomatic(context.getParent(), context.getPort(), callbacks)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            },
            onJobFinished
        );
    }
}
