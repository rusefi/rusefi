package com.rusefi;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class ConfigurationImage {
    private final static String BIN_HEADER = "RUSEFI0.1";
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

    public void saveToFile(String fileName) throws IOException {
        FileOutputStream fos = new FileOutputStream(fileName);
        byte[] bytes = BIN_HEADER.getBytes();
        if (bytes.length != BIN_HEADER.length())
            throw new IllegalStateException("Encoding issue");
        fos.write(bytes);
        fos.write(content);
        fos.close();
        System.out.println("Saved to " + fileName);
    }

    public boolean readFromFile(String fileName) throws IOException {
        FileInputStream fis = new FileInputStream(fileName);
        byte[] header = new byte[BIN_HEADER.length()];
        int result = fis.read(header);
        if (result != header.length)
            return false;
        if (!Arrays.equals(header, BIN_HEADER.getBytes()))
            return false;
        result = fis.read(content);
        return result == content.length;
    }

    public byte[] getContent() {
        return content;
    }
}
