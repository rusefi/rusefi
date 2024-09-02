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
       final ConfigurationImage image = new ConfigurationImage(size);
        final int result = fis.read(image.getContent());
        return result == image.getContent().length ? image : null;
    }

    public static byte[] getFileContent(ConfigurationImage configurationImage) {
        try {
            try (ByteArrayOutputStream baos = new ByteArrayOutputStream()) {
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
