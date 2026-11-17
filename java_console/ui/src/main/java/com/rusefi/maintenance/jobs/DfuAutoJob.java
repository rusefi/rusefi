package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.FirmwareSelection;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;

public class DfuAutoJob extends AbstractAutoFlashJob {
    @Nullable
    private final String firmwareBinFile;
    private final CalibrationsHelper.FirmwareUpdatePolicy policy;
    @Nullable private FirmwareSelection selection;

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
        this(port, parent, connectivityContext, linkManager, firmwareBinFile, policy, null);
    }

    public DfuAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                      final @Nullable LinkManager linkManager, final @Nullable String firmwareBinFile,
                      final CalibrationsHelper.FirmwareUpdatePolicy policy,
                      final @Nullable FirmwareSelection selection) {
        super("Update ECU firmware", port, parent, connectivityContext, linkManager);
        this.firmwareBinFile = firmwareBinFile;
        this.policy = policy;
        this.selection = selection;
    }

    @Override
    protected boolean isFlashAllowed(final BinaryProtocol bp, final UpdateOperationCallbacks callbacks) {
        if (selection != null) {
            if (!selection.matchesBinPath(firmwareBinFile)) {
                callbacks.firmwareUpdateBlocked("The firmware file does not match the selected update. Please restart the update.");
                return false;
            }
            return selection.revalidateForBin(bp.signature, callbacks);
        }
        if (firmwareBinFile != null) {
            callbacks.firmwareUpdateBlocked("Couldn't verify which ECU board and branch this firmware is for. Please restart the update.");
            return false;
        }
        selection = FirmwareSelection.latest(bp.signature, null, false, callbacks);
        return selection != null && selection.revalidateForBin(bp.signature, callbacks);
    }

    @Override
    protected boolean flash(final LinkManager lm, final BinaryProtocol bp, final UpdateOperationCallbacks callbacks) {
        if (selection != null && !selection.revalidateForBin(bp.signature, callbacks)) {
            return false;
        }
        return DfuFlasher.doAutoDfu(
            context.getParent(), context.getPort(), bp, lm, callbacks, connectivityContext, firmwareBinFile, policy, selection);
    }
}
