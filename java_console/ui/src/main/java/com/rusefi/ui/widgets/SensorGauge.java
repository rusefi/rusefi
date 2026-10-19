package com.rusefi.ui.widgets;

import com.devexperts.logging.Logging;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCategory;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorSubscription;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.GaugesPanel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.HashSet;
import java.util.Set;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Round gauge
 * <p>
 * Double-click, or "Pop out to window" in the right-click menu, creates a {@link DetachedSensor}
 * <p>
 * Date: 7/9/14
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see GaugesPanel
 */

public class SensorGauge {
    private static final Logging log = getLogging(SensorGauge.class);

    public static final String CHANGE_GAUGE = "Change gauge";
    static final String POP_OUT = "Pop out to window";
    private static final String HINT_LINE_1 = "Double-click to detach";
    private static final String HINT_LINE_2 = "Right-click for options";

    public interface GaugeChangeListener {
        /**
         * This event happens when user decides to switch the kind of gauge
         * displayed by this control
         *
         * @param gaugeName new type
         */
        void onSensorChange(String gaugeName);
    }

    public static void createGaugeBody(UIContext uiContext, final String gaugeName, final JPanelWithListener wrapper, final GaugeChangeListener listener,
                                       final JMenuItem extraMenuItem) {

        IniFileModel iniFile = uiContext.iniFileState.getIniFileModel();
        if (iniFile != null) {
            GaugeModel gaugeModel = iniFile.getGauge(gaugeName);
            if (gaugeModel != null) {
                createGauge(uiContext, gaugeName, wrapper, listener, extraMenuItem, gaugeModel);
            } else {
                log.warn("Gauge not found: " + gaugeName);
            }
        } else {
            wrapper.removeAllChildrenAndListeners();
            wrapper.add(new JLabel("Connecting..."));
            log.warn("Gauge not found by " + gaugeName);
        }
    }

