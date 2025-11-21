package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;
import java.util.Objects;

public class DfuSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public DfuSwitchJob(final PortResult port, final JComponent parent) {
        super("DFU switch", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                PortResult port = context.getPort();
                Objects.requireNonNull(port, "port");
                ProgramSelector.rebootToDfu(context.getParent(), port.port, callbacks);
            },
            onJobFinished
        );
    }
}
