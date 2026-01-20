package com.rusefi.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileReader;
import com.rusefi.LocalIniFileProvider;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;

public class ReadCurrentIniTest {
    @Test
    public void test() throws FileNotFoundException {
        IniFileModel ini = IniFileReader.readIniFile("../" + LocalIniFileProvider.INI_FILE_FOR_SIMULATOR);
        Assertions.assertNotNull(ini);
    }
}
