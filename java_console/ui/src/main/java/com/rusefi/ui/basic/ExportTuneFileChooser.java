package com.rusefi.ui.basic;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.jobs.ExportTuneJob;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.io.File;

public class ExportTuneFileChooser {
    private static final String TUNE_TO_EXPORT_DEFAULT_DIRECTORY_PROPERTY_NAME = "tune_to_export_default_directory";
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final JFileChooser tuneToExportFileChooser = createTuneToExportFileChooser();

    public ExportTuneFileChooser(final SingleAsyncJobExecutor singleAsyncJobExecutor) {
        this.singleAsyncJobExecutor = singleAsyncJobExecutor;
    }

    public void showFileChooserToExportTuneAction(
        final BinaryProtocol bp,
        final LinkManager lm,
        final JComponent parent
    ) {
        final int selectedOption = tuneToExportFileChooser.showSaveDialog(parent);
        if (selectedOption == JFileChooser.APPROVE_OPTION) {
            File selectedFile = tuneToExportFileChooser.getSelectedFile();
            String path = selectedFile.getAbsolutePath();
            if (!path.toLowerCase().endsWith(".msq")) {
                path += ".msq";
                selectedFile = new File(path);
            }
            saveTuneToExportDefaultDirectory(selectedFile.getParent());
            String baseName = path.substring(0, path.length() - 4);
            ExportTuneJob.exportTuneViaLiveConnection(bp, lm, parent, baseName, singleAsyncJobExecutor);
        }
    }

    private static JFileChooser createTuneToExportFileChooser() {
        final JFileChooser fc = new JFileChooser();
        fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
        fc.setFileFilter(new FileNameExtensionFilter("Tune files (.msq)", "msq"));

        final String currentDirectory = loadTuneToExportDefaultDirectory();
        if (currentDirectory != null && !currentDirectory.isEmpty()) {
            fc.setCurrentDirectory(new File(currentDirectory));
        }

        return fc;
    }

    private static void saveTuneToExportDefaultDirectory(final String path) {
        PersistentConfiguration.getConfig().getRoot().setProperty(
            TUNE_TO_EXPORT_DEFAULT_DIRECTORY_PROPERTY_NAME,
            path
        );
        PersistentConfiguration.getConfig().save();
    }

    private static String loadTuneToExportDefaultDirectory() {
        return PersistentConfiguration.getConfig().getRoot().getProperty(
            TUNE_TO_EXPORT_DEFAULT_DIRECTORY_PROPERTY_NAME,
            ""
        );
    }
}
