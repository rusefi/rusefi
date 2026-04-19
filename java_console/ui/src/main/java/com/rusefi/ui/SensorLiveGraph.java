package com.rusefi.ui;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.NamedThreadFactory;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCategory;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.sensor_logs.CustomBinaryLogEntry;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.LinkedList;
import java.util.concurrent.ThreadFactory;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 8/22/2015.
 */
@SuppressWarnings("InfiniteLoopStatement")
public class SensorLiveGraph extends JPanel {
    private final static ThreadFactory THREAD_FACTORY = new NamedThreadFactory("SensorLiveGraph", true);
    private static final int COUNT = 30;
    private static final String SENSOR_TYPE = "sensor";
    private static final String PERIOD = "period";
    private static final String USE_AUTO_SCALE = "auto_scale";
    private static final String UPPER = "upper";
    private static final String LOWER = "lower";

    private final LinkedList<Double> values = new LinkedList<>();
    private final UIContext uiContext;
    private final Node config;
    private final JMenuItem extraItem;
    @NotNull
    private ChangePeriod period;
    private String gaugeName;
    private boolean autoScale;
    private double customUpper;
    private double customLower;

    public SensorLiveGraph(UIContext uiContext, Node config, final String defaultGaugeName, JMenuItem extraItem) {
        this.uiContext = uiContext;
        this.config = config;
        this.extraItem = extraItem;
        this.gaugeName = config.getProperty(SENSOR_TYPE, defaultGaugeName);

        Thread thread = THREAD_FACTORY.newThread(createRunnable());
        thread.start();
        period = ChangePeriod.lookup(config.getProperty(PERIOD));
        if (period == null) {
            period = ChangePeriod._200;
        }
        autoScale = config.getBoolProperty(USE_AUTO_SCALE);
        customUpper = config.getDoubleProperty(UPPER, Double.NaN);
        customLower = config.getDoubleProperty(LOWER, Double.NaN);

        MouseListener mouseListener = new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isRightMouseButton(e)) {
                    showPopupMenu(e);
//                } else if (e.getClickCount() == 2) {
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
                        Thread.sleep(period.getMs());
                    } catch (InterruptedException e) {
                        throw new IllegalStateException(e);
                    }
                    if (!GaugesPanel.IS_PAUSED)
                        grabNewValue();
                }
            }
        };
    }

    private void grabNewValue() {
        double value = Double.NaN;
        String channelName = null;
        BinaryProtocol bp = uiContext.getLinkManager().getBinaryProtocol();
        byte[] response = SensorCentral.getInstance().getResponse();
        if (bp != null && response != null) {
            IniFileModel iniFile = bp.getIniFileNullable();
            if (iniFile != null) {
                try {
                    GaugeModel gaugeModel = iniFile.getGauge(gaugeName);
                    if (gaugeModel != null) {
                        channelName = gaugeModel.getChannel();
                        IniField field = iniFile.getOutputChannel(channelName);
                        if (field instanceof ScalarIniField) {
                            CustomBinaryLogEntry entry = new CustomBinaryLogEntry(gaugeModel, (ScalarIniField) field);
                            value = entry.getValue(response);
                        }
                    }
                } catch (Exception e) {
                    // fall back to sensor central
                }
            }
        }

        if (Double.isNaN(value) && channelName != null) {
            value = SensorCentral.getInstance().getValue(channelName);
        }

        addValue(value);
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                AutoupdateUtil.trueLayoutAndRepaint(SensorLiveGraph.this);
            }
        });
    }

    private void showPopupMenu(MouseEvent e) {
        JPopupMenu pm = new JPopupMenu();

        addChangeSensorItems(pm);
        pm.add(new JSeparator());
        addChangePeriodItems(pm);
        JMenuItem scale = new JMenu("Scale");

        final JCheckBoxMenuItem as = new JCheckBoxMenuItem("Auto scale");
        as.setSelected(autoScale);
        as.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                autoScale = as.isSelected();
                config.setBoolProperty(USE_AUTO_SCALE, autoScale);
            }
        });
        scale.add(as);

        final JCheckBoxMenuItem lowerValue = new JCheckBoxMenuItem("Use custom lower");
        lowerValue.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (lowerValue.isSelected()) {
                    String initial = Double.isNaN(customLower) ? "" : Double.toString(customLower);
                    String lower = JOptionPane.showInputDialog("Enter lower value", initial);
                    if (!Node.isNumeric(lower))
                        return;
                    customLower = Double.parseDouble(lower);
                    config.setProperty(LOWER, customLower);
                }
            }
        });

        final JCheckBoxMenuItem upperValue = new JCheckBoxMenuItem("Use custom upper");
        upperValue.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (upperValue.isSelected()) {
                    String initial = Double.isNaN(customUpper) ? "" : Double.toString(customUpper);
                    String upper = JOptionPane.showInputDialog("Enter upper value", initial);
                    if (!Node.isNumeric(upper))
                        return;
                    customUpper = Double.parseDouble(upper);
                    config.setProperty(UPPER, customUpper);
                }
            }
        });

        scale.add(upperValue);
        scale.add(lowerValue);

        pm.add(scale);

        pm.add(extraItem);
        pm.show(e.getComponent(), e.getX(), e.getY());
    }

    enum ChangePeriod {
        _3000(3000, "3 seconds"),
        _1000(1000, "1 second"),
        _200(200, "second / 5"),
        _100(100, "second / 10"),
        _50(50, "second / 20"),;

        private final int ms;
        private final String text;

        ChangePeriod(int ms, String text) {
            this.ms = ms;
            this.text = text;
        }

        public int getMs() {
            return ms;
        }

        public String getText() {
            return text;
        }

        public static ChangePeriod lookup(String value) {
            for (ChangePeriod cp : ChangePeriod.values()) {
                if ((cp.getMs() + "").equals(value))
                    return cp;
            }
            return ChangePeriod._200;
        }
    }

    private void addChangePeriodItems(JPopupMenu pm) {
        JMenuItem mi = new JMenu("Refresh period");
        pm.add(mi);
        for (final ChangePeriod cp : ChangePeriod.values()) {
            JCheckBoxMenuItem i = new JCheckBoxMenuItem(cp.getText());
            i.setSelected(cp == period);
            i.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    period = cp;
                    config.setProperty(PERIOD, period.getMs());
                }
            });
            mi.add(i);
        }
    }

    private void addChangeSensorItems(JPopupMenu pm) {
        BinaryProtocol bp = uiContext.getLinkManager().getBinaryProtocol();
        if (bp != null) {
            IniFileModel iniFile = bp.getIniFileNullable();
            if (iniFile != null) {
                for (final String category : iniFile.getGaugeCategories().keySet()) {
                    JMenuItem cmi = new JMenu(category);
                    pm.add(cmi);
                    for (final GaugeModel gauge : iniFile.getGaugeCategories().get(category).getGauges()) {
                        JMenuItem mi = new JMenuItem(gauge.getTitle());
                        mi.addActionListener(new ActionListener() {
                            @Override
                            public void actionPerformed(ActionEvent e) {
                                setSensor(gauge.getName());
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
            pm.add(cmi);

            for (final Sensor s : Sensor.getSensorsForCategory(sc.getName())) {
                JMenuItem mi = new JMenuItem(s.getName());
                mi.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        setSensor(s.getName());
                    }
                });
                cmi.add(mi);
            }
        }
    }

    private synchronized void setSensor(String gaugeName) {
        this.gaugeName = gaugeName;
        values.clear();
        config.setProperty(SENSOR_TYPE, gaugeName);
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

        VisibleRange range = getRange();

        paintGraph(g, d, range.minValue, range.maxValue);

        g.setColor(Color.red);
        int minY = d.height;
        int maxY = g.getFont().getSize();
        g.drawString(String.format("%.2f", range.minValue), 5, minY);
        g.drawString(String.format("%.2f", (range.minValue + range.maxValue) / 2), 5, (minY + maxY) / 2);
        g.drawString(String.format("%.2f", range.maxValue), 5, maxY);

        g.setColor(Color.blue);
        String title = null;
        BinaryProtocol bp = uiContext.getLinkManager().getBinaryProtocol();
        if (bp != null) {
            IniFileModel iniFile = bp.getIniFileNullable();
            if (iniFile != null) {
                GaugeModel gaugeModel = iniFile.getGauge(gaugeName);
                if (gaugeModel != null) {
                    title = gaugeModel.getTitle();
                }
            }
        }
        if (title == null) {
            Sensor s = Sensor.lookup(gaugeName, null);
            title = s == null ? gaugeName : s.getName();
        }
        String sensorName = title + " ";
        int nameWidth = g.getFontMetrics().stringWidth(sensorName);
        g.drawString(sensorName, d.width - nameWidth, g.getFont().getSize());

        Font f = g.getFont();
        g.setFont(new Font(f.getName(), f.getStyle(), 3 * f.getSize()));

        if (!values.isEmpty())
            paintLastValue(g, d);
    }

    private VisibleRange getRange() {
        VisibleRange range;
        if (autoScale) {
            range = VisibleRange.findRange(values);
        } else {
            double maxValue = 0;
            double minValue = 0;

            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp != null) {
                IniFileModel iniFile = bp.getIniFileNullable();
                if (iniFile != null) {
                    GaugeModel gaugeModel = iniFile.getGauge(gaugeName);
                    if (gaugeModel != null) {
                        maxValue = gaugeModel.getHighValue();
                        minValue = gaugeModel.getLowValue();
                    }
                }
            }

            range = new VisibleRange(Double.isNaN(customLower) ? minValue : customLower,
                    Double.isNaN(customUpper) ? maxValue : customUpper);
        }
        return range;
    }

    private void paintLastValue(Graphics g, Dimension d) {
        Double last = values.getLast();
        if (!Double.isNaN(last)) {
            String currentValue = String.format("%.2f", last);
            g.drawString(currentValue, (d.width - g.getFontMetrics().stringWidth(currentValue)) / 2, d.height / 2 + g.getFont().getSize() / 2);
        }
    }

    private void paintGraph(Graphics g, Dimension d, double minValue, double maxValue) {
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
    }

    private static class VisibleRange {
        private final double minValue;
        private final double maxValue;

        public VisibleRange(double minValue, double maxValue) {
            this.minValue = minValue;
            this.maxValue = maxValue;
        }

        public static VisibleRange findRange(LinkedList<Double> values) {
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
            return new VisibleRange(minValue, maxValue);
        }
    }
}
