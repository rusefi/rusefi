package com.opensr5.ini;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;
import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ts.TsHelper;
import org.jetbrains.annotations.Nullable;

import java.io.*;

public class PrimeTunerStudioCache {
    private static final String ECU_DEF_FOLDER = TsHelper.TS_ROOT +
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
        String ecuDefLocalFileName = iniFileModel.getSignature().replaceAll(" ", "");
        String fullEcuDefFileName = ECU_DEF_FOLDER + ecuDefLocalFileName + ".ini";
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
