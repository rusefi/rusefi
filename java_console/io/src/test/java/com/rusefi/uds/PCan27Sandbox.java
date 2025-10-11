package com.rusefi.uds;

import com.rusefi.io.can.PCanHelper;
import com.rusefi.io.can.isotp.IsoTpCanDecoder;
import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANStatus;

import java.util.ArrayList;
import java.util.List;

public class PCan27Sandbox {
    public static void main(String[] args) {
        PCANBasic can = PCanHelper.create();
        {
            TPCANStatus status = PCanHelper.init(can);
            if (status != TPCANStatus.PCAN_ERROR_OK) {
                throw new IllegalStateException("Unhappy PCAN " + status);
            }
        }
        PCanHelper.send(can, 0x6F4, new byte[]{0x18, 0x06, 0x27, 0x11, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF,});


        IsoTpCanDecoder decoder = new IsoTpCanDecoder(1, new byte[]{0x18, 0x30, 0, 2});


        List<Byte> list = new ArrayList<>();

        while (true) {

            TPCANMsg rx = new TPCANMsg(Byte.MAX_VALUE);
            TPCANStatus status = can.Read(PCanHelper.CHANNEL, rx, null);


            if (status == TPCANStatus.PCAN_ERROR_OK && rx.getID() == 0x618) {
                PCanHelper.debugPacket(rx);
                byte[] newBytes = decoder.decodePacket(rx.getData());
                for (byte n : newBytes) {
                    list.add(n);
                }
                System.out.println("total " + list);
            }

        }

    }
}
