package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.SensorCentral;
import org.junit.jupiter.api.Test;

import java.util.Collections;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;

class IndicatorPanelOutputDemandTest {
    private static final String CHANNEL = "issue10170FrontIndicator";

    @Test
    void onlyActiveIndicatorDemandsExpressionChannels() {
        IndicatorModel indicator = new IndicatorModel(
            "{ " + CHANNEL + " > 0 }", "Off", "On", "black", "white", "green", "white");
        IndicatorPanel panel = new IndicatorPanel(
            Collections.singletonList(indicator), mock(IniFileModel.class), 0);

        try {
            assertFalse(hasDemand());
            panel.setActive(true);
            assertTrue(hasDemand());
            assertFalse(SensorCentral.getInstance().getOutputChannelDemand().getChannels().contains("off"));
            assertFalse(SensorCentral.getInstance().getOutputChannelDemand().getChannels().contains("on"));
            panel.setActive(false);
            assertFalse(hasDemand());
        } finally {
            panel.destroy();
        }
        panel.setActive(true);
        assertFalse(hasDemand());
    }

    private static boolean hasDemand() {
        return SensorCentral.getInstance().getOutputChannelDemand().getChannels()
            .contains(CHANNEL.toLowerCase());
    }
}
