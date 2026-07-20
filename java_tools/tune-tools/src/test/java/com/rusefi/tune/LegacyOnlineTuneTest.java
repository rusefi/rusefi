package com.rusefi.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class LegacyOnlineTuneTest {
    private static final String RESOURCES = "src/test/resources/";
    private static final String CURRENT_INI = "../../firmware/tunerstudio/generated/";

    @Test
    public void online2024UaefiTuneAcceptsLabelsWithNewDocumentationSuffixes() throws Exception {
        Msq tune = Msq.readTune(RESOURCES + "online-tune-1615-2024.msq");
        IniFileModel currentIni = IniFileReaderUtil.readIniFile(CURRENT_INI + "rusefi_uaefi.ini");
        ConfigurationImage image = tune.asImage(currentIni);

        String[][] expected = {
            {"mainRelayPin", "B9 Main Relay Weak Low Side output 1 (no flyback here)"},
            {"fuelPumpPin", "B16 Low Side output 4 / Fuel Pump Relay (has flyback D5)"},
            {"fanPin", "B8 Fan Relay Weak Low Side output 2 (no flyback here)"},
            {"triggerInputPins1", "C17 VR2-/HALL max9924 best for normal 12+ tooth wheels"},
        };
        for (String[] fieldAndLabel : expected) {
            IniField field = currentIni.findIniField(fieldAndLabel[0]).orElseThrow();
            assertEquals("\"" + fieldAndLabel[1] + "\"",
                ConfigurationImageGetterSetter.getStringValue(field, image));
        }
    }

    @Test
    public void online2025UaefiTuneAcceptsAddedPinCapability() throws Exception {
        Msq tune = Msq.readTune(RESOURCES + "online-tune-1748-2025.msq");
        IniFileModel currentIni = IniFileReaderUtil.readIniFile(CURRENT_INI + "rusefi_uaefi.ini");

        ConfigurationImage image = tune.asImage(currentIni);
        IniField field = currentIni.findIniField("boostControlPin").orElseThrow();
        assertEquals("\"B17 Low Side output 3 or injector 7 (has flyback D4)\"",
            ConfigurationImageGetterSetter.getStringValue(field, image));
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
