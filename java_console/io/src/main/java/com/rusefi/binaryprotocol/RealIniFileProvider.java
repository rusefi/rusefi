package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.core.SignatureHelper;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.jetbrains.annotations.NotNull;

import java.io.FileNotFoundException;

import static com.devexperts.logging.Logging.getLogging;

public class RealIniFileProvider implements IniFileProvider {
    private static final Logging log = getLogging(RealIniFileProvider.class);
    @Override
    @NotNull
    public IniFileModel provide(String signature) throws IniNotFoundException {
        /**
         * first we look at {@link SignatureHelper#LOCAL_INI_CACHE_FOLDER}
         * second we attempt downloading
         * third we look via {@link SignatureHelper#EXTRA_INI_SOURCE} environment variable
         */
        String localIniFile = SignatureHelper.downloadIfNotAvailable(SignatureHelper.getUrl(signature));
        if (localIniFile == null) {
            log.info("Failed to download " + signature + " maybe custom board?");
            // 4th option: current folder
            localIniFile = IniLocator.findIniFile(".");
        }
        if (localIniFile == null) {
            // 5th option: one level up or environment variable direction
            localIniFile = IniLocator.findIniFile(IniFileReader.INI_FILE_PATH);
        }
        if (localIniFile == null)
            throw new IniNotFoundException("Failed to locate .ini file in five different places!");
        IniFileModel iniFileModel;
        try {
            iniFileModel = IniFileReaderUtil.readIniFile(localIniFile);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
        return iniFileModel;
    }
}
