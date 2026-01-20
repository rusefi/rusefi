package com.opensr5.ini;

import com.devexperts.logging.Logging;
import com.rusefi.ini.reader.IniFileReader;

public class IniFileModelSandbox {
    private static final Logging log = Logging.getLogging(IniFileModelSandbox.class);

    public static void main(String[] args) {
        IniFileModel iniFile = IniFileModelTestHelper.findAndReadIniFile(IniFileReader.INI_FILE_PATH);
        log.info("Dialogs: " + iniFile.getDialogs());
    }
}
