package com.opensr5.ini;

import com.rusefi.core.FindFileHelper;
import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.ini.reader.IniFileReaderUtil;

import java.io.FileNotFoundException;

public class PrimeTunerStudioCacheSandbox {
    public static void main(String[] args) throws FileNotFoundException {
        String localIniFile = FindFileHelper.findFile("../firmware/tunerstudio/generated",
            IniFileReader.RUSEFI_INI_PREFIX + "_proteus_f7",
            IniFileReader.INI_FILE_SUFFIX,
            (fileDirectory, fileName) -> {
                throw new IllegalStateException();
            }, true);
        System.out.println("Working with " + localIniFile);
        IniFileModel iniFileModel = IniFileReaderUtil.readIniFile(localIniFile);
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
    }
}
