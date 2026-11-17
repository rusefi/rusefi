package com.rusefi.io.commands;

import org.jetbrains.annotations.NotNull;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.nio.ByteOrder;

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
        packOffsetAndSize(offset, requestSize, packet, ByteOrder.LITTLE_ENDIAN);
    }

    public static void packOffsetAndSize(int offset, int requestSize, byte[] packet, ByteOrder order) {
        putShort(packet, 0, toWire16(offset, order));
        putShort(packet, 2, toWire16(requestSize, order));
    }

    public static void packPageOffsetAndSize(int offset, int requestSize, byte[] packet) {
        packPageOffsetAndSize(0, offset, requestSize, packet);
    }

    public static void packPageOffsetAndSize(int page, int offset, int requestSize, byte[] packet) {
        packPageOffsetAndSize(page, offset, requestSize, packet, ByteOrder.LITTLE_ENDIAN);
    }

    public static void packPageOffsetAndSize(int page, int offset, int requestSize, byte[] packet, ByteOrder order) {
        putShort(packet, 0, toWire16(page, order));
        putShort(packet, 2, toWire16(offset, order));
        putShort(packet, 4, toWire16(requestSize, order));
    }

    /**
     * Command arguments (page/offset/count) follow the .ini's declared {@code endianness}.
     * {@link com.rusefi.binaryprotocol.IoHelper#putShort} writes big-endian, so little-endian
     * targets (rusEFI) pre-swap while a big-endian target (e.g. PowerPC) writes as-is.
     */
    public static int toWire16(int value, ByteOrder order) {
        return order == ByteOrder.BIG_ENDIAN ? value : swap16(value);
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

    // valueOf/valueOf2 parse requests on the rusEFI-emulating server side (TCP proxy/simulator),
    // which is always little-endian; they intentionally do not take a ByteOrder.
    public static ByteRange valueOf(byte[] payload) throws IOException {
        try (DataInputStream dis = createPayLoadStream(payload)) {
            int offset = swap16(dis.readShort());
            int count = swap16(dis.readShort());
            return new ByteRange(offset, count);
        }
    }

    public static ByteRange valueOf2(byte[] payload) throws IOException {
        try (DataInputStream dis = createPayLoadStream(payload)) {
            /* page */dis.readShort();
            int offset = swap16(dis.readShort());
            int count = swap16(dis.readShort());
            return new ByteRange(offset, count);
        }
    }
}
