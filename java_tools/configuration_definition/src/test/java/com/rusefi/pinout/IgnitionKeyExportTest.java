package com.rusefi.pinout;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ReaderStateImpl;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.FileReader;
import java.io.StringWriter;
import java.io.Writer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.charset.StandardCharsets;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.rusefi.test.ConfigDefinitionTest.FIRMWARE;
import static org.junit.jupiter.api.Assertions.*;

class IgnitionKeyExportTest {
    @TempDir Path temporary;

    @Test
    void boardDefaultIgnitionKeySurvivesMsqExport() throws Exception {
        ReaderStateImpl state = new ReaderStateImpl();
        try (FileReader gpio = new FileReader(FIRMWARE + "/controllers/algo/rusefi_hw_stm32_enums.h");
             FileReader adc = new FileReader(FIRMWARE + "/controllers/algo/rusefi_hw_adc_enums.h")) {
            state.getEnumsReader().read(gpio);
            state.getEnumsReader().read(adc);
        }
        // Read the actual connector metadata without writing generated board files.
        PinoutLogic pins = new PinoutLogic(new FileSystemBoardInputsReaderImpl(
            FIRMWARE + "/config/boards/m74_9") {
            @Override public Writer getBoardNamesWriter() { return new StringWriter(); }
            @Override public Writer getOutputsWriter() { return new StringWriter(); }
            @Override public Writer getBoardPinNamesWriter() { return new StringWriter(); }
        });
        pins.registerBoardSpecificPinNames(state.getVariableRegistry(),
            state.getEnumsReader().parseState, state.getEnumsReader());
        state.getVariableRegistry().readPrependValues(FIRMWARE + "/integration/rusefi_config.txt", true);
        String definitions = new String(Files.readAllBytes(Paths.get(FIRMWARE, "integration/rusefi_config.txt")), StandardCharsets.UTF_8);
        Matcher field = Pattern.compile("(?m)^\\s*(\\w+) ignitionKeyDigitalPin$").matcher(definitions);
        assertTrue(field.find());
        Matcher custom = Pattern.compile("(?m)^custom " + field.group(1) + " 2 (.+)$").matcher(definitions);
        assertTrue(custom.find());
        String options = state.getVariableRegistry().get(field.group(1) + "_enum");
        Path iniFile = temporary.resolve("key.ini");
        Files.write(iniFile, ("[MegaTune]\n signature = \"key-export-test\"\n[Constants]\n"
            + "ochBlockSize = 0\npageReadCommand = \"R\"\nnPages = 1\npageSize = 2\npage = 1\n"
            + "ignitionKeyDigitalPin = " + custom.group(1).replace("@OFFSET@", "0")
                .replaceAll("\\$\\w+", Matcher.quoteReplacement(options)) + "\n").getBytes(StandardCharsets.UTF_8));
        IniFileModel ini = IniFileReaderUtil.readIniFile(iniFile.toString());
        // L9779_PIN_KEY is stored as little-endian U16 280; never replace it with NONE.
        ConfigurationImage image = new ConfigurationImage(new byte[]{0x18, 0x01});
        Msq tune = MsqFactory.valueOf(image, ini);
        Path msqFile = temporary.resolve("key.msq");
        tune.writeXmlFile(msqFile.toString());
        assertTrue(new String(Files.readAllBytes(msqFile), StandardCharsets.UTF_8).contains("Ignition key"));
        assertArrayEquals(image.getContent(), Msq.readTune(msqFile.toString()).asImage(ini).getContent());
        assertEquals(2, ini.getAllIniFields().get("ignitionKeyDigitalPin").getSize());
    }
}
