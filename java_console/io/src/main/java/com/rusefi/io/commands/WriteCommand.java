package com.rusefi.io.commands;

import org.jetbrains.annotations.NotNull;

public class WriteCommand {
    private static final int HEADER_SIZE = 6;
    public static byte @NotNull [] getWritePacket(byte[] content, int contentOffset, int ecuOffset, int size) {
        byte[] packet = new byte[HEADER_SIZE + size];
        ByteRange.packPageOffsetAndSize(ecuOffset, size, packet);

        System.arraycopy(content, contentOffset, packet, HEADER_SIZE, size);
        return packet;
    }
}
