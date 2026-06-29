package com.rusefi.ui.widgets;

import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Horizontal strip of 8 text-only gauges for the Tuning view.
 * Always visible so the user has live data while tuning.
 * Defaults are taken from the INI frontpage gauge section.
 */
public class TextGaugeStrip {
    private static final String GAUGE_CHANNEL = "channel";
    private static final int GAUGE_COUNT = 8;

    private final JPanel content = new JPanel();
    private final List<TextGauge> gauges = new ArrayList<>();

    public TextGaugeStrip(UIContext uiContext, Node config) {

        content.setLayout(new BoxLayout(content, BoxLayout.X_AXIS));
        content.setBorder(BorderFactory.createEmptyBorder(2, 4, 2, 4));

        List<String> defaults = getFrontPageDefaults(uiContext);

        for (int i = 0; i < GAUGE_COUNT; i++) {
            final int index = i;
            String channel = config.getChild("gauge_" + index).getProperty(GAUGE_CHANNEL, defaults.get(index));
            TextGauge gauge = new TextGauge(uiContext, channel, newName -> {
                config.getChild("gauge_" + index).setProperty(GAUGE_CHANNEL, newName);
            });
            gauges.add(gauge);
            content.add(gauge.getContent());
            if (i < GAUGE_COUNT - 1) {
                content.add(Box.createHorizontalStrut(2));
            }
        }
    }

    private static List<String> getFrontPageDefaults(UIContext uiContext) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini != null) {
            FrontPageModel frontPage = ini.getFrontPage();
            if (frontPage != null && !frontPage.getGaugeNames().isEmpty()) {
                List<String> result = new ArrayList<>(frontPage.getGaugeNames());
                while (result.size() < GAUGE_COUNT) {
                    result.add(result.get(result.size() - 1));
                }
                return result.subList(0, GAUGE_COUNT);
            }
        }
        return FALLBACK_GAUGES;
    }

    private static final List<String> FALLBACK_GAUGES = List.of(
        "RPMGauge", "CLTGauge", "TPSGauge", "MAPGauge",
        "afr1Gauge", "VBattGauge", "dwellGauge", "correctedIgnAdvGauge"
    );

    public JPanel getContent() {
        return content;
    }

    public void destroy() {
        for (TextGauge gauge : gauges) {
            gauge.destroy();
        }
        gauges.clear();
    }
}
