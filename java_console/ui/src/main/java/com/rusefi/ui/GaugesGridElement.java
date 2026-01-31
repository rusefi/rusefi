package com.rusefi.ui;

import com.rusefi.core.Sensor;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.widgets.JPanelWithListener;
import com.rusefi.ui.widgets.SensorGauge;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;

/**
 * This panel contains either a {@link SensorGauge} or {@link SensorLiveGraph}
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 8/21/2015.
 */
public class GaugesGridElement {
    private static final String GAUGE_TYPE = "gauge";
    private static final String IS_LIVE_GRAPH = "type";

    private final JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
    private final UIContext uiContext;
    private final Node config;
    private final String gaugeName;

    private GaugesGridElement(UIContext uiContext, Node config, String gaugeName) {
        this.uiContext = uiContext;
        this.config = config;
        this.gaugeName = gaugeName;
    }

    private void rebuildAsLiveBarElement() {
        wrapper.setLayout(new GridLayout(2, 1));

        JMenuItem switchToGauge = getJMenuItem("Switch to Gauge Mode", false);

        wrapper.add(new SensorLiveGraph(uiContext, config.getChild("top"), gaugeName, switchToGauge));
        wrapper.add(new SensorLiveGraph(uiContext, config.getChild("bottom"), Sensor.RPMGauge.name(), switchToGauge));
    }

    private void rebuild() {
        // Clear existing components before rebuilding to prevent duplication on reconnect
        wrapper.removeAllChildrenAndListeners();
        if (config.getBoolProperty(IS_LIVE_GRAPH)) {
            rebuildAsLiveBarElement();
        } else {
            rebuildAsCircleGauge();
        }
    }

    private void rebuildAsCircleGauge() {
        SensorGauge.GaugeChangeListener gaugeChangeListener = new SensorGauge.GaugeChangeListener() {
            @Override
            public void onSensorChange(String gaugeName) {
                config.setProperty(GAUGE_TYPE, gaugeName);
            }
        };

        JMenuItem switchToLiveGraph = getJMenuItem("Switch to Live Graph", true);

        wrapper.setLayout(new BorderLayout());
        SensorGauge.createGaugeBody(uiContext, gaugeName, wrapper, gaugeChangeListener, switchToLiveGraph);

    }

    private @NotNull JMenuItem getJMenuItem(String menuText, boolean value) {
        JMenuItem switchToLiveGraph = new JMenuItem(menuText);
        switchToLiveGraph.addActionListener(e -> {
            wrapper.removeAllChildrenAndListeners();
            config.setBoolProperty(IS_LIVE_GRAPH, value);

            rebuild();
        });
        return switchToLiveGraph;
    }

    public JPanelWithListener getContent() {
        return wrapper;
    }

    public static JComponent create(UIContext uiContext, final Node config, String gaugeName) {
        GaugesGridElement gaugesGridElement = new GaugesGridElement(uiContext, config, gaugeName);
        ConnectionStatusLogic.INSTANCE.addAndFireListener(new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                SwingUtilities.invokeLater(() -> {
                    gaugesGridElement.rebuild();
                });
            }
        });
        return gaugesGridElement.getContent();
    }

    @Override
    public String toString() {
        return "GaugesGridElement{" +
            gaugeName +
            '}';
    }
}
