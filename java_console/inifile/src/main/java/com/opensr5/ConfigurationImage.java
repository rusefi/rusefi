package com.opensr5;

import com.rusefi.shared.FileUtil;
import org.jetbrains.annotations.NotNull;

import java.nio.ByteBuffer;
import java.util.Arrays;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/6/2015
 */
public class ConfigurationImage {
    /**
     * This constant is used
     * 1) as a header while saving configuration to a binary file
     * 2) as RomRaider RomID#internalIdString
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

    public static byte[] extractContent(byte[] rom) {
        if (rom.length < BIN_HEADER.length())
            return null;
        byte[] result = new byte[rom.length - BIN_HEADER.length()];
        System.arraycopy(rom, BIN_HEADER.length(), result, 0, result.length);
        return result;
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
