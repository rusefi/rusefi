package com.rusefi.ui;

import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCategory;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.LinkedList;

/**
 * (c) Andrey Belomutskiy
 * 8/22/2015.
 */
@SuppressWarnings("InfiniteLoopStatement")
public class SensorLiveGraph extends JPanel {
    private static final int COUNT = 30;
    private static final String SENSOR_TYPE = "sensor";

    private final LinkedList<Double> values = new LinkedList<>();
    private final Node config;
    private final JMenuItem extraItem;
    private Sensor sensor;

    public SensorLiveGraph(Node config, final Sensor defaultSensor, JMenuItem extraItem) {
        this.config = config;
        this.extraItem = extraItem;
        String gaugeName = config.getProperty(SENSOR_TYPE, defaultSensor.name());
        this.sensor = Sensor.lookup(gaugeName, defaultSensor);

        Thread thread = new Thread(createRunnable());
        thread.setDaemon(true);
        thread.start();

        MouseListener mouseListener = new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    showPopupMenu(e);
                } else if (e.getClickCount() == 2) {
//                    handleDoubleClick(e, gauge, sensor);
                }
            }
        };

        addMouseListener(mouseListener);

        setBorder(BorderFactory.createLineBorder(Color.black));
    }

    @NotNull
    private Runnable createRunnable() {
        return new Runnable() {
            @Override
            public void run() {
                while (true) {
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        throw new IllegalStateException(e);
                    }
                    double value = SensorCentral.getInstance().getValue(sensor);
                    addValue(value);
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            UiUtils.trueRepaint(SensorLiveGraph.this);
                        }
                    });
                }
            }
        };
    }

    private void showPopupMenu(MouseEvent e) {
        JPopupMenu pm = new JPopupMenu();

        for (final SensorCategory sc : SensorCategory.values()) {
            JMenuItem cmi = new JMenu(sc.getName());
            pm.add(cmi);

            for (final Sensor s : Sensor.getSensorsForCategory(sc.getName())) {
                JMenuItem mi = new JMenuItem(s.getName());
                mi.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        setSensor(s);
                    }
                });
                cmi.add(mi);
            }
        }
        pm.add(extraItem);
        pm.show(e.getComponent(), e.getX(), e.getY());
    }

    private synchronized void setSensor(Sensor sensor) {
        this.sensor = sensor;
        values.clear();
        config.setProperty(SENSOR_TYPE, sensor.name());
    }

    private synchronized void addValue(double value) {
        if (values.size() > COUNT)
            values.removeFirst();
        values.add(value);
    }

    @Override
    public synchronized void paint(Graphics g) {
        super.paint(g);

        Dimension d = getSize();
        if (d.height < 5)
            return; // it's hopeless
        g.setColor(Color.black);

        double minValue = Double.MAX_VALUE;
        double maxValue = -Double.MAX_VALUE;
        for (double value : values) {
            minValue = Math.min(minValue, value);
            maxValue = Math.max(maxValue, value);
        }

        if (minValue == maxValue) { // double equals should work here, should it?
            minValue = 0.9 * maxValue - 1;
            maxValue = 1.1 * maxValue + 1;
        } else {
            // expand the range just a bit for borders
            double diff = maxValue - minValue;
            minValue -= 0.05 * diff;
            maxValue += 0.05 * diff;
        }

        int index = 0;
        int prevX = 0;
        int prevY = 0;
        for (double value : values) {
            int x = d.width * index / values.size();

            int y = (int) (d.height - (value - minValue) * d.height / (maxValue - minValue));

            g.drawOval(x, y, 3, 3);

            if (index > 0) {
                g.drawLine(x, y, prevX, prevY);
            }

            prevX = x;
            prevY = y;
            index++;
        }

        g.setColor(Color.red);
        int minY = d.height;
        int maxY = g.getFont().getSize();
        g.drawString(String.format("%.2f", minValue), 5, minY);
        g.drawString(String.format("%.2f", (minValue + maxValue) / 2), 5, (minY + maxY) / 2);
        g.drawString(String.format("%.2f", maxValue), 5, maxY);

        g.setColor(Color.blue);
        String sensorName = sensor.getName() + " ";
        int nameWidth = g.getFontMetrics().stringWidth(sensorName);
        g.drawString(sensorName, d.width - nameWidth, g.getFont().getSize());
    }
}
