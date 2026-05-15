package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;

public class DfuAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final ConnectivityContext connectivityContext;
    private final @Nullable LinkManager linkManager;

    public DfuAutoJob(final PortResult port, final JComponent parent, ConnectivityContext connectivityContext) {
        this(port, parent, connectivityContext, null);
    }

    public DfuAutoJob(final PortResult port, final JComponent parent, ConnectivityContext connectivityContext,
                      final @Nullable LinkManager linkManager) {
        super("DFU update", new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
        this.linkManager = linkManager;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        try {
            final boolean ok;
            if (linkManager != null) {
                BinaryProtocol bp = linkManager.getBinaryProtocol();
                ok = DfuFlasher.doAutoDfu(context.getParent(), context.getPort(), bp, linkManager, callbacks, connectivityContext);
            } else {
                ok = DfuFlasher.doAutoDfu(context.getParent(), context.getPort(), callbacks, connectivityContext);
            }
            if (ok) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        } finally {
            onJobFinished.run();
        }
        if (linkManager != null) {
            try {
                connectivityContext.getSerialPortScanner().suspend().await(30, java.util.concurrent.TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            linkManager.getCommandQueue().clearPendingCommands();
            try {
                linkManager.reconnect();
            } finally {
                connectivityContext.getSerialPortScanner().cachePort(
                    new PortResult(context.getPort().port, context.getPort().type)
                );
                connectivityContext.getSerialPortScanner().resume();
            }
        }
    }
}
