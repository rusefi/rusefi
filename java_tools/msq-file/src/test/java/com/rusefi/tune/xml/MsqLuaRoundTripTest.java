package com.rusefi.tune.xml;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.ByteArrayInputStream;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

class MsqLuaRoundTripTest {
    @Test
    void snapshotKeepsLuaWhitespaceAndEmptyScripts(@TempDir Path dir) throws Throwable {
        Path file = dir.resolve("snapshot.msq");
        ConfigurationImage lua = new ConfigurationImage(32, ByteOrder.LITTLE_ENDIAN);
        byte[] script = " \treturn 1  \n".getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(script, 0, lua.getContent(), 0, script.length);

        assertEquals(new String(script, StandardCharsets.US_ASCII), snapshot(lua, file).get("luaScript").getValue());
        assertEquals("", snapshot(new ConfigurationImage(32, ByteOrder.LITTLE_ENDIAN), file).get("luaScript").getValue());
    }

    @Test
    void snapshotRejectsNonAsciiLuaBytes(@TempDir Path dir) {
        ConfigurationImage lua = new ConfigurationImage(32, ByteOrder.LITTLE_ENDIAN);
        lua.getContent()[0] = (byte) 0x80;

        assertThrows(IllegalArgumentException.class, () -> snapshot(lua, dir.resolve("snapshot.msq")));
    }

    private static Map<String, Constant> snapshot(ConfigurationImage lua, Path file) throws Throwable {
        String text = "signature = \"lua-msq-test\"\n"
            + "nPages = 2\n"
            + "pageIdentifier = \"\\x00\\x00\", \"\\x00\\x04\"\n"
            + "pageSize = 8, 32\n"
            + "pageReadCommand = \"R\", \"R\"\n"
            + "ochBlockSize = 0\n"
            + "burnCommand = \"B\", \"B\"\n"
            + "[Constants]\n"
            + "page = 1\n"
            + "mainValue = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n"
            + "page = 2\n"
            + "luaScript = string, ASCII, 0, 32\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "lua-msq-test.ini", new IniFileMetaInfoImpl(raw));
        Msq tune = MsqFactory.valueOf(Map.of(0, new ConfigurationImage(8, ByteOrder.LITTLE_ENDIAN), 0x0400, lua), ini);
        tune.writeXmlFile(file.toString());
        return Msq.readTune(file.toString()).getConstantsAsMap();
    }
}
