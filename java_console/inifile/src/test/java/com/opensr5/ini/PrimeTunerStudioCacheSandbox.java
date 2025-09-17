package com.opensr5.ini;

import com.rusefi.core.FindFileHelper;

public class PrimeTunerStudioCacheSandbox {
    public static void main(String[] args) {
        String localIniFile = FindFileHelper.findFile("../firmware/tunerstudio/generated", IniFileModelImpl.RUSEFI_INI_PREFIX + "_proteus_f7", IniFileModelImpl.RUSEFI_INI_SUFFIX, new FindFileHelper.AdditionalFileHandler() {
            @Override
            public void onAdditionalFile(String fileDirectory, String fileName) {

            }
        }, true);
        System.out.println("Working with " + localIniFile);
        IniFileModelImpl iniFileModel = IniFileModelImpl.readIniFile(localIniFile);
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
    }
}
