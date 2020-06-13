package com.rusefi.ui;

import com.opensr5.ini.IniFileModel;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.tune.xml.Msq;
import com.rusefi.xml.XmlUtil;
import org.junit.Test;

import java.io.IOException;

/**
 * from IDEA this unit test needs to be exectuted with "empty" working directory
 */
public class TuneReadWriteTest {
    private static final String PATH = "ui/src/test/resources/frankenso/";

    @Test
    public void testReadTsTune() throws Exception {
        IniFileModel.getInstance().readIniFile(PATH + "mainController.ini");
//        Msq tsTune = XmlUtil.readModel(Msq.class, PATH + "CurrentTune.msq");
//        System.out.println(tsTune);


        String binary = PATH + "current_configuration.rusefi_binary";
        System.out.println("Reading " + binary);
        ConfigurationImageFile.readFromFile(binary);
    }
}
