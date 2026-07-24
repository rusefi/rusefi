package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.CalibrationsHelper;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;

public class DfuAutoJob extends AbstractAutoFlashJob {
    @Nullable
    private final String firmwareBinFile;
    private final CalibrationsHelper.FirmwareUpdatePolicy policy;

    public DfuAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                      final @Nullable LinkManager linkManager) {
        this(port, parent, connectivityContext, linkManager, null);
    }

    public DfuAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                      final @Nullable LinkManager linkManager, final @Nullable String firmwareBinFile) {
        this(port, parent, connectivityContext, linkManager, firmwareBinFile,
            CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION);
    }

    public DfuAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                      final @Nullable LinkManager linkManager, final @Nullable String firmwareBinFile,
                      final CalibrationsHelper.FirmwareUpdatePolicy policy) {
        super("DFU update", port, parent, connectivityContext, linkManager);
        this.firmwareBinFile = firmwareBinFile;
        this.policy = policy;
    }

    @Override
    protected boolean flash(final LinkManager lm, final BinaryProtocol bp, final UpdateOperationCallbacks callbacks) {
        return DfuFlasher.doAutoDfu(
            context.getParent(), context.getPort(), bp, lm, callbacks, connectivityContext, firmwareBinFile, policy);
    }
}
