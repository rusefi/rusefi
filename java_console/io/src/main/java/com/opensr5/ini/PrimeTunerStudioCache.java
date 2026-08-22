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
     * How deep {@link IniLocator#findIniFileRecursively} descends when looking for an ini in
     * subfolders. Matches {@code RealIniFileProvider.MAX_LOCAL_INI_DEPTH} so the startup lookup
     * sees the same files as the connect-time provider.
     */
    private static final int MAX_LOCAL_INI_DEPTH = 3;

    public static String findLocalIniFile() {
        return findLocalIniFile(".", IniFileReader.INI_FILE_PATH);
    }

    /**
     * Search order mirrors the connect-time provider (see {@code RealIniFileProvider.provide}):
     * top-level {@code currentDir} and {@code fallbackPath}, then both recursively. The recursive
     * steps matter when the console runs from a folder whose ini sits in a nested subfolder (an
     * unzipped bundle dir, a repo checkout): without them the startup {@code IniFileModel} is null,
     * gauges keep their raw key names and the front-page indicator panel never appears until a
     * connection status change happens to rebuild the UI.
     */
    static String findLocalIniFile(String currentDir, String fallbackPath) {
        @Nullable String localIniFile = IniLocator.findIniFile(currentDir);
        if (localIniFile == null) {
            // another option: one level up or environment variable direction
            localIniFile = IniLocator.findIniFile(fallbackPath);
        }
        if (localIniFile == null) {
            localIniFile = IniLocator.findIniFileRecursively(currentDir, null, MAX_LOCAL_INI_DEPTH);
        }
        if (localIniFile == null) {
            localIniFile = IniLocator.findIniFileRecursively(fallbackPath, null, MAX_LOCAL_INI_DEPTH);
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
