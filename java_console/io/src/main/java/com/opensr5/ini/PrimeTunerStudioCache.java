package com.opensr5.ini;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;
import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ts.TsHelper;
import org.jetbrains.annotations.Nullable;

import java.io.*;

public class PrimeTunerStudioCache {
    /**
     * TunerStudio's own .ini cache: every ECU definition TS has ever loaded lands here as
     * {@code <signature without spaces>.ini}. We prime it on our side (see {@link #prime}) and,
     * symmetrically, read from it when the rusefi.com download fails - a custom/unpublished build
     * that TunerStudio has already talked to is still available locally.
     */
    static final String ECU_DEF_FOLDER = TsHelper.TS_ROOT +
        "TunerStudio" +
        File.separator + "config" +
        File.separator + "ecuDef" +
        File.separator;

    private static final Logging log = Logging.getLogging(PrimeTunerStudioCache.class);

    public static void prime(IniFileModel iniFileModel, String localIniFile) {
        if (new File(ECU_DEF_FOLDER).isDirectory()) {
            log.info("Trying to prime " + ECU_DEF_FOLDER);
        } else {
            log.warn("Not found " + ECU_DEF_FOLDER);
            return;
        }
        String fullEcuDefFileName = ECU_DEF_FOLDER + ecuDefFileName(iniFileModel.getSignature());
        if (new File(fullEcuDefFileName).exists()) {
            log.info(fullEcuDefFileName + " already present");
            return;
        }

        try {
            FileUtil.copyFile(localIniFile, fullEcuDefFileName);
        } catch (IOException e) {
            log.error("While trying to prime", e);
        }
        log.info("Done " + fullEcuDefFileName);
    }

    /**
     * @return TunerStudio's cached .ini for this signature, or null if TS has never seen it
     */
    @Nullable
    public static String findInTunerStudioCache(String signature) {
        return findInTunerStudioCache(signature, ECU_DEF_FOLDER);
    }

    // package-private for tests
    @Nullable
    static String findInTunerStudioCache(String signature, String ecuDefFolder) {
        if (signature == null) {
            return null;
        }
        File candidate = new File(ecuDefFolder + ecuDefFileName(signature));
        if (!candidate.isFile()) {
            log.info("Not found in TunerStudio cache: " + candidate);
            return null;
        }
        // the file is named after the signature but we still verify the content: a stale or
        // hand-edited file with the wrong signature would be worse than no file at all
        String actualSignature = IniLocator.readIniSignature(candidate);
        if (!signature.equals(actualSignature)) {
            log.warn("TunerStudio cache " + candidate + " declares signature [" + actualSignature + "], expected [" + signature + "]");
            return null;
        }
        log.info("Found in TunerStudio cache " + candidate);
        return candidate.getAbsolutePath();
    }

    private static String ecuDefFileName(String signature) {
        return signature.replaceAll(" ", "") + ".ini";
    }

    public static String findLocalIniFile() {
        @Nullable String localIniFile = IniLocator.findIniFile(".");
        if (localIniFile == null) {
            // another option: one level up or environment variable direction
            localIniFile = IniLocator.findIniFile(IniFileReader.INI_FILE_PATH);
        }
        return localIniFile;
    }

    public static void primeWithLocalFile() {
        @Nullable String localIniFile = findLocalIniFile();
        if (localIniFile == null) {
            log.warn(".ini file not located located");
            return;
        }
        IniFileModel iniFileModel;
        try {
            iniFileModel = IniFileReaderUtil.readIniFile(localIniFile);
        } catch (FileNotFoundException e) {
            log.warn("error " + e);
            return;
        }
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
    }
}
