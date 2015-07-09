package com.rusefi.ui.logview;

import java.io.File;

/**
 * 4/7/14
 * (c) Andrey Belomutskiy
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
