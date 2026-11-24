package com.opensr5.ini.test;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

public class CustomOutputListTuneTest {
    private static final byte[] IMAGE = {0x5A, 47, 0, 0x6B};

    @Test
    public void gpioListTuneDoesNotConvertAgainstDifferentOutputList() throws IniParsingException {
        IniFileModel customIni = outputListIni("gpio_list",
            "0=\"NONE\",5=\"Output\",47=\"Auxiliary pin\"");
        IniFileModel standardIni = outputListIni("output_pin_e_list", "0=\"NONE\",5=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(IMAGE.clone()), customIni);

        assertEquals("\"Auxiliary pin\"", tune.getConstantsAsMap().get("outputPin").getValue());
        assertArrayEquals(IMAGE, tune.asImage(customIni).getContent());
        assertEquals(customIni.getSignature(), standardIni.getSignature());

        // Direct conversion must remain strict: the UI can ask for this tune's matching custom INI.
        IllegalArgumentException error = assertThrows(IllegalArgumentException.class,
            () -> tune.asImage(standardIni));
        assertEquals("outputPin: Enum name not found \"Auxiliary pin\"", error.getMessage());
    }

    @Test
    public void customListNamesDoNotEnableSuffixCompatibility() throws IniParsingException {
        IniFileModel sourceIni = outputListIni("output_pin_e_list", "0=\"NONE\",47=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(IMAGE.clone()), sourceIni);
        String documentedOptions = "0=\"NONE\",47=\"Output (documentation)\"";
        IniFileModel standardIni = outputListIni("output_pin_e_list", documentedOptions);
        IniFileModel customIni = outputListIni("custom_output_list", documentedOptions);

        assertTrue(((EnumIniField) standardIni.getAllIniFields().get("outputPin")).isPinEnum());
        assertFalse(((EnumIniField) customIni.getAllIniFields().get("outputPin")).isPinEnum());
        assertArrayEquals(IMAGE, tune.asImage(standardIni).getContent());
        Msq customTune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(IMAGE.clone()), customIni);
        assertArrayEquals(IMAGE, customTune.asImage(customIni).getContent());

        // Without metadata identifying a pin list, cross-label suffix matching is intentionally unsupported.
        IllegalArgumentException error = assertThrows(IllegalArgumentException.class,
            () -> tune.asImage(customIni));
        assertEquals("outputPin: Enum name not found \"Output\"", error.getMessage());
    }

    @Test
    public void unknownRawPinOrdinalIsExportedExplicitlyButNotRestoredWithoutDefinitionEvidence()
            throws IniParsingException {
        IniFileModel ini = outputListIni("gpio_list", "0=\"NONE\",5=\"Output\",47=\"Auxiliary pin\"");
        byte[] image = {0x5A, (byte) 200, 0, 0x6B};
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(image.clone()), ini);
        String value = EnumIniField.unknownOrdinalValue(200);

        assertEquals(value, tune.getConstantsAsMap().get("outputPin").getValue());
        IllegalArgumentException error = assertThrows(IllegalArgumentException.class, () -> tune.asImage(ini));
        assertEquals("outputPin: Cannot load unknown pin. Use the original tune with the INI used to save it, "
            + "or choose a supported pin.",
            error.getMessage());
    }

    private static IniFileModel outputListIni(String listName, String options) throws IniParsingException {
        String text = "signature = \"custom-output-list-test\"\n"
            + "nPages = 1\npageSize = 4\npageReadCommand = \"R\"\nochBlockSize = 1\n"
            + "#define " + listName + "=" + options + "\n"
            + "[Constants]\npage = 1\n"
            + "before = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n"
            + "outputPin = bits, U16, 1, [0:8], $" + listName + "\n"
            + "after = scalar, U08, 3, \"\", 1, 0, 0, 255, 0\n";
        RawIniFile lines = IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(lines, "synthetic.ini", new IniFileMetaInfoImpl(lines));
    }
}
