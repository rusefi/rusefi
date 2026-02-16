package com.rusefi.ui.widgets;

import com.devexperts.logging.Logging;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCategory;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.ui.GaugesPanel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import eu.hansolo.steelseries.gauges.Radial;
import eu.hansolo.steelseries.tools.BackgroundColor;
import eu.hansolo.steelseries.tools.ColorDef;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Round gauge
 * <p>
 * On double-click a {@link DetachedSensor} is created
 * <p>
 * Date: 7/9/14
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see GaugesPanel
 */

public class SensorGauge {
    private static final Logging log = getLogging(SensorGauge.class);

    private static final String HINT_LINE_1 = "Double-click to detach";
    private static final String HINT_LINE_2 = "Right-click to change";

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
        final Radial gauge = createRadial(gaugeModel.getHighValue(), gaugeModel.getLowValue(), gaugeModel);

        UiUtils.setToolTip(gauge, HINT_LINE_1, HINT_LINE_2);
        UiUtils.setToolTip(wrapper, HINT_LINE_1, HINT_LINE_2);

        gauge.setBackgroundColor(BackgroundColor.LIGHT_GRAY);

        String channelName = gaugeModel.getChannel();
        SensorCentral.getInstance().addListener(channelName,
            value -> {
                if (GaugesPanel.IS_PAUSED)
                    return;
                gauge.setValue(value);
            }
        );

        // For expression-based labels, show empty placeholder until resolved by the data cycle
        if (gaugeModel.getTitleValue().isExpression()) {
            gauge.setTitle("");
        }
        if (gaugeModel.getUnitsValue().isExpression()) {
            gauge.setUnitString("");
        }

        if (gaugeModel.getTitleValue().isExpression() || gaugeModel.getUnitsValue().isExpression()) {
            SensorCentral.getInstance().addListener((SensorCentral.ResponseListener) () ->
                SwingUtilities.invokeLater(() -> applyResolvedLabels(gauge, gaugeName))
            );
        }

        gauge.setValue(SensorCentral.getInstance().getValue(channelName));
        gauge.setLcdDecimals(2);

        MouseListener mouseListener = new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    showPopupMenu(uiContext, e, wrapper, listener, extraMenuItem);
                } else if (e.getClickCount() == 2) {
                    handleDoubleClick(uiContext, e, gauge, gaugeName);
                }
            }
        };
        gauge.addMouseListener(mouseListener);
        wrapper.removeAllChildrenAndListeners();
        wrapper.addMouseListener(mouseListener);
        wrapper.add(gauge, BorderLayout.CENTER);
        AutoupdateUtil.trueLayoutAndRepaint(wrapper.getParent());
    }

    private static void showPopupMenu(UIContext uiContext, MouseEvent e, JPanelWithListener wrapper, GaugeChangeListener listener,
                                      JMenuItem extraMenuItem) {
        JPopupMenu pm = new JPopupMenu();
        fillGaugeMenuItems(uiContext, pm, wrapper, listener, extraMenuItem);
        if (extraMenuItem != null)
            pm.add(extraMenuItem);
        pm.show(e.getComponent(), e.getX(), e.getY());
    }

    private static void fillGaugeMenuItems(UIContext uiContext, JPopupMenu popupMenu, final JPanelWithListener wrapper, final GaugeChangeListener listener, final JMenuItem extraMenuItem) {
        BinaryProtocol bp = uiContext.getLinkManager().getBinaryProtocol();
        if (bp != null) {
            IniFileModel iniFile = bp.getIniFileNullable();
            if (iniFile != null) {
                for (final String category : iniFile.getGaugeCategories().keySet()) {
                    JMenuItem cmi = new JMenu(category);
                    popupMenu.add(cmi);
                    for (final GaugeModel gauge : iniFile.getGaugeCategories().get(category).getGauges()) {
                        JMenuItem mi = new JMenuItem(gauge.getTitle());
                        mi.addActionListener(new ActionListener() {
                            @Override
                            public void actionPerformed(ActionEvent e) {
                                createGaugeBody(uiContext, gauge.getName(), wrapper, listener, extraMenuItem);
                                listener.onSensorChange(gauge.getName());
                            }
                        });
                        cmi.add(mi);
                    }
                }
                return;
            }
        }

        for (final SensorCategory sc : SensorCategory.values()) {
            JMenuItem cmi = new JMenu(sc.getName());
            popupMenu.add(cmi);

            for (final Sensor s : Sensor.getSensorsForCategory(sc.getName())) {
                JMenuItem mi = new JMenuItem(s.getName());
                mi.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        createGaugeBody(uiContext, s.name(), wrapper, listener, extraMenuItem);
                        listener.onSensorChange(s.name());
                    }
                });
                cmi.add(mi);
            }
        }
    }

    private static void handleDoubleClick(UIContext uiContext, MouseEvent e, Radial gauge, String gaugeName) {
        final DetachedSensor ds = new DetachedSensor(uiContext, gaugeName, DetachedSensor.DEFAULT_WIDTH);

        ds.show(e);
    }

    private static void applyResolvedLabels(Radial gauge, String gaugeName) {
        ISensorHolder.ResolvedGaugeLabels labels = SensorCentral.getInstance().getResolvedLabels(gaugeName);
        if (labels != null) {
            gauge.setTitle(labels.getTitle());
            gauge.setUnitString(labels.getUnits());
            gauge.repaint();
        }
    }

    public static Radial createRadial(double maxValue, double minValue, GaugeModel gaugeModel) {
        Radial radial1 = new Radial();
        radial1.setTitle(gaugeModel.getTitle());
        radial1.setUnitString(gaugeModel.getUnits());

        radial1.setMinValue(minValue);
        if (minValue == maxValue) {
            // a bit of a hack to survive not great input data
            radial1.setMaxValue(minValue + 10);
        } else {
            radial1.setMaxValue(maxValue);
        }
        radial1.setThresholdVisible(false);
        radial1.setPointerColor(ColorDef.RED);

        radial1.setValue(0);
        return radial1;
    }
}


