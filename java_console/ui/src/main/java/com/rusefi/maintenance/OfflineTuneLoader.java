package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.tune.xml.Msq;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.Component;
import java.io.File;
import java.io.FileNotFoundException;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Loads a tune from an MSQ file without requiring a live ECU connection.
 * Extracts the signature from the MSQ, fetches the matching INI (with manual
 * fallback), and produces a ConfigurationImage ready for offline editing.
 */
public class OfflineTuneLoader {
    private static final Logging log = getLogging(OfflineTuneLoader.class);

    private OfflineTuneLoader() {}

    public static class Result {
        public final Msq msq;
        public final IniFileModel ini;
        public final ConfigurationImage image;
        public final String signature;

        public Result(Msq msq, IniFileModel ini, ConfigurationImage image, String signature) {
            this.msq = msq;
            this.ini = ini;
            this.image = image;
            this.signature = signature;
        }
    }

    /**
     * Loads an MSQ file, resolves the matching INI, and returns a Result with
     * the parsed MSQ, INI model, ConfigurationImage, and signature.
     *
     * @param msqPath absolute path to the .msq file
     * @param parent  parent component for file chooser / error dialogs (may be null)
     * @return Result containing msq, ini, image, and signature; or null if user cancels
     */
    public static Result loadTuneFromFile(String msqPath, Component parent) {
        Msq msq;
        try {
            msq = Msq.readTune(msqPath);
        } catch (Exception e) {
            log.error("Failed to read MSQ: " + msqPath, e);
            showErrorDialog(parent, "Failed to read tune file:\n" + e.getMessage());
            return null;
        }

        String signature = msq.getVersionInfo() != null ? msq.getVersionInfo().getSignature() : null;
        if (signature == null || signature.isEmpty()) {
            showErrorDialog(parent, "MSQ file does not contain a signature.\nCannot determine which INI file to use.");
            return null;
        }

        IniFileModel ini = tryLoadIni(signature, parent);
        if (ini == null) {
            return null;
        }

        ConfigurationImage image = msq.asImage(ini);
        return new Result(msq, ini, image, signature);
    }

    private static IniFileModel tryLoadIni(String signature, Component parent) {
        // Attempt automatic resolution via RealIniFileProvider (download + local fallback)
        try {
            RealIniFileProvider provider = new RealIniFileProvider();
            IniFileModel ini = provider.provide(signature);
            if (ini != null) {
                log.info("Resolved INI for signature: " + signature);
                return ini;
            }
        } catch (IniNotFoundException e) {
            log.info("Automatic INI resolution failed for " + signature + ": " + e.getMessage());
        } catch (Exception e) {
            log.info("Automatic INI resolution failed for " + signature + ": " + e.getMessage());
        }

        // Manual fallback: let user pick an INI file
        log.info("Attempting manual INI selection for signature: " + signature);
        return promptForIniFile(parent, signature);
    }

    private static IniFileModel promptForIniFile(Component parent, String signature) {
        int choice = JOptionPane.showConfirmDialog(
            parent,
            "Could not automatically find an INI file for signature:\n" + signature + "\n\nWould you like to select an INI file manually?",
            "INI File Not Found",
            JOptionPane.YES_NO_OPTION,
            JOptionPane.WARNING_MESSAGE
        );
        if (choice != JOptionPane.YES_OPTION) {
            return null;
        }

        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("INI files", "ini"));
        if (chooser.showOpenDialog(parent) != JFileChooser.APPROVE_OPTION) {
            return null;
        }

        File iniFile = chooser.getSelectedFile();
        try {
            IniFileModel ini = IniFileReaderUtil.readIniFileChecked(iniFile.getAbsolutePath());
            log.info("Manually loaded INI: " + iniFile.getAbsolutePath());
            return ini;
        } catch (FileNotFoundException e) {
            showErrorDialog(parent, "INI file not found:\n" + iniFile.getAbsolutePath());
            return null;
        } catch (IniParsingException e) {
            showErrorDialog(parent, "Failed to parse INI file:\n" + e.getMessage());
            return null;
        } catch (Exception e) {
            showErrorDialog(parent, "Failed to read INI file:\n" + e.getMessage());
            return null;
        }
    }

    private static void showErrorDialog(Component parent, String message) {
        JOptionPane.showMessageDialog(
            parent,
            message,
            "Error",
            JOptionPane.ERROR_MESSAGE
        );
    }
}
