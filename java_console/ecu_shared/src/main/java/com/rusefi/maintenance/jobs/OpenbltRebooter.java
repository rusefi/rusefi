package com.rusefi.maintenance.jobs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.BootloaderHelper;
import com.rusefi.io.IoStream;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.FirmwareOperations;
import com.rusefi.maintenance.FirmwareSelection;

import java.io.IOException;

import javax.swing.*;

public class OpenbltRebooter {
    public static final OpenBltSwitchJob.Rebooter PRODUCTION_REBOOTER = new OpenBltSwitchJob.Rebooter() {
        @Override
        public void rebootToOpenblt(final JComponent parent, final BinaryProtocol bp,
                                    final UpdateOperationCallbacks callbacks) {
            // call the static helper, NOT this override — bare rebootToOpenblt(...) recurses into itself
            OpenbltRebooter.rebootToOpenblt(parent, bp, callbacks);
        }

        @Override
        public void rebootToOpenblt(final JComponent parent, final String port,
                                    final UpdateOperationCallbacks callbacks) {
            FirmwareOperations.rebootToOpenblt(parent, port, callbacks);
        }
    };

    /**
     * Send reboot-to-OpenBLT via an already-open BinaryProtocol connection.
     * Use this from ConsoleUI context where the port is held by LinkManager.
     */
    public static boolean rebootToOpenblt(JComponent parent, BinaryProtocol binaryProtocol, UpdateOperationCallbacks callbacks) {
        if (binaryProtocol == null) {
            callbacks.logLine("Not connected?");
            callbacks.error();
            return false;
        }
        return BootloaderHelper.sendBootloaderRebootCommand(
            parent, binaryProtocol.signature, binaryProtocol.getStream(), callbacks, Integration.CMD_REBOOT_OPENBLT);
    }

    public static boolean rebootToOpenblt(JComponent parent, BinaryProtocol binaryProtocol,
                                          UpdateOperationCallbacks callbacks, FirmwareSelection selection) {
        if (binaryProtocol == null) {
            callbacks.logLine("Not connected?");
            callbacks.error();
            return false;
        }
        try {
            String liveSignature = BinaryProtocol.getSignature(binaryProtocol.getStream());
            return rebootWithFreshSignature(parent, binaryProtocol.getStream(), liveSignature, callbacks, selection);
        } catch (IOException e) {
            callbacks.firmwareUpdateBlocked("Unable to re-read ECU signature before OpenBLT handoff: " + e.getMessage());
            return false;
        }
    }

    static boolean rebootWithFreshSignature(JComponent parent, IoStream stream, String liveSignature,
                                            UpdateOperationCallbacks callbacks, FirmwareSelection selection) {
        if (selection == null || !selection.revalidateForSrec(liveSignature, callbacks)) {
            return false;
        }
        return BootloaderHelper.sendBootloaderRebootCommand(parent, liveSignature, stream, callbacks,
            Integration.CMD_REBOOT_OPENBLT, selection);
    }
}
