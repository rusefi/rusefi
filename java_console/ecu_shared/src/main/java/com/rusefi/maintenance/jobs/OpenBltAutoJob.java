package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.FirmwareOperations;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.FirmwareSelection;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;

public class OpenBltAutoJob extends AbstractAutoFlashJob {
    @Nullable
    private final String firmwareSrecFile;
    private final CalibrationsHelper.FirmwareUpdatePolicy policy;
    @Nullable private FirmwareSelection selection;

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
        this(port, parent, connectivityContext, linkManager, firmwareSrecFile, policy, null);
    }

    public OpenBltAutoJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                          final @Nullable LinkManager linkManager, final @Nullable String firmwareSrecFile,
                          final CalibrationsHelper.FirmwareUpdatePolicy policy,
                          final @Nullable FirmwareSelection selection) {
        super("Update ECU firmware",
            port, parent, connectivityContext, linkManager);
        this.firmwareSrecFile = firmwareSrecFile;
        this.policy = policy;
        this.selection = selection;
    }

    @Override
    protected boolean isFlashAllowed(final BinaryProtocol bp, final UpdateOperationCallbacks callbacks) {
        if (LinkManager.isSlcanPort(context.getPort().port)) {
            callbacks.logLine("Automatic OpenBLT flashing over SLCAN is not supported.");
            return false;
        }
        if (selection == null) {
            selection = firmwareSrecFile == null
                ? FirmwareSelection.latest(bp.signature, null, false, callbacks)
                : FirmwareSelection.fromSrec(bp.signature, firmwareSrecFile, callbacks);
            if (selection == null) {
                return false;
            }
        }
        if (!selection.matchesSrecPath(firmwareSrecFile)) {
            callbacks.firmwareUpdateBlocked("The firmware file does not match the selected update. Please restart the update.");
            return false;
        }
        return selection.revalidateForSrec(bp.signature, callbacks);
    }

    @Override
    protected boolean flash(final LinkManager lm, final BinaryProtocol bp, final UpdateOperationCallbacks callbacks) {
        if (isAutomaticCanPort(context.getPort().port)) {
            return FirmwareOperations.flashOpenbltCanAutomatic(
                context.getParent(), context.getPort(), bp, lm, callbacks, connectivityContext,
                firmwareSrecFile, policy, selection);
        }
        return FirmwareOperations.flashOpenbltSerialAutomatic(
            context.getParent(), context.getPort(), bp, lm, callbacks, connectivityContext, firmwareSrecFile, policy, selection);
    }

    private static boolean isAutomaticCanPort(String port) {
        return LinkManager.PCAN.equals(port) || LinkManager.SOCKET_CAN.equals(port);
    }
}
