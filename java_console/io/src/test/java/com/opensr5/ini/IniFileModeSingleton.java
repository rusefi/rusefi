package com.opensr5.ini;

public class IniFileModeSingleton {
    private static IniFileModel INSTANCE;

    public static synchronized IniFileModel getInstance() {
        if (INSTANCE == null) {
            INSTANCE = IniFileModelTestHelper.findAndReadIniFile(IniFileModelImpl.INI_FILE_PATH);
        }
        return INSTANCE;
    }
}
