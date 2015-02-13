package com.rusefi.ui.widgets;

import com.irnems.core.Sensor;
import com.irnems.core.SensorCategory;
import com.irnems.core.SensorCentral;
import eu.hansolo.steelseries.gauges.Radial;
import eu.hansolo.steelseries.tools.ColorDef;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.text.DecimalFormat;
import java.text.Format;
import java.util.Hashtable;

/**
 * Date: 7/9/14
 * (c) Andrey Belomutskiy 2012-2014
 */

public class SensorGauge {

    public static Component createGauge(final Sensor sensor) {
        return createGauge(sensor, null);
    }

    private static Component createGauge(Sensor sensor, GaugeChangeListener listener) {
        JPanel wrapper = new JPanel(new BorderLayout());

        createGaugeBody(sensor, wrapper, listener);

        return wrapper;
    }

    interface GaugeChangeListener {
        void onChange(Sensor sensor);
    }

    private static void createGaugeBody(final Sensor sensor, final JPanel wrapper, final GaugeChangeListener listener) {
        final Radial gauge = createRadial(sensor.getName(), sensor.getUnits(), sensor.getMaxValue(), sensor.getMinValue());

        UpDownImage.setTwoLineToolTip(gauge, "Double-click to detach", "Right-click to change");

        gauge.setBackgroundColor(sensor.getColor());

        SensorCentral.getInstance().addListener(sensor, new SensorCentral.SensorListener() {
            public void onSensorUpdate(double value) {
                gauge.setValue(sensor.translateValue(value));
            }
        });
        gauge.setLcdDecimals(2);

        gauge.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    showPopupMenu(e, wrapper, listener);
                } else if (e.getClickCount() == 2) {
                    handleDoubleClick(e, gauge, sensor);
                }
            }
        });
        wrapper.removeAll();
        wrapper.add(gauge, BorderLayout.CENTER);
        UpDownImage.trueRepaint(wrapper.getParent());
        UpDownImage.trueLayout(wrapper.getParent());
    }

    private static void showPopupMenu(MouseEvent e, JPanel wrapper, GaugeChangeListener listener) {
        JPopupMenu pm = new JPopupMenu();
        fillGaugeItems(pm, wrapper, listener);
        pm.show(e.getComponent(), e.getX(), e.getY());
    }

    private static void fillGaugeItems(JPopupMenu popupMenu, final JPanel wrapper, final GaugeChangeListener listener) {
        for (final SensorCategory sc : SensorCategory.values()) {
            JMenuItem cmi = new JMenu(sc.getName());
            popupMenu.add(cmi);

            for (final Sensor s : Sensor.getSensorsForCategory(sc.getName())) {
                JMenuItem mi = new JMenuItem(s.getName());
                mi.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        createGaugeBody(s, wrapper, listener);
                        if (listener != null)
                            listener.onChange(s);
                    }
                });
                cmi.add(mi);
            }
        }
    }

    private static void handleDoubleClick(MouseEvent e, Radial gauge, Sensor sensor) {
        int width = gauge.getSize().width;
        final DetachedSensor ds = new DetachedSensor(sensor, width);

        GaugeChangeListener listener = new GaugeChangeListener() {
            @Override
            public void onChange(Sensor sensor) {
                ds.onChange(sensor);
            }
        };
        ds.content.add(createGauge(sensor, listener), BorderLayout.CENTER);
        ds.content.add(ds.mockControlPanel, BorderLayout.SOUTH);


        ds.frame.add(ds.content);
        ds.show(e);
    }

    final static Hashtable<Integer, JComponent> SLIDER_LABELS = new Hashtable<>();

    static {
        Format f = new DecimalFormat("0.0");
        for (int i = 0; i <= 50; i += 5) {
            JLabel label = new JLabel(f.format(i * 0.1));
            label.setFont(label.getFont().deriveFont(Font.PLAIN));
            SLIDER_LABELS.put(i, label);
        }
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


