package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final ConnectivityContext connectivityContext;
    private final LinkManager linkManager;

    public OpenBltAutoJob(final PortResult port, final JComponent parent, ConnectivityContext connectivityContext) {
        this(port, parent, connectivityContext, null);
    }

    public OpenBltAutoJob(final PortResult port, final JComponent parent, ConnectivityContext connectivityContext, final LinkManager linkManager) {
        super("OpenBLT via Serial", new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
        this.linkManager = linkManager;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (linkManager != null) {
                    // Release the active ConsoleUI connection so the port is free for
                    // calibrations backup and OpenBLT operations, then restart the scanner
                    // so waitForEcuPortDisappeared / waitForNewOpenBltPortAppeared work.
                    linkManager.disconnect();
                    connectivityContext.getSerialPortScanner().restartTimer();
                }
                if (ProgramSelector.flashOpenbltSerialAutomatic(context.getParent(), context.getPort(), callbacks, connectivityContext)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
                // if trigger from console, restart the linkManager, even in case of error
                if (linkManager != null) {
                    linkManager.reconnect();
                }
            },
            onJobFinished
        );
    }
}
