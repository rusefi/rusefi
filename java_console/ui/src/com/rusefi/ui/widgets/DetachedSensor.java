package com.rusefi.ui.widgets;

import com.rusefi.core.Sensor;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.text.DecimalFormat;
import java.text.Format;
import java.util.Arrays;
import java.util.Collection;
import java.util.Hashtable;

/**
 * (c) Andrey Belomutskiy
 * 11/2/14
 */
public class DetachedSensor {
    private static final Collection<Sensor> MOCKABLE = Arrays.asList(Sensor.CLT, Sensor.AFR, Sensor.IAT, Sensor.MAF,
            Sensor.TPS);

    private final static Hashtable<Integer, JComponent> SLIDER_LABELS = new Hashtable<>();

    static {
        Format f = new DecimalFormat("0.0");
        for (int i = 0; i <= 50; i += 5) {
            JLabel label = new JLabel(f.format(i * 0.1));
            label.setFont(label.getFont().deriveFont(Font.PLAIN));
            SLIDER_LABELS.put(i, label);
        }
    }

    /**
     * We need to trick the JSlider into displaying float values
     */
    private static final int _5_VOLTS_WITH_DECIMAL = 50;

    private final JPanel content = new JPanel(new BorderLayout());
    private final JFrame frame;
    private final JPanel mockControlPanel = new JPanel(new BorderLayout());
    private Sensor sensor;
    private int width;

    public DetachedSensor(Sensor sensor, int width) {
        this.sensor = sensor;
        this.width = width;
        frame = new JFrame();
        frame.setAlwaysOnTop(true);
        onChange(sensor);
    }

    void create(Sensor sensor) {
        SensorGauge.GaugeChangeListener listener = new SensorGauge.GaugeChangeListener() {
            @Override
            public void onSensorChange(Sensor sensor) {
                onChange(sensor);
            }
        };
        content.add(SensorGauge.createGauge(sensor, listener, null), BorderLayout.CENTER);
        content.add(mockControlPanel, BorderLayout.SOUTH);

        frame.add(content);
    }

    public void show(MouseEvent e) {
        frame.setVisible(true);
        frame.setLocation(e.getXOnScreen(), e.getYOnScreen());
    }

    public void onChange(Sensor sensor) {
        this.sensor = sensor;
        frame.setTitle(sensor.getName());
        showMockControl();
    }

    public void showMockControl() {
        mockControlPanel.removeAll();
        boolean isMockable = isMockable();
        if (isMockable) {
            Component mockComponent = createMockVoltageSlider(sensor);
            mockControlPanel.add(mockComponent);
        }
        UiUtils.trueLayout(content);
        int size = width;
        int h = isMockable ? (int) (size * 1.5) : size;
        frame.setSize(size, h);
    }

    private boolean isMockable() {
        return MOCKABLE.contains(sensor) && LinkManager.isStimulationMode;
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
}
