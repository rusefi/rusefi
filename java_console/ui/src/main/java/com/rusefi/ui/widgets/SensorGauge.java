package com.rusefi.ui.widgets;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
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
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Round gauge
 * <p>
 * Right-click changes the gauge; the gear shown on hover holds its display options.
 * Double-click, or "Pop out to window" on the gear, creates a {@link DetachedSensor}
 * <p>
 * Date: 7/9/14
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see GaugesPanel
 */

public class SensorGauge {
    private static final Logging log = getLogging(SensorGauge.class);

    static final String POP_OUT = "Pop out to window";
    private static final String HINT_LINE_1 = "Right-click to change gauge, double-click to detach";
    private static final String HINT_LINE_2 = "Gear (top right) for display options";

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
        createGaugeBody(uiContext, gaugeName, wrapper, listener, menuItems(extraMenuItem), GaugeDisplayKind.ROUND,
            GaugeScaleOverrides.NONE);
    }

    /**
     * @param extraMenuItems added to the gear menu before "Pop out to window"
     * @param kind round, text or bar; live graphs are built by their own class
     * @param overrides the user's replacements for the .ini range, danger value and decimals
     */
    public static void createGaugeBody(UIContext uiContext, final String gaugeName, final JPanelWithListener wrapper, final GaugeChangeListener listener,
                                       final List<JMenuItem> extraMenuItems, GaugeDisplayKind kind,
                                       GaugeScaleOverrides overrides) {

        IniFileModel iniFile = uiContext.iniFileState.getIniFileModel();
        if (iniFile != null) {
            GaugeModel gaugeModel = iniFile.getGauge(gaugeName);
            if (gaugeModel != null) {
                createGauge(uiContext, gaugeName, wrapper, listener, extraMenuItems, gaugeModel, kind, overrides);
            } else {
                log.warn("Gauge not found: " + gaugeName);
            }
        } else {
            wrapper.removeAllChildrenAndListeners();
            wrapper.add(new JLabel("Connecting..."));
            log.warn("Gauge not found by " + gaugeName);
        }
    }

    static void createGauge(UIContext uiContext, String gaugeName, JPanelWithListener wrapper, GaugeChangeListener listener,
                            List<JMenuItem> extraMenuItems, GaugeModel gaugeModel, GaugeDisplayKind kind,
                            GaugeScaleOverrides overrides) {
        final JComponent component = createDisplay(kind, gaugeModel, iniScale(uiContext, gaugeModel).with(overrides));
        final GaugeDisplay gauge = (GaugeDisplay) component;
        UiUtils.setToolTip(component, HINT_LINE_1, HINT_LINE_2);
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
        GaugeOptionsButton.install(component, () -> createOptionsMenu(extraMenuItems,
            () -> popOut(uiContext, popOutLocation(component), gaugeName)));
        MouseListener mouseListener = new MouseAdapter() {
            // The menu follows the platform popup trigger (press on macOS/Linux, release on
            // Windows). mouseClicked is lost whenever the mouse moves a pixel between press
            // and release, which made right-click open nothing now and then.
            @Override
            public void mousePressed(MouseEvent e) {
                showMenuOnPopupTrigger(e);
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                showMenuOnPopupTrigger(e);
            }

            private void showMenuOnPopupTrigger(MouseEvent e) {
                if (e.isPopupTrigger()) {
                    GaugePopups.show(createPopupMenu(uiContext, wrapper, listener, extraMenuItems, kind), e);
                }
            }

            @Override
            public void mouseClicked(MouseEvent e) {
                if (!SwingUtilities.isRightMouseButton(e) && e.getClickCount() == 2) {
                    popOut(uiContext, e.getLocationOnScreen(), gaugeName);
                }
            }
        };
        component.addMouseListener(mouseListener);
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
        wrapper.add(component, BorderLayout.CENTER);
        AutoupdateUtil.trueLayoutAndRepaint(wrapper.getParent());
    }

    /**
     * Right-click menu: search, then the gauge groups, so a gauge is one step away. Display
     * settings are on the gear, see {@link #createOptionsMenu}.
     * Package-private so tests inspect the same menu users see.
     */
    static JPopupMenu createPopupMenu(UIContext uiContext, JPanelWithListener wrapper, GaugeChangeListener listener,
                                      JMenuItem extraMenuItem) {
        return createPopupMenu(uiContext, wrapper, listener, menuItems(extraMenuItem), GaugeDisplayKind.ROUND);
    }

    static JPopupMenu createPopupMenu(UIContext uiContext, JPanelWithListener wrapper, GaugeChangeListener listener,
                                      List<JMenuItem> extraMenuItems, GaugeDisplayKind kind) {
        JPopupMenu pm = new JPopupMenu();
        fillGaugeMenuItems(uiContext, pm, wrapper, listener, extraMenuItems, kind);
        GaugeMenuSearch.addTo(pm);
        return pm;
    }

    /** Gear menu: the caller's display options, then "Pop out to window". */
    static JPopupMenu createOptionsMenu(List<JMenuItem> extraMenuItems, Runnable popOut) {
        JPopupMenu pm = new JPopupMenu();
        for (JMenuItem item : extraMenuItems) {
            pm.add(item);
        }
        if (!extraMenuItems.isEmpty()) {
            pm.addSeparator();
        }
        JMenuItem popOutItem = new JMenuItem(POP_OUT);
        popOutItem.setToolTipText("Open this gauge in its own always-on-top window (also: double-click)");
        popOutItem.addActionListener(e -> popOut.run());
        pm.add(popOutItem);
        return pm;
    }

    /**
     * Changing the gauge keeps its presentation but not the scale settings, which were chosen
     * for the previous channel.
     */
    static void fillGaugeMenuItems(UIContext uiContext, JComponent popupMenu, final JPanelWithListener wrapper,
                                   final GaugeChangeListener listener, final List<JMenuItem> extraMenuItems, GaugeDisplayKind kind) {
        addGaugeCatalog(uiContext, popupMenu, gaugeName -> {
            createGaugeBody(uiContext, gaugeName, wrapper, listener, extraMenuItems, kind, GaugeScaleOverrides.NONE);
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

    static JComponent createDisplay(GaugeDisplayKind kind, GaugeModel model, GaugeScale scale) {
        switch (kind) {
            case TEXT:
                return new DigitalGauge(scale, model);
            case HORIZONTAL_BAR:
                return new BarGauge(scale, model, false);
            case VERTICAL_BAR:
                return new BarGauge(scale, model, true);
            default:
                return new CircularGauge(scale, model);
        }
    }

    /** The scale {@code model} defines, with .ini expressions evaluated against the connected ECU's tune. */
    public static GaugeScale iniScale(UIContext uiContext, GaugeModel model) {
        BinaryProtocol protocol = uiContext.getBinaryProtocol();
        ConfigurationImage configuration = protocol == null ? null : protocol.getControllerConfiguration();
        return GaugeScale.fromIni(model, uiContext.iniFileState.getIniFileModel(), configuration);
    }

    private static List<JMenuItem> menuItems(JMenuItem item) {
        return item == null ? Collections.emptyList() : Collections.singletonList(item);
    }

    private static Point popOutLocation(JComponent gauge) {
        return gauge.isShowing() ? gauge.getLocationOnScreen() : new Point();
    }

    private static void popOut(UIContext uiContext, Point screenLocation, String gaugeName) {
        new DetachedSensor(uiContext, gaugeName, DetachedSensor.DEFAULT_WIDTH).show(screenLocation);
    }

}
