package com.rusefi.tune;

import com.opensr5.*;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

public class TuneReadWriteTest {
    private static final String PATH = "src/test/resources/";
    static final String TUNE_NAME = PATH + "test-CurrentTune.msq";
    static final String TEST_INI = PATH + "test-mainController.ini";
    private static final String TEST_BINARY_FILE = PATH + "current_configuration.binary_image";
    private static final int LEGACY_TOTAL_CONFIG_SIZE = 20000;

    private final IniFileModel model;

    {
        try {
            model = IniFileReaderUtil.readIniFile(TEST_INI);
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
/*
    @Test
    public void testCopyCode() {
        String tableReference = "config->ve";
        String tableName = "veTable";

        String copyMethodBody = TS2C.getCopyMethodBody(tableReference, model, tableName);
        assertEquals("\tcopyArray(config->veLoadBins, hardCodedveRpmBins);\n" +
                "\tcopyArray(config->veRpmBins, hardCodedveLoadBins);\n" +
                "\tcopyTable(config->veTable, hardCodedveTable);\n", copyMethodBody);
    }
*/
    @Test
    public void testMeta() throws IOException {
        final ConfigurationImageWithMeta configurationImage = ConfigurationImageFile.readFromFile(TEST_BINARY_FILE);
        assertNotNull(configurationImage);
        final ConfigurationImageMeta meta = configurationImage.getMeta();
        assertInstanceOf(ConfigurationImageMetaVersion0_0.class, meta);
        final ConfigurationImageMetaVersion0_0 metaVersion0_0 = (ConfigurationImageMetaVersion0_0) meta;
        assertEquals(LEGACY_TOTAL_CONFIG_SIZE, metaVersion0_0.getImageSize());
        assertEquals("rusEFI TEST_BRANCH.TEST_TAG.TEST_DEVICE.TEST_HASH\0", metaVersion0_0.getEcuSignature());
    }

    @Test
    public void testCompareBinaryToTSTune() throws Exception {
        Msq tsTune = Msq.readTune(TUNE_NAME);
        System.out.println(tsTune);
        assertNotNull("signature", tsTune.getVersionInfo().getSignature());

        Constant flow = tsTune.findPage().findParameter("injector_flow");
        assertNotNull(flow);
        assertEquals("2", flow.getDigits());

        ConfigurationImage tsBinaryData = tsTune.asImage(model);

        System.out.println("Reading " + TEST_BINARY_FILE);
        ConfigurationImage fileBinaryData = ConfigurationImageFile.readFromFile(TEST_BINARY_FILE).getConfigurationImage();

        int mismatchCounter = compareImages(tsBinaryData, fileBinaryData, model);
        assertEquals(3, mismatchCounter);
    }

    @Test
    public void testWriteAndReadTSTune() throws Exception {
        ConfigurationImage fileBinaryData = ConfigurationImageFile.readFromFile(TEST_BINARY_FILE).getConfigurationImage();

        Path path = Files.createTempFile("unit_test_", ".xml");
        String fileName = path.getFileName().toString();

        // writing TS XML tune file with rusEFI code
        Msq tuneFromBinary = MsqFactory.valueOf(fileBinaryData, model);
        tuneFromBinary.writeXmlFile(fileName);

        Constant batteryCorrection = tuneFromBinary.findPage().findParameter("injector_battLagCorrBins");
        assertNotNull(batteryCorrection);
        assertEquals("2", batteryCorrection.getDigits());

        Constant flow = tuneFromBinary.findPage().findParameter("injector_flow");
        assertNotNull(flow);
        assertEquals("2", flow.getDigits());

        Constant nonEmptyFormula = tuneFromBinary.findPage().findParameter("legacyFormulas1");
        assertNotNull(nonEmptyFormula);

        /**
         * Empty strings values should be omitted from the tune
         */
        Constant emptyFormula = tuneFromBinary.findPage().findParameter("legacyFormulas2");
        assertNull(emptyFormula);

        Constant enumField = tuneFromBinary.findPage().findParameter("acRelayPin");
        // quotes are expected
        assertEquals("\"NONE\"", enumField.getValue());

        // and now reading that XML back
        Msq tuneFromFile = Msq.readTune(fileName);
        assertNotNull(tuneFromFile.getVersionInfo().getSignature());

        ConfigurationImage binaryDataFromXml = tuneFromFile.asImage(model);

        /**
         * Looks like I am not getting something right around Field#FIELD_PRECISION
         * See also TuneWriterTest :(
         */
        assertEquals(66, compareImages(binaryDataFromXml, fileBinaryData, model), "Binary match expected");
        // todo: looks like this is not removing the temporary file?
        Files.delete(path);
    }

    private static int compareImages(ConfigurationImage image1, ConfigurationImage fileData, IniFileModel ini) {
        byte[] tsBinaryDataContent = image1.getContent();
        byte[] fileBinaryDataContent = fileData.getContent();

        int mismatchCounter = 0;

        for (int i = 0; i < tsBinaryDataContent.length; i++) {
            byte tsByte = tsBinaryDataContent[i];
            byte fileByte = fileBinaryDataContent[i];
            if (tsByte != fileByte) {
                IniField field = ini.findByOffset(i);
                if (field instanceof ScalarIniField) {
                    System.out.println("    Image " + ConfigurationImageGetterSetter.getValue(field, image1));
                    System.out.println("FileImage " + ConfigurationImageGetterSetter.getValue(field, fileData));
                }
                System.out.println("Mismatch at offset=" + i + ", " + (field == null ? "(no field)" : field) + " runtime=" + tsByte + "/file=" + fileByte);
                mismatchCounter++;
            }
        }
        System.out.println("Total mismatch count " + mismatchCounter);
        return mismatchCounter;
    }
}
