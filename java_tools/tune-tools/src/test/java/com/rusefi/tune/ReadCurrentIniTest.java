package com.rusefi.tune;

import com.opensr5.ini.IniFileModel;
import com.rusefi.tools.tune.WriteSimulatorConfiguration;
import org.junit.jupiter.api.Test;

import static org.junit.Assert.assertNotNull;

public class ReadCurrentIniTest {
    @Test
    public void test() {
        IniFileModel ini = new IniFileModel().readIniFile("../" + WriteSimulatorConfiguration.INI_FILE_FOR_SIMULATOR);
        assertNotNull(ini);
    }
}
