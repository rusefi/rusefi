package com.rusefi.ui.widgets;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniValue;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/** A Tuning strip card draws its value's bar and history using the range and limits of its gauge. */
class TextGaugeTraceTest {
    @Test
    void cardTakesRangeAndLimitsFromItsGauge() {
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

        TextGauge gauge = new TextGauge(uiContext, "afr1Gauge", name -> { });
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

        TextGauge gauge = new TextGauge(uiContext, "noSuchGauge", name -> { });
        try {
            assertFalse(gauge.getTraceForTests().hasRange());
        } finally {
            gauge.destroy();
        }
    }
}
