package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final LinkManager linkManager;

    public OpenBltSwitchJob(final PortResult port, final JComponent parent) {
        this(port, parent, null);
    }

    public OpenBltSwitchJob(final PortResult port, final JComponent parent, final LinkManager linkManager) {
        super("OpenBLT switch", new SerialPortWithParentComponentJobContext(port, parent));
        this.linkManager = linkManager;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (linkManager != null) {
                    BinaryProtocol bp = linkManager.getBinaryProtocol();
                    ProgramSelector.rebootToOpenblt(context.getParent(), bp, callbacks);
                    // Disconnect before the ECU drops the link; prevents ConnectionWatchdog
                    // from calling restart() → connect(port, false) → ExitUtil.exit().
                    linkManager.disconnect();
                } else {
                    ProgramSelector.rebootToOpenblt(context.getParent(), context.getPort().port, callbacks);
                }
            },
            onJobFinished
        );
    }
}
