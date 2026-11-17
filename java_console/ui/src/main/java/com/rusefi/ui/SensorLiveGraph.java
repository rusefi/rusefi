package com.rusefi.ui;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.NamedThreadFactory;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCategory;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.CircularGauge;
import com.rusefi.ui.widgets.CircularGauge.Zone;
import com.rusefi.ui.widgets.GaugeMenuSearch;
import com.rusefi.ui.widgets.GaugeScale;
import com.rusefi.ui.widgets.SensorGauge;
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
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
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
    /** Zoom to the readings instead of the full gauge range; the older "auto_scale" setting is ignored. */
    private static final String ZOOM_TO_READINGS = "zoom_to_readings";
    private static final String UPPER = "upper";
    private static final String LOWER = "lower";
    static final String WAITING = "Waiting for data" + GaugeStyle.ELLIPSIS;

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
        autoScale = config.getBoolProperty(ZOOM_TO_READINGS);
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
        JMenu scale = new JMenu("Scale");

        JRadioButtonMenuItem full = new JRadioButtonMenuItem(FULL_SCALE, !autoScale);
        full.setToolTipText("Show the gauge's whole range, so its warning and danger zones are always in view");
        JRadioButtonMenuItem zoom = new JRadioButtonMenuItem(ZOOM_TO_FIT, autoScale);
        zoom.setToolTipText("Fit the scale to the recent readings to see small changes");
        ButtonGroup mode = new ButtonGroup();
        mode.add(full);
        mode.add(zoom);
        full.addActionListener(e -> setAutoScale(false));
        zoom.addActionListener(e -> setAutoScale(true));
        scale.add(full);
        scale.add(zoom);
        scale.addSeparator();

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

    static final String FULL_SCALE = "Full scale";
    static final String ZOOM_TO_FIT = "Zoom to readings";

    private void setAutoScale(boolean zoom) {
        autoScale = zoom;
        config.setBoolProperty(ZOOM_TO_READINGS, zoom);
        repaint();
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
        // the same .ini limits and zones as the round gauge, so every presentation flags a reading alike
        GaugeScale scale = model == null ? null : SensorGauge.iniScale(uiContext, model);
        CircularGauge.Zones zones = scale == null ? null : CircularGauge.Zones.of(scale);
        Zone zone = zones == null || !Double.isFinite(last) ? Zone.NORMAL : zones.zoneOf(last, 0);

        // Header: title on the left, the latest value and its units on the right.
        Font valueFont = GaugeStyle.font(Font.BOLD, (float) clamp(face.height * 0.13, 12, 28));
        Font unitsFont = GaugeStyle.font(Font.PLAIN, valueFont.getSize2D() * 0.5f);
        FontMetrics valueMetrics = g.getFontMetrics(valueFont);
        double headerBaseline = face.y + pad + valueMetrics.getAscent() * 0.9;
        String units = model == null ? "" : GaugeMenuLabel.getUnits(uiContext, model);
        g.setFont(unitsFont);
        g.setColor(GaugeStyle.scale());
        double unitsWidth = g.getFontMetrics().stringWidth(units);
        GaugeStyle.drawText(g, units, right, headerBaseline, 1);
        int decimals = model == null ? 2 : Math.max(0, Math.min(6, model.getValueDecimalPlaces()));
        String text = Double.isFinite(last) ? format(last, decimals) : "---";
        g.setFont(valueFont);
        g.setColor(!Double.isFinite(last) ? GaugeStyle.tick() : zoneColor(zone, GaugeStyle.text()));
        double valueRight = right - unitsWidth - (units.isEmpty() ? 0 : valueFont.getSize2D() * 0.2);
        GaugeStyle.drawText(g, text, valueRight, headerBaseline, 1);
        double valueLeft = valueRight - valueMetrics.stringWidth(text);
        Font titleFont = GaugeStyle.font(Font.PLAIN, (float) clamp(face.height * 0.08, 10, 15));
        if (zone != Zone.NORMAL) {
            // the zone in words beside the value: readable however the plot is scaled
            valueLeft = paintZoneChip(g, zone, valueLeft - valueFont.getSize2D() * 0.4, headerBaseline, titleFont);
        }
        g.setFont(titleFont);
        g.setColor(GaugeStyle.scale());
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

        List<ZoneBand> bands = zones == null ? new ArrayList<>() : bands(zones);
        for (ZoneBand band : bands) {
            Rectangle2D.Double area = band.area(plot);
            if (area != null && band.zone != Zone.BELOW_SCALE) {
                g.setColor(band.zone == Zone.DANGER ? GaugeStyle.graphDangerBand() : GaugeStyle.graphWarningBand());
                g.fill(area);
            }
        }
        g.setStroke(new BasicStroke(1f));
        g.setColor(GaugeStyle.graphGrid());
        for (int i = 0; i <= 4; i++) {
            double y = plotTop + (plotBottom - plotTop) * i / 4;
            g.draw(new Line2D.Double(plotLeft, y, right, y));
        }
        g.setFont(labelFont);
        g.setColor(GaugeStyle.tick());
        for (int i = 0; i < labels.length; i++) {
            double y = plotTop + (plotBottom - plotTop) * i / 2;
            GaugeStyle.drawText(g, labels[i], plotLeft - 8, y + labelMetrics.getAscent() / 2.0 - 1, 1);
        }

        Shape oldClip = g.getClip();
        g.clip(new Rectangle2D.Double(plotLeft, plotTop - 6, right - plotLeft + 1, plotBottom - plotTop + 12));
        // New readings enter on the right; a missing reading breaks the line.
        double step = (right - plotLeft) / COUNT;
        List<Path2D.Double> lines = new ArrayList<>();
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
            lines.add(paintSegment(g, plot, samples, start, end, step));
            start = end + 1;
        }
        // the line again in the zone's colour wherever it runs inside a warning or danger band
        Shape plotClip = g.getClip();
        g.setStroke(new BasicStroke(2.2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
        for (ZoneBand band : bands) {
            Rectangle2D.Double area = band.area(plot);
            if (area == null) {
                continue;
            }
            g.setClip(plotClip);
            // a reading on the plot's edge is drawn a few pixels past it; a band that goes on beyond the edge reaches
            // out to cover it. One that ends at the scale minimum does not: readings below the minimum are not flagged.
            double above = band.to > range.maxValue ? 6 : 0;
            double below = band.from < range.minValue ? 6 : 0;
            double bottomEdge = area.y + area.height + below;
            if (scale != null && scale.isEngineSpeed() && band.from <= 0 && band.to > 0) {
                // a stopped engine (0 rpm) is not flagged, so the line along 0 keeps its colour
                bottomEdge = Math.min(bottomEdge, plot.y(0) - 2);
            }
            g.clip(new Rectangle2D.Double(area.x, area.y - above, area.width, Math.max(0, bottomEdge - area.y + above)));
            g.setColor(lineColor(band.zone));
            for (Path2D.Double line : lines) {
                g.draw(line);
            }
        }
        g.setClip(oldClip);

        if (!hasReading(samples)) {
            g.setFont(GaugeStyle.font(Font.PLAIN, (float) clamp(face.height * 0.08, 10, 14)));
            g.setColor(GaugeStyle.tick());
            GaugeStyle.drawText(g, WAITING, (plotLeft + right) / 2,
                (plotTop + plotBottom) / 2 + g.getFontMetrics().getAscent() / 2.0 - 1, 0);
        }
        if (Double.isFinite(last)) {
            double x = right;
            double y = plot.y(Math.max(range.minValue, Math.min(range.maxValue, last)));
            // the last value: a dot in the card colour, ringed in the line colour
            g.setColor(GaugeStyle.cardTop());
            g.fill(new Ellipse2D.Double(x - 4.5, y - 4.5, 9, 9));
            g.setColor(lineColor(zone));
            g.setStroke(new BasicStroke(2.5f));
            g.draw(new Ellipse2D.Double(x - 4.5, y - 4.5, 9, 9));
        }
    }

    /** A smooth line through consecutive readings, over a fill that fades towards the bottom; returns the line. */
    private Path2D.Double paintSegment(Graphics2D g, Plot plot, double[] samples, int start, int end, double step) {
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
            g.setPaint(new GradientPaint(0, (float) plot.top, GaugeStyle.graphAreaTop(), 0, (float) plot.bottom, GaugeStyle.graphAreaBottom()));
            g.fill(area);
        }
        g.setColor(GaugeStyle.graphLine());
        g.setStroke(new BasicStroke(2.2f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
        g.draw(line);
        return line;
    }

    private static Color lineColor(Zone zone) {
        return zone == Zone.DANGER ? GaugeStyle.danger() : zone == Zone.WARNING ? GaugeStyle.graphWarningLine()
            : zone == Zone.BELOW_SCALE ? GaugeStyle.tick() : GaugeStyle.graphLine();
    }

    private static Color zoneColor(Zone zone, Color normal) {
        return zone == Zone.DANGER ? GaugeStyle.danger() : zone == Zone.WARNING ? GaugeStyle.warning()
            : zone == Zone.BELOW_SCALE ? GaugeStyle.tick() : normal;
    }

    /** "WARNING", "DANGER" or "NO SENSOR?" on a tinted chip ending at {@code right}; returns the chip's left edge. */
    private static double paintZoneChip(Graphics2D g, Zone zone, double right, double baseline, Font font) {
        Font chipFont = font.deriveFont(Font.BOLD, font.getSize2D() * 0.8f);
        g.setFont(chipFont);
        FontMetrics metrics = g.getFontMetrics();
        String text = zone == Zone.DANGER ? "DANGER" : zone == Zone.WARNING ? "WARNING" : CircularGauge.NO_SENSOR;
        double padX = metrics.getHeight() * 0.45;
        double width = padX + metrics.stringWidth(text) + padX;
        double height = metrics.getAscent() * 1.5;
        double x = right - width;
        double y = baseline - metrics.getAscent() * 1.15;
        g.setColor(zone == Zone.DANGER ? GaugeStyle.dangerFill() : zone == Zone.WARNING ? GaugeStyle.warningFill()
            : GaugeStyle.track());
        g.fill(new RoundRectangle2D.Double(x, y, width, height, height, height));
        Color ink = zone == Zone.DANGER ? GaugeStyle.danger() : zone == Zone.WARNING ? GaugeStyle.warningText()
            : GaugeStyle.scale();
        g.setColor(ink);
        double textBaseline = y + (height + metrics.getAscent() - metrics.getDescent()) / 2;
        GaugeStyle.drawText(g, text, x + padX, textBaseline, -1);
        return x;
    }

    /** Where a warning or danger zone lies on the value axis; {@code to} is infinite for the danger zone above. */
    static final class ZoneBand {
        final double from;
        final double to;
        final Zone zone;

        ZoneBand(double from, double to, Zone zone) {
            this.from = from;
            this.to = to;
            this.zone = zone;
        }

        /** The part of the plot this band covers, or null when it lies outside the visible range. */
        Rectangle2D.Double area(Plot plot) {
            double low = Math.max(from, plot.range.minValue);
            double high = Math.min(to, plot.range.maxValue);
            if (!(high > low)) {
                // below the scale at full scale: no area, but the line drawn just under the plot's edge belongs to it
                return zone == Zone.BELOW_SCALE && to >= plot.range.minValue
                    ? new Rectangle2D.Double(plot.left, plot.bottom, plot.right - plot.left, 0) : null;
            }
            double top = plot.y(high);
            return new Rectangle2D.Double(plot.left, top, plot.right - plot.left, plot.y(low) - top);
        }

        @Override
        public String toString() {
            return zone + " " + from + ".." + to;
        }
    }

    /**
     * The zones from low to high, as the round gauge flags them; empty zones are left out. Below the scale is a
     * band too, unfilled, so the line there turns grey.
     */
    static List<ZoneBand> bands(CircularGauge.Zones zones) {
        List<ZoneBand> bands = new ArrayList<>();
        bands.add(new ZoneBand(Double.NEGATIVE_INFINITY, zones.getMin(), Zone.BELOW_SCALE));
        if (zones.getLowDanger() > zones.getMin()) {
            bands.add(new ZoneBand(zones.getMin(), zones.getLowDanger(), Zone.DANGER));
        }
        if (zones.getLowWarning() > zones.getLowDanger()) {
            bands.add(new ZoneBand(zones.getLowDanger(), zones.getLowWarning(), Zone.WARNING));
        }
        if (zones.getDanger() > zones.getHighWarning()) {
            bands.add(new ZoneBand(zones.getHighWarning(), zones.getDanger(), Zone.WARNING));
        }
        bands.add(new ZoneBand(zones.getDanger(), Double.POSITIVE_INFINITY, Zone.DANGER));
        return bands;
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

    static final class Plot {
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

    /**
     * The full gauge range (the .ini bounds, expressions resolved against the tune, or the custom ones), or the
     * readings' own range when zooming to them or when the range is unknown.
     */
    VisibleRange getRange(double[] samples, GaugeModel model) {
        if (!autoScale) {
            double min = model == null ? Double.NaN : resolvedBound(model.getLowValueValue());
            double max = model == null ? Double.NaN : resolvedBound(model.getHighValueValue());
            min = Double.isNaN(customLower) ? min : customLower;
            max = Double.isNaN(customUpper) ? max : customUpper;
            if (Double.isFinite(min) && Double.isFinite(max) && max > min) {
                return new VisibleRange(min, max);
            }
        }
        return VisibleRange.findRange(samples);
    }

    /** A range bound, NaN while an expression cannot be resolved (e.g. before the tune is read). */
    private double resolvedBound(com.opensr5.ini.IniValue value) {
        if (!value.isExpression()) {
            return value.getNumericValue();
        }
        return CircularGauge.resolve(value, uiContext.iniFileState.getIniFileModel(),
            uiContext.getCurrentConfigurationImage(), Double.NaN);
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
