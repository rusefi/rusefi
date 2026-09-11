package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
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
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

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
    }

    public static ManualIniPicker manualPicker = null;

    interface IniDownloader {
        String findOrDownload(Pair<String, String> location, boolean allowDownload);
    }

    private final IniDownloader iniDownloader;

    public RealIniFileProvider() {
        this(SignatureHelper::downloadIfNotAvailable);
    }

    RealIniFileProvider(IniDownloader iniDownloader) {
        this.iniDownloader = iniDownloader;
    }

    /**
     * Signatures for which a picker has already been opened. The port scanner calls {@link #provide}
     * concurrently and on every probe cycle, so the atomic add both prevents duplicate pickers and
     * stops a cancelled picker from immediately reopening (issue #9774 follow-up). A successful pick
     * is cached as a real .ini by {@link SignatureHelper#importIntoCache}.
     * ponytail: session-lifetime set, cleared only by restart — good enough; a "retry" menu item
     * can clear it later if anyone asks.
     */
    private static final Set<String> promptedSignatures = ConcurrentHashMap.newKeySet();
    private static final Set<String> reportedMissingSignatures = ConcurrentHashMap.newKeySet();

    public static void clearMissingIniStateForTests() {
        promptedSignatures.clear();
        reportedMissingSignatures.clear();
    }

    public static void allowManualPrompt(String signature) {
        promptedSignatures.remove(signature);
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

    @Override
    @NotNull
    public IniFileModel provide(String signature) throws IniNotFoundException {
        // 1. Bundled .ini (one level up, or env override) (#10031)
        String localIniFile = IniLocator.findIniFile(IniFileReader.INI_FILE_PATH, signature);
        if (localIniFile == null) {
            // 2. Current folder
            localIniFile = IniLocator.findIniFile(".", signature);
        }
        if (localIniFile == null) {
            // 3. Cache or download from server
            // Once the manual picker has been requested, keep checking its cache but do not make every
            // later scanner probe repeat the same unavailable remote lookup (#10158).
            localIniFile = iniDownloader.findOrDownload(
                SignatureHelper.getUrl(signature), !promptedSignatures.contains(signature));
        }
        if (localIniFile == null) {
            // 4. TunerStudio's own ecuDef cache: the server may not have this build (custom board,
            // local compile) while TunerStudio has already loaded its .ini
            localIniFile = importFromTunerStudioCache(signature);
        }
        ManualIniPicker picker = manualPicker;
        if (localIniFile == null) {
            if (reportedMissingSignatures.add(signature)) {
                String message = "Failed to download " + signature + " maybe custom board?";
                log.info(message);
                statusConsumer.logLine(message);
            }
            if (picker != null && promptedSignatures.add(signature)) {
                // The UI caches the eventual selection asynchronously; a later probe will find it.
                picker.pick(signature);
            }
        }
        if (localIniFile == null) {
            throw new IniNotFoundException("Failed to locate matching .ini file");
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
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
        return iniFileModel;
    }
}
