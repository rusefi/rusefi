package com.opensr5.ini.test;

import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;

import static org.junit.jupiter.api.Assertions.assertNotNull;

public class IniMenuReaderTest {
    @Test
    public void testMenuFileReading() throws FileNotFoundException {
        // yes, it's a bit lazy to access an existing, not very relevant test data file. Guilty!
        String iniPath = "../../java_console/io/src/test/java/com/rusefi/io/pin_output_mode_with_and_without_dollar/test_data/rusefi_uaefi.ini";
        IniFileModel model = IniFileReaderUtil.readIniFile(iniPath);
        assertNotNull(model);
        // This is what we are looking for
        assertNotNull(model.getMenuDialog(), "menuDialog should be loaded");
    }
}
