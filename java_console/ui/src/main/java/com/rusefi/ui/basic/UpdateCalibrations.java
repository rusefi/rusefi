package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.maintenance.jobs.UpdateCalibrationsJob;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.io.File;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class UpdateCalibrations {
    private static final Logging log = getLogging(UpdateCalibrations.class);

    private static final String BINARY_IMAGE_DEFAULT_DIRECTORY_PROPERTY_NAME = "binary_image_default_directory";
    private final SingleAsyncJobExecutor singleAsyncJobExecutor;
    private final JFileChooser calibrationsFileChooser = UpdateCalibrations.createConfigurationImageFileChooser();

    UpdateCalibrations(final SingleAsyncJobExecutor singleAsyncJobExecutor) {
        this.singleAsyncJobExecutor = singleAsyncJobExecutor;
    }

    void updateCalibrationsAction(PortResult port, JComponent parent, ConnectivityContext connectivityContext) {
        final int selectedOption = calibrationsFileChooser.showOpenDialog(parent);
        if (selectedOption == JFileChooser.APPROVE_OPTION) {
            final File selectedFile = calibrationsFileChooser.getSelectedFile();
            UpdateCalibrations.saveBinaryImageDefaultDirectory(selectedFile.getParent());
            try {
                final ConfigurationImageWithMeta calibrationsImage = ConfigurationImageFile.readFromFile(
                    selectedFile.getAbsolutePath()
                );
                singleAsyncJobExecutor.startJob(new UpdateCalibrationsJob(port, calibrationsImage, connectivityContext), parent);
            } catch (final IOException e) {
                final String errorMsg = String.format(
                    "Failed to load calibrations from file %s",
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

    private static JFileChooser createConfigurationImageFileChooser() {
        final JFileChooser fc = new JFileChooser();
        fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
        fc.setFileFilter(new FileNameExtensionFilter("Calibrations files (.zip)", "zip"));

        final String currentDirectory = loadBinaryImageDefaultDirectory();
        if (currentDirectory != null) {
            fc.setCurrentDirectory(new File(currentDirectory));
        }

        return fc;
    }

    private static void saveBinaryImageDefaultDirectory(final String path) {
        PersistentConfiguration.getConfig().getRoot().setProperty(
            BINARY_IMAGE_DEFAULT_DIRECTORY_PROPERTY_NAME,
            path
        );
        PersistentConfiguration.getConfig().save();
    }

    private static String loadBinaryImageDefaultDirectory() {
        return PersistentConfiguration.getConfig().getRoot().getProperty(
            BINARY_IMAGE_DEFAULT_DIRECTORY_PROPERTY_NAME,
            ""
        );
    }
}
