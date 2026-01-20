package com.opensr5;

import com.rusefi.core.ByteBufferUtil;
import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;
import java.util.Objects;

/**
 * mutable presentation of ECU calibrations
 * in the MCU firmware that's config/engineConfiguration
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/6/2015
 */
public class ConfigurationImage {
    private final byte[] content;

    public ConfigurationImage(int size) {
        content = new byte[size];
    }

    public ConfigurationImage(byte[] content) {
        this.content = Objects.requireNonNull(content);
    }

    @NotNull
    public ByteBuffer getByteBuffer(int offset, int size) {
        return ByteBufferUtil.littleEndianWrap(content, offset, size);
    }

    public boolean isEmpty() {
        return content.length == 0;
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
