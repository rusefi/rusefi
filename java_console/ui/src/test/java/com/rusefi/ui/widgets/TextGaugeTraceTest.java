package com.rusefi.ui.widgets;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniValue;
import com.opensr5.ini.IniFileMetaInfo;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.JMenuItem;
import javax.swing.SwingUtilities;
import java.io.ByteArrayInputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/** A Tuning strip card draws its value's bar and history using the range and limits of its gauge. */
class TextGaugeTraceTest {
    private static UIContext withAfrGauge() {
        // afr1Gauge from gauge_declarations.ini
        GaugeModel afr = new GaugeModel("afr1Gauge", "AFRValue",
            IniValue.ofString("Air/Fuel Ratio"), IniValue.ofString("AFR"),
            IniValue.ofNumeric(10), IniValue.ofNumeric(19.4),
            IniValue.ofNumeric(12), IniValue.ofNumeric(13),
            IniValue.ofNumeric(15), IniValue.ofNumeric(16),
            IniValue.ofNumeric(2), IniValue.ofNumeric(2));
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.getGauge("afr1Gauge")).thenReturn(afr);
        UIContext uiContext = new UIContext();
        uiContext.setIniFileModelForOffline(ini);
        return uiContext;
    }

    @Test
    void cardTakesRangeAndLimitsFromItsGauge() {
        UIContext uiContext = withAfrGauge();

        TextGauge gauge = new TextGauge(uiContext, "afr1Gauge", new Node(), name -> { });
        try {
            assertTrue(gauge.getTraceForTests().hasRange());
            gauge.getTraceForTests().add(16.5);
            assertEquals(CircularGauge.Zone.DANGER, gauge.getTraceForTests().zone());
        } finally {
            gauge.destroy();
        }
    }

    @Test
    void unknownChannelDrawsNothing() {
        UIContext uiContext = new UIContext();
        uiContext.setIniFileModelForOffline(mock(IniFileModel.class));

        TextGauge gauge = new TextGauge(uiContext, "noSuchGauge", new Node(), name -> { });
        try {
            assertFalse(gauge.getTraceForTests().hasRange());
        } finally {
            gauge.destroy();
        }
    }

    @Test
    void scaleSettingsReplaceTheIniRangeAndAreKeptOnTheSlot() {
        UIContext uiContext = withAfrGauge();
        Node slot = new Node();
        TextGauge gauge = new TextGauge(uiContext, "afr1Gauge", slot, name -> { });
        try {
            assertEquals(TextGauge.SCALE, ((JMenuItem) gauge.createPopupMenu(uiContext, name -> { })
                .getComponent(0)).getText());

            gauge.setScaleOverrides(uiContext, new GaugeScaleOverrides(10.0, 20.0, 18.0, null));
            gauge.getTraceForTests().add(15);
            assertEquals(0.5, gauge.getTraceForTests().fraction(), 1e-9);
            // the .ini warning limit still applies inside the new range
            assertEquals(CircularGauge.Zone.WARNING, gauge.getTraceForTests().zone());
            gauge.getTraceForTests().add(18);
            assertEquals(CircularGauge.Zone.DANGER, gauge.getTraceForTests().zone());
            assertEquals(20.0, GaugeScaleOverrides.read(slot).getMax());
        } finally {
            gauge.destroy();
        }

        // a card restored from the same slot keeps the setting
        TextGauge restored = new TextGauge(uiContext, "afr1Gauge", slot, name -> { });
        try {
            restored.getTraceForTests().add(15);
            assertEquals(0.5, restored.getTraceForTests().fraction(), 1e-9);
        } finally {
            restored.destroy();
        }
    }

    @Test
    void noScaleItemForAGaugeTheIniDoesNotKnow() {
        UIContext uiContext = new UIContext();
        uiContext.setIniFileModelForOffline(mock(IniFileModel.class));
        TextGauge gauge = new TextGauge(uiContext, "noSuchGauge", new Node(), name -> { });
        try {
            assertEquals(0, gauge.createPopupMenu(uiContext, name -> { }).getComponentCount());
        } finally {
            gauge.destroy();
        }
    }

    @Test
    void aSteadyReadingStillFeedsTheHistory() throws Throwable {
        // an unplugged CLT sensor reads the same value in every frame
        String text = "[OutputChannels]\n"
            + "steadyChannel = scalar, F32, 0, \"C\", 1, 0\n"
            + "[GaugeConfigurations]\n"
            + "steadyGauge = steadyChannel, \"Coolant temp\", \"C\", -50, 140, -15, 1, 95, 110, 1, 1\n";
        IniFileModel ini = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))),
            "steady.ini", mock(IniFileMetaInfo.class));
        UIContext uiContext = new UIContext();
        uiContext.iniFileState.setIniFileModelForTest(ini);
        SensorCentral sensors = SensorCentral.getInstance();
        ConnectionStatusValue previous = ConnectionStatusLogic.INSTANCE.getValue();
        ConnectionStatusLogic.INSTANCE.setValue(ConnectionStatusValue.CONNECTED);
        TextGauge gauge = new TextGauge(uiContext, "steadyGauge", new Node(), name -> { });
        try {
            gauge.setActive(true);
            assertTrue(sensors.getOutputChannelDemand().getChannels().contains("steadychannel"));
            SwingUtilities.invokeAndWait(() -> { });
            int before = gauge.getTraceForTests().historySize();
            for (int i = 0; i < 3; i++) {
                // output-channel offsets follow the protocol response-code byte
                byte[] frame = ByteBuffer.allocate(5).order(ByteOrder.LITTLE_ENDIAN)
                    .put((byte) 0).putFloat(-41.5f).array();
                sensors.grabSensorValues(frame, ini, null);
                SwingUtilities.invokeAndWait(() -> { });
            }
            assertEquals(before + 3, gauge.getTraceForTests().historySize());
            // below the .ini range of -40 the bar would stay empty; this gauge starts at -50
            assertTrue(gauge.getTraceForTests().fraction() > 0);
        } finally {
            gauge.destroy();
            ConnectionStatusLogic.INSTANCE.setValue(previous);
        }
        assertFalse(sensors.getOutputChannelDemand().getChannels().contains("steadychannel"));
    }
}
