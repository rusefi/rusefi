package com.rusefi.io.commands;

public class GetOutputsCommand {
    public static byte[] createRequest(int offset, int size) {
        byte[] packet = new byte[4];
        ByteRange.packOffsetAndSize(offset, size, packet);
        return packet;
    }
}
