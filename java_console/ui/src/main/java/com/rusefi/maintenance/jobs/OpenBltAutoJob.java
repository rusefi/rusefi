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
        if (linkManager != null) {
            // Release the active ConsoleUI connection so the port is free for
            // calibrations backup and OpenBLT operations.
            linkManager.disconnect();
        }
        try {
            if (ProgramSelector.flashOpenbltSerialAutomatic(context.getParent(), context.getPort(), callbacks, connectivityContext)) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        } finally {
            // Notify the UI that the job is complete so tabs are re-enabled and the
            // "Updating" overlay is cleared before we begin the reconnect sequence.
            // TODO: maybe add some delay/progress bar/something to indicate the user that we are reconnecting to the ecu after the update
            onJobFinished.run();
        }

        // Reconnect the linkManager after the flash — this happens after onJobFinished
        // so the UI is already responsive while we wait.
        if (linkManager != null) {
            // Suspend the scanner and wait for any in-progress port inspection to
            // finish before reconnecting. SerialPortScanner.inspectPort opens the
            // same serial port; if it runs concurrently with reconnection, the open
            // fails with "no permissions" because the port is already held.
            try {
                connectivityContext.getSerialPortScanner().suspend().await(30, java.util.concurrent.TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            // Discard commands queued in the previous session so they are not
            // replayed after reconnection (ej: a stale "set date" would mismatch the
            // new session's confirmation and crash the connection).
            linkManager.getCommandQueue().clearPendingCommands();
            try {
                linkManager.reconnect();
            } finally {
                // Cache the reconnected port so the scanner does not re-inspect it on
                // the next scan cycle and compete with the newly established connection.
                connectivityContext.getSerialPortScanner().cachePort(
                    new PortResult(context.getPort().port, context.getPort().type)
                );
                connectivityContext.getSerialPortScanner().resume();
            }
        }
    }
}