    static void createGauge(UIContext uiContext, String gaugeName, JPanelWithListener wrapper, GaugeChangeListener listener, JMenuItem extraMenuItem,
                            GaugeModel gaugeModel) {
        BinaryProtocol protocol = uiContext.getBinaryProtocol();
        final CircularGauge gauge = new CircularGauge(gaugeModel, uiContext.iniFileState.getIniFileModel(),
            protocol == null ? null : protocol.getControllerConfiguration());
        UiUtils.setToolTip(gauge, HINT_LINE_1, HINT_LINE_2);
        UiUtils.setToolTip(wrapper, HINT_LINE_1, HINT_LINE_2);

        String channelName = gaugeModel.getChannel();
        ISensorCentral.ListenerToken valueToken = SensorCentral.getInstance().addListener(channelName, gauge::setValue);

        // Expression-based labels stay empty until the data cycle resolves them.
        final SensorCentral.ResponseListenerToken responseToken;
        boolean dynamicTitle = gaugeModel.getTitleValue().isExpression()
            && TsStringFunction.containsStringFunction(gaugeModel.getTitle());
        boolean dynamicUnits = gaugeModel.getUnitsValue().isExpression()
            && TsStringFunction.containsStringFunction(gaugeModel.getUnits());
        if (dynamicTitle || dynamicUnits) {
            Set<String> labelsSensors = new HashSet<>();
            if (dynamicTitle) {
                gauge.setTitle("");
                labelsSensors.addAll(ExpressionEvaluator.extractVariables(gaugeModel.getTitle()));
            }
            if (dynamicUnits) {
                gauge.setUnitString("");
                labelsSensors.addAll(ExpressionEvaluator.extractVariables(gaugeModel.getUnits()));
            }
            SensorCentral.ResponseListener responseListener = () -> SwingUtilities.invokeLater(() -> {
                ISensorHolder.ResolvedGaugeLabels labels = SensorCentral.getInstance().getResolvedLabels(gaugeName);
                if (labels != null) {
                    gauge.setTitle(labels.getTitle());
                    gauge.setUnitString(labels.getUnits());
                }
            });
            responseToken = SensorCentral.getInstance().addListener(
                responseListener, new SensorSubscription(labelsSensors.toArray(new String[0])));
        } else {
            responseToken = null;
        }

        gauge.setValue(SensorCentral.getInstance().getValue(channelName));
        MouseListener mouseListener = new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    createPopupMenu(uiContext, wrapper, listener, extraMenuItem,
                        () -> popOut(uiContext, e, gaugeName)).show(e.getComponent(), e.getX(), e.getY());
                } else if (e.getClickCount() == 2) {
                    popOut(uiContext, e, gaugeName);
                }
            }
        };
        gauge.addMouseListener(mouseListener);
        // Remove previous gauge's children and run its cleanup actions (removes its
        // SensorCentral listeners), then register cleanup for the new listeners above.
        wrapper.removeAllChildrenAndListeners();
        wrapper.addCleanupAction(valueToken::remove);
        wrapper.addActiveStateAction(valueToken::setActive);
        if (responseToken != null) {
            wrapper.addCleanupAction(responseToken::remove);
            wrapper.addActiveStateAction(responseToken::setActive);
        }
        wrapper.addMouseListener(mouseListener);
        wrapper.add(gauge, BorderLayout.CENTER);
        AutoupdateUtil.trueLayoutAndRepaint(wrapper.getParent());
    }

    /**
     * Search, then "Change gauge", the caller's display options, and "Pop out to window".
     * Package-private so tests inspect the same menu users see.
     */
    static JPopupMenu createPopupMenu(UIContext uiContext, JPanelWithListener wrapper, GaugeChangeListener listener,
                                      JMenuItem extraMenuItem, Runnable popOut) {
        JPopupMenu pm = new JPopupMenu();
        JMenu changeGauge = new JMenu(CHANGE_GAUGE);
        fillGaugeMenuItems(uiContext, changeGauge, wrapper, listener, extraMenuItem);
        pm.add(changeGauge);
        if (extraMenuItem != null) {
            pm.add(extraMenuItem);
        }
        pm.addSeparator();
        JMenuItem popOutItem = new JMenuItem(POP_OUT);
        popOutItem.setToolTipText("Open this gauge in its own always-on-top window (also: double-click)");
        popOutItem.addActionListener(e -> popOut.run());
        pm.add(popOutItem);
        GaugeMenuSearch.addTo(pm, changeGauge);
        return pm;
    }

    static void fillGaugeMenuItems(UIContext uiContext, JComponent popupMenu, final JPanelWithListener wrapper, final GaugeChangeListener listener, final JMenuItem extraMenuItem) {
        addGaugeCatalog(uiContext, popupMenu, gaugeName -> {
            createGaugeBody(uiContext, gaugeName, wrapper, listener, extraMenuItem);
            listener.onSensorChange(gaugeName);
        });
    }

    /** Adds one submenu per gauge category; choosing a gauge passes its name to {@code onSelect}. */
    public static void addGaugeCatalog(UIContext uiContext, JComponent menu, Consumer<String> onSelect) {
        IniFileModel iniFile = uiContext.iniFileState.getIniFileModel();
        if (iniFile != null) {
            for (final String category : iniFile.getGaugeCategories().keySet()) {
                JMenuItem cmi = new JMenu(category);
                menu.add(cmi);
                for (final GaugeModel gauge : iniFile.getGaugeCategories().get(category).getGauges()) {
                    JMenuItem mi = new JMenuItem(GaugeMenuLabel.getTitle(uiContext, gauge));
                    mi.addActionListener(e -> onSelect.accept(gauge.getName()));
                    cmi.add(mi);
                }
            }
            return;
        }

        for (final SensorCategory sc : SensorCategory.values()) {
            JMenuItem cmi = new JMenu(sc.getName());
            menu.add(cmi);

            for (final Sensor s : Sensor.getSensorsForCategory(sc.getName())) {
                JMenuItem mi = new JMenuItem(s.getName());
                mi.addActionListener(e -> onSelect.accept(s.name()));
                cmi.add(mi);
            }
        }
    }

    private static void popOut(UIContext uiContext, MouseEvent e, String gaugeName) {
        new DetachedSensor(uiContext, gaugeName, DetachedSensor.DEFAULT_WIDTH).show(e);
    }

}
