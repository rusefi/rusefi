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
    static final String DISPLAY_AS = "Display as";
    static final String ROUND_GAUGE = "Round gauge";
    static final String LIVE_GRAPH = "Live graph";

    private final JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
    private final UIContext uiContext;
    private final Node config;
    private final String gaugeName;
    private volatile boolean destroyed;

    private GaugesGridElement(UIContext uiContext, Node config, String gaugeName) {
        this.uiContext = uiContext;
        this.config = config;
        this.gaugeName = gaugeName;
        config.setProperty(GAUGE_TYPE, config.getProperty(GAUGE_TYPE, gaugeName));
    }

    private void rebuildAsLiveBarElement() {
        wrapper.setLayout(new GridLayout(2, 1));

        addLiveGraph(new SensorLiveGraph(uiContext, config.getChild("top"), config.getProperty(GAUGE_TYPE), createDisplayAsMenu(true)));
        addLiveGraph(new SensorLiveGraph(uiContext, config.getChild("bottom"), Sensor.RPMGauge.name(), createDisplayAsMenu(true)));
    }

    private void addLiveGraph(SensorLiveGraph graph) {
        wrapper.addCleanupAction(graph::destroy);
        wrapper.addActiveStateAction(graph::setActive);
        wrapper.add(graph);
    }

    private void rebuild() {
        if (destroyed) {
            return;
        }
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

        wrapper.setLayout(new BorderLayout());
        SensorGauge.createGaugeBody(uiContext, config.getProperty(GAUGE_TYPE), wrapper, gaugeChangeListener,
            createDisplayAsMenu(false));

    }

    /** "Display as" submenu with the current presentation checked. */
    // Package-private so tests drive the same menu users see.
    @NotNull JMenu createDisplayAsMenu(boolean liveGraph) {
        JMenu menu = new JMenu(DISPLAY_AS);
        ButtonGroup group = new ButtonGroup();
        menu.add(createDisplayAsItem(ROUND_GAUGE, false, liveGraph, group));
        menu.add(createDisplayAsItem(LIVE_GRAPH, true, liveGraph, group));
        return menu;
    }

    private @NotNull JMenuItem createDisplayAsItem(String text, boolean value, boolean current, ButtonGroup group) {
        JRadioButtonMenuItem item = new JRadioButtonMenuItem(text, value == current);
        group.add(item);
        item.addActionListener(e -> {
            if (value == current) {
                return;
            }
            wrapper.removeAllChildrenAndListeners();
            config.setBoolProperty(IS_LIVE_GRAPH, value);

            rebuild();
        });
        return item;
    }

    public JPanelWithListener getContent() {
        return wrapper;
    }

    public void setActive(boolean active) {
        wrapper.setActive(active);
    }

    /**
     * Remove this element's {@link ConnectionStatusLogic} listener and tear down its inner components.
     * Must be called before discarding a {@code GaugesGridElement} (e.g. on grid resize)
     * to prevent stale listeners from accumulating and keeping gauge widgets alive.
     */
    public void destroy() {
        destroyed = true;
        if (connectionStatusListener != null) {
            ConnectionStatusLogic.INSTANCE.removeListener(connectionStatusListener);
            connectionStatusListener = null;
        }
        wrapper.removeAllChildrenAndListeners();
    }

    private ConnectionStatusLogic.Listener connectionStatusListener;

    public static GaugesGridElement create(UIContext uiContext, final Node config, String gaugeName) {
        GaugesGridElement gaugesGridElement = new GaugesGridElement(uiContext, config, gaugeName);
        gaugesGridElement.connectionStatusListener = isConnected -> {
            if (SwingUtilities.isEventDispatchThread()) {
                gaugesGridElement.rebuild();
            } else {
                SwingUtilities.invokeLater(gaugesGridElement::rebuild);
            }
        };
        ConnectionStatusLogic.INSTANCE.addAndFireListener(gaugesGridElement.connectionStatusListener);
        return gaugesGridElement;
    }

    @Override
    public String toString() {
        return "GaugesGridElement{" +
            gaugeName +
            '}';
    }
}
