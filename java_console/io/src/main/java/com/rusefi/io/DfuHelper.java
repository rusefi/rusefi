package com.rusefi.io;

import com.rusefi.FileLog;
import com.rusefi.RusEfiSignature;
import com.rusefi.SignatureHelper;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.ui.StatusConsumer;

import javax.swing.*;
import java.io.IOException;
import java.util.concurrent.CountDownLatch;

import static com.rusefi.Timeouts.SECOND;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class DfuHelper {
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
        String bundleName = Autoupdate.readBundleFullName();
        if (bundleName != null && s != null) {
            String signatureWithPrefix;
            if ("all".equals(s.getBundle())) {
                signatureWithPrefix = PREFIX;
            } else {
                signatureWithPrefix = PREFIX + "_" + s.getBundle();
            }

            if (!bundleName.equalsIgnoreCase(signatureWithPrefix)) {
                String message = String.format("Your ECU has firmware reporting as: \"%s\"\nThis console will flash it with firmware: \"%s\"\nThis looks wrong, like you have downloaded the wrong bundle for your hardware.\nDo you want to attempt to flash this firmware anyway?\nWARNING: doing this may render your ECU inoperable. Proceed with caution!", s.getBundle(), bundleName);
                FileLog.MAIN.logLine(message);

                boolean shouldContinue = false;
                CountDownLatch latch = new CountDownLatch(1);

                SwingUtilities.invokeLater(() -> {
                    int result = JOptionPane.showConfirmDialog(parent, message, "Warning", JOptionPane.YES_NO_OPTION, JOptionPane.ERROR_MESSAGE);

                    // Any option other than explicit YES should not continue
                    shouldContinue = result == JOptionPane.YES_OPTION;
                });

                // wait for the user to pick an option
                latch.await();

                // in case of mismatched bundle type we are supposed do close connection
                // and properly handle the case of user hitting "Update Firmware" again
                // closing connection is a mess on Windows so it's simpler to just exit
                if (!shouldContinue) {
                    System.exit(-5);
                }

                return shouldContinue;
            }
        }

        sendDfuRebootCommand(stream, messages);
        return true;
    }
}
