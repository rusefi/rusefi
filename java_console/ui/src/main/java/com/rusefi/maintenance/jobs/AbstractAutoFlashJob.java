package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.concurrent.TimeUnit;

abstract class AbstractAutoFlashJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    protected final ConnectivityContext connectivityContext;
    private final @Nullable LinkManager linkManager;

    protected AbstractAutoFlashJob(final String name, final PortResult port, final JComponent parent,
                                   final ConnectivityContext connectivityContext,
                                   final @Nullable LinkManager linkManager) {
        super(name, new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
        this.linkManager = linkManager;
    }

    protected abstract boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks callbacks);

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        final LinkManager lm = linkManager;
        try {
            if (lm == null) {
                callbacks.logLine("ERROR: No live LinkManager available — auto-connect to an ECU first.");
                callbacks.error();
                return;
            }
            final BinaryProtocol bp = JobHelper.awaitBinaryProtocol(lm, callbacks);
            if (bp == null) {
                callbacks.logLine("Timed out waiting for connection.");
                callbacks.error();
                return;
            }
            if (flash(lm, bp, callbacks)) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        } finally {
            // Notify the UI that the job is complete so tabs are re-enabled and the
            // "Updating" overlay is cleared before we begin the reconnect sequence.
            // TODO: maybe add some delay/progress bar/something to indicate reconnect progress
            onJobFinished.run();
        }

        // lm is non-null here — null path returned inside try
        try {
            connectivityContext.getSerialPortScanner().suspend().await(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        lm.getCommandQueue().clearPendingCommands();
        try {
            lm.reconnect();
        } finally {
            connectivityContext.getSerialPortScanner().cachePort(
                new PortResult(context.getPort().port, context.getPort().type)
            );
            connectivityContext.getSerialPortScanner().resume();
        }
    }

}
