package com.opensr5.ini;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.nio.channels.FileChannel;

public class PrimeTunerStudioCache {
    private static final String ECU_DEF_FOLDER = System.getProperty("user.home") +
        File.separator + ".efiAnalytics" +
        File.separator + "TunerStudio" +
        File.separator + "config" +
        File.separator + "ecuDef" +
        File.separator;

    private static final Logging log = Logging.getLogging(PrimeTunerStudioCache.class);

    public static void prime(IniFileModelImpl iniFileModel, String localIniFile) {
        if (new File(ECU_DEF_FOLDER).isDirectory()) {
            log.info("Trying to prime " + ECU_DEF_FOLDER);
        } else {
            log.warn("Not found " + ECU_DEF_FOLDER);
            return;
        }
        String ecuDefLocalFileName = iniFileModel.getSignature().replaceAll(" ", "");
        String fullEcuDefFileName = ECU_DEF_FOLDER + ecuDefLocalFileName;
        if (new File(fullEcuDefFileName).exists()) {
            log.info(fullEcuDefFileName + " already present");
            return;
        }

        try {
            FileChannel localIniFileChannel = new FileInputStream(localIniFile).getChannel();
            FileChannel dest = new FileOutputStream(fullEcuDefFileName).getChannel();
            dest.transferFrom(localIniFileChannel, 0, localIniFileChannel.size());
        } catch (IOException e) {
            log.error("While trying to prime", e);
        }
        log.info("Done " + fullEcuDefFileName);
    }

    public static void primeWithLocalFile() {
        @Nullable String localIniFile = IniFileModelImpl.findIniFile(".");
        if (localIniFile == null) {
            // another option: one level up or environment variable direction
            localIniFile = IniFileModelImpl.findIniFile(IniFileModelImpl.INI_FILE_PATH);
            if (localIniFile == null) {
                log.warn("No .ini");
                return;
            }
        }
        IniFileModelImpl iniFileModel = IniFileModelImpl.readIniFile(localIniFile);
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
    }
}
