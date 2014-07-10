package com.rusefi.ui.widgets;

import com.irnems.core.Sensor;
import com.irnems.core.SensorCentral;
import com.rusefi.io.CommandQueue;
import eu.hansolo.steelseries.gauges.Radial;
import eu.hansolo.steelseries.tools.ColorDef;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.text.DecimalFormat;
import java.text.Format;
import java.util.Arrays;
import java.util.Collection;
import java.util.Hashtable;

/**
 * Date: 7/9/14
 * (c) Andrey Belomutskiy 2012-2014
 */

public class SensorGauge {
    private static final Collection<Sensor> MOCKABLE = Arrays.asList(Sensor.CLT, Sensor.AFR, Sensor.IAT, Sensor.MAF,
            Sensor.TPS);
    /**
     * We need to trick the JSlider into displaying float values
     */
    private static final int _5_VOLTS_WITH_DECIMAL = 50;

    public static Component createGauge(final Sensor sensor) {
        return createGauge(sensor, 1);
    }

    public static Component createGauge(final Sensor sensor, final double correction) {
        final Radial gauge = createRadial(sensor.getName(), sensor.getUnits(), sensor.getMaxValue(), sensor.getMinValue());

        gauge.setBackgroundColor(sensor.getColor());

        SensorCentral.getInstance().addListener(sensor, new SensorCentral.AdcListener() {
            public void onAdcUpdate(SensorCentral model, double value) {
                gauge.setValue(value * correction);
            }
        });
        gauge.setLcdDecimals(2);

        gauge.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    int size = gauge.getSize().width;
                    JFrame n = new JFrame(sensor.getName());
                    boolean isMockable = MOCKABLE.contains(sensor);
                    int h = isMockable ? (int) (size * 1.5) : size;

                    JPanel content = new JPanel(new BorderLayout());

                    content.add(createGauge(sensor, correction), BorderLayout.CENTER);

                    if (isMockable)
                        content.add(createMockVoltageSlider(sensor), BorderLayout.SOUTH);

                    n.setSize(size, h);
                    n.setAlwaysOnTop(true);
                    n.add(content);
                    n.setVisible(true);
                    n.setLocation(e.getXOnScreen(), e.getYOnScreen());
                }
            }
        });

        return gauge;
    }

    private final static Hashtable<Integer, JComponent> SLIDER_LABELS = new Hashtable<Integer, JComponent>();

    static {
        Format f = new DecimalFormat("0.0");
        for (int i = 0; i <= 50; i += 5) {
            JLabel label = new JLabel(f.format(i * 0.1));
            label.setFont(label.getFont().deriveFont(Font.PLAIN));
            SLIDER_LABELS.put(i, label);
        }
    }


    private static Component createMockVoltageSlider(final Sensor sensor) {
        /**
         */
        final JSlider slider = new JSlider(0, _5_VOLTS_WITH_DECIMAL);
        slider.setLabelTable(SLIDER_LABELS);
        slider.setPaintLabels(true);
        slider.setPaintTicks(true);
        slider.setMajorTickSpacing(10);
        slider.setMinorTickSpacing(5);

        slider.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                double value = slider.getValue() / 10.0;
                CommandQueue.getInstance().write("set_mock_" + sensor.name().toLowerCase() + "_voltage " + value);
            }
        });

        return slider;
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


