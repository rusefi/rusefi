package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class OfflineTuneLoaderCustomIniTest {
    private static final byte[] IMAGE = {0x5A, 47, 0, 0x6B};
    private static final String SIGNATURE = "custom-output-list-test";

    @Test
    void matchingCustomIniIsUsedForThisLoadWithoutChangingTheDefaultIni() throws IniParsingException {
        IniFileModel customIni = outputListIni("custom_output_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel automaticIni = outputListIni("output_pin_e_list", "0=\"NONE\",5=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(IMAGE.clone()), customIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedTune(
                tune, SIGNATURE, automaticIni, errors::add, signature -> customIni);

        assertNotNull(result);
        assertSame(customIni, result.ini);
        assertArrayEquals(IMAGE, result.image.getContent());
        assertTrue(errors.isEmpty());
        assertThrows(IllegalArgumentException.class, () -> tune.asImage(automaticIni));
    }

    @Test
    void cancellingCustomIniSelectionDoesNotReturnAnImage() throws IniParsingException {
        IniFileModel customIni = outputListIni("custom_output_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel automaticIni = outputListIni("output_pin_e_list", "0=\"NONE\",5=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(IMAGE.clone()), customIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedTune(
                tune, SIGNATURE, automaticIni, errors::add, signature -> null);

        assertNull(result);
        assertTrue(errors.get(0).contains("outputPin: Enum name not found"));
        assertTrue(errors.get(0).contains("used to save this .msq tune"));
        assertTrue(errors.get(0).contains("not the destination ECU's .ini"));
        assertTrue(errors.get(0).contains("The tune was not opened"));
    }

    @Test
    void customIniWithDifferentSignatureIsRejected() throws IniParsingException {
        IniFileModel sourceIni = outputListIni("custom_output_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel wrongSignatureIni = outputListIni("custom_output_list", "0=\"NONE\",47=\"Auxiliary pin\"",
                "different-signature");
        IniFileModel automaticIni = outputListIni("output_pin_e_list", "0=\"NONE\",5=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(IMAGE.clone()), sourceIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedTune(
                tune, SIGNATURE, automaticIni, errors::add, signature -> wrongSignatureIni);

        assertNull(result);
        assertTrue(errors.get(0).contains("does not match this tune"));
        assertTrue(errors.get(0).contains("The tune was not opened"));
    }

    @Test
    void customIniConversionFailureDoesNotReturnAnImage() throws IniParsingException {
        IniFileModel sourceIni = outputListIni("custom_output_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel incompleteCustomIni = outputListIni("custom_output_list", "0=\"NONE\",5=\"Output\"");
        IniFileModel automaticIni = outputListIni("output_pin_e_list", "0=\"NONE\",5=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(IMAGE.clone()), sourceIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedTune(
                tune, SIGNATURE, automaticIni, errors::add, signature -> incompleteCustomIni);

        assertNull(result);
        assertTrue(errors.get(0).contains("outputPin: Enum name not found"));
        assertTrue(errors.get(0).contains("The tune was not opened"));
    }

    @Test
    void customIniMissingTuneFieldDoesNotReturnZeroFilledImage() throws IniParsingException {
        IniFileModel sourceIni = outputListIni("custom_output_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel customIniMissingOutput = outputListIniWithoutOutput(
                "custom_output_list", "0=\"NONE\",47=\"Auxiliary pin\"");
        IniFileModel automaticIni = outputListIni("output_pin_e_list", "0=\"NONE\",5=\"Output\"");
        Msq tune = MsqFactory.valueOf(ConfigurationImageTestHelper.newLittleEndian(IMAGE.clone()), sourceIni);
        List<String> errors = new ArrayList<>();

        OfflineTuneLoader.Result result = OfflineTuneLoader.loadResolvedTune(
                tune, SIGNATURE, automaticIni, errors::add, signature -> customIniMissingOutput);

        assertNull(result);
        assertTrue(errors.get(0).contains("missing tune field outputPin"));
        assertTrue(errors.get(0).contains("The tune was not opened"));
    }

    @Test
    void malformedIniMetadataIsReportedInsteadOfEscapingThePicker() throws Exception {
        Path malformedIni = Files.createTempFile("malformed-ini-metadata", ".ini");
        Files.write(malformedIni, ("signature = \"test\"\n"
                + "nPages = not-a-number\n"
                + "ochBlockSize = 1\n"
                + "pageSize = 4\n"
                + "pageReadCommand = \"R\"\n").getBytes(StandardCharsets.UTF_8));
        List<String> errors = new ArrayList<>();
        try {
            IniFileModel ini = OfflineTuneLoader.readCustomIni(malformedIni.toFile(), errors::add);
            assertNull(ini);
            assertTrue(errors.get(0).contains("Could not read selected INI"));
            assertTrue(errors.get(0).contains("not-a-number"));
        } finally {
            Files.deleteIfExists(malformedIni);
        }
    }

    private static IniFileModel outputListIni(String listName, String options) throws IniParsingException {
        return outputListIni(listName, options, SIGNATURE);
    }

    private static IniFileModel outputListIni(String listName, String options, String signature)
            throws IniParsingException {
        return makeIni(listName, options, signature, true);
    }

    private static IniFileModel outputListIniWithoutOutput(String listName, String options)
            throws IniParsingException {
        return makeIni(listName, options, SIGNATURE, false);
    }

    private static IniFileModel makeIni(String listName, String options, String signature, boolean outputPresent)
            throws IniParsingException {
        String text = "signature = \"" + signature + "\"\n"
                + "nPages = 1\npageSize = 4\npageReadCommand = \"R\"\nochBlockSize = 1\n"
                + "#define " + listName + "=" + options + "\n"
                + "[Constants]\npage = 1\n"
                + "before = scalar, U08, 0, \"\", 1, 0, 0, 255, 0\n"
                + (outputPresent ? "outputPin = bits, U16, 1, [0:8], $" + listName + "\n" : "")
                + "after = scalar, U08, 3, \"\", 1, 0, 0, 255, 0\n";
        RawIniFile lines = IniFileReaderUtil.read(
                new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(lines, "synthetic.ini", new IniFileMetaInfoImpl(lines));
    }
}
