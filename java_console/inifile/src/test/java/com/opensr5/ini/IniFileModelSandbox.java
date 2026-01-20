package com.opensr5.ini;

import com.devexperts.logging.Logging;

public class IniFileModelSandbox {
    private static final Logging log = Logging.getLogging(IniFileModelSandbox.class);

    public static void main(String[] args) {
        IniFileModel iniFile = IniFileModelTestHelper.findAndReadIniFile(IniFileModelImpl.INI_FILE_PATH);
        log.info("Dialogs: " + iniFile.getDialogs());
    }
}
