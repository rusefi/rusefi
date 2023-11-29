package com.rusefi.io.can;

import org.jetbrains.annotations.NotNull;
import peak.can.basic.*;

import static peak.can.basic.TPCANMessageType.PCAN_MESSAGE_STANDARD;

public class PCanHelper {
    public static final TPCANHandle CHANNEL = TPCANHandle.PCAN_USBBUS1;

    @NotNull
    public static PCANBasic create() {
        PCANBasic can = new PCANBasic();
        can.initializeAPI();
        return can;
    }

    static TPCANStatus init(PCANBasic can) {
        return can.Initialize(CHANNEL, TPCANBaudrate.PCAN_BAUD_500K, TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
    }

    static TPCANStatus send(PCANBasic can, int id, byte[] payLoad) {
        TPCANMsg msg = new TPCANMsg(id, PCAN_MESSAGE_STANDARD.getValue(),
                (byte) payLoad.length, payLoad);
        return can.Write(CHANNEL, msg);
    }
}
