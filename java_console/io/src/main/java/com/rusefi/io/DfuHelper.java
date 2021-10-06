package com.rusefi.io;

import com.rusefi.FileLog;
import com.rusefi.RusEfiSignature;
import com.rusefi.SignatureHelper;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;

import javax.swing.*;
import java.io.IOException;

import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class DfuHelper {
    private static final String PREFIX = "rusefi_bundle";

    public static void sendDfuRebootCommand(IoStream stream, StringBuilder messages) {
        byte[] command = BinaryProtocol.getTextCommandBytes(Fields.CMD_REBOOT_DFU);
        try {
            stream.sendPacket(command);
            stream.close();
            messages.append(String.format("Reboot command [%s] sent into %s!\n", Fields.CMD_REBOOT_DFU, stream));
        } catch (IOException e) {
            messages.append("Error " + e);
        }
    }

    public static boolean sendDfuRebootCommand(JComponent parent, String signature, IoStream stream, StringBuilder messages) {
        RusEfiSignature s = SignatureHelper.parse(signature);
        String bundleName = Autoupdate.readBundleFullName();
        if (bundleName != null && s != null) {
            String signatureWithPrefix;
            if ("all".equals(s.getBundle())) {
                signatureWithPrefix = PREFIX;
            } else {
                signatureWithPrefix = PREFIX + "_" + s.getBundle();
            }

            if (!bundleName.equalsIgnoreCase(signatureWithPrefix)) {
                String message = String.format("You have \"%s\" controller does not look right to program it with \"%s\"", s.getBundle(), bundleName);
                FileLog.MAIN.logLine(message);

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
