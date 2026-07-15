package com.rusefi.ui.basic;

import com.rusefi.ConnectivityContext;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.jobs.ImportTuneJob;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.io.File;
import java.util.function.Consumer;

public class ImportTuneFileChooser {
    private static final String TUNE_TO_IMPORT_DEFAULT_DIRECTORY_PROPERTY_NAME = "tune_to_import_default_directory";
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final Consumer<String> errorHandler;
    private final JFileChooser tuneToImportFileChooser = createTuneToImportFileChooser();

    public ImportTuneFileChooser(final SingleAsyncJobExecutor singleAsyncJobExecutor, Consumer<String> errorHandler) {
        this.singleAsyncJobExecutor = singleAsyncJobExecutor;
        this.errorHandler = errorHandler;
    }

    public void showFileChooserToImportTuneAction(
        final BinaryProtocol bp,
        final LinkManager lm,
        final JComponent parent,
        final ConnectivityContext connectivityContext
    ) {
        final int selectedOption = tuneToImportFileChooser.showOpenDialog(parent);
        if (selectedOption == JFileChooser.APPROVE_OPTION) {
            final File selectedFile = tuneToImportFileChooser.getSelectedFile();
            saveTuneToImportDefaultDirectory(selectedFile.getParent());
            ImportTuneJob.importTuneIntoDeviceViaLiveConnection(bp, lm, parent, connectivityContext,
                selectedFile.getAbsolutePath(), singleAsyncJobExecutor, errorHandler);
        }
    }

    public void showFileChooserToImportTuneAction(
        final LinkManager lm,
        final JComponent parent,
        final ConnectivityContext connectivityContext
    ) {
        final int selectedOption = tuneToImportFileChooser.showOpenDialog(parent);
        if (selectedOption == JFileChooser.APPROVE_OPTION) {
            final File selectedFile = tuneToImportFileChooser.getSelectedFile();
            saveTuneToImportDefaultDirectory(selectedFile.getParent());
            ImportTuneJob.importTuneIntoDeviceViaLiveConnection(lm, parent, connectivityContext,
                selectedFile.getAbsolutePath(), singleAsyncJobExecutor, errorHandler);
        }
    }

    private static JFileChooser createTuneToImportFileChooser() {
        final JFileChooser fc = new JFileChooser();
        fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
        fc.setFileFilter(new FileNameExtensionFilter("Tune files (.msq)", "msq"));

        final String currentDirectory = loadTuneToImportDefaultDirectory();
        if (currentDirectory != null) {
            fc.setCurrentDirectory(new File(currentDirectory));
        }

        return fc;
    }

    private static void saveTuneToImportDefaultDirectory(final String path) {
        PersistentConfiguration.getConfig().getRoot().setProperty(
            TUNE_TO_IMPORT_DEFAULT_DIRECTORY_PROPERTY_NAME,
            path
        );
        PersistentConfiguration.getConfig().save();
    }

    private static String loadTuneToImportDefaultDirectory() {
        return PersistentConfiguration.getConfig().getRoot().getProperty(
            TUNE_TO_IMPORT_DEFAULT_DIRECTORY_PROPERTY_NAME,
            ""
        );
    }
}
