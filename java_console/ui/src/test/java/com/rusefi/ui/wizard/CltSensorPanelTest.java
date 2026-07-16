package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.SensorCentral;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import java.io.InputStream;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.*;

class CltSensorPanelTest {
    @Test
    void validatesCustomCalibration() {
        Map<String, String> valid = calibration("-20", "18000", "24", "2100", "120", "100");

        assertNull(CltSensorPanel.validate("D16", valid, "2490"));
        assertEquals("Select a CLT analog input.", CltSensorPanel.validate("NONE", valid, "2490"));

        Map<String, String> unordered = calibration("20", "18000", "10", "2100", "120", "100");
        assertEquals("Temperatures must increase from point 1 to point 3.",
            CltSensorPanel.validate("D16", unordered, "2490"));

        Map<String, String> invalidResistance = calibration("-20", "0", "24", "2100", "120", "100");
        assertEquals("Resistance values must be between 0 and 1,000,000 ohms.",
            CltSensorPanel.validate("D16", invalidResistance, "2490"));
        assertEquals("Pullup resistance must be between 0 and 200,000 ohms.",
            CltSensorPanel.validate("D16", valid, "NaN"));

        Map<String, String> duplicateResistance = calibration("-20", "2100", "24", "2100", "120", "100");
        assertEquals("Resistance values must be different at each point.",
            CltSensorPanel.validate("D16", duplicateResistance, "2490"));

        Map<String, String> outOfRangeTemperature = calibration("-56", "18000", "24", "2100", "120", "100");
        assertEquals("Temperatures must be between -55 and 200 °C.",
            CltSensorPanel.validate("D16", outOfRangeTemperature, "2490"));

        Map<String, String> invalidCurve = calibration("-13.9", "73300", "23.5", "53100", "60", "2280");
        assertEquals("Calibration points do not produce a valid thermistor curve.",
            CltSensorPanel.validate("D16", invalidCurve, "2490"));

        Map<String, String> validCurve = calibration("-13.9", "73300", "23.7", "10630", "60", "2280");
        assertNull(CltSensorPanel.validate("D16", validCurve, "2490"));
    }

    @Test
    void appliesIniPresetWithoutMutatingSource() throws Throwable {
        UIContext context = WizardSandbox.createOfflineContext(loadTestIni());
        WizardConfig cfg = WizardConfig.snapshot(context);
        assertNotNull(cfg);
        DialogModel.SettingOption gm = cfg.ini.getDialogs().get(CltSensorPanel.DIALOG_KEY)
            .getSettingSelectors().get(0).getOptions().get(0);
        IniField firstTemperature = cfg.ini.findIniField("clt_tempC_1").orElseThrow();

        com.opensr5.ConfigurationImage modified = cfg.image.clone();
        CltSensorPanel.applyAssignments(cfg.ini, modified, gm.getAssignments());

        assertEquals("-40.0", ConfigurationImageGetterSetter.getStringValue(firstTemperature, modified));
        assertEquals("0.0", ConfigurationImageGetterSetter.getStringValue(firstTemperature, cfg.image));
        assertEquals("\"no\"", ConfigurationImageGetterSetter.getStringValue(
            cfg.ini.findIniField(CltSensorPanel.LINEAR_FIELD).orElseThrow(), modified));
    }

    @Test
    void liveListenersAreRemovedWhenPanelHides() throws Throwable {
        UIContext context = WizardSandbox.createOfflineContext(loadTestIni());
        ISensorCentral sensors = mock(ISensorCentral.class);
        SensorCentral.ListenerToken voltageToken = mock(SensorCentral.ListenerToken.class);
        SensorCentral.ListenerToken cltToken = mock(SensorCentral.ListenerToken.class);
        when(sensors.addListener(anyString(), any())).thenReturn(voltageToken, cltToken);
        when(sensors.getValue(anyString())).thenReturn(Double.NaN);
        CltSensorPanel panel = new CltSensorPanel(context, sensors);

        panel.onShow();
        assertTrue(panel.areLiveListenersActiveForTests());
        verify(sensors).addListener(eq(CltSensorPanel.RAW_CLT_CHANNEL), any());
        verify(sensors).addListener(eq(CltSensorPanel.CLT_CHANNEL), any());

        panel.onHide();
        assertFalse(panel.areLiveListenersActiveForTests());
        verify(voltageToken).remove();
        verify(cltToken).remove();
    }

    @Test
    void unmatchedCalibrationStartsInCustomMode() throws Throwable {
        CltSensorPanel panel = new CltSensorPanel(WizardSandbox.createOfflineContext(loadTestIni()));

        assertTrue(panel.isCustomModeVisibleForTests());
        assertEquals(CltSensorPanel.CARD_WIDTH, panel.getCardForTests().getPreferredSize().width);
        assertEquals(CltSensorPanel.CARD_WIDTH, panel.getCardForTests().getMaximumSize().width);
    }

    @Test
    void outOfRangeStoredAdcChannelRequiresNewSelection() {
        TreeMap<Integer, String> values = new TreeMap<>();
        for (int i = 0; i < 15; i++) {
            values.put(i, "Channel " + i);
        }
        EnumIniField field = new EnumIniField("clt_adcChannel", 0, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(values), 0, 7);

        assertNull(AbstractWizardStep.readValue(field, new ConfigurationImage(new byte[]{16})));
    }

    @Test
    void containerStepOrderMatchesCatalog() throws Throwable {
        WizardContainer container = new WizardContainer(WizardSandbox.createOfflineContext(loadTestIni()));
        container.startWizard();

        assertEquals(
            WizardCatalog.flaggedSteps().stream().map(step -> step.flagName).collect(Collectors.toList()),
            container.assembledStepFlagsForTests());
    }

    private static Map<String, String> calibration(String t1, String r1, String t2, String r2,
                                                    String t3, String r3) {
        Map<String, String> values = new LinkedHashMap<>();
        values.put("clt_tempC_1", t1);
        values.put("clt_resistance_1", r1);
        values.put("clt_tempC_2", t2);
        values.put("clt_resistance_2", r2);
        values.put("clt_tempC_3", t3);
        values.put("clt_resistance_3", r3);
        return values;
    }

    private static IniFileModel loadTestIni() throws Throwable {
        try (InputStream stream = CltSensorPanelTest.class.getResourceAsStream("/january.ini")) {
            assertNotNull(stream);
            RawIniFile content = IniFileReaderUtil.read(stream, "/january.ini");
            return IniFileReaderUtil.readIniFile(content, "/january.ini", new IniFileMetaInfoImpl(content));
        }
    }
}
