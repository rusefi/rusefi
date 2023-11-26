package com.rusefi.ui.engine;

import com.rusefi.config.generated.Fields;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.waves.EngineReport;
import com.rusefi.waves.RevolutionLog;
import com.rusefi.waves.TimeAxisTranslator;

import javax.swing.*;
import java.awt.*;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * This is a renderer of an individual {@link EngineReport} - this makes a simple Logical Analyzer
 * <p/>
 * <p/>
 * Date: 6/23/13
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see EngineSnifferPanel
 * @see EngineReport
 */
public class UpDownImage extends JPanel {
    private static final int TIMESCALE_MULT = (int) (20 * EngineReport.ENGINE_SNIFFER_TICKS_PER_MS); // 20ms
    private static final int LINE_SIZE = 20;
    private static final int GAP_POSITIONS = 5;
    public static final Color TIME_SCALE_COLOR = Color.red;
    public static final Color ENGINE_CYCLE_COLOR = new Color(0, 153, 0);
    private static final BasicStroke TIME_SCALE_STROKE = new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 10.0f,
            new float[]{7.0f, 21.0f}, 0.0f);
    private static final BasicStroke ENGINE_CYCLE_STROKE = new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 10.0f,
            new float[]{21.0f, 7.0f}, 0.0f);

    public EngineReport engineReport;
    private StringBuilder revolutions;
    private final String name;
    private TimeAxisTranslator translator;
    private RevolutionLog time2rpm = RevolutionLog.parseRevolutions(null);
    /**
     * firmware is sending {@link Fields#PROTOCOL_OUTPIN}
     */
    private String pin = "NO PIN";

    /**
     * we have variable color depending on signal name
     */
    private Color signalBody = Color.lightGray;
    private Color signalBorder = Color.GRAY;

    private boolean renderText = true;

    public void setRenderText(boolean renderText) {
        this.renderText = renderText;
    }

    public UpDownImage(final String name) {
        this(EngineReport.MOCK, name);
        setToolTip();
        String pin = ChannelNaming.INSTANCE.channelName2PhysicalPin.get(name);
        if (pin != null)
            setPhysicalPin(pin);
    }

    public void setSignalBody(Color signalBody) {
        this.signalBody = signalBody;
    }

    public void setSignalBorder(Color signalBorder) {
        this.signalBorder = signalBorder;
    }

    public void setToolTip() {
        // no physical pin information in simulator
        String secondLine = LinkManager.isSimulationMode ? "" : "Physical pin: " + pin;
        UiUtils.setToolTip(this, "Channel " + NameUtil.getUiName(name), secondLine);
    }

    public UpDownImage(EngineReport wr, String name) {
        this.name = name;
        setWaveReport(wr, null);
        setOpaque(true);
        translator = createTranslator();
    }

    public void setTranslator(TimeAxisTranslator translator) {
        this.translator = translator;
    }

    private final TimeAxisTranslator _translator = new TimeAxisTranslator() {
            @Override
            public int timeToScreen(double time, int width) {
                return UpDownImage.this.engineReport.getTimeAxisTranslator().timeToScreen(time, width);
            }

            @Override
            public double screenToTime(int screen, int width) {
                return UpDownImage.this.engineReport.getTimeAxisTranslator().screenToTime(screen, width);
            }

            @Override
            public int getMaxTime() {
                return UpDownImage.this.engineReport.getTimeAxisTranslator().getMaxTime();
            }

            @Override
            public int getMinTime() {
                return UpDownImage.this.engineReport.getTimeAxisTranslator().getMinTime();
            }

            @Override
            public String toString() {
                return "TimeAxisTranslator";
            }
        };

    public TimeAxisTranslator createTranslator() {
        return this._translator;
    }

    public void setWaveReport(EngineReport wr, StringBuilder revolutions) {
        this.engineReport = wr;
        propagateDwellIntoSensor(wr);
        this.revolutions = revolutions;
    }

    private void propagateDwellIntoSensor(EngineReport wr) {
        Sensor sensor = NameUtil.name2sensor.get(name);
        if (sensor == null)
            return;

        if (!wr.getList().isEmpty()) {
            EngineReport.UpDown last = wr.getList().get(wr.getList().size() - 1);
            SensorCentral.getInstance().setValue(last.getDuration(), sensor);
        }
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        Graphics2D g2 = (Graphics2D) g;

        Dimension d = getSize();
        g.setColor(getBackground());
        g.fillRect(0, 0, d.width, d.height);

        if (this.renderText) {
            paintScaleLines(g2, d);
        }

        drawStartOfRevolution(g2, d);

        d.height = (int)(0.95 * d.height);

        AtomicInteger gapIndex = new AtomicInteger();

        for (EngineReport.UpDown upDown : engineReport.getList())
            paintUpDown(d, upDown, g, gapIndex);

        g2.setColor(Color.black);

        int line = 0;

        if (!this.renderText) {
            return;
        }

        String mouseOverText = NameUtil.getUiName(name);

        // if we have a pin name, append that
        if (pin != null) {
            mouseOverText += "/" + pin;
        }

        g.drawString(mouseOverText, 5, ++line * LINE_SIZE);

        // When the row gets small, omit event count
        if (d.height > 40) {
            g.drawString(engineReport.getList().size() + " events", 5, ++line * LINE_SIZE);
        }
    }

    private void drawStartOfRevolution(Graphics2D g2, Dimension d) {
        if (revolutions == null)
            return;

        RevolutionLog time2rpm = RevolutionLog.parseRevolutions(revolutions);

        Stroke oldStroke = g2.getStroke();

        g2.setStroke(ENGINE_CYCLE_STROKE);
        for (int time : time2rpm.keySet()) {
            int x = translator.timeToScreen(time, d.width);
            g2.setColor(ENGINE_CYCLE_COLOR);
            g2.drawLine(x, 0, x, d.height);

            if (isShowTdcLabel()) {
                g2.rotate(Math.PI / 2);
                g2.drawString("#1 TDC", 0, -x - 3);
                g2.rotate(-Math.PI / 2);
            }
        }

        g2.setStroke(oldStroke);
    }

    protected boolean isShowTdcLabel() {
        return false;
    }

    /**
     * This method draws a vertical line every 100 milliseconds
     */
    private void paintScaleLines(Graphics2D g2, Dimension d) {
        int fromMs = translator.getMinTime() / TIMESCALE_MULT;
        int toMs = translator.getMaxTime() / TIMESCALE_MULT;

        if (toMs - fromMs > d.getWidth() / 5) {
            /**
             * we should not paint time scale lines if the chart is so long that the whole screen would be covered with
             * lines
             * todo: maybe draw scale lines in seconds, not milliseconds?
             */
            return;
        }

        Stroke oldStroke = g2.getStroke();
        g2.setStroke(TIME_SCALE_STROKE);
        g2.setColor(TIME_SCALE_COLOR);

        // start at +1 so we don't render a line at the left edge
        for (int ms = fromMs + 1; ms <= toMs; ms++) {
            int tick = ms * TIMESCALE_MULT;
            int x = translator.timeToScreen(tick, d.width);
            g2.drawLine(x, 0, x, d.height);
        }

        g2.setStroke(oldStroke);
    }

    private void paintUpDown(Dimension d, EngineReport.UpDown upDown, Graphics g, AtomicInteger gapIndex) {
        int x1 = translator.timeToScreen(upDown.upTime, d.width);
        int x2 = translator.timeToScreen(upDown.downTime, d.width);

        // Draw the filled in rectangle body
        g.setColor(signalBody);
        g.fillRect(x1, 0, x2 - x1, d.height);

        // Draw the outline box
        g.setColor(signalBorder);
        g.drawLine(x1, 0, x2, 0);
        g.drawLine(x1, 0, x1, d.height);
        g.drawLine(x2, 0, x2, d.height);
        g.drawLine(x1, d.height, x2, d.height);

        // No text if shorter than 25px
        if (d.height < 25) {
            return;
        }

        // '-1' actually means 'not first wheel' it's coming from
        if (!Double.isNaN(upDown.prevGap)) {
            gapIndex.set(gapIndex.incrementAndGet() % GAP_POSITIONS);
            g.drawString(String.format("gap %.2f", upDown.prevGap), x1, d.height / 2 + gapIndex.get() * g.getFont().getSize());
        }
        if (!Double.isNaN(upDown.gap)) {
            gapIndex.set(gapIndex.incrementAndGet() % GAP_POSITIONS);
            g.drawString(String.format("gap %.2f", upDown.gap), x2, d.height / 2 + gapIndex.get() * g.getFont().getSize());
        }

        if (!this.renderText) {
            return;
        }

        final int duration = upDown.getDuration();

        // don't render duration for zero duration or for trigger
        if (duration != 0 && upDown.upTriggerCycleIndex == -1) {
            g.setColor(Color.red);
            String durationString = String.format(" %.2fms", duration / EngineReport.ENGINE_SNIFFER_TICKS_PER_MS);
            g.drawString(durationString, x1, 15);
        }

        if (upDown.upTriggerCycleIndex != -1) {
            g.setColor(Color.darkGray);
            g.drawString("" + upDown.upTriggerCycleIndex, x1, (int) (0.25 * d.height));
        }

        // Skip second index if invalid or equal to start index
        if (upDown.downTriggerCycleIndex != -1 && upDown.upTriggerCycleIndex != upDown.downTriggerCycleIndex) {
            g.setColor(Color.darkGray);
            g.drawString("" + upDown.downTriggerCycleIndex, x2, (int) (0.25 * d.height));
        }

        // No angle text if shorter than 50px
        if (d.height < 50) {
            return;
        }

        int offset = 3;
        g.setColor(Color.black);

        double fromAngle = time2rpm.getCrankAngleByTime(upDown.upTime);
        String fromAngleStr = RevolutionLog.angle2string(fromAngle);
        g.drawString(fromAngleStr, x1 + offset, (int) (0.5 * d.height));

        double toAngle = time2rpm.getCrankAngleByTime(upDown.downTime);
        String toAngleStr = RevolutionLog.angle2string(toAngle);
        g.drawString(toAngleStr, x1 + offset, (int) (0.75 * d.height));
    }

    public void setRevolutions(StringBuilder revolutions) {
        time2rpm = RevolutionLog.parseRevolutions(revolutions);
    }

    public void setPhysicalPin(String pin) {
        this.pin = pin;
        setToolTip();
    }

    @Override
    public String toString() {
        return "UpDownImage{" +
                "} " + super.toString();
    }
}
