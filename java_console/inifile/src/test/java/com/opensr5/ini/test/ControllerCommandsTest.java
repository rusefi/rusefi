package com.opensr5.ini.test;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.util.Map;

import static com.opensr5.ini.test.IniFileReaderTest.readLines;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

public class ControllerCommandsTest {
    @Test
    public void testControllerCommands() {
        String ini = "[ControllerCommands]\n" +
                "cmd_test_spk1  = \"Z\\x00\\x12\\x00\\x01\"\n" +
                "cmd_test_spk2  = \"Z\\x00\\x12\\x00\\x02\"";

        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(ini.getBytes()));
        IniFileModel model = readLines(raw);

        Map<String, String> commands = model.getControllerCommands();
        assertNotNull(commands);
        assertEquals("Z\\x00\\x12\\x00\\x01", commands.get("cmd_test_spk1"));
        assertEquals("Z\\x00\\x12\\x00\\x02", commands.get("cmd_test_spk2"));
    }
}
