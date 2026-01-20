package com.opensr5.ini;

public class IniFileModeSingleton {
    private static IniFileModelImpl INSTANCE;

    public static synchronized IniFileModelImpl getInstance() {
        if (INSTANCE == null) {
            INSTANCE = IniFileModelTestHelper.findAndReadIniFile(IniFileModelImpl.INI_FILE_PATH);
        }
        return INSTANCE;
    }
}
