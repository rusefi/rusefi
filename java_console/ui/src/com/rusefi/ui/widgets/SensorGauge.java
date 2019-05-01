package com.rusefi.ui.widgets;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCategory;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.GaugesPanel;
import com.rusefi.ui.util.UiUtils;
import eu.hansolo.steelseries.gauges.Radial;
import eu.hansolo.steelseries.tools.ColorDef;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/**
 * Round gauge
 *
 * On double-click a {@link DetachedSensor} is created
 *
 * Date: 7/9/14
 * (c) Andrey Belomutskiy 2012-2014
 * @see GaugesPanel
 */

public class SensorGauge {
    private static final String HINT_LINE_1 = "Double-click to detach";
    private static final String HINT_LINE_2 = "Right-click to change";

    public static Component createGauge(Sensor sensor, GaugeChangeListener listener, JMenuItem extraMenuItem) {
        JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());

        createGaugeBody(sensor, wrapper, listener, extraMenuItem);

        return wrapper;
    }

    public interface GaugeChangeListener {
        /**
         * This event happens when user decides to switch the kind of gauge
         * displayed by this control
         *
         * @param sensor new type
         */
        void onSensorChange(Sensor sensor);
    }

    public static void createGaugeBody(final Sensor sensor, final JPanelWithListener wrapper, final GaugeChangeListener listener,
                                        final JMenuItem extraMenuItem) {
        final Radial gauge = createRadial(sensor.getName(), sensor.getUnits(), sensor.getMaxValue(), sensor.getMinValue());

        UiUtils.setToolTip(gauge, HINT_LINE_1, HINT_LINE_2);
        UiUtils.setToolTip(wrapper, HINT_LINE_1, HINT_LINE_2);

        gauge.setBackgroundColor(sensor.getColor());

        SensorCentral.getInstance().addListener(sensor, new SensorCentral.SensorListener() {
            public void onSensorUpdate(double value) {
                if (GaugesPanel.IS_PAUSED)
                    return;
                gauge.setValue(sensor.translateValue(value));
            }
        });
        gauge.setValue(sensor.translateValue(SensorCentral.getInstance().getValue(sensor)));
        gauge.setLcdDecimals(2);

        MouseListener mouseListener = new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    showPopupMenu(e, wrapper, listener, extraMenuItem);
                } else if (e.getClickCount() == 2) {
                    handleDoubleClick(e, gauge, sensor);
                }
            }
        };
        gauge.addMouseListener(mouseListener);
        wrapper.removeAllChildrenAndListeners();
        wrapper.addMouseListener(mouseListener);
        wrapper.add(gauge, BorderLayout.CENTER);
        UiUtils.trueRepaint(wrapper.getParent());
        UiUtils.trueLayout(wrapper.getParent());
    }

    private static void showPopupMenu(MouseEvent e, JPanelWithListener wrapper, GaugeChangeListener listener,
                                      JMenuItem extraMenuItem) {
        JPopupMenu pm = new JPopupMenu();
        fillGaugeMenuItems(pm, wrapper, listener, extraMenuItem);
        if (extraMenuItem != null)
            pm.add(extraMenuItem);
        pm.show(e.getComponent(), e.getX(), e.getY());
    }

    private static void fillGaugeMenuItems(JPopupMenu popupMenu, final JPanelWithListener wrapper, final GaugeChangeListener listener, final JMenuItem extraMenuItem) {
        for (final SensorCategory sc : SensorCategory.values()) {
            JMenuItem cmi = new JMenu(sc.getName());
            popupMenu.add(cmi);

            for (final Sensor s : Sensor.getSensorsForCategory(sc.getName())) {
                JMenuItem mi = new JMenuItem(s.getName());
                mi.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        createGaugeBody(s, wrapper, listener, extraMenuItem);
                        listener.onSensorChange(s);
                    }
                });
                cmi.add(mi);
            }
        }
    }

    private static void handleDoubleClick(MouseEvent e, Radial gauge, Sensor sensor) {
        int width = gauge.getSize().width;
        final DetachedSensor ds = new DetachedSensor(sensor, width);

        ds.show(e);
    }

    public static Radial createRadial(String title, String units, double maxValue, double minValue) {
//        final Section[] SECTIONS =
//                {
//                        new Section(0, to, Color.red)
//                };

        Radial radial1 = new Radial();
//        radial1.setSections(SECTIONS);
        radial1.setTitle(title);
        radial1.setUnitString(units);

        //radial1.setTrackStop(to);

        radial1.setMinValue(minValue);
        radial1.setMaxValue(maxValue);
        radial1.setThresholdVisible(false);
        radial1.setPointerColor(ColorDef.RED);

        radial1.setValue(0);
        return radial1;
    }
}


