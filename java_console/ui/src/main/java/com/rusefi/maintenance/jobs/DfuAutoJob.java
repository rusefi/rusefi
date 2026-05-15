package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;

public class DfuAutoJob extends AbstractAutoFlashJob {
    public DfuAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                      final @Nullable LinkManager linkManager) {
        super("DFU update", port, parent, connectivityContext, linkManager);
    }

    @Override
    protected boolean flash(final LinkManager lm, final BinaryProtocol bp, final UpdateOperationCallbacks callbacks) {
        return DfuFlasher.doAutoDfu(context.getParent(), context.getPort(), bp, lm, callbacks, connectivityContext);
    }
}
