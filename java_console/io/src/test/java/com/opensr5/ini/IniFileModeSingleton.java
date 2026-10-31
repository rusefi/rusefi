package com.opensr5.ini;

import com.rusefi.ini.reader.IniFileReader;

public class IniFileModeSingleton {
    private static IniFileModel INSTANCE;

    public static synchronized IniFileModel getInstance() {
        if (INSTANCE == null) {
            INSTANCE = IniFileModelTestHelper.findAndReadIniFile(IniFileReader.INI_FILE_PATH);
        }
        return INSTANCE;
    }
}
