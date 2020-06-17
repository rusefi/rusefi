package com.rusefi.ui.logview;

import java.io.File;

/**
 * 4/7/14
 * Andrey Belomutskiy, (c) 2013-2020
 */
class FileItem {
    private final File file;

    public FileItem(File file) {
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
