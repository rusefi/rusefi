package com.rusefi.io;

import com.rusefi.binaryprotocol.BinaryProtocol;

import java.io.IOException;

public class BootloaderCommsHelper {
    static boolean sendBootloaderRebootCommand(IoStream stream, UpdateOperationCallbacks callbacks, String cmd) {
        byte[] command = BinaryProtocol.getTextCommandBytes(cmd);
        try {
            stream.sendPacket(command);
            callbacks.logLine(String.format("Reboot command [%s] sent into %s!\n", cmd, stream));
            return true;
        } catch (IOException e) {
            callbacks.logLine("Error " + e);
            return false;
        }
    }
}
