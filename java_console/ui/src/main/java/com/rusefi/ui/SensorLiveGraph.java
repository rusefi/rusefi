package com.rusefi.ui;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.NamedThreadFactory;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCategory;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.GaugeMenuSearch;
import com.rusefi.ui.widgets.GaugeOptionsButton;
import com.rusefi.ui.widgets.GaugePopups;
import com.rusefi.ui.widgets.GaugeStyle;
import com.rusefi.ui.widgets.GaugeMenuLabel;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Path2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.util.LinkedList;
import java.util.Locale;
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
    static final String WAITING = "Waiting for data" + GaugeStyle.ELLIPSIS;
    private static final Color DANGER_BAND = new Color(0xFF, 0xCD, 0xD2, 110);
    private static final Color AREA_TOP = new Color(0xF6, 0xC4, 0x45, 120);
    private static final Color AREA_BOTTOM = new Color(0xF6, 0xC4, 0x45, 0);

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
    private final Thread thread;
    private volatile boolean active = true;
    private volatile boolean running = true;
    private ISensorCentral.ListenerToken demandToken;
    private String demandChannel;

    public SensorLiveGraph(UIContext uiContext, Node config, final String defaultGaugeName, JMenuItem extraItem) {
        this.uiContext = uiContext;
        this.config = config;
        this.extraItem = extraItem;
        this.gaugeName = config.getProperty(SENSOR_TYPE, defaultGaugeName);

        period = ChangePeriod.lookup(config.getProperty(PERIOD));
        if (period == null) {
            period = ChangePeriod._200;
        }
        autoScale = config.getBoolProperty(USE_AUTO_SCALE);
        customUpper = config.getDoubleProperty(UPPER, Double.NaN);
        customLower = config.getDoubleProperty(LOWER, Double.NaN);
        updateDemandToken();
        thread = THREAD_FACTORY.newThread(createRunnable());
        thread.start();

        MouseListener mouseListener = new MouseAdapter() {
            // Popup trigger, not mouseClicked: see SensorGauge's mouse listener.
            @Override
            public void mousePressed(MouseEvent e) {
                if (e.isPopupTrigger()) {
                    showPopupMenu(e);
                }
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                if (e.isPopupTrigger()) {
                    showPopupMenu(e);
                }
            }
        };

        addMouseListener(mouseListener);
        GaugeOptionsButton.install(this, this::createOptionsMenu);

        setOpaque(false);
    }

    @NotNull
    private Runnable createRunnable() {
        return new Runnable() {
            @Override
            public void run() {
                while (running) {
                    // Read first, so a new graph shows its first point without waiting a period.
                    if (active) {
                        grabNewValue();
                    }
                    try {
                        Thread.sleep(period.getMs());
                    } catch (InterruptedException e) {
                        if (!running) {
                            return;
                        }
                    }
                }
            }
        };
    }

    private void grabNewValue() {
        String channelName = getChannelName();
        if (!channelName.equals(demandChannel)) {
            // The .ini can load after this graph was created, revealing the real channel.
            updateDemandToken();
        }

        addValue(SensorCentral.getInstance().getValue(channelName));
        // A new reading changes only the picture, never the layout; repaint() is thread-safe.
        repaint();
    }

    private void showPopupMenu(MouseEvent e) {
        GaugePopups.show(createPopupMenu(), e);
    }

    /** Right-click menu: search, then the gauge groups; graph options are on the gear. */
    // Package-private so tests inspect the same menu users see.
    JPopupMenu createPopupMenu() {
        JPopupMenu pm = new JPopupMenu();
        addChangeSensorItems(pm);
        GaugeMenuSearch.addTo(pm);
        return pm;
    }

    /** Gear menu: graph options, then the caller's display options. */
    // Package-private so tests inspect the same menu users see.
    JPopupMenu createOptionsMenu() {
        JPopupMenu pm = new JPopupMenu();
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

        if (extraItem != null) {
            pm.addSeparator();
            pm.add(extraItem);
        }
        return pm;
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

    void addChangeSensorItems(JComponent pm) {
        IniFileModel iniFile = uiContext.iniFileState.getIniFileModel();
        if (iniFile != null) {
            for (final String category : iniFile.getGaugeCategories().keySet()) {
                JMenuItem cmi = new JMenu(category);
                pm.add(cmi);
                for (final GaugeModel gauge : iniFile.getGaugeCategories().get(category).getGauges()) {
                    JMenuItem mi = new JMenuItem(GaugeMenuLabel.getTitle(uiContext, gauge));
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

    // Package-private so tests change the gauge as the menu does.
    synchronized void setSensor(String gaugeName) {
        this.gaugeName = gaugeName;
        values.clear();
        config.setProperty(SENSOR_TYPE, gaugeName);
        updateDemandToken();
    }

    /**
     * Output channels are polled only while subscribed by channel name, so the graph must
     * subscribe to its gauge's channel, not to the gauge name.
     */
    private synchronized void updateDemandToken() {
        if (demandToken != null) {
            demandToken.remove();
        }
        demandChannel = getChannelName();
        demandToken = SensorCentral.getInstance().addListener(demandChannel, value -> { });
        demandToken.setActive(active);
    }

    /** The output channel behind the gauge, or the name itself for legacy sensors and unknown gauges. */
    private String getChannelName() {
        GaugeModel gaugeModel = findGaugeModel();
        return gaugeModel == null ? gaugeName : gaugeModel.getChannel();
    }

    public synchronized void setActive(boolean active) {
        this.active = active;
        if (demandToken != null) {
            demandToken.setActive(active);
        }
    }

    public synchronized void destroy() {
        running = false;
        thread.interrupt();
        if (demandToken != null) {
            demandToken.remove();
            demandToken = null;
        }
    }

    synchronized void addValue(double value) {
        if (values.size() > COUNT)
            values.removeFirst();
        values.add(value);
    }

    /** Gauge definition from the loaded .ini, available with or without an ECU connection. */
    private GaugeModel findGaugeModel() {
        IniFileModel iniFile = uiContext.iniFileState.getIniFileModel();
        return iniFile == null ? null : iniFile.getGauge(gaugeName);
    }

    @Override
    protected void paintComponent(Graphics graphics) {
        super.paintComponent(graphics);
        int width = getWidth();
        int height = getHeight();
        if (width < 40 || height < 40) {
            return;
        }
        double[] samples;
        synchronized (this) {
            samples = new double[values.size()];
            int i = 0;
            for (double value : values) {
                samples[i++] = value;
            }
        }
        Graphics2D g = (Graphics2D) graphics.create();
        try {
            g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            g.setRenderingHint(RenderingHints.KEY_STROKE_CONTROL, RenderingHints.VALUE_STROKE_PURE);
            RoundRectangle2D.Double face = GaugeStyle.paintCard(g, width, height);
            paintGraph(g, face, samples, findGaugeModel());
        } finally {
            g.dispose();
        }
    }

    private void paintGraph(Graphics2D g, RoundRectangle2D.Double face, double[] samples, GaugeModel model) {
        double pad = Math.max(8, 0.06 * Math.min(face.width, face.height));
        double left = face.x + pad;
        double right = face.x + face.width - pad;
        double bottom = face.y + face.height - pad;
        double last = samples.length == 0 ? Double.NaN : samples[samples.length - 1];
        double danger = dangerThreshold(model);
        boolean inDanger = Double.isFinite(last) && last >= danger;

        // Header: title on the left, the latest value and its units on the right.
        Font valueFont = GaugeStyle.font(Font.BOLD, (float) clamp(face.height * 0.13, 12, 28));
        Font unitsFont = GaugeStyle.font(Font.PLAIN, valueFont.getSize2D() * 0.5f);
        FontMetrics valueMetrics = g.getFontMetrics(valueFont);
        double headerBaseline = face.y + pad + valueMetrics.getAscent() * 0.9;
        String units = units(model);
        g.setFont(unitsFont);
        g.setColor(GaugeStyle.SCALE);
        double unitsWidth = g.getFontMetrics().stringWidth(units);
        GaugeStyle.drawText(g, units, right, headerBaseline, 1);
        int decimals = model == null ? 2 : Math.max(0, Math.min(6, model.getValueDecimalPlaces()));
        String text = Double.isFinite(last) ? format(last, decimals) : "---";
        g.setFont(valueFont);
        g.setColor(!Double.isFinite(last) ? GaugeStyle.TICK : inDanger ? GaugeStyle.DANGER : GaugeStyle.TEXT);
        double valueRight = right - unitsWidth - (units.isEmpty() ? 0 : valueFont.getSize2D() * 0.2);
        GaugeStyle.drawText(g, text, valueRight, headerBaseline, 1);
        double valueLeft = valueRight - valueMetrics.stringWidth(text);
        g.setFont(GaugeStyle.font(Font.PLAIN, (float) clamp(face.height * 0.08, 10, 15)));
        g.setColor(GaugeStyle.SCALE);
        GaugeStyle.drawText(g, GaugeStyle.fit(g, getTitle(model), valueLeft - left - pad), left, headerBaseline, -1);

        // Plot: scale labels on the left, light grid lines, the danger band, then the series.
        VisibleRange range = getRange(samples, model);
        Font labelFont = GaugeStyle.font(Font.PLAIN, (float) clamp(face.height * 0.06, 9, 12));
        FontMetrics labelMetrics = g.getFontMetrics(labelFont);
        double plotTop = headerBaseline + valueMetrics.getDescent() + Math.max(6, face.height * 0.05)
            + labelMetrics.getAscent() / 2.0;
        double plotBottom = bottom - labelMetrics.getAscent() / 2.0;
        if (plotBottom - plotTop < 10) {
            return;
        }
        int labelDecimals = labelDecimals(range);
        String[] labels = {format(range.maxValue, labelDecimals),
            format((range.minValue + range.maxValue) / 2, labelDecimals), format(range.minValue, labelDecimals)};
        double labelWidth = 0;
        for (String label : labels) {
            labelWidth = Math.max(labelWidth, labelMetrics.stringWidth(label));
        }
        double plotLeft = left + labelWidth + 8;
        if (right - plotLeft < 20) {
            return;
        }
        Plot plot = new Plot(plotLeft, plotTop, right, plotBottom, range);

        if (danger < range.maxValue) {
            double y = plot.y(Math.max(danger, range.minValue));
            g.setColor(DANGER_BAND);
            g.fill(new Rectangle2D.Double(plotLeft, plotTop, right - plotLeft, y - plotTop));
        }
        g.setStroke(new BasicStroke(1f));
        g.setColor(GaugeStyle.GRID);
        for (int i = 0; i <= 4; i++) {
            double y = plotTop + (plotBottom - plotTop) * i / 4;
            g.draw(new Line2D.Double(plotLeft, y, right, y));
        }
        g.setFont(labelFont);
        g.setColor(GaugeStyle.TICK);
        for (int i = 0; i < labels.length; i++) {
            double y = plotTop + (plotBottom - plotTop) * i / 2;
            GaugeStyle.drawText(g, labels[i], plotLeft - 8, y + labelMetrics.getAscent() / 2.0 - 1, 1);
        }

        Shape oldClip = g.getClip();
        g.clip(new Rectangle2D.Double(plotLeft, plotTop - 6, right - plotLeft + 1, plotBottom - plotTop + 12));
        // New readings enter on the right; a missing reading breaks the line.
        double step = (right - plotLeft) / COUNT;
        int start = 0;
        while (start < samples.length) {
            if (!Double.isFinite(samples[start])) {
                start++;
                continue;
            }
            int end = start;
            while (end + 1 < samples.length && Double.isFinite(samples[end + 1])) {
                end++;
            }
            paintSegment(g, plot, samples, start, end, step);
            start = end + 1;
        }
        g.setClip(oldClip);
        if (!hasReading(samples)) {
            g.setFont(GaugeStyle.font(Font.PLAIN, (float) clamp(face.height * 0.08, 10, 14)));
            g.setColor(GaugeStyle.TICK);
            GaugeStyle.drawText(g, WAITING, (plotLeft + right) / 2,
                (plotTop + plotBottom) / 2 + g.getFontMetrics().getAscent() / 2.0 - 1, 0);
        }
        if (Double.isFinite(last)) {
            double x = right;
            double y = plot.y(Math.max(range.minValue, Math.min(range.maxValue, last)));
            g.setColor(Color.WHITE);
            g.fill(new Ellipse2D.Double(x - 4.5, y - 4.5, 9, 9));
            g.setColor(inDanger ? GaugeStyle.DANGER : GaugeStyle.LINE);
            g.setStroke(new BasicStroke(2.5f));
            g.draw(new Ellipse2D.Double(x - 4.5, y - 4.5, 9, 9));
        }
    }

    /** A smooth line through consecutive readings, over a fill that fades towards the bottom. */
    private void paintSegment(Graphics2D g, Plot plot, double[] samples, int start, int end, double step) {
        int count = end - start + 1;
        double[] xs = new double[count];
        double[] ys = new double[count];
        for (int i = 0; i < count; i++) {
            xs[i] = plot.right - (samples.length - 1 - (start + i)) * step;
            ys[i] = plot.y(samples[start + i]);
        }
        Path2D.Double line = new Path2D.Double();
        line.moveTo(xs[0], ys[0]);
        for (int i = 1; i < count; i++) {
            // Catmull-Rom through the neighbouring readings, as cubic Bezier segments.
            double x0 = xs[Math.max(0, i - 2)];
            double y0 = ys[Math.max(0, i - 2)];
            double x3 = xs[Math.min(count - 1, i + 1)];
            double y3 = ys[Math.min(count - 1, i + 1)];
            line.curveTo(xs[i - 1] + (xs[i] - x0) / 6, ys[i - 1] + (ys[i] - y0) / 6,
                xs[i] - (x3 - xs[i - 1]) / 6, ys[i] - (y3 - ys[i - 1]) / 6, xs[i], ys[i]);
        }
        if (count > 1) {
            Path2D.Double area = new Path2D.Double(line);
            area.lineTo(xs[count - 1], plot.bottom);
            area.lineTo(xs[0], plot.bottom);
            area.closePath();
            g.setPaint(new GradientPaint(0, (float) plot.top, AREA_TOP, 0, (float) plot.bottom, AREA_BOTTOM));
            g.fill(area);
        }
        g.setColor(GaugeStyle.LINE);
        g.setStroke(new BasicStroke(2.2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
        g.draw(line);
    }

    private static boolean hasReading(double[] samples) {
        for (double sample : samples) {
            if (Double.isFinite(sample)) {
                return true;
            }
        }
        return false;
    }

    private String getTitle(GaugeModel model) {
        if (model != null) {
            return GaugeMenuLabel.getTitle(uiContext, model);
        }
        Sensor s = Sensor.lookup(gaugeName, null);
        return s == null ? gaugeName : s.getName();
    }

    private static String units(GaugeModel model) {
        return model == null || model.getUnitsValue().isExpression() ? "" : model.getUnits();
    }

    /** Readings at or above this are drawn as danger; infinite when the gauge defines none. */
    private static double dangerThreshold(GaugeModel model) {
        if (model == null || model.getHighDangerValueValue().isExpression()) {
            return Double.POSITIVE_INFINITY;
        }
        double danger = model.getHighDangerValue();
        return Double.isFinite(danger) && danger < model.getHighValue() ? danger : Double.POSITIVE_INFINITY;
    }

    private static int labelDecimals(VisibleRange range) {
        double span = range.maxValue - range.minValue;
        return span >= 100 ? 0 : span >= 10 ? 1 : 2;
    }

    private static String format(double value, int decimals) {
        return String.format(Locale.ROOT, "%." + decimals + "f", value == 0 ? 0 : value);
    }

    private static double clamp(double value, double min, double max) {
        return Math.max(min, Math.min(max, value));
    }

    private static final class Plot {
        final double left;
        final double top;
        final double right;
        final double bottom;
        final VisibleRange range;

        Plot(double left, double top, double right, double bottom, VisibleRange range) {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
            this.range = range;
        }

        double y(double value) {
            return bottom - (value - range.minValue) * (bottom - top) / (range.maxValue - range.minValue);
        }
    }

    /** Fixed .ini (or custom) bounds, or the readings' own range when auto scaling or no bounds are known. */
    VisibleRange getRange(double[] samples, GaugeModel model) {
        if (!autoScale) {
            double min = model == null || model.getLowValueValue().isExpression() ? Double.NaN : model.getLowValue();
            double max = model == null || model.getHighValueValue().isExpression() ? Double.NaN : model.getHighValue();
            min = Double.isNaN(customLower) ? min : customLower;
            max = Double.isNaN(customUpper) ? max : customUpper;
            if (Double.isFinite(min) && Double.isFinite(max) && max > min) {
                return new VisibleRange(min, max);
            }
        }
        return VisibleRange.findRange(samples);
    }

    static class VisibleRange {
        final double minValue;
        final double maxValue;

        VisibleRange(double minValue, double maxValue) {
            this.minValue = minValue;
            this.maxValue = maxValue;
        }

        /** Range of the finite readings with a small margin; 0..1 when there are none. */
        static VisibleRange findRange(double[] values) {
            double minValue = Double.POSITIVE_INFINITY;
            double maxValue = Double.NEGATIVE_INFINITY;
            for (double value : values) {
                if (Double.isFinite(value)) {
                    minValue = Math.min(minValue, value);
                    maxValue = Math.max(maxValue, value);
                }
            }
            if (minValue > maxValue) {
                return new VisibleRange(0, 1);
            }
            if (minValue == maxValue) {
                double margin = Math.max(1, Math.abs(maxValue) * 0.1);
                return new VisibleRange(minValue - margin, maxValue + margin);
            }
            // expand the range just a bit for borders
            double diff = maxValue - minValue;
            return new VisibleRange(minValue - 0.05 * diff, maxValue + 0.05 * diff);
        }
    }
}
