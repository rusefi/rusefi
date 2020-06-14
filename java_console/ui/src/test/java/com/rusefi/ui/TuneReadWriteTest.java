package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.config.generated.Fields;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.xml.XmlUtil;
import org.junit.Test;

import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Objects;

import static org.junit.Assert.assertEquals;

/**
 * from IDEA this unit test needs to be exectuted with "empty" working directory
 */
public class TuneReadWriteTest {
    private static final String PATH = "ui/src/test/resources/frankenso/";

    @Test
    public void testReadTsTune() throws Exception {
        XmlUtil.setParserImpl();

        IniFileModel.getInstance().readIniFile(PATH + "mainController.ini");
        Msq tsTune = XmlUtil.readModel(Msq.class, PATH + "CurrentTune.msq");
        System.out.println(tsTune);

        ConfigurationImage tsBinaryData = makeBinaryTune(tsTune, IniFileModel.getInstance());


        String binary = PATH + "current_configuration.rusefi_binary";
        System.out.println("Reading " + binary);
        ConfigurationImage fileBinaryData = ConfigurationImageFile.readFromFile(binary);


        byte[] tsBinaryDataContent = tsBinaryData.getContent();
        byte[] fileBinaryDataContent = fileBinaryData.getContent();

        int mismatchCounter = 0;

        for (int i = 0; i < tsBinaryDataContent.length; i++) {
            byte tsByte = tsBinaryDataContent[i];
            byte fileByte = fileBinaryDataContent[i];
            if (tsByte != fileByte) {
                IniField field = IniFileModel.getInstance().findByOffset(i);
                System.out.println("Mismatch at " + (field == null ? "offset " + i : field) + " " + tsByte + "/" + fileByte);
                mismatchCounter++;
            }
        }
        System.out.println("Total mismatch count " + mismatchCounter);
        // for different legit reasons we have some mismatches
        assertEquals(7, mismatchCounter);
    }

    private ConfigurationImage makeBinaryTune(Msq tsTune, IniFileModel instance) {
        ConfigurationImage ci = new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE);

        for (Constant constant : tsTune.getPage().constant) {
            if (constant.getName().startsWith("UNALLOCATED_SPACE")) {
                continue;
            }
            IniField field = instance.allIniFields.get(constant.getName());
            Objects.requireNonNull(field, "Field for " + constant.getName());
            System.out.println("Setting " + field);
            field.setValue(ci, constant);
        }
        return ci;
    }
}
