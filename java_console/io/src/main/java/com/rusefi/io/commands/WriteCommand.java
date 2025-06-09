package com.rusefi.io.commands;

import org.jetbrains.annotations.NotNull;

public class WriteCommand {
    public static byte @NotNull [] getWritePacket(byte[] content, int contentOffset, int ecuOffset, int size) {
        byte[] packet = new byte[4 + size];
        ByteRange.packOffsetAndSize(ecuOffset, size, packet);

        System.arraycopy(content, contentOffset, packet, 4, size);
        return packet;
    }
}
