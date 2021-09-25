package com.rusefi.io;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;

import java.io.IOException;

public class DfuHelper {
    public static void sendDfuRebootCommand(IoStream stream, StringBuilder messages) {



        if (1==1)
            return;

        byte[] command = BinaryProtocol.getTextCommandBytes(Fields.CMD_REBOOT_DFU);
        try {
            stream.sendPacket(command);
            stream.close();
            messages.append("Reboot command sent!\n");
        } catch (IOException e) {
            messages.append("Error " + e);
        }
    }
}
