package com.rusefi.binaryprotocol;

import java.util.zip.CRC32;

/**
 * Utility methods for {@link BinaryProtocol}
 *
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class IoHelper {
    public static int getCrc32(byte[] packet) {
        return getCrc32(packet, 0, packet.length);
    }

    public static int getCrc32(byte[] packet, int offset, int length) {
        CRC32 c = new CRC32();
        c.update(packet, offset, length);
        return (int) c.getValue();
    }

    /**
     * this method adds two bytes for packet size before and four bytes for IoHelper after
     */
    public static byte[] makeCrc32Packet(byte[] command) {
        byte[] packet = new byte[command.length + 6];

        packet[0] = (byte) (command.length / 256);
        packet[1] = (byte) command.length;

        System.arraycopy(command, 0, packet, 2, command.length);
        int crc = getCrc32(command);

        putInt(packet, packet.length - 4, crc);
        return packet;
    }

    public static int swap16(int x) {
        return (((x & 0xFF) << 8) | ((x) >> 8));
    }

    static int swap32(int x) {
        return (((x) >> 24) & 0xff) | (((x) << 8) & 0xff0000) | (((x) >> 8) & 0xff00) | (((x) << 24) & 0xff000000);
    }

    private static void putInt(byte[] packet, int offset, int value) {
        int index = offset + 3;
        for (int i = 0; i < 4; i++) {
            packet[index--] = (byte) value;
            value >>= 8;
        }
    }

    public static void putShort(byte[] packet, int offset, int value) {
        int index = offset + 1;
        for (int i = 0; i < 2; i++) {
            packet[index--] = (byte) value;
            value >>= 8;
        }
    }

    /**
     * @return true if packet has the expected response code
     */
    static boolean checkResponseCode(byte[] response, byte code) {
        return response != null && response.length > 0 && response[0] == code;
    }
}
