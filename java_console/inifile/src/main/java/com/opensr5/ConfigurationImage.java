package com.opensr5;

import com.rusefi.core.FileUtil;
import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/6/2015
 */
public class ConfigurationImage {
    /**
     * This constant is used as a header while saving configuration to a binary file
     */
    public final static String BIN_HEADER = "OPEN_SR5_0.1";
    private final byte[] content;

    public ConfigurationImage(int size) {
        content = new byte[size];
    }

    public ConfigurationImage(byte[] content) {
        this.content = content;
    }

    @NotNull
    public ByteBuffer getByteBuffer(int offset, int size) {
        return FileUtil.littleEndianWrap(content, offset, size);
    }

    public int getSize() {
        return content.length;
    }

    public byte[] getContent() {
        return content;
    }

    @SuppressWarnings("MethodDoesntCallSuperMethod")
    @Override
    public ConfigurationImage clone() {
        byte[] copy = content.clone();
        return new ConfigurationImage(copy);
    }

    /**
     * @return a COPY of the specified range
     */
    public byte[] getRange(int offset, int size) {
        byte[] r = new byte[size];
        System.arraycopy(content, offset, r, 0, size);
        return r;
    }

    @Override
    public String toString() {
        return "ConfigurationImage{" +
                "size=" + content.length +
                '}';
    }
}
