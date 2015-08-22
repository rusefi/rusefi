package com.rusefi.ui;

import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.util.LinkedList;
import java.util.Random;

/**
 * (c) Andrey Belomutskiy
 * 8/22/2015.
 */
public class SensorLiveGraph extends JPanel {
    private static final int COUNT = 30;

    private final LinkedList<Double> values = new LinkedList<>();

    public SensorLiveGraph() {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                Random r = new Random();
                while (true) {
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        throw new IllegalStateException(e);
                    }
                    addValue(r.nextInt(1221));
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            UiUtils.trueRepaint(SensorLiveGraph.this);
                        }
                    });
                }
            }
        });
        thread.setDaemon(true);
        thread.start();

        setBorder(BorderFactory.createLineBorder(Color.red));
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

        double minValue = Double.MAX_VALUE;
        double maxValue = Double.MIN_VALUE;
        for (double value : values) {
            minValue = Math.min(minValue, value);
            maxValue = Math.max(maxValue, value);
        }

        int index = 0;
        for (double value : values) {
            int x = d.width * index / values.size();

            int y = (int) (d.height - (value - minValue) * d.height / (maxValue - minValue));

            g.drawOval(x, y, 3, 3);
            index++;
        }
    }
//
//    @Override
//    public Dimension getPreferredSize() {
//        return new Dimension(300, 300);
//    }
}
