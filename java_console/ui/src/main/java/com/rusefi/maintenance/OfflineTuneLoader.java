package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.Component;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Loads a tune from an MSQ file without requiring a live ECU connection.
 * Extracts the signature from the MSQ, fetches the matching INI (with manual
 * fallback), and produces a ConfigurationImage ready for offline editing.
 */
public class OfflineTuneLoader {
    private static final Logging log = getLogging(OfflineTuneLoader.class);
    private static final String SOURCE_INI_GUIDANCE =
        "Select the .ini file used to save this .msq tune (for example, the project's mainController.ini), "
            + "not the destination ECU's .ini.";

    private OfflineTuneLoader() {}

    interface CustomIniPicker {
        IniFileModel pick(String signature);
    }

    interface SourceIniProvider {
        IniFileModel provide(String signature) throws IniNotFoundException;
    }

    private static class IniResolution {
        final IniFileModel ini;
        final boolean selectedByUser;

        IniResolution(IniFileModel ini, boolean selectedByUser) {
            this.ini = ini;
            this.selectedByUser = selectedByUser;
        }
    }

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
        return loadTuneFromFile(msqPath, parent, errorHandler, signature -> pickCustomIni(parent, signature), true);
    }

    /**
     * Loads the MSQ and its standard source INI for a connected tune merge. No
     * ConfigurationImage is needed because values are migrated into the ECU's
     * target image using the source and target INIs.
     */
    public static Result loadConnectedTuneFromFile(String msqPath, Consumer<String> errorHandler) {
        return loadTuneFromFile(msqPath, null, errorHandler, signature -> pickCustomIni(null, signature), false);
    }

    /**
     * Connected imports can use the already-loaded destination schema when the MSQ was saved
     * against that exact signature. Different signatures still need the genuine source INI for
     * version-specific migration.
     */
    public static Result loadConnectedTuneFromFile(String msqPath, IniFileModel destinationIni,
                                                   Consumer<String> errorHandler) {
        return loadConnectedTuneFromFile(msqPath, destinationIni, errorHandler,
            signature -> pickCustomIni(null, signature),
            signature -> new RealIniFileProvider(false).provide(signature));
    }

    static Result loadConnectedTuneFromFile(String msqPath, IniFileModel destinationIni,
                                            Consumer<String> errorHandler,
                                            CustomIniPicker customIniPicker,
                                            SourceIniProvider sourceIniProvider) {
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
        if (destinationIni != null && signature.equals(destinationIni.getSignature())) {
            return loadConnectedTune(msq, signature, null);
        }
        return loadResolvedSourceIni(msq, signature, errorHandler, customIniPicker, false, sourceIniProvider);
    }

    static Result loadTuneFromFile(String msqPath, Component parent, Consumer<String> errorHandler,
                                   CustomIniPicker customIniPicker, boolean requireImage) {
        return loadTuneFromFile(msqPath, parent, errorHandler, customIniPicker, requireImage,
            signature -> new RealIniFileProvider(false).provide(signature));
    }

    static Result loadTuneFromFile(String msqPath, Component parent, Consumer<String> errorHandler,
                                   CustomIniPicker customIniPicker, boolean requireImage,
                                   SourceIniProvider sourceIniProvider) {
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

        return loadResolvedSourceIni(msq, signature, errorHandler, customIniPicker, requireImage,
            sourceIniProvider);
    }

    static Result loadResolvedSourceIni(Msq msq, String signature, Consumer<String> errorHandler,
                                        CustomIniPicker customIniPicker, boolean requireImage,
                                        SourceIniProvider sourceIniProvider) {
        IniResolution resolution = tryLoadIni(msq, signature, errorHandler, customIniPicker, sourceIniProvider);
        if (resolution == null) {
            return null;
        }
        if (!requireImage) {
            return loadConnectedTune(msq, signature, resolution.ini);
        }
        return loadResolvedTune(msq, signature, resolution.ini, errorHandler,
            resolution.selectedByUser ? ignored -> null : customIniPicker);
    }

    static Result loadConnectedTune(Msq msq, String signature, IniFileModel ini) {
        return new Result(msq, ini, null, signature);
    }

    static Result loadResolvedTune(Msq msq, String signature, IniFileModel ini,
                                   Consumer<String> errorHandler, CustomIniPicker customIniPicker) {
        try {
            ConfigurationImage image = msq.asImage(ini);
            return new Result(msq, ini, image, signature);
        } catch (RuntimeException conversionError) {
            log.info("Tune does not convert with automatically resolved INI: " + conversionError.getMessage());
            IniFileModel customIni = customIniPicker.pick(signature);
            if (customIni == null) {
                errorHandler.accept(conversionFailureMessage(signature, conversionError));
                return null;
            }
            if (!signature.equals(customIni.getSignature())) {
                errorHandler.accept("The selected INI does not match this tune.\nExpected signature: " + signature
                        + "\nSelected signature: " + customIni.getSignature()
                        + "\n" + SOURCE_INI_GUIDANCE + " The tune was not opened.");
                return null;
            }
            String missingField = firstMissingTuneField(msq, customIni);
            if (missingField != null) {
                errorHandler.accept("The selected INI is missing tune field " + missingField + ".\n"
                        + SOURCE_INI_GUIDANCE + " The tune was not opened.");
                return null;
            }
            try {
                ConfigurationImage image = msq.asImage(customIni);
                return new Result(msq, customIni, image, signature);
            } catch (RuntimeException customConversionError) {
                log.error("Tune does not convert with selected custom INI", customConversionError);
                errorHandler.accept(conversionFailureMessage(signature, customConversionError));
                return null;
            }
        }
    }

    private static String firstMissingTuneField(Msq msq, IniFileModel ini) {
        Page page = msq.findPage();
        if (page == null) {
            return null;
        }
        for (Constant constant : page.constant) {
            String name = constant.getName();
            // These are storage padding entries, not tune fields; Msq.asImage also ignores them.
            if (!name.startsWith("UNALLOCATED_SPACE") && !ini.getAllIniFields().containsKey(name)) {
                return name;
            }
        }
        return null;
    }

    private static String conversionFailureMessage(String signature, RuntimeException error) {
        return "Could not convert the tune using an INI with signature " + signature + ":\n"
                + error.getMessage() + "\n" + SOURCE_INI_GUIDANCE + " The tune was not opened.";
    }

    private static IniFileModel pickCustomIni(Component parent, String signature) {
        AtomicReference<IniFileModel> result = new AtomicReference<>();
        Runnable choose = () -> {
            JFileChooser chooser = new JFileChooser();
            chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
            chooser.setFileFilter(new FileNameExtensionFilter("INI files", "ini"));
            chooser.setDialogTitle("Select the source INI used for this MSQ tune");
            chooser.setAccessory(new JLabel("<html>Select the .ini file used to save this .msq tune "
                + "(for example, the project's mainController.ini),<br>"
                + "not the destination ECU's .ini.</html>"));
            if (chooser.showOpenDialog(parent) != JFileChooser.APPROVE_OPTION) {
                return;
            }
            result.set(readCustomIni(chooser.getSelectedFile(), message -> showErrorDialog(parent, message)));
        };
        try {
            if (SwingUtilities.isEventDispatchThread()) {
                choose.run();
            } else {
                SwingUtilities.invokeAndWait(choose);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            log.error("Custom INI selection interrupted", e);
        } catch (java.lang.reflect.InvocationTargetException e) {
            log.error("Custom INI selection failed", e);
        }
        return result.get();
    }

    static IniFileModel readCustomIni(File file, Consumer<String> errorHandler) {
        try {
            return IniFileReaderUtil.readIniFileChecked(file.getAbsolutePath());
        } catch (FileNotFoundException | IniParsingException | RuntimeException e) {
            errorHandler.accept("Could not read selected INI:\n" + e.getMessage());
            return null;
        }
    }

    private static IniResolution tryLoadIni(Msq msq, String signature, Consumer<String> errorHandler,
                                           CustomIniPicker customIniPicker,
                                           SourceIniProvider sourceIniProvider) {
        try {
            IniFileModel ini = sourceIniProvider.provide(signature);
            log.info("Resolved INI for signature: " + signature);
            return new IniResolution(ini, false);
        } catch (IniNotFoundException e) {
            log.info("INI resolution failed for " + signature + ": " + e.getMessage());
            IniFileModel selectedIni = customIniPicker.pick(signature);
            if (selectedIni == null) {
                errorHandler.accept("No INI file available for this tune (signature: " + signature + ").\n"
                    + e.getMessage() + "\n" + SOURCE_INI_GUIDANCE + " The tune was not opened.");
                return null;
            }
            if (!signature.equals(selectedIni.getSignature())) {
                errorHandler.accept("The selected INI does not match this tune.\nExpected signature: " + signature
                    + "\nSelected signature: " + selectedIni.getSignature()
                    + "\n" + SOURCE_INI_GUIDANCE + " The tune was not opened.");
                return null;
            }
            String missingField = firstMissingTuneField(msq, selectedIni);
            if (missingField != null) {
                errorHandler.accept("The selected INI is missing tune field " + missingField + ".\n"
                    + SOURCE_INI_GUIDANCE + " The tune was not opened.");
                return null;
            }
            return new IniResolution(selectedIni, true);
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
