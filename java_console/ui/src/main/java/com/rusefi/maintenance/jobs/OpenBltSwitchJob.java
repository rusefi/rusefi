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
                    // Intentionally NOT calling linkManager.disconnect() here. Switching to OpenBLT is a
                    // transient state: the board comes back either as an ECU on a (possibly renumbered) port
                    // or, after the bootloader times out with no flash, jumps straight back to firmware. We
                    // want auto-reconnect to resume in both cases. disconnect() sets isDisconnectedByUser=true,
                    // which permanently blocks ConnectionWatchdog.restart() *and* the ConsoleUI
                    // follow-renumbered-port listener, so the live session would never come back
                    // (online → OpenBLT → online without flashing hangs forever).
                    // The old ExitUtil.exit() hazard the disconnect guarded against is gone: restart() now
                    // reconnects via connect(port, isScanningForEcu=true), which suppresses the exit. [tag:better_ux_for_flashing]
                } else {
                    ProgramSelector.rebootToOpenblt(context.getParent(), context.getPort().port, callbacks);
                }
            },
            onJobFinished
        );
    }
}
