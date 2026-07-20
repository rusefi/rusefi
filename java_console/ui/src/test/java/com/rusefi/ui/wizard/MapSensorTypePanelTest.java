package com.rusefi.ui.wizard;

import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.SensorCentral;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;

import javax.swing.*;
import java.io.InputStream;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.*;

class MapSensorTypePanelTest {
    @Test
    void validatesCustomCalibrationOnlyForCustomSensors() {
        assertNull(MapSensorTypePanel.validate("PA0", "MPX4250", "", "", "", ""));
        assertNull(MapSensorTypePanel.validate("PA0", "Custom", "0.5", "20", "4.5", "250"));
        assertEquals("Select a MAP analog input.",
            MapSensorTypePanel.validate("Disabled", "MPX4250", "", "", "", ""));
        assertEquals("Select a MAP sensor model.",
            MapSensorTypePanel.validate("PA0", null, "", "", "", ""));
        assertEquals("Enter valid voltage and pressure values.",
            MapSensorTypePanel.validate("PA0", "Custom", "bad", "20", "4.5", "250"));
        assertEquals("MAP voltages must be between 0 and 5 V.",
            MapSensorTypePanel.validate("PA0", "Custom", "-0.1", "20", "4.5", "250"));
        assertEquals("Low and high calibration points must be different.",
            MapSensorTypePanel.validate("PA0", "Custom", "0.5", "20", "0.5", "250"));
    }

    @Test
    void usesTpsStyleCardAndShowsCustomCalibration() throws Throwable {
        MapSensorTypePanel panel = new MapSensorTypePanel(WizardSandbox.createOfflineContext(loadTestIni()));

        assertEquals(MapSensorTypePanel.CARD_WIDTH, panel.getCardForTests().getPreferredSize().width);
        assertEquals(MapSensorTypePanel.CARD_WIDTH, panel.getCardForTests().getMaximumSize().width);
        assertTrue(panel.isCustomCalibrationVisibleForTests());

        panel.selectSensorTypeForTests("MPX4250");
        assertFalse(panel.isCustomCalibrationVisibleForTests());
        panel.selectSensorTypeForTests("Custom");
        assertTrue(panel.isCustomCalibrationVisibleForTests());
    }

    @Test
    void liveListenersAreRemovedWhenPanelHides() throws Throwable {
        UIContext context = WizardSandbox.createOfflineContext(loadTestIni());
        ISensorCentral sensors = mock(ISensorCentral.class);
        SensorCentral.ListenerToken voltageToken = mock(SensorCentral.ListenerToken.class);
        SensorCentral.ListenerToken mapToken = mock(SensorCentral.ListenerToken.class);
        SensorCentral.ListenerToken validityToken = mock(SensorCentral.ListenerToken.class);
        when(sensors.addListener(anyString(), any())).thenReturn(voltageToken, mapToken, validityToken);
        when(sensors.getValue(anyString())).thenReturn(Double.NaN);
        MapSensorTypePanel panel = new MapSensorTypePanel(context, sensors);

        panel.onShow();
        assertTrue(panel.areLiveListenersActiveForTests());
        verify(sensors).addListener(eq(MapSensorTypePanel.RAW_MAP_CHANNEL), any());
        verify(sensors).addListener(eq(MapSensorTypePanel.MAP_CHANNEL), any());
        ArgumentCaptor<SensorCentral.SensorListener> validityListener =
            ArgumentCaptor.forClass(SensorCentral.SensorListener.class);
        verify(sensors).addListener(eq(MapSensorTypePanel.MAP_VALID_CHANNEL), validityListener.capture());

        validityListener.getValue().onSensorUpdate(1);
        SwingUtilities.invokeAndWait(() -> { });
        assertEquals("Good MAP Input", panel.getMapValidityTextForTests());
        validityListener.getValue().onSensorUpdate(0);
        SwingUtilities.invokeAndWait(() -> { });
        assertEquals("BAD Map Input", panel.getMapValidityTextForTests());

        panel.onHide();
        assertFalse(panel.areLiveListenersActiveForTests());
        verify(voltageToken).remove();
        verify(mapToken).remove();
        verify(validityToken).remove();
    }

    private static IniFileModel loadTestIni() throws Throwable {
        try (InputStream stream = MapSensorTypePanelTest.class.getResourceAsStream("/january.ini")) {
            assertNotNull(stream);
            RawIniFile content = IniFileReaderUtil.read(stream, "/january.ini");
            return IniFileReaderUtil.readIniFile(content, "/january.ini", new IniFileMetaInfoImpl(content));
        }
    }
}
