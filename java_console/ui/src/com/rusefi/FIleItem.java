package com.rusefi;

import java.io.File;

/**
 * 4/7/14
 * (c) Andrey Belomutskiy
 */
public class FIleItem {
    private final File file;

    public FIleItem(File file) {
        this.file = file;
    }

    public File getFile() {
        return file;
    }

    @Override
    public String toString() {
        return file.getName() + " " + file.length();
    }
}
