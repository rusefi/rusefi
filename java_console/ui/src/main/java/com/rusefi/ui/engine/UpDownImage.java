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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.text.SimpleDateFormat;
import java.util.Date;

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
    private static final int TIMESCALE_MULT = (int) (100 * EngineReport.ENGINE_SNIFFER_TICKS_PER_MS); // 100ms
    private static final SimpleDateFormat FORMAT = new SimpleDateFormat("yyyy-MM-dd'T'hh:mm:ss");
    private static final int LINE_SIZE = 20;
    public static final Color TIME_SCALE_COLOR = Color.red;
    public static final Color ENGINE_CYCLE_COLOR = new Color(0, 153, 0);
    private static final BasicStroke TIME_SCALE_STROKE = new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 10.0f,
            new float[]{7.0f, 21.0f}, 0.0f);
    private static final BasicStroke ENGINE_CYCLE_STROKE = new BasicStroke(1, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 10.0f,
            new float[]{21.0f, 7.0f}, 0.0f);

    private long lastUpdateTime;
    private EngineReport engineReport;
    private StringBuilder revolutions;
    private final String name;
    private TimeAxisTranslator translator;
    private RevolutionLog time2rpm = RevolutionLog.parseRevolutions(null);
    /**
     * firmware is sending {@link Fields#PROTOCOL_OUTPIN}
     */
    private String pin = "NO PIN";
    private long mouseEnterTime;
    /**
     * we have variable color depending on signal name
     */
    private Color signalBody = Color.lightGray;
    private Color signalBorder = Color.blue;

    private final Timer repaintTimer = new Timer(1000, new ActionListener() {
        @Override
        public void actionPerformed(ActionEvent e) {
            UiUtils.trueRepaint(UpDownImage.this);
        }
    });
    public boolean showMouseOverText = true;
    private int currentMouseX = -100;

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
        addMouseMotionListener(new MouseAdapter() {
            @Override
            public void mouseMoved(MouseEvent e) {
                currentMouseX = e.getX();
                UiUtils.trueRepaint(UpDownImage.this);
            }
        });
        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                mouseEnterTime = System.currentTimeMillis();
                UiUtils.trueRepaint(UpDownImage.this);
                repaintTimer.restart();
            }
        });
    }

    public UpDownImage setTranslator(TimeAxisTranslator translator) {
        this.translator = translator;
        return this;
    }

    public TimeAxisTranslator createTranslator() {
        return new TimeAxisTranslator() {
            @Override
            public int timeToScreen(int time, int width) {
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
    }

    public void setWaveReport(EngineReport wr, StringBuilder revolutions) {
        this.engineReport = wr;
        propagateDwellIntoSensor(wr);
        this.revolutions = revolutions;
        lastUpdateTime = System.currentTimeMillis();
        UiUtils.trueRepaint(this);
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

        if (showMouseOverText)
            paintScaleLines(g2, d);

        drawStartOfRevolution(g2, d);

        for (EngineReport.UpDown upDown : engineReport.getList())
            paintUpDown(d, upDown, g);


        g2.setColor(Color.black);

        int line = 0;
        boolean justEntered = System.currentTimeMillis() - mouseEnterTime < 1000;
        Font f = getFont();
        if (justEntered) {
            g.setFont(f.deriveFont(Font.BOLD, f.getSize() * 3));
            g.setColor(Color.red);
        }
        if (showMouseOverText) {
            String mouseOverText = NameUtil.getUiName(name);
            if (pin != null)
                mouseOverText += "/" + pin;
            g.drawString(mouseOverText, 5, ++line * LINE_SIZE + (justEntered ? 30 : 0));
        }
        if (justEntered) {
            // revert font & color
            g.setFont(f);
            g.setColor(Color.black);
        }

        if (showMouseOverText) {
            g.drawString("Showing " + engineReport.getList().size() + " events", 5, ++line * LINE_SIZE);
            // todo: this has to be broken in case of real engine since 'SYS_TICKS_PER_MS' here is not correct?
//            g.drawString("Total seconds: " + (duration / EngineReport.SYS_TICKS_PER_MS / 1000.0), 5, ++line * LINE_SIZE);
            g.drawString(FORMAT.format(new Date(lastUpdateTime)), 5, ++line * LINE_SIZE);
        }

    }

    private void drawStartOfRevolution(Graphics2D g2, Dimension d) {
        if (revolutions == null)
            return;

        RevolutionLog time2rpm = RevolutionLog.parseRevolutions(revolutions);

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
    }

    protected boolean isShowTdcLabel() {
        return false;
    }

    /**
     * This method draws a vertical line every 100 milliseconds
     */
    private void paintScaleLines(Graphics2D g2, Dimension d) {
        int fromMs = translator.getMinTime() / TIMESCALE_MULT;
        g2.setStroke(TIME_SCALE_STROKE);
        g2.setColor(TIME_SCALE_COLOR);

        int toMs = translator.getMaxTime() / TIMESCALE_MULT;

        if (toMs - fromMs > d.getWidth() / 5) {
            /**
             * we should not paint time scale lines if the chart is so long that the whole screen would be covered with
             * lines
             * todo: maybe draw scale lines in seconds, not milliseconds?
             */
            return;
        }

        for (int ms = fromMs; ms <= toMs; ms++) {
            int tick = ms * TIMESCALE_MULT;
            int x = translator.timeToScreen(tick, d.width);
            g2.drawLine(x, 0, x, d.height);
        }
    }

    private void paintUpDown(Dimension d, EngineReport.UpDown upDown, Graphics g) {
        int x1 = translator.timeToScreen(upDown.upTime, d.width);
        int x2 = translator.timeToScreen(upDown.downTime, d.width);

        int y = (int) (0.2 * d.height);

        g.setColor(signalBody);
        g.fillRect(x1, y, x2 - x1, d.height - y);

        g.setColor(signalBorder);
        g.drawLine(x1, y, x2, y);
        g.drawLine(x1, y, x1, d.height);
        g.drawLine(x2, y, x2, d.height);

        if (showMouseOverText) {
            g.setColor(Color.red);
            String durationString = String.format(" %.2fms", upDown.getDuration() / EngineReport.ENGINE_SNIFFER_TICKS_PER_MS);
            g.drawString(durationString, x1, (int) (0.5 * d.height));

            double fromAngle = time2rpm.getCrankAngleByTime(upDown.upTime);
            double toAngle = time2rpm.getCrankAngleByTime(upDown.downTime);

            String fromAngleStr = RevolutionLog.angle2string(fromAngle);

            g.setColor(Color.darkGray);
            if (upDown.upTriggerCycleIndex != -1) {
                g.drawString("" + upDown.upTriggerCycleIndex, x1, (int) (0.25 * d.height));
//            System.out.println("digital_event," + upDown.upIndex + "," + fromAngleStr);
            }
            if (upDown.downTriggerCycleIndex != -1) {
                g.drawString("" + upDown.downTriggerCycleIndex, x2, (int) (0.25 * d.height));
//            System.out.println("digital_event," + upDown.downIndex + "," + toAngleStr);
            }

            int offset = 3;
            g.setColor(Color.black);
            g.drawString(fromAngleStr, x1 + offset, (int) (0.75 * d.height));

            g.setColor(Color.black);

            if (Math.abs(x1 - currentMouseX) < 5) {
                double angleDuration = toAngle - fromAngle;
                String durationStr = RevolutionLog.angle2string(angleDuration);
                g.drawString(durationStr, x1 + offset, (int) (1.0 * d.height));
            } else {
                String toAngleStr = RevolutionLog.angle2string(toAngle);
                g.drawString(toAngleStr, x1 + offset, (int) (1.0 * d.height));
            }
        }
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
