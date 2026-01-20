package com.opensr5.ini;

import java.io.FileNotFoundException;

public class IniFileModelTestHelper {
    public static IniFileModelImpl findAndReadIniFile(String iniFilePath) {
        final String fileName = findMetaInfoFile(iniFilePath);
        try {
            return IniFileModelImpl.readIniFile(fileName);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    private static String findMetaInfoFile(String iniFilePath) {
        String iniFileName = IniFileModelImpl.findIniFile(iniFilePath);
        if (iniFileName == null)
            throw new IllegalStateException("Not found " + IniFileModelImpl.RUSEFI_INI_PREFIX + "*" + IniFileModelImpl.RUSEFI_INI_SUFFIX + " in " + iniFilePath);
        return iniFileName;
    }
}
