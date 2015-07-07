package com.rusefi;

import java.io.*;
import java.util.Arrays;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class ConfigurationImage {
    /**
     * This constant is used
     * 1) as a header while saving configuration to a binary file
     * 2) as RomRaider RomID#internalIdString
     */
    public final static String BIN_HEADER = "RUSEFI0.1";
    private byte content[];

    public ConfigurationImage(int size) {
        content = new byte[size];
    }

    public ConfigurationImage(byte[] content) {
        this.content = content;
    }

    public int getSize() {
        return content.length;
    }

    public byte[] getFileContent() {
        try {
            try (ByteArrayOutputStream baos = new ByteArrayOutputStream()) {
                byte[] bytes = BIN_HEADER.getBytes();
                if (bytes.length != BIN_HEADER.length())
                    throw new IllegalStateException("Encoding issue");
                baos.write(bytes);
                baos.write(content);
                return baos.toByteArray();
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public void saveToFile(String fileName) throws IOException {
        FileOutputStream fos = new FileOutputStream(fileName);
        fos.write(getFileContent());
        fos.close();
        System.out.println("Saved to " + fileName);
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

    @SuppressWarnings("CloneDoesntCallSuperClone")
    @Override
    public ConfigurationImage clone() {
        byte[] copy = content.clone();
        return new ConfigurationImage(copy);
    }

    public byte[] getRange(Integer first, int size) {
        byte[] r = new byte[size];
        System.arraycopy(content, first, r, 0, size);
        return r;
    }
}
