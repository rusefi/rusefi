package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.tune.xml.Msq;
import org.junit.Before;
import org.junit.Test;

import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.Assert.assertEquals;

/**
 * from IDEA this unit test needs to be executed with "empty" working directory
 */
public class TuneReadWriteTest {
    private static final String PATH = "ui/src/test/resources/frankenso/";
    private static final String TEST_INI = PATH + "mainController.ini";
    private static final String TEST_BINARY_FILE = PATH + "current_configuration.rusefi_binary";

    @Before
    public void before() {
        IniFileModel.getInstance().readIniFile(TEST_INI);
    }

    @Test
    public void testCompareBinaryToTSTune() throws Exception {
        Msq tsTune = Msq.readTune(PATH + "CurrentTune.msq");
        System.out.println(tsTune);

        ConfigurationImage tsBinaryData = tsTune.asImage(IniFileModel.getInstance());

        System.out.println("Reading " + TEST_BINARY_FILE);
        ConfigurationImage fileBinaryData = ConfigurationImageFile.readFromFile(TEST_BINARY_FILE);

        int mismatchCounter = compareImages(tsBinaryData, fileBinaryData);
        assertEquals(0, mismatchCounter);
    }

    @Test
    public void testWriteAndReadTSTune() throws Exception {
        ConfigurationImage fileBinaryData = ConfigurationImageFile.readFromFile(TEST_BINARY_FILE);

        Path path = Files.createTempFile("unit_test_", ".xml");
        String fileName = path.getFileName().toString();

        // writing TS XML tune file with rusEFI code
        Msq tuneFromBinary = Msq.valueOf(fileBinaryData);
        tuneFromBinary.writeXmlFile(fileName);

        // and now reading that XML back
        Msq tuneFromFile = Msq.readTune(fileName);

        ConfigurationImage binaryDataFromXml = tuneFromFile.asImage(IniFileModel.getInstance());

        assertEquals(0, compareImages(binaryDataFromXml, fileBinaryData));
        // todo: looks like this is not removing the temporary file?
        Files.delete(path);
    }

    private static int compareImages(ConfigurationImage image1, ConfigurationImage fileData) {
        byte[] tsBinaryDataContent = image1.getContent();
        byte[] fileBinaryDataContent = fileData.getContent();

        int mismatchCounter = 0;

        for (int i = 0; i < tsBinaryDataContent.length; i++) {
            byte tsByte = tsBinaryDataContent[i];
            byte fileByte = fileBinaryDataContent[i];
            if (tsByte != fileByte) {
                IniField field = IniFileModel.getInstance().findByOffset(i);
                System.out.println("Mismatch at offset=" + i + ", " + (field == null ? "(no field)" : field) + " runtime=" + tsByte + "/file=" + fileByte);
                mismatchCounter++;
            }
        }
        System.out.println("Total mismatch count " + mismatchCounter);
        return mismatchCounter;
    }
}
