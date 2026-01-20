package com.opensr5.ini;

import com.rusefi.core.FindFileHelper;
import org.jetbrains.annotations.Nullable;

public class IniLocator {
    public static @Nullable String findIniFile(String iniFilePath) {
        return FindFileHelper.findFile(iniFilePath, IniFileModelImpl.RUSEFI_INI_PREFIX, IniFileModelImpl.RUSEFI_INI_SUFFIX, (fileDirectory, fileName) -> {
            throw new IllegalStateException("Unique match expected " + fileName);
        }, true);
    }
}
