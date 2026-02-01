package com.rusefi.ui.widgets;

import com.rusefi.SensorTypeHelper;
import com.rusefi.core.Sensor;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.enums.SensorType;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.IMethodInvocation;
import com.rusefi.io.InvocationConfirmationListener;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.UIContext;
import com.rusefi.core.preferences.storage.Node;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.text.DecimalFormat;
import java.text.Format;
import java.util.Arrays;
import java.util.Collection;
import java.util.Hashtable;
import java.util.concurrent.atomic.AtomicReference;

import static com.rusefi.config.generated.Integration.CMD_SET_SENSOR_MOCK;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 11/2/14
 */
public class DetachedSensor {
    private static final String NAME = "name";
    private static final String WIDTH = "width";

    private static final Collection<Sensor> MOCKABLE = Arrays.asList(
            Sensor.CLTGauge,
            Sensor.LAMBDAVALUE,
            Sensor.IATGauge,
            Sensor.MAFMEASURED,
            Sensor.MAPGauge,
            Sensor.TPSVALUE);

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
    @NotNull
    private final UIContext uiContext;
    private final int width;

    public DetachedSensor(UIContext uiContext, Sensor sensor, int width) {
        this.uiContext = uiContext;
        this.width = width;
        frame = new JFrame();
        frame.setAlwaysOnTop(true);
        onChange(sensor);

        uiContext.DetachedRepositoryINSTANCE.add(this);
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                uiContext.DetachedRepositoryINSTANCE.remove(DetachedSensor.this);
                frame.dispose();
            }
        });
        create();
    }

    void create() {
        SensorGauge.GaugeChangeListener listener = this::onChange;
        content.add(SensorGauge.createGauge(uiContext, sensor, listener, null), BorderLayout.CENTER);
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
            Component mockComponent = createMockValueSlider(uiContext.getCommandQueue(), sensor);
            mockControlPanel.add(mockComponent);
        }
        AutoupdateUtil.trueLayoutAndRepaint(content);
        int size = width;
        int h = isMockable ? (int) (size * 1.5) : size;
        frame.setSize(size, h);
    }

    private boolean isMockable() {
        return MOCKABLE.contains(sensor) && LinkManager.isSimulationMode;
    }

    public static Component createMockValueSlider(CommandQueue commandQueue, final Sensor sensor) {
        SensorType sensorType = SensorTypeHelper.valueOfAnyCase(sensor.getName());

        int maxValue = 200;
        final JSlider slider = new JSlider(0, maxValue);
        slider.setLabelTable(SLIDER_LABELS);
        slider.setPaintLabels(true);
        slider.setPaintTicks(true);
        slider.setMajorTickSpacing(10);
        slider.setMinorTickSpacing(5);

        AtomicReference<Double> pendingValue = new AtomicReference<>();

        IMethodInvocation commandSender = new IMethodInvocation() {
            @Override
            public String getCommand() {
                return CMD_SET_SENSOR_MOCK + " " + sensorType.ordinal() + " " + pendingValue.get();
            }

            @Override
            public int getTimeout() {
                return CommandQueue.DEFAULT_TIMEOUT;
            }

            @Override
            public InvocationConfirmationListener getListener() {
                return InvocationConfirmationListener.VOID;
            }

            @Override
            public boolean isFireEvent() {
                return false;
            }
        };


        slider.addChangeListener(e -> {
            double value = slider.getValue();
            pendingValue.set(value);

            /*
             * User might be changing slider faster than commands are being send
             * We only add commandSender into the queue only if not already pending in order to only send one command with latest requested value and not a sequence of commands.
             */
            commandQueue.addIfNotPresent(commandSender);
        });

        return slider;
    }

    public void saveConfig(Node child) {
        child.setProperty(NAME, sensor.name());
        child.setProperty(WIDTH, frame.getWidth());
        child.setProperty(XPOS, frame.getLocation().x);
        child.setProperty(YPOS, frame.getLocation().y);
    }

    public static void create(UIContext uiContext, Node child) {
        Sensor sensor = Sensor.lookup(child.getProperty(NAME, Sensor.RPMGauge.name()), Sensor.RPMGauge);
        int width = child.getIntProperty(WIDTH, 256);
        int xpos = child.getIntProperty(XPOS, 0);
        int ypos = child.getIntProperty(YPOS, 0);
        DetachedSensor ds = new DetachedSensor(uiContext, sensor, width);
        ds.frame.setLocation(xpos, ypos);
        ds.frame.setVisible(true);
    }
}
