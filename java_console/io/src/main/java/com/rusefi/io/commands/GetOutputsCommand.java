package com.rusefi.io.commands;

import java.nio.ByteOrder;

public class GetOutputsCommand {

    public static byte[] createRequest(int offset, int size) {
        return createRequest(offset, size, ByteOrder.LITTLE_ENDIAN);
    }

    public static byte[] createRequest(int offset, int size, ByteOrder order) {
        byte[] packet = new byte[4];
        ByteRange.packOffsetAndSize(offset, size, packet, order);
        return packet;
    }

}
