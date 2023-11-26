package com.rusefi.core;

import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Minor mess: we also have FileUtils in io
 */
public class FileUtil {
    public static final String RUSEFI_SETTINGS_FOLDER = System.getProperty("user.home") + File.separator + ".rusEFI";

    public static void close(Closeable closeable) {
        if (closeable != null) {
            try {
                closeable.close();
            } catch (IOException ignored) {
                // ignored
            }
        }
    }

    @NotNull
    public static ByteBuffer littleEndianWrap(byte[] array, int offset, int length) {
        ByteBuffer wrapped = ByteBuffer.wrap(array, offset, length);
        wrapped.order(ByteOrder.LITTLE_ENDIAN);
        return wrapped;
    }
}
