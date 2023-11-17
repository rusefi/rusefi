package com.rusefi.io;

import com.devexperts.logging.Logging;
import com.rusefi.binaryprotocol.BinaryProtocol;

import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

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
}
