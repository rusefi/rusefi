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
                    // close() — NOT disconnect() — to release the serial port for the transient OpenBLT
                    // state. The board comes back either as an ECU on a (possibly renumbered on Linux) port
                    // or jumps straight back to firmware after the bootloader times out with no flash; we
                    // want auto-reconnect to resume in both cases.
                    //
                    // Why close() and not either extreme:
                    //  - disconnect() sets isDisconnectedByUser=true, permanently blocking
                    //    ConnectionWatchdog.restart() AND the ConsoleUI follow-renumbered-port listener —
                    //    the session would never come back.
                    //  - Leaving the LinkManager fully active keeps the watchdog
                    //    reconnecting while the SerialPortScanner also probes the returning node. On Linux
                    //    the board re-enumerates onto a NEW ttyACMx that is never cached (Unknown results
                    //    aren't cached), so the scanner re-probes it every cycle and the two openers
                    //    mutually EBUSY ("maybe no permissions?") forever → console stuck disconnected.
                    //    (Windows keeps a sticky COM number) This mirrors the working DFU
                    //    path, where DfuFlasher opens/closes its own stream and the live LM is released.
                    // close() frees the port but leaves isDisconnectedByUser=false, so the returning ECU is
                    // classified by the scanner uncontested and a single reconnect brings the console back. [tag:better_ux_for_flashing]
                    linkManager.close();
                } else {
                    ProgramSelector.rebootToOpenblt(context.getParent(), context.getPort().port, callbacks);
                }
            },
            onJobFinished
        );
    }
}
