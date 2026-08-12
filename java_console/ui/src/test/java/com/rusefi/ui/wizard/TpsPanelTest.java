package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.config.FieldType;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.SensorCentral;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import java.io.InputStream;
import java.util.TreeMap;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.*;

class TpsPanelTest {
    @Test
    void validatesManualEndpointsInEitherDirection() {
        assertNull(TpsPanel.validate("PA0", "0.5", "4.5"));
        assertNull(TpsPanel.validate("PA0", "4.5", "0.5"));
        assertNull(TpsPanel.validate("PA0", "0.5", "0.6"));
        assertEquals("Select a TPS analog input.", TpsPanel.validate("Disabled", "0.5", "4.5"));
        assertEquals("Enter valid Closed and WOT voltages.", TpsPanel.validate("PA0", "NaN", "4.5"));
        assertEquals("TPS voltages must be between 0 and 5 V.", TpsPanel.validate("PA0", "-0.1", "4.5"));
        assertEquals("Closed and WOT voltages must differ by at least 0.1 V.",
            TpsPanel.validate("PA0", "1.00", "1.05"));
    }

    @Test
    void liveListenersAreRemovedWhenPanelHides() throws Throwable {
        UIContext context = WizardSandbox.createOfflineContext(loadTestIni());
        ISensorCentral sensors = mock(ISensorCentral.class);
        SensorCentral.ListenerToken voltageToken = mock(SensorCentral.ListenerToken.class);
        SensorCentral.ListenerToken tpsToken = mock(SensorCentral.ListenerToken.class);
        when(sensors.addListener(anyString(), any())).thenReturn(voltageToken, tpsToken);
        when(sensors.getValue(anyString())).thenReturn(Double.NaN);
        TpsPanel panel = new TpsPanel(context, sensors);

        panel.onShow();
        assertTrue(panel.areLiveListenersActiveForTests());
        verify(sensors).addListener(eq(TpsPanel.RAW_TPS_CHANNEL), any());
        verify(sensors).addListener(eq(TpsPanel.TPS_CHANNEL), any());

        panel.onHide();
        assertFalse(panel.areLiveListenersActiveForTests());
        verify(voltageToken).remove();
        verify(tpsToken).remove();
    }

    @Test
    void cardUsesBoundedScreenshotWidth() throws Throwable {
        TpsPanel panel = new TpsPanel(WizardSandbox.createOfflineContext(loadTestIni()));

        assertEquals(TpsPanel.CARD_WIDTH, panel.getCardForTests().getPreferredSize().width);
        assertEquals(TpsPanel.CARD_WIDTH, panel.getCardForTests().getMaximumSize().width);
    }

    @Test
    void grabButtonsCaptureCurrentVoltage() throws Throwable {
        ISensorCentral sensors = mock(ISensorCentral.class);
        when(sensors.getValue(TpsPanel.RAW_TPS_CHANNEL)).thenReturn(0.512, 4.487, Double.NaN);
        TpsPanel panel = new TpsPanel(WizardSandbox.createOfflineContext(loadTestIni()), sensors);

        panel.grabClosedForTests();
        panel.grabWotForTests();
        assertEquals("0.512", panel.getClosedVoltageForTests());
        assertEquals("4.487", panel.getWotVoltageForTests());

        panel.grabClosedForTests();
        assertEquals("0.512", panel.getClosedVoltageForTests());
    }

    @Test
    void outOfRangeStoredAdcChannelRequiresNewSelection() {
        TreeMap<Integer, String> values = new TreeMap<>();
        for (int i = 0; i < 15; i++) {
            values.put(i, "Channel " + i);
        }
        EnumIniField field = new EnumIniField("tps1_1AdcChannel", 0, FieldType.UINT8,
            new EnumIniField.EnumKeyValueMap(values), 0, 7);

        assertNull(TpsPanel.readEnum(field, new ConfigurationImage(new byte[]{16})));
    }

    private static IniFileModel loadTestIni() throws Throwable {
        try (InputStream stream = TpsPanelTest.class.getResourceAsStream("/january.ini")) {
            assertNotNull(stream);
            RawIniFile content = IniFileReaderUtil.read(stream, "/january.ini");
            return IniFileReaderUtil.readIniFile(content, "/january.ini", new IniFileMetaInfoImpl(content));
        }
    }
}
