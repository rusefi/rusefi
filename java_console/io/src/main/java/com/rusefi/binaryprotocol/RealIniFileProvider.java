package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.opensr5.ini.IniLocator;
import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.Pair;
import com.rusefi.ui.StatusConsumer;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.concurrent.atomic.AtomicLong;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.BooleanSupplier;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

public class RealIniFileProvider implements IniFileProvider {
    private static final Logging log = getLogging(RealIniFileProvider.class);

    /**
     * Last-resort manual .ini source: when download and local lookups all fail, the UI registers
     * a picker here so the user can point at a local .ini. Kept as an interface so this (io) module
     * stays Swing-free. See {@link com.rusefi.core.SignatureHelper#importIntoCache}.
     */
    public interface ManualIniPicker {
        void pick(String signature);

        default void searching(String signature, String port, long generation) {
        }

        default void searchFinished(String signature, String port, long generation) {
        }

        default void progress(String signature, String port, long generation,
                              IniLocator.LookupProgress progress) {
            if (progress.stage == IniLocator.LookupStage.DRIVE_ROOTS
                    && progress.totalRoots < 0 && progress.completedRoots == 0) {
                searching(signature, port, generation);
            }
        }

        default void lookupFinished(String signature, String port, long generation) {
            searchFinished(signature, port, generation);
        }

        default void resolved(String signature, String port, long generation) {
        }

        default void pick(String signature, String port, long generation) {
            pick(signature);
        }
    }

    public static ManualIniPicker manualPicker = null;

    interface IniDownloader {
        String findOrDownload(String signature, Pair<String, String> location, boolean allowDownload);
    }

    interface RootIniLookup {
        IniLocator.RootSearchResult find(String signature, Consumer<IniLocator.LookupProgress> progress);
    }

    interface TunerStudioIniLookup {
        String find(String signature);
    }

    interface IniImporter {
        String importIni(String signature, byte[] source) throws IOException;
    }

    private final IniDownloader iniDownloader;
    private final boolean manualPickerEnabled;
    private final RootIniLookup rootIniLookup;
    private final IniImporter iniImporter;
    private final TunerStudioIniLookup tunerStudioIniLookup;

    public RealIniFileProvider() {
        this((signature, location, allowDownload) ->
                SignatureHelper.downloadMatchingIniIfNotAvailable(location, allowDownload, signature), true,
            IniLocator::findMatchingIniOnWindows, SignatureHelper::importIntoCache);
    }

    public RealIniFileProvider(boolean manualPickerEnabled) {
        this((signature, location, allowDownload) ->
                SignatureHelper.downloadMatchingIniIfNotAvailable(location, allowDownload, signature),
            manualPickerEnabled,
            IniLocator::findMatchingIniOnWindows, SignatureHelper::importIntoCache);
    }

    RealIniFileProvider(IniDownloader iniDownloader) {
        this(iniDownloader, true, IniLocator::findMatchingIniOnWindows, SignatureHelper::importIntoCache);
    }

    RealIniFileProvider(IniDownloader iniDownloader, boolean manualPickerEnabled) {
        this(iniDownloader, manualPickerEnabled,
            IniLocator::findMatchingIniOnWindows, SignatureHelper::importIntoCache);
    }

    RealIniFileProvider(IniDownloader iniDownloader, boolean manualPickerEnabled,
                        RootIniLookup rootIniLookup, IniImporter iniImporter) {
        this(iniDownloader, manualPickerEnabled, rootIniLookup, iniImporter,
            RealIniFileProvider::importFromTunerStudioCache);
    }

    RealIniFileProvider(IniDownloader iniDownloader, boolean manualPickerEnabled,
                        RootIniLookup rootIniLookup, IniImporter iniImporter,
                        TunerStudioIniLookup tunerStudioIniLookup) {
        this.iniDownloader = iniDownloader;
        this.manualPickerEnabled = manualPickerEnabled;
        this.rootIniLookup = rootIniLookup;
        this.iniImporter = iniImporter;
        this.tunerStudioIniLookup = tunerStudioIniLookup;
    }

    /** A cancelled fallback stays dismissed for this signature/port pair (#9774 follow-up). */
    private static final Set<String> promptedSignatures = ConcurrentHashMap.newKeySet();
    private static final Set<String> reportedMissingSignatures = ConcurrentHashMap.newKeySet();

    public static void clearMissingIniStateForTests() {
        promptedSignatures.clear();
        reportedMissingSignatures.clear();
    }

    public static void allowManualPrompt(String signature) {
        allowManualPrompt(signature, null);
    }

