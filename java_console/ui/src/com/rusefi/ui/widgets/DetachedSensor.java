package com.rusefi.ui.widgets;

import com.rusefi.config.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.GaugesPanel;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
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
    private static final String NAME = "name";
    private static final String WIDTH = "width";
    /**
     * @see Fields#MOCK_IAT_COMMAND
     */
    private static final Collection<Sensor> MOCKABLE = Arrays.asList(
            Sensor.CLT,
            Sensor.AFR,
            Sensor.IAT,
            Sensor.MAF,
            Sensor.MAP,
            Sensor.TPS);

    private final static Hashtable<Integer, JComponent> SLIDER_LABELS = new Hashtable<>();
    public static final String XPOS = "xpos";
    public static final String YPOS = "ypos";

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
        this.width = width;
        frame = new JFrame();
        frame.setAlwaysOnTop(true);
        onChange(sensor);

        GaugesPanel.DetachedRepository.INSTANCE.add(this);
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                GaugesPanel.DetachedRepository.INSTANCE.remove(DetachedSensor.this);
                frame.dispose();
            }
        });
        create();
    }

    void create() {
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
        return MOCKABLE.contains(sensor) && LinkManager.isSimulationMode;
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
                CommandQueue.getInstance().write("set mock_" + sensor.name().toLowerCase() + "_voltage " + value);
            }
        });

        return slider;
    }

    public void saveConfig(Node child) {
        child.setProperty(NAME, sensor.name());
        child.setProperty(WIDTH, frame.getWidth());
        child.setProperty(XPOS, frame.getLocation().x);
        child.setProperty(YPOS, frame.getLocation().y);
    }

    public static void create(Node child) {
        Sensor sensor = Sensor.lookup(child.getProperty(NAME, Sensor.RPM.name()), Sensor.RPM);
        int width = child.getIntProperty(WIDTH, 256);
        int xpos = child.getIntProperty(XPOS, 0);
        int ypos = child.getIntProperty(YPOS, 0);
        DetachedSensor ds = new DetachedSensor(sensor, width);
        ds.frame.setLocation(xpos, ypos);
        ds.frame.setVisible(true);
    }
}
