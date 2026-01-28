package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.UiProperties;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BundleUtil;

import javax.swing.*;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

/**
 * this code is shared between DFU and OpenBLT tracks
 */
public class BootloaderHelper {
    private static final Logging log = getLogging(BootloaderHelper.class);

    public static boolean sendBootloaderRebootCommand(JComponent parent, String signature, IoStream stream, UpdateOperationCallbacks callbacks, String command) {
        RusEfiSignature controllerSignature = SignatureHelper.parse(signature);
        String fileSystemBundleTarget = BundleUtil.getBundleTarget();
        if (fileSystemBundleTarget != null && controllerSignature != null) {
            // hack: QC firmware self-identifies as "normal" not QC firmware :(
            // [tag:QC_firmware]
            if (!UiProperties.skipEcuTypeDetection() &&
                !fileSystemBundleTarget.equalsIgnoreCase(controllerSignature.getBundleTarget()) && !fileSystemBundleTarget.contains("_QC_")) {
                String message = String.format("You have \"%s\" controller does not look right to program it with \"%s\"", controllerSignature.getBundleTarget(), fileSystemBundleTarget);
                log.info(message);

                SwingUtilities.invokeLater(() -> {
                    JOptionPane.showMessageDialog(parent, message);
                    // in case of mismatched bundle type we are supposed do close connection
                    // and properly handle the case of user hitting "Update Firmware" again
                    // closing connection is a mess on Windows so it's simpler to just exit
                    new Thread(() -> {
                        // let's have a delay and separate thread to address
                        // "wrong bundle" warning text sometimes not visible #3267
                        sleep(5 * SECOND);
                        System.exit(-5);
                    }).start();
                });

                return false;
            }
        }

        BootloaderCommsHelper.sendBootloaderRebootCommand(stream, callbacks, command);
        return true;
    }
}
