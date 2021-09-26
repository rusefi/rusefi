package com.rusefi.io;

import com.rusefi.RusEfiSignature;
import com.rusefi.SignatureHelper;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;

import javax.swing.*;
import java.io.IOException;

public class DfuHelper {
    public static void sendDfuRebootCommand(IoStream stream, StringBuilder messages) {
        byte[] command = BinaryProtocol.getTextCommandBytes(Fields.CMD_REBOOT_DFU);
        try {
            stream.sendPacket(command);
            stream.close();
            messages.append("Reboot command sent!\n");
        } catch (IOException e) {
            messages.append("Error " + e);
        }
    }

    public static boolean sendDfuRebootCommand(JComponent parent, String signature, IoStream stream, StringBuilder messages) {
        RusEfiSignature s = SignatureHelper.parse(signature);
        String bundleName = Autoupdate.readBundleFullName();
        if (bundleName != null && s != null) {
            if (!bundleName.equalsIgnoreCase(s.getBundle())) {
                JOptionPane.showMessageDialog(parent, String.format("You have \"%s\" controller does not look right to program it with \"%s\"", s.getBundle(), bundleName));
                // in case of mismatched bundle type we are supposed do close connection
                // and properly handle the case of user hitting "Update Firmware" again
                // closing connection is a mess on Windows so it's simpler to just exit
                System.exit(-5);
                return false;
            }
        }

        sendDfuRebootCommand(stream, messages);
        return true;
    }
}
