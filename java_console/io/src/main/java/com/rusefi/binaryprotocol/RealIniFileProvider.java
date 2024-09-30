package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.core.SignatureHelper;

public class RealIniFileProvider implements IniFileProvider {
    @Override
    public IniFileModel provide(String signature) {
        /**
         * first we look at {@link SignatureHelper#LOCAL_INI_CACHE_FOLDER}
         * second we attempt downloading
         * third we look via {@link SignatureHelper#EXTRA_INI_SOURCE} environment variable
         */
        String localIniFile = SignatureHelper.downloadIfNotAvailable(SignatureHelper.getUrl(signature));
        if (localIniFile == null) {
            // 4th option: current folder
            localIniFile = IniFileModelImpl.findIniFile(".");
        }
        if (localIniFile == null) {
            // 5th option: one level up or environment variable direction
            localIniFile = IniFileModelImpl.findIniFile(IniFileModelImpl.INI_FILE_PATH);
        }
        if (localIniFile == null)
            throw new IllegalStateException("Failed to locate .ini file in five different places!");
        return new IniFileModelImpl().readIniFile(localIniFile);
    }
}
