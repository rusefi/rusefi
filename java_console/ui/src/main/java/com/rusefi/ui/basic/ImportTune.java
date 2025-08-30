package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.maintenance.jobs.ImportTuneJob;
import com.rusefi.tune.xml.Msq;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.xml.bind.JAXBException;

import java.io.File;

import static com.devexperts.logging.Logging.getLogging;

public class ImportTune {
    private static final Logging log = getLogging(ImportTune.class);
    private static final String TUNE_TO_IMPORT_DEFAULT_DIRECTORY_PROPERTY_NAME = "tune_to_import_default_directory";
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final JFileChooser tuneToImportFileChooser = createTuneToImportFileChooser();

    public ImportTune(final SingleAsyncJobExecutor singleAsyncJobExecutor) {
        this.singleAsyncJobExecutor = singleAsyncJobExecutor;
    }

    void importTuneAction(
        final PortResult port,
        final JComponent parent,
        final ConnectivityContext connectivityContext
    ) {
        final int selectedOption = tuneToImportFileChooser.showOpenDialog(parent);
        if (selectedOption == JFileChooser.APPROVE_OPTION) {
            final File selectedFile = tuneToImportFileChooser.getSelectedFile();
            saveTuneToImportDefaultDirectory(selectedFile.getParent());
            try {
                final Msq tuneToImport = Msq.readTune(selectedFile.getAbsolutePath());
                singleAsyncJobExecutor.startJob(new ImportTuneJob(port, tuneToImport, connectivityContext), parent);
            } catch (JAXBException e) {
                final String errorMsg = String.format(
                    "Failed to load tune to import from file %s",
                    selectedFile.getAbsolutePath()
                );
                log.error(errorMsg, e);
                JOptionPane.showMessageDialog(
                    parent,
                    errorMsg,
                    "Error",
                    JOptionPane.ERROR_MESSAGE
                );
            }
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
