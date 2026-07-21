package com.rusefi.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

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
    public void online2023SemanticPinRenameIsReportedForUserResolution() throws Exception {
        Msq tune = Msq.readTune(RESOURCES + "online-tune-1393-2023.msq");
        IniFileModel currentIni = IniFileReaderUtil.readIniFile(CURRENT_INI + "rusefi_alphax-4chan.ini");

        Msq.ApplyResult result = tune.applyOntoWithReport(
            new ConfigurationImage(currentIni.getMetaInfo().getPageSize(0)), currentIni, null);

        assertTrue(result.getFatalErrors().isEmpty());
        assertEquals(2, result.getUnresolvedPins().size());
        assertEquals("vvtPins1", result.getUnresolvedPins().get(0).getConstant().getName());
        assertEquals("\"F2 - VVT#1\"", result.getUnresolvedPins().get(0).getConstant().getValue());
        assertEquals("vvtPins2", result.getUnresolvedPins().get(1).getConstant().getName());
    }

    @Test
    public void nonPinEnumFailureRemainsFatal() throws Exception {
        IniFileModel currentIni = IniFileReaderUtil.readIniFile(CURRENT_INI + "rusefi_uaefi.ini");
        Msq tune = Msq.create(currentIni.getMetaInfo().getPageSize(0), "test");
        tune.findPage().constant.add(new com.rusefi.tune.xml.Constant(
            "trigger_type", null, "\"not a trigger\"", null));

        Msq.ApplyResult result = tune.applyOntoWithReport(
            new ConfigurationImage(currentIni.getMetaInfo().getPageSize(0)), currentIni, null);

        assertFalse(result.getFatalErrors().isEmpty());
        assertTrue(result.getUnresolvedPins().isEmpty());
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
