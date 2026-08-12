package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.maintenance.CalibrationsHelper;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;

public class OpenBltAutoJob extends AbstractAutoFlashJob {
    @Nullable
    private final String firmwareSrecFile;
    private final CalibrationsHelper.FirmwareUpdatePolicy policy;

    public OpenBltAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                          final @Nullable LinkManager linkManager) {
        this(port, parent, connectivityContext, linkManager, null);
    }

    public OpenBltAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                          final @Nullable LinkManager linkManager, final @Nullable String firmwareSrecFile) {
        this(port, parent, connectivityContext, linkManager, firmwareSrecFile,
            CalibrationsHelper.FirmwareUpdatePolicy.FORWARD_MIGRATION);
    }

    public OpenBltAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                          final @Nullable LinkManager linkManager, final @Nullable String firmwareSrecFile,
                          final CalibrationsHelper.FirmwareUpdatePolicy policy) {
        super("OpenBLT via Serial", port, parent, connectivityContext, linkManager);
        this.firmwareSrecFile = firmwareSrecFile;
        this.policy = policy;
    }

    @Override
    protected boolean flash(final LinkManager lm, final BinaryProtocol bp, final UpdateOperationCallbacks callbacks) {
        return ProgramSelector.flashOpenbltSerialAutomatic(
            context.getParent(), context.getPort(), bp, lm, callbacks, connectivityContext, firmwareSrecFile, policy);
    }
}
