package com.opensr5.ini;

import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.ini.reader.IniFileReaderUtil;

import java.io.FileNotFoundException;

public class IniFileModelTestHelper {
    public static IniFileModel findAndReadIniFile(String iniFilePath) {
        final String fileName = findMetaInfoFile(iniFilePath);
        try {
            return IniFileReaderUtil.readIniFile(fileName);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    private static String findMetaInfoFile(String iniFilePath) {
        String iniFileName = IniLocator.findIniFile(iniFilePath);
        if (iniFileName == null)
            throw new IllegalStateException("Not found " + IniFileReader.RUSEFI_INI_PREFIX + "*" + IniFileReader.INI_FILE_SUFFIX + " in " + iniFilePath);
        return iniFileName;
    }
}
