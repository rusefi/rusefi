package com.rusefi.io;

import com.rusefi.binaryprotocol.BinaryProtocol;

import java.io.IOException;

public class DfuHelper {
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
}
