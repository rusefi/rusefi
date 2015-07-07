package com.rusefi.io;

import com.rusefi.ConfigurationImage;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Arrays;

/**
 * (c) Andrey Belomutskiy
 * 6/20/2015.
 */
public class ConfigurationImageFile {
    public static ConfigurationImage readFromFile(String fileName) throws IOException {
        File file = new File(fileName);
        int size = (int) file.length();

        int contentSize = size - ConfigurationImage.BIN_HEADER.length();

        FileInputStream fis = new FileInputStream(fileName);
        byte[] header = new byte[ConfigurationImage.BIN_HEADER.length()];
        int result = fis.read(header);
        if (result != header.length)
            return null;
        if (!Arrays.equals(header, ConfigurationImage.BIN_HEADER.getBytes()))
            return null;
        ConfigurationImage image = new ConfigurationImage(contentSize);
        result = fis.read(image.getContent());
        return result == image.getContent().length ? image : null;
    }
}
