package com.opensr5.io;

import com.opensr5.ConfigurationImage;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.util.Arrays;

/**
 * Utility class to read/write {@link ConfigurationImage} into a file
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 6/20/2015.
 */
public class ConfigurationImageFile {
    private ConfigurationImageFile() {
    }

    public static ConfigurationImage readFromFile(String fileName) throws IOException {
        File file = new File(fileName);
        int size = (int) file.length();
        FileInputStream fis = new FileInputStream(fileName);

        return readFromStream(size, fis);
    }

    @Nullable
    private static ConfigurationImage readFromStream(int size, FileInputStream fis) throws IOException {
        int contentSize = size - ConfigurationImage.BIN_HEADER.length();

        byte[] header = new byte[ConfigurationImage.BIN_HEADER.length()];
        int result = fis.read(header);
        if (result != header.length) {
            System.err.println("Error reading header bytes, got " + result);
            return null;
        }
        if (!Arrays.equals(header, ConfigurationImage.BIN_HEADER.getBytes())) {
            System.err.println("Header mismatch");
            return null;
        }
        ConfigurationImage image = new ConfigurationImage(contentSize);
        result = fis.read(image.getContent());
        return result == image.getContent().length ? image : null;
    }

    public static byte[] getFileContent(ConfigurationImage configurationImage) {
        try {
            try (ByteArrayOutputStream baos = new ByteArrayOutputStream()) {
                byte[] bytes = ConfigurationImage.BIN_HEADER.getBytes();
                if (bytes.length != ConfigurationImage.BIN_HEADER.length())
                    throw new IllegalStateException("Encoding issue");
                baos.write(bytes);
                baos.write(configurationImage.getContent());
                return baos.toByteArray();
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public static void saveToFile(ConfigurationImage configurationImage, String fileName) throws IOException {
        System.out.printf("Saving " + configurationImage.getSize() + " bytes of configuration into " + fileName);
        FileOutputStream fos = new FileOutputStream(fileName);
        fos.write(getFileContent(configurationImage));
        fos.close();
        System.out.println("Saved to " + fileName);
    }
}
