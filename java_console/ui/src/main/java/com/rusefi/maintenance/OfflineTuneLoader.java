package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.tune.xml.Msq;

import javax.swing.*;
import java.awt.Component;
import java.util.function.Consumer;

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
        return loadTuneFromFile(msqPath, parent, message -> showErrorDialog(parent, message));
    }

    public static Result loadTuneFromFile(String msqPath, Component parent, Consumer<String> errorHandler) {
        Msq msq;
        try {
            msq = Msq.readTune(msqPath);
        } catch (Exception e) {
            log.error("Failed to read MSQ: " + msqPath, e);
            errorHandler.accept("Failed to read tune file:\n" + e.getMessage());
            return null;
        }

        String signature = msq.getVersionInfo() != null ? msq.getVersionInfo().getSignature() : null;
        if (signature == null || signature.isEmpty()) {
            errorHandler.accept("MSQ file does not contain a signature.\nCannot determine which INI file to use.");
            return null;
        }

        IniFileModel ini = tryLoadIni(signature, errorHandler);
        if (ini == null) {
            return null;
        }

        ConfigurationImage image = msq.asImage(ini);
        return new Result(msq, ini, image, signature);
    }

    private static IniFileModel tryLoadIni(String signature, Consumer<String> errorHandler) {
        // RealIniFileProvider handles download, local lookup, and the manual .ini picker
        // (registered via ManualIniFilePicker) as a last resort — which also caches the picked file.
        try {
            IniFileModel ini = new RealIniFileProvider().provide(signature);
            log.info("Resolved INI for signature: " + signature);
            return ini;
        } catch (IniNotFoundException e) {
            // Genuine failure OR the user declined the manual .ini picker — either way the tune can't
            // open, so surface the outcome instead of failing silently (the read/signature errors above
            // already dialog; this makes the INI path consistent). #9730
            log.info("INI resolution failed for " + signature + ": " + e.getMessage());
            errorHandler.accept("No INI file available for this tune (signature: " + signature + ").\n"
                    + "The tune was not opened.");
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
