package com.rusefi.ui;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.IniValue;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.WellKnownGauges;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class GaugesPanelOutputDemandTest {
    private static final String CHANNEL = "issue10170GaugeChannel";

    @Test
    void onlyActivePanelDemandsItsGaugeChannels() throws Exception {
        IniFileModelMocks.GaugeRegistry gauges = IniFileModelMocks.mutableWithGauges();
        gauges.register(new GaugeModel(
            "RPMGauge", CHANNEL,
            IniValue.ofExpression("RPM - engine speed"),
            IniValue.ofExpression("{ bitStringValue(labels, useMetricOnInterface) }"),
            IniValue.ofNumeric(0), IniValue.ofNumeric(8000),
            IniValue.ofNumeric(0), IniValue.ofNumeric(1000),
            IniValue.ofNumeric(7000), IniValue.ofNumeric(8000),
            IniValue.ofNumeric(0), IniValue.ofNumeric(0)));

        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(gauges.model);
        Node config = new Node();
        config.setProperty("gauges_rows", 1);
        config.setProperty("gauges_cols", 1);

        GaugesPanel[] holder = new GaugesPanel[1];
        SwingUtilities.invokeAndWait(() -> holder[0] = new GaugesPanel(context, config));
        SwingUtilities.invokeAndWait(() -> { });
        GaugesPanel panel = holder[0];

        assertFalse(hasChannelDemand());
        assertFalse(hasDemand(WellKnownGauges.RPMGauge.getOutputChannelName()));
        SwingUtilities.invokeAndWait(() -> panel.setActive(true));
        assertTrue(hasChannelDemand());
        assertTrue(hasDemand(WellKnownGauges.RPMGauge.getOutputChannelName()));
        assertTrue(hasDemand("useMetricOnInterface"));
        assertFalse(hasDemand("engine"));
        assertFalse(hasDemand("rpm"));
        assertFalse(hasDemand("speed"));
        SwingUtilities.invokeAndWait(() -> panel.setActive(false));
        assertFalse(hasChannelDemand());
        assertFalse(hasDemand(WellKnownGauges.RPMGauge.getOutputChannelName()));

        SwingUtilities.invokeAndWait(panel::destroy);
        assertFalse(hasChannelDemand());
    }

    private static boolean hasChannelDemand() {
        return hasDemand(CHANNEL);
    }

    private static boolean hasDemand(String channel) {
        return SensorCentral.getInstance().getOutputChannelDemand().getChannels()
            .contains(channel.toLowerCase());
    }
}
