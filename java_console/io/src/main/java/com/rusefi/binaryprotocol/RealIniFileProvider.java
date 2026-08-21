package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.core.SignatureHelper;
import com.rusefi.ui.StatusConsumer;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import org.jetbrains.annotations.NotNull;

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
     * How deep {@link IniLocator#findIniFileRecursively} descends when looking for an ini in
     * subfolders of the working directory / bundle root. Deep enough for a repo checkout
     * (e.g. firmware/tunerstudio/generated) but bounded to keep the scan cheap.
     */
    private static final int MAX_LOCAL_INI_DEPTH = 3;

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
            // 2.5 Current folder and its subfolders, so an ini unpacked into a nested dir is
            //     found automatically instead of prompting the user on every start.
            localIniFile = IniLocator.findIniFileRecursively(".", signature, MAX_LOCAL_INI_DEPTH);
        }
        if (localIniFile == null) {
            // 2.6 Bundled root recursively (the bundle ini usually sits one level up, but a dev
            //     may run the console from a nested folder of the checkout).
            localIniFile = IniLocator.findIniFileRecursively(IniFileReader.INI_FILE_PATH, signature, MAX_LOCAL_INI_DEPTH);
        }
        if (localIniFile != null) {
            // Remember the location: copy it into the ini_database cache so the next start
            // resolves it instantly without rescanning or prompting.
            rememberIniLocation(signature, localIniFile);
        } else {
            // 3. Cache or download from server; a cached .ini whose embedded signature diverges
            //    from the requested one (firmware rebuilt on a later day) is dropped as stale.
            localIniFile = SignatureHelper.downloadIfNotAvailable(SignatureHelper.getUrl(signature), signature);
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

    /**
     * Copies a locally found .ini into the ini_database cache (keyed by the signature hash) so
     * its location survives restarts. Best effort: a failure only means the scan re-runs next
     * time. Never imports the cache file onto itself, and leaves an existing entry alone.
     */
    private static void rememberIniLocation(String signature, String localIniFile) {
        try {
            String cacheTarget = SignatureHelper.getLocalIniCacheFile(signature);
            if (cacheTarget == null) {
                return;
            }
            File source = new File(localIniFile);
            File target = new File(cacheTarget);
            if (source.getCanonicalPath().equals(target.getCanonicalPath())) {
                return; // the found file IS the cache entry
            }
            if (target.exists()) {
                return; // already remembered
            }
            SignatureHelper.importIntoCache(signature, source);
            log.info("Remembered ini location " + localIniFile + " in cache as " + cacheTarget);
        } catch (IOException e) {
            log.info("Could not remember ini location " + localIniFile + ": " + e);
        }
    }
}
