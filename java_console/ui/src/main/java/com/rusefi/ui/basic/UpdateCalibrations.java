package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.maintenance.jobs.UpdateCalibrationsJob;
import com.rusefi.maintenance.CalibrationsInfo;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.io.File;
import java.io.IOException;
import java.util.Map;
import java.util.Objects;
import com.opensr5.ConfigurationImageWithMeta;

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
                if (port.getCalibrations() == null) {
                    throw new IOException("ECU calibration layout is unavailable");
                }
                final Map<Integer, ConfigurationImageWithMeta> pages =
                    ConfigurationImageFile.readPagesFromFile(selectedFile.getAbsolutePath());
                final String archiveSignature = pages.get(0).getMeta().getEcuSignature();
                final String ecuSignature = port.getCalibrations().getIniFile().getSignature();
                if (!Objects.equals(archiveSignature, ecuSignature)) {
                    throw new IOException(String.format(
                        "Calibration signature `%s` does not match ECU signature `%s`",
                        archiveSignature,
                        ecuSignature
                    ));
                }
                final CalibrationsInfo calibrations = new CalibrationsInfo(
                    port.getCalibrations().getIniFile(),
                    pages,
                    java.util.Collections.emptySet()
                ).withAllPagesToWrite();
                singleAsyncJobExecutor.startJob(new UpdateCalibrationsJob(port, calibrations, connectivityContext), parent);
            } catch (final Exception e) {
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
