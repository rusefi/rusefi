package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.ui.StatusConsumer;

import javax.swing.*;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class DfuHelper {
    private static final Logging log = getLogging(DfuHelper.class);
    private static final String PREFIX = "rusefi_bundle";

    public static void sendDfuRebootCommand(IoStream stream, StatusConsumer messages) {
        byte[] command = BinaryProtocol.getTextCommandBytes(Fields.CMD_REBOOT_DFU);
        try {
            stream.sendPacket(command);
            stream.close();
            messages.append(String.format("Reboot command [%s] sent into %s!\n", Fields.CMD_REBOOT_DFU, stream));
        } catch (IOException e) {
            messages.append("Error " + e);
        }
    }

    public static boolean sendDfuRebootCommand(JComponent parent, String signature, IoStream stream, StatusConsumer messages) {
        RusEfiSignature s = SignatureHelper.parse(signature);
        String bundleName = BundleUtil.readBundleFullName();
        if (bundleName != null && s != null) {
            String signatureWithPrefix;
            if ("all".equals(s.getBundle())) {
                signatureWithPrefix = PREFIX;
            } else {
                signatureWithPrefix = PREFIX + "_" + s.getBundle();
            }

            // hack: QC firmare self-identifies as "normal" not QC firmware :(
            if (!bundleName.equalsIgnoreCase(signatureWithPrefix) && !bundleName.contains("_QC_")) {
                String message = String.format("You have \"%s\" controller does not look right to program it with \"%s\"", s.getBundle(), bundleName);
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

        sendDfuRebootCommand(stream, messages);
        return true;
    }
}
