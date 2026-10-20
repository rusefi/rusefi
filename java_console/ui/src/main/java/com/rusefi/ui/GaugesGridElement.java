package com.rusefi.ui;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.core.Sensor;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.widgets.GaugeDisplayKind;
import com.rusefi.ui.widgets.GaugeMenuLabel;
import com.rusefi.ui.widgets.GaugeScaleOverrides;
import com.rusefi.ui.widgets.JPanelWithListener;
import com.rusefi.ui.widgets.SensorGauge;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.util.Arrays;

/**
 * This panel contains either a {@link SensorGauge} or {@link SensorLiveGraph}
 * <p>
 * Andrey Belomutskiy, (c) 2013-2020
 * 8/21/2015.
 */
public class GaugesGridElement {
    private static final String GAUGE_TYPE = "gauge";
    /** Legacy flag, still written so older consoles show live graphs (other kinds fall back to round). */
    private static final String IS_LIVE_GRAPH = "type";
    static final String DISPLAY = "display";
    static final String DISPLAY_AS = "Display as";
    static final String SCALE = "Scale\u2026";

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

        addLiveGraph(new SensorLiveGraph(uiContext, config.getChild("top"), config.getProperty(GAUGE_TYPE),
            createDisplayAsMenu(GaugeDisplayKind.LIVE_GRAPH)));
        addLiveGraph(new SensorLiveGraph(uiContext, config.getChild("bottom"), Sensor.RPMGauge.name(),
            createDisplayAsMenu(GaugeDisplayKind.LIVE_GRAPH)));
        // Lay the graphs out now, as the round gauge path does; otherwise they stay zero-sized
        // until some later layout pass and the empty graph appears with a delay.
        wrapper.doLayout();
        AutoupdateUtil.trueLayoutAndRepaint(wrapper.getParent());
        wrapper.repaint();
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
        GaugeDisplayKind kind = getDisplayKind();
        if (kind == GaugeDisplayKind.LIVE_GRAPH) {
            rebuildAsLiveBarElement();
        } else {
            rebuildAsGauge(kind);
        }
    }

    GaugeDisplayKind getDisplayKind() {
        GaugeDisplayKind kind = GaugeDisplayKind.lookup(config.getProperty(DISPLAY, null));
        if (kind != null) {
            return kind;
        }
        return config.getBoolProperty(IS_LIVE_GRAPH) ? GaugeDisplayKind.LIVE_GRAPH : GaugeDisplayKind.ROUND;
    }

    private void rebuildAsGauge(GaugeDisplayKind kind) {
        SensorGauge.GaugeChangeListener gaugeChangeListener = new SensorGauge.GaugeChangeListener() {
            @Override
            public void onSensorChange(String gaugeName) {
                config.setProperty(GAUGE_TYPE, gaugeName);
                // Scale settings were chosen for the previous channel.
                GaugeScaleOverrides.clear(config);
            }
        };

        wrapper.setLayout(new BorderLayout());
        SensorGauge.createGaugeBody(uiContext, config.getProperty(GAUGE_TYPE), wrapper, gaugeChangeListener,
            Arrays.asList(createDisplayAsMenu(kind), createScaleMenuItem()), kind, GaugeScaleOverrides.read(config));
    }

    /** "Scale…" item: replaces the .ini min, max, danger value and decimals of this slot's gauge. */
    // Package-private so tests drive the same menu users see.
    @NotNull JMenuItem createScaleMenuItem() {
        JMenuItem item = new JMenuItem(SCALE);
        item.setToolTipText("Set this gauge's min, max, danger value and decimals");
        item.addActionListener(e -> {
            GaugeScaleDialog dialog = createScaleDialog();
            if (dialog == null) {
                return;
            }
            GaugeScaleOverrides overrides = dialog.show(wrapper);
            if (overrides != null) {
                setScaleOverrides(overrides);
            }
        });
        return item;
    }

    /** The dialog for the slot's current gauge, or {@code null} while its .ini definition is unknown. */
    GaugeScaleDialog createScaleDialog() {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        GaugeModel model = ini == null ? null : ini.getGauge(config.getProperty(GAUGE_TYPE));
        if (model == null) {
            return null;
        }
        String units = model.getUnitsValue().isExpression() ? null : model.getUnits();
        return new GaugeScaleDialog(GaugeMenuLabel.getTitle(uiContext, model), units,
            SensorGauge.iniScale(uiContext, model), GaugeScaleOverrides.read(config));
    }

    void setScaleOverrides(GaugeScaleOverrides overrides) {
        overrides.writeTo(config);
        rebuild();
    }

    /** "Display as" submenu with the current presentation checked. */
    // Package-private so tests drive the same menu users see.
    @NotNull JMenu createDisplayAsMenu(GaugeDisplayKind current) {
        JMenu menu = new JMenu(DISPLAY_AS);
        ButtonGroup group = new ButtonGroup();
        for (GaugeDisplayKind kind : GaugeDisplayKind.values()) {
            JRadioButtonMenuItem item = new JRadioButtonMenuItem(kind.getLabel(), kind == current);
            group.add(item);
            item.addActionListener(e -> setDisplayKind(kind));
            menu.add(item);
        }
        return menu;
    }

    void setDisplayKind(GaugeDisplayKind kind) {
        if (kind == getDisplayKind()) {
            return;
        }
        wrapper.removeAllChildrenAndListeners();
        config.setProperty(DISPLAY, kind.getKey());
        config.setBoolProperty(IS_LIVE_GRAPH, kind == GaugeDisplayKind.LIVE_GRAPH);
        rebuild();
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
