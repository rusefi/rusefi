package com.rusefi;

import java.io.File;

public class IoUtil3 {
    public static String prependIfNotAbsolute(String prefix, String fileName) {
        if (new File(fileName).isAbsolute()) {
            return fileName;
        } else {
            return prefix + fileName;
        }
    }
}
