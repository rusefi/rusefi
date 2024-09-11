package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileModel;
import com.rusefi.core.SignatureHelper;

public class RealIniFileProvider implements IniFileProvider {
    @Override
    public IniFileModel provide(String signature) {
        String localIniFile = SignatureHelper.downloadIfNotAvailable(SignatureHelper.getUrl(signature));
        if (localIniFile == null)
            throw new IllegalStateException("Failed to download for " + signature);
        return new IniFileModel().readIniFile(localIniFile);
    }
}