    public static void allowManualPrompt(String signature, String port) {
        promptedSignatures.remove(promptKey(signature, port));
    }

    private StatusConsumer statusConsumer = StatusConsumer.ANONYMOUS;

    public void setStatusConsumer(StatusConsumer statusConsumer) {
        this.statusConsumer = statusConsumer;
    }

    /**
     * @return .ini copied from TunerStudio's cache into ours (so the next connect is a plain cache
     * hit), the TunerStudio file itself if the copy failed, or null if TunerStudio does not have it
     */
    @Nullable
    private static String importFromTunerStudioCache(String signature) {
        String tsIniFile = PrimeTunerStudioCache.findInTunerStudioCache(signature);
        if (tsIniFile == null) {
            return null;
        }
        try {
            String imported = SignatureHelper.importIntoCache(signature, new File(tsIniFile));
            if (imported != null) {
                return imported;
            }
        } catch (IOException e) {
            log.warn("Failed to import " + tsIniFile + " into local cache: " + e);
        }
        return tsIniFile;
    }

    private static final class ProgressReporter {
        private final ManualIniPicker picker;
        private final String signature;
        private final String port;
        private final long generation;
        private final AtomicLong sequence = new AtomicLong();
        private IniLocator.LookupStage currentStage;
        private boolean active = true;

        ProgressReporter(ManualIniPicker picker, String signature, String port, long generation) {
            this.picker = picker;
            this.signature = signature;
            this.port = port;
            this.generation = generation;
        }

        synchronized void stage(IniLocator.LookupStage stage) {
            if (!active || picker == null) {
                return;
            }
            currentStage = stage;
            int totalRoots = stage == IniLocator.LookupStage.DRIVE_ROOTS ? -1 : 0;
            publish(new IniLocator.LookupProgress(stage, 0, totalRoots, 0, sequence.getAndIncrement()));
        }

        synchronized void rootProgress(IniLocator.LookupProgress progress) {
            if (!active || picker == null || currentStage != IniLocator.LookupStage.DRIVE_ROOTS) {
                return;
            }
            publish(new IniLocator.LookupProgress(progress.stage, progress.completedRoots,
                progress.totalRoots, progress.activeWorkers, sequence.getAndIncrement()));
        }

        synchronized void finish() {
            if (!active) {
                return;
            }
            active = false;
            if (picker != null) {
                publish(new IniLocator.LookupProgress(
                    IniLocator.LookupStage.COMPLETE, 0, 0, 0, sequence.getAndIncrement()));
                picker.lookupFinished(signature, port, generation);
            }
        }

        private void publish(IniLocator.LookupProgress progress) {
            picker.progress(signature, port, generation, progress);
        }
    }

    private String findAutomaticIni(String signature, String promptKey, BooleanSupplier connectionCurrent,
                                    ProgressReporter progress) throws IniNotFoundException {
        checkConnectionCurrent(connectionCurrent);
        progress.stage(IniLocator.LookupStage.BUNDLED_INI);
        String localIniFile = IniLocator.findIniFile(IniFileReader.INI_FILE_PATH, signature);

        if (localIniFile == null) {
            checkConnectionCurrent(connectionCurrent);
            progress.stage(IniLocator.LookupStage.CURRENT_DIRECTORY);
            localIniFile = IniLocator.findIniFile(".", signature);
        }

        if (localIniFile == null) {
            checkConnectionCurrent(connectionCurrent);
            if (IniLocator.isWindowsRootSearchSupported()) {
                progress.stage(IniLocator.LookupStage.DRIVE_ROOTS);
            }
            IniLocator.RootSearchResult rootIni = rootIniLookup.find(signature, progress::rootProgress);
            checkConnectionCurrent(connectionCurrent);
            if (rootIni == null) {
                rootIni = IniLocator.RootSearchResult.unavailable();
            }
            if (rootIni.getStatus() == IniLocator.RootSearchResult.Status.CANCELLED) {
                throw new IniNotFoundException("INI lookup cancelled because the connection is no longer current");
            }
            if (rootIni.getStatus() == IniLocator.RootSearchResult.Status.FOUND) {
                try {
                    localIniFile = iniImporter.importIni(signature, rootIni.getIni());
                    if (!matchesSignature(localIniFile, signature)) {
                        log.warn("Imported drive-root INI did not match the requested signature");
                        localIniFile = null;
                    }
                } catch (IOException e) {
                    log.warn("Failed to import matching drive-root INI snapshot: " + e);
                }
            }
        }

        if (localIniFile == null) {
            checkConnectionCurrent(connectionCurrent);
            progress.stage(IniLocator.LookupStage.CONSOLE_CACHE_DOWNLOAD);
            // Once the manual picker has been requested, keep checking its cache but do not make every
            // later scanner probe repeat the same unavailable remote lookup (#10158).
            localIniFile = iniDownloader.findOrDownload(signature,
                SignatureHelper.getUrl(signature), !promptedSignatures.contains(promptKey));
            if (!matchesSignature(localIniFile, signature)) {
                localIniFile = null;
            }
        }

        if (localIniFile == null) {
            checkConnectionCurrent(connectionCurrent);
            progress.stage(IniLocator.LookupStage.TUNERSTUDIO_CACHE);
            localIniFile = tunerStudioIniLookup.find(signature);
            if (!matchesSignature(localIniFile, signature)) {
                localIniFile = null;
            }
        }
        return localIniFile;
    }

