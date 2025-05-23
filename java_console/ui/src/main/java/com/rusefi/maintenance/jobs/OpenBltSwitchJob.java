package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public OpenBltSwitchJob(final PortResult port, final JComponent parent) {
        super("OpenBLT switch", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> ProgramSelector.rebootToOpenblt(context.getParent(), context.getPort().port, callbacks),
            onJobFinished
        );
    }
}
