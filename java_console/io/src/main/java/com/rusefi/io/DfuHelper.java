package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.StatusConsumer;

import javax.swing.*;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class DfuHelper {
    private static final Logging log = getLogging(DfuHelper.class);
    private static final String PREFIX = "rusefi_bundle";

    public static void sendDfuRebootCommand(IoStream stream, UpdateOperationCallbacks callbacks, String cmd) {
        byte[] command = BinaryProtocol.getTextCommandBytes(cmd);
        try {
            stream.sendPacket(command);
            stream.close();
            callbacks.log(String.format("Reboot command [%s] sent into %s!\n", cmd, stream));
        } catch (IOException e) {
            callbacks.log("Error " + e);
        }
    }

    public static boolean sendDfuRebootCommand(JComponent parent, String signature, IoStream stream, UpdateOperationCallbacks callbacks, String command) {
        RusEfiSignature controllerSignature = SignatureHelper.parse(signature);
        String fileSystemBundleTarget = BundleUtil.getBundleTarget();
        if (fileSystemBundleTarget != null && controllerSignature != null) {
            // hack: QC firmware self-identifies as "normal" not QC firmware :(
            if (!fileSystemBundleTarget.equalsIgnoreCase(controllerSignature.getBundleTarget()) && !fileSystemBundleTarget.contains("_QC_")) {
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

        sendDfuRebootCommand(stream, callbacks, command);
        return true;
    }
}
