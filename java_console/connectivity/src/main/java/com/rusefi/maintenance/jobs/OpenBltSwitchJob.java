package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;

import javax.swing.*;

public class OpenBltSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    /**
     * Seam for unit tests around the static reboot-to-bootloader commands, so OpenBltSwitchJobTest
     * can assert the reboot-then-close() choreography without hardware. Production wires
     * {@link OpenbltRebooter#rebootToOpenblt}.
     */
    interface Rebooter {
        void rebootToOpenblt(JComponent parent, BinaryProtocol bp, UpdateOperationCallbacks callbacks);

        void rebootToOpenblt(JComponent parent, String port, UpdateOperationCallbacks callbacks);
    }

    private final LinkManager linkManager;
    private final Rebooter rebooter;

    // package-private: unit tests inject a recording rebooter, see Rebooter
    public OpenBltSwitchJob(final PortResult port, final JComponent parent, final LinkManager linkManager,
                     final Rebooter rebooter) {
        super("OpenBLT switch", new SerialPortWithParentComponentJobContext(port, parent));
        this.linkManager = linkManager;
        this.rebooter = rebooter;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (linkManager != null) {
                    BinaryProtocol bp = linkManager.getBinaryProtocol();
                    rebooter.rebootToOpenblt(context.getParent(), bp, callbacks);
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
                    rebooter.rebootToOpenblt(context.getParent(), context.getPort().port, callbacks);
                }
            },
            onJobFinished
        );
    }
}
