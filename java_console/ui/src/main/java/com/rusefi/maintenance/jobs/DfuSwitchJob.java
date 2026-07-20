package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;
import java.util.Objects;

public class DfuSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    interface Rebooter {
        void rebootToDfu(JComponent parent, String port, UpdateOperationCallbacks callbacks);
    }

    private final Rebooter rebooter;

    public DfuSwitchJob(final PortResult port, final JComponent parent) {
        this(port, parent, ProgramSelector::rebootToDfu);
    }

    DfuSwitchJob(final PortResult port, final JComponent parent, final Rebooter rebooter) {
        super("DFU switch", new SerialPortWithParentComponentJobContext(port, parent));
        this.rebooter = rebooter;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                PortResult port = context.getPort();
                Objects.requireNonNull(port, "port");
                rebooter.rebootToDfu(context.getParent(), port.port, callbacks);
            },
            onJobFinished
        );
    }
}
