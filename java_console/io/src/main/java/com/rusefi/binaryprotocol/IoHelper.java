package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.rusefi.io.IoStream;

import java.util.zip.CRC32;

/**
 * Utility methods for {@link BinaryProtocol}
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/6/2015
 */
public class IoHelper {
    private static final Logging log = Logging.getLogging(IoHelper.class);

    static {
        log.configureDebugEnabled(false);
    }

    public static int getCrc32(byte[] packet) {
        return getCrc32(packet, packet.length);
    }

    private static int getCrc32(byte[] packet, int length) {
        CRC32 c = new CRC32();
        c.update(packet, 0, length);
        return (int) c.getValue();
    }

    /**
     * this method adds two bytes for packet size before and four bytes for IoHelper after
     */
    public static byte[] makeCrc32Packet(byte[] command) {
        if (log.debugEnabled())
            log.info("makeCrc32Packet: raw packet " + IoStream.printByteArray(command));
        byte[] packet = new byte[command.length + 6];

        putShort(packet, 0, command.length);

        System.arraycopy(command, 0, packet, 2, command.length);
        int crc = getCrc32(command);

        if (log.debugEnabled())
            log.info(String.format("makeCrc32Packet: CRC 0x%08X", crc));
        putInt(packet, packet.length - 4, crc);
        return packet;
    }

    public static int swap16(int x) {
        return (((x & 0xff) << 8) | ((x >> 8) & 0xff));
    }

    public static int swap32(int x) {
        return (((x) >> 24) & 0xff) | (((x) << 8) & 0xff0000) | (((x) >> 8) & 0xff00) | (((x) << 24) & 0xff000000);
    }

    public static void putInt(byte[] packet, int offset, int value) {
        packet[offset + 3] = (byte) value;
        packet[offset + 2] = (byte) (value >> 8);
        packet[offset + 1] = (byte) (value >> 16);
        packet[offset] = (byte) (value >> 24);
    }

    public static void putShort(byte[] packet, int offset, int value) {
        packet[offset + 1] = (byte) value;
        packet[offset] = (byte) (value >> 8);
    }

    /**
     * @return true if packet has the expected response code
     */
    public static boolean checkResponseCode(byte[] response, byte code) {
        return response != null && response.length > 0 && response[0] == code;
    }

    public static int getInt(byte firstByte, byte secondByte) {
        return firstByte * 256 + (secondByte & 0xFF);
    }
}
