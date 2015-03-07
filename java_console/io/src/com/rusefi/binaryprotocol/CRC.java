package com.rusefi.binaryprotocol;

import java.util.zip.CRC32;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class CRC {
    public static int crc32(byte buf[], int size) {
        CRC32 c = new CRC32();
        c.update(buf, 0, size);
        return (int) c.getValue();
    }

    public static int crc32(byte[] packet) {
        return crc32(packet, packet.length);
    }
}
