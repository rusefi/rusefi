package com.rusefi.io.commands;

import org.jetbrains.annotations.NotNull;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

import static com.rusefi.binaryprotocol.IoHelper.putShort;
import static com.rusefi.binaryprotocol.IoHelper.swap16;

public class ByteRange {
    private final int offset;
    private final int count;

    private ByteRange(int offset, int count) {
        this.offset = offset;
        this.count = count;
    }

    @NotNull
    private static DataInputStream createPayLoadStream(byte[] payload) {
        return new DataInputStream(new ByteArrayInputStream(payload, 1, payload.length - 1));
    }

    public static void packOffsetAndSize(int offset, int requestSize, byte[] packet) {
        putShort(packet, 0, swap16(offset));
        putShort(packet, 2, swap16(requestSize));
    }

    public int getOffset() {
        return offset;
    }

    public int getCount() {
        return count;
    }

    @Override
    public String toString() {
        return "{" +
                "offset=" + offset +
                ", count=" + count +
                '}';
    }

    public static ByteRange valueOf(byte[] payload) throws IOException {
        try (DataInputStream dis = createPayLoadStream(payload)) {
            int offset = swap16(dis.readShort());
            int count = swap16(dis.readShort());
            return new ByteRange(offset, count);
        }
    }
}
