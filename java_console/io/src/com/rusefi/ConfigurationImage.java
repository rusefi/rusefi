package com.rusefi;

import java.io.FileOutputStream;
import java.io.IOException;

/**
 * (c) Andrey Belomutskiy
 * 3/6/2015
 */
public class ConfigurationImage {
    private final static String BIN_HEADER = "RUSEFI0.1";
    private final int size;
    private byte content[];

    public ConfigurationImage(int size) {
        this.size = size;
        content = new byte[size];
    }

    public int getSize() {
        return size;
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

    public byte[] getContent() {
        return content;
    }
}