    private static void checkConnectionCurrent(BooleanSupplier connectionCurrent) throws IniNotFoundException {
        if (!connectionCurrent.getAsBoolean()) {
            throw new IniNotFoundException("INI lookup cancelled because the connection is no longer current");
        }
    }

    private static boolean matchesSignature(String iniPath, String signature) {
        if (iniPath == null) {
            return false;
        }
        try {
            return signature.equals(SignatureHelper.readIniSignature(new File(iniPath)));
        } catch (IOException e) {
            return false;
        }
    }

    @Override
    @NotNull
    public IniFileModel provide(String signature) throws IniNotFoundException {
        return provide(signature, null, BinaryProtocol.nextConnectionGeneration(), () -> true);
    }

    @Override
    @NotNull
    public IniFileModel provide(String signature, String port, long connectionGeneration)
            throws IniNotFoundException {
        return provide(signature, port, connectionGeneration, () -> true);
    }

    @Override
    @NotNull
    public IniFileModel provide(String signature, String port, long connectionGeneration,
                                BooleanSupplier connectionCurrent) throws IniNotFoundException {
        ManualIniPicker picker = manualPickerEnabled ? manualPicker : null;
        String promptKey = promptKey(signature, port);
        ProgressReporter progress = new ProgressReporter(picker, signature, port, connectionGeneration);
        String localIniFile;
        try {
            localIniFile = findAutomaticIni(signature, promptKey, connectionCurrent, progress);
        } finally {
            progress.finish();
        }
        if (localIniFile == null) {
            if (!connectionCurrent.getAsBoolean()) {
                throw new IniNotFoundException("INI lookup cancelled because the connection is no longer current");
            }
            if (reportedMissingSignatures.add(signature)) {
                String message = "Failed to download " + signature + " maybe custom board?";
                log.info(message);
                statusConsumer.logLine(message);
            }
            if (picker != null && promptedSignatures.add(promptKey)) {
                picker.pick(signature, port, connectionGeneration);
            }
        }
        if (localIniFile == null) {
            throw new IniNotFoundException("Failed to locate matching .ini file");
        }
        if (!connectionCurrent.getAsBoolean()) {
            throw new IniNotFoundException("INI lookup cancelled because the connection is no longer current");
        }
        IniFileModel iniFileModel;
        try {
                iniFileModel = IniFileReaderUtil.readIniFileChecked(localIniFile);
        } catch (IniParsingException e) {
            new File(localIniFile).delete();
            throw new IniNotFoundException("Parsing error: " + e, e);
        } catch (FileNotFoundException e) {
            throw new IniNotFoundException(e.toString());
        }
        if (!signature.equals(iniFileModel.getSignature())) {
            if (!connectionCurrent.getAsBoolean()) {
                throw new IniNotFoundException("INI lookup cancelled because the connection is no longer current");
            }
            // A manually selected file can be cached under another firmware's hash.
            // Its offsets, enum ordinals and table dimensions cannot decode this tune.
            if (picker != null && promptedSignatures.add(promptKey)) {
                picker.pick(signature, port, connectionGeneration);
            }
            throw new IniNotFoundException("INI signature mismatch in " + localIniFile
                + "\nExpected: " + signature + "\nFound: " + iniFileModel.getSignature()
                + "\nSelect the original INI for this firmware; changing its signature does not convert its layout.");
        }
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
        if (picker != null && connectionCurrent.getAsBoolean()) {
            picker.resolved(signature, port, connectionGeneration);
        }
        return iniFileModel;
    }

    private static String promptKey(String signature, String port) {
        return String.valueOf(signature) + '\0' + String.valueOf(port);
    }
}
