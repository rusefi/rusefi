package com.rusefi.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class LegacyOnlineTuneTest {
    private static final String RESOURCES = "src/test/resources/";
    private static final String CURRENT_INI = "../../firmware/tunerstudio/generated/";

    @Test
    public void online2024UaefiTuneRejectsRenamedOutputLabels() throws Exception {
        Msq tune = Msq.readTune(RESOURCES + "online-tune-1615-2024.msq");
        IniFileModel currentIni = IniFileReaderUtil.readIniFile(CURRENT_INI + "rusefi_uaefi.ini");

        for (String fieldName : new String[]{"mainRelayPin", "fuelPumpPin", "fanPin"}) {
            Constant value = tune.findPage().findParameter(fieldName);
            IniField field = currentIni.findIniField(fieldName).orElseThrow();

            IllegalArgumentException exception = assertThrows(IllegalArgumentException.class,
                () -> ConfigurationImageGetterSetter2.setValue(field,
                    new ConfigurationImage(currentIni.getMetaInfo().getPageSize(0)), value));
            assertEquals(fieldName + ": Enum name not found " + value.getValue(), exception.getMessage());
        }
    }

    @Test
    public void online2023AlphaxTuneStillAcceptsUnchangedOutputLabel() throws Exception {
        Msq tune = Msq.readTune(RESOURCES + "online-tune-1593-2023.msq");
        IniFileModel currentIni = IniFileReaderUtil.readIniFile(CURRENT_INI + "rusefi_alphax-4chan.ini");

        ConfigurationImage image = tune.asImage(currentIni);
        IniField field = currentIni.findIniField("mainRelayPin").orElseThrow();
        assertEquals("\"F5 - MAIN\"", ConfigurationImageGetterSetter.getStringValue(field, image));
    }
}
