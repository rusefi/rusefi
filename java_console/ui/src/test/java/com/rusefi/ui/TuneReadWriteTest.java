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

import java.util.Objects;

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

        makeBinaryTune(tsTune, IniFileModel.getInstance());


        String binary = PATH + "current_configuration.rusefi_binary";
        System.out.println("Reading " + binary);
        ConfigurationImageFile.readFromFile(binary);
    }

    private void makeBinaryTune(Msq tsTune, IniFileModel instance) {
        ConfigurationImage ci = new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE);

        for (Constant constant : tsTune.getPage().constant) {
            if (constant.getName().startsWith("UNALLOCATED_SPACE")) {
                continue;
            }
            IniField field = instance.allIniFields.get(constant.getName());
            Objects.requireNonNull(field, "Field for " + constant.getName());
            field.setValue(ci, constant);
        }
    }
}
