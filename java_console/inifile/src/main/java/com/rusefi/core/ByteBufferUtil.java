package com.rusefi.core;

import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class ByteBufferUtil {
    @NotNull
    public static ByteBuffer littleEndianWrap(byte[] array, int offset, int length) {
        return wrap(array, offset, length, ByteOrder.LITTLE_ENDIAN);
    }

    /** Wrap with the byte order declared by the controller's ini, see IniFileModel#getByteOrder() */
    @NotNull
    public static ByteBuffer wrap(byte[] array, int offset, int length, ByteOrder order) {
        ByteBuffer wrapped = ByteBuffer.wrap(array, offset, length);
        wrapped.order(order);
        return wrapped;
    }
}
