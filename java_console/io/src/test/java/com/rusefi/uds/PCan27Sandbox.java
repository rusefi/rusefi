package com.rusefi.uds;

import com.rusefi.io.can.PCanHelper;
import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANStatus;

public class PCan27Sandbox {
    public static void main(String[] args) {
        PCANBasic can = PCanHelper.create();
        TPCANStatus status = PCanHelper.init(can);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            throw new IllegalStateException("Unhappy PCAN " + status);
        }
        PCanHelper.send(can, 0x6F4, new byte[]{0x18, 0x06, 0x27, 0x11, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF,});
    }
}
