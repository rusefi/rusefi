package com.rusefi.uds;

import com.devexperts.logging.Logging;
import com.rusefi.io.can.SocketCanConnector;
import com.rusefi.util.HexBinary;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static com.devexperts.logging.Logging.getLogging;

public class M74_9_UdsSandbox {
    public static final byte RND = (byte) 0xA1;
    private static final Logging log = getLogging(M74_9_UdsSandbox.class);

    private static final int UDS_OUT = 0x7E0;
    private static final int OX27 = 0x27;
    private static final int OX67 = 0x67;

    public static void main(String[] args) {
        boolean useSocketCan = args.length > 0 && args[0].toLowerCase().contains("socket");

        CanConnector connector;
        if (useSocketCan) {
            connector = SocketCanConnector.create();
        } else {
            connector = PCanConnector.createPCanConnector();
        }
        startThread(connector);
        // hello
        connector.send(UDS_OUT, new byte[]{0x02, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00});
    }

    private static void startThread(CanConnector connector) {
        Runnable r = () -> {
            while (true) {
                CanConnector.CanPacket packet = connector.read();

                if (packet != null) {

                    if (packet.id() != CanConnector.UDS_IN) {
//                        System.out.println("Wrong ID skipping");
                        continue;
                    }
//                    PCanHelper.debugPacket(rx);

                    byte[] data = packet.payload();
                    if (isProgramAck(data)) {
                        log.info("Program Request ACK");
                        // technically this is IsoTP
                        connector.send(UDS_OUT, new byte[]{0x03, OX27, 0x01, RND, 0x00, 0x00, 0x00, 0x00});


                    } else if (isSeed(data)) {

                        ByteBuffer byteBuffer = ByteBuffer.wrap(data, 3, 4);
                        byteBuffer.order(ByteOrder.BIG_ENDIAN);
                        int seed = byteBuffer.getInt();
                        log.info(String.format("SECURITY_ACCESS REQUEST_SEED %x from %s", seed, HexBinary.printByteArray(data)));

                        int key = M74_9_SeedKeyCalculator.Uds_Security_CalcKey(M74_9_SeedKeyCalculator.BOOTLOADER_SECRET, seed, RND);

                        // technically this is IsoTP
                        byte[] keyResponse = new byte[]{0x06, OX27, 0x02, 0, 0, 0, 0, 0x00};
                        ByteBuffer responseBuffer = ByteBuffer.wrap(keyResponse, 3, 4);
                        responseBuffer.putInt(key);

                        log.info(String.format("SECURITY_ACCESS SEND_KEY %x from %s", key, HexBinary.printByteArray(keyResponse)));
                        connector.send(UDS_OUT, keyResponse);

                    } else if (startsWith(data, new byte[]{0x03, 0x7F, OX27, 0x35})) {
                        log.error("UNHAPPY :(");
                        System.exit(-1);
                    } else if (startsWith(data, new byte[]{0x02, OX67, 0x02})) {
                        log.error("HAPPILY UNLOCKED :)");
                        System.exit(0);
                    }
                }
            }
        };
        Thread t = new Thread(r);
//        t.setDaemon(true);
        t.start();
    }

    private static boolean isSeed(byte[] data) {
        return startsWith(data, new byte[]{0x06, OX67, 0x01});
    }

    private static boolean isProgramAck(byte[] data) {
        return startsWith(data, new byte[]{0x06, 0x50, 0x02});
    }

    private static boolean startsWith(byte[] data, byte[] bytes) {
        if (data.length < bytes.length)
            return false;
        for (int i = 0; i < bytes.length; i++) {
            if (data[i] != bytes[i]) {
                return false;
            }

        }
        return true;
    }
}
