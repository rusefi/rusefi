package com.rusefi.uds;

import com.rusefi.io.can.PCanHelper;
import com.rusefi.io.can.isotp.IsoTpCanDecoder;
import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANStatus;
import peak.can.basic.TPCANTimestamp;

import java.util.ArrayList;
import java.util.List;

public class PCan27Sandbox {

    static TPCANTimestamp tb = new TPCANTimestamp();

    static {
        tb.setMillis(1000);
    }

    public static void main(String[] args) {
        PCANBasic can = PCanHelper.create();
        {
            TPCANStatus status = PCanHelper.init(can);
            if (status != TPCANStatus.PCAN_ERROR_OK) {
                throw new IllegalStateException("Unhappy PCAN " + status);
            }
        }

        IsoTpCanDecoder decoder = new IsoTpCanDecoder(1, new byte[]{0x18, 0x30, 0, 2});

        List<Byte> list = new ArrayList<>();

//        ReadDataById
//        PCanHelper.send(can, 0x6F4, new byte[]{0x18, 0x03, 0x22, (byte) 0xF1, (byte) 0x01});

        PCanHelper.send(can, 0x6F4, new byte[]{0x18, 0x02, 0x10, (byte) 0x03});

        extracted(can, decoder, list);


        PCanHelper.send(can, 0x6F4, new byte[]{0x18, 0x06, 0x27, 0x11, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF,});


        extracted(can, decoder, list);

    }

    private static void extracted(PCANBasic can, IsoTpCanDecoder decoder, List<Byte> list) {
        while (true) {

            TPCANMsg rx = new TPCANMsg(Byte.MAX_VALUE);
            TPCANStatus status = can.Read(PCanHelper.CHANNEL, rx, tb);

            if (status != TPCANStatus.PCAN_ERROR_OK) {
                //System.out.println("Unhappy " + rx);
                continue;
            }


            if (rx.getID() == 0x396 ||
                rx.getID() == 0x39A ||
                rx.getID() == 0x3FD ||
                rx.getID() == 0x518 ||
                rx.getID() == 0x598) {
                // boring
                continue;

            }


            if (rx.getID() == 0x618) {
                PCanHelper.debugPacket(rx);
                byte[] newBytes = decoder.decodePacket(rx.getData());
                for (byte n : newBytes) {
                    list.add(n);
                }
                System.out.println("total " + list);
            } else {
                System.out.println("Got " + Integer.toString(rx.getID(), 16));
            }

        }
    }
}
