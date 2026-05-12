package com.rusefi.core;

import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class ByteBufferUtil {
    @NotNull
    public static ByteBuffer littleEndianWrap(byte[] array, int offset, int length) {
        ByteBuffer wrapped = ByteBuffer.wrap(array, offset, length);
        wrapped.order(ByteOrder.LITTLE_ENDIAN);
        return wrapped;
    }
}
