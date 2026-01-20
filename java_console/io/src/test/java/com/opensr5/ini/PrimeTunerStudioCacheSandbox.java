package com.opensr5.ini;

import com.rusefi.core.FindFileHelper;

import java.io.FileNotFoundException;

public class PrimeTunerStudioCacheSandbox {
    public static void main(String[] args) throws FileNotFoundException {
        String localIniFile = FindFileHelper.findFile("../firmware/tunerstudio/generated",
            IniFileModelImpl.RUSEFI_INI_PREFIX + "_proteus_f7",
            IniFileModelImpl.RUSEFI_INI_SUFFIX,
            (fileDirectory, fileName) -> {
                throw new IllegalStateException();
            }, true);
        System.out.println("Working with " + localIniFile);
        IniFileModelImpl iniFileModel = IniFileReader.readIniFile(localIniFile);
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
    }
}
