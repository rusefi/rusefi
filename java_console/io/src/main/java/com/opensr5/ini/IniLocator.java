package com.opensr5.ini;

import com.rusefi.core.FindFileHelper;
import com.rusefi.ini.reader.IniFileReader;
import org.jetbrains.annotations.Nullable;

public class IniLocator {
    public static @Nullable String findIniFile(String iniFilePath) {
        return FindFileHelper.findFile(iniFilePath, IniFileReader.RUSEFI_INI_PREFIX, IniFileReader.INI_FILE_SUFFIX, (fileDirectory, fileName) -> {
            throw new IllegalStateException("Unique match expected " + fileName);
        }, true);
    }
}
