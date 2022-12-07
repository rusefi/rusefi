package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.binaryprotocol.MsqFactory;
import com.rusefi.config.generated.Fields;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.Assert.*;

/**
 * from IDEA this unit test needs to be executed with "empty" working directory
 */
@Ignore // todo: https://github.com/rusefi/rusefi/issues/4669
public class TuneReadWriteTest {
    private static final String PATH = "src/test/resources/frankenso/";
    private static final String TEST_INI = PATH + "mainController.ini";
    private static final String TEST_BINARY_FILE = PATH + "current_configuration.rusefi_binary";
    private static final int LEGACY_TOTAL_CONFIG_SIZE = 20000;

    @Before
    public void before() {
        IniFileModel.getInstance().readIniFile(TEST_INI);
    }

    @Test
    public void testCompareBinaryToTSTune() throws Exception {
        Msq tsTune = Msq.readTune(PATH + "CurrentTune.msq");
        System.out.println(tsTune);
        assertNotNull("signature", tsTune.getVersionInfo().getSignature());

        Constant flow = tsTune.findPage().findParameter("injector_flow");
        assertNotNull(flow);
        assertEquals("2", flow.getDigits());

        ConfigurationImage tsBinaryData = tsTune.asImage(IniFileModel.getInstance(), LEGACY_TOTAL_CONFIG_SIZE);

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
        Msq tuneFromBinary = MsqFactory.valueOf(fileBinaryData);
        tuneFromBinary.writeXmlFile(fileName);

        Constant batteryCorrection = tuneFromBinary.findPage().findParameter("injector_battLagCorrBins");
        assertNotNull(batteryCorrection);
        assertEquals("2", batteryCorrection.getDigits());

        Constant flow = tuneFromBinary.findPage().findParameter("injector_flow");
        assertNotNull(flow);
        assertEquals("2", flow.getDigits());

        Constant nonEmptyFormula = tuneFromBinary.findPage().findParameter("fsioFormulas1");
        assertNotNull(nonEmptyFormula);

        /**
         * Empty strings values should be omitted from the tune
         */
        Constant emptyFormula = tuneFromBinary.findPage().findParameter("fsioFormulas2");
        assertNull(emptyFormula);

        Constant enumField = tuneFromBinary.findPage().findParameter("acRelayPin");
        // quotes are expected
        assertEquals("\"NONE\"", enumField.getValue());

        // and now reading that XML back
        Msq tuneFromFile = Msq.readTune(fileName);
        assertNotNull(tuneFromFile.getVersionInfo().getSignature());

        ConfigurationImage binaryDataFromXml = tuneFromFile.asImage(IniFileModel.getInstance(), LEGACY_TOTAL_CONFIG_SIZE);

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
