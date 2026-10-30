package com.rusefi.ui.widgets.tune;

import com.rusefi.core.preferences.storage.PersistentConfiguration;

import javax.swing.*;
import java.io.File;

/**
 * Load Tune, Save Tune and the start-up offline load open in the folder of the last tune file
 * loaded or saved, also after a restart.
 */
public class TuneFileFolder {
    private static final String PROPERTY = "tune_file_folder";

    private TuneFileFolder() {
    }

    public static void openIn(JFileChooser chooser) {
        String folder = PersistentConfiguration.getConfig().getRoot().getProperty(PROPERTY, "");
        if (!folder.isEmpty() && new File(folder).isDirectory()) {
            chooser.setCurrentDirectory(new File(folder));
        }
    }

    public static void remember(File tuneFile) {
        File folder = tuneFile.getAbsoluteFile().getParentFile();
        if (folder == null) {
            return;
        }
        PersistentConfiguration.getConfig().getRoot().setProperty(PROPERTY, folder.getPath());
        PersistentConfiguration.getConfig().save();
    }
}
