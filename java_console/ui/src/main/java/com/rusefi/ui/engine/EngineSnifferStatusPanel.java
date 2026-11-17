package com.rusefi.ui.engine;

import com.rusefi.ui.widgets.ConsoleFonts;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.waves.TimeAxisTranslator;
import com.rusefi.waves.EngineReport;
import com.rusefi.waves.RevolutionLog;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.util.Map;

/**
 * Status strip at the bottom of Digital Sniffer - {@link EngineSnifferPanel}: the time, crank angle
 * and RPM under the mouse on the left, the legend of the vertical lines on the right. Styled like the
 * console status bar [tag:console_theme].
 * <p/>
 * <p/>
 * Date: 12/26/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class EngineSnifferStatusPanel {
    static final String NO_VALUE = "—";
    static final String ENGINE_CYCLE_LEGEND = "Engine cycle (#1 TDC)";
    static final String TIME_SCALE_LEGEND = "20 ms grid";

    public final JPanel infoPanel = new JPanel(new BorderLayout()) {
        @Override
        protected void paintComponent(Graphics g) {
            g.setColor(themed("Console.StatusBar.background", getBackground()));
            g.fillRect(0, 0, getWidth(), getHeight());
            g.setColor(themed("Separator.foreground", Color.LIGHT_GRAY));
            g.drawLine(0, 0, getWidth(), 0);
        }
    };
    private final JLabel timeLabel = value("-00.00000 sec");
    private final JLabel angleLabel = value("-0000.0");
    private final JLabel rpmLabel = value("00000");
    private final DashedLineIcon engineCycleIcon = new DashedLineIcon(SnifferPalette.LIGHT.engineCycle);
    private final DashedLineIcon timeScaleIcon = new DashedLineIcon(SnifferPalette.LIGHT.timeScale);
    private final JPanel legend;
    private TimeAxisTranslator translator =EngineReport.MOCK.getTimeAxisTranslator();

    private RevolutionLog time2rpm = RevolutionLog.parseRevolutions(null);

    public final MouseMotionAdapter motionAdapter = new MouseMotionAdapter() {
        @Override
        public void mouseMoved(MouseEvent event) {
            int x = event.getX();

            UpDownImage s = (UpDownImage) event.getSource();

            /**
             * Time which corresponds to the mouse cursor screen location
             */
            double time = translator.screenToTime(x, s.getWidth());
            timeLabel.setText(String.format("%.5f sec", time));

            String text = time2rpm == null ? "n/a" : time2rpm.getCrankAngleByTimeString(time);
            angleLabel.setText(text);

            Map.Entry<Integer, Integer> e = time2rpm.getTimeAndRpm(time);
            if (e == null) {
                rpmLabel.setText("n/a");
            } else {
                rpmLabel.setText("" + e.getValue());
            }
        }
    };

    public EngineSnifferStatusPanel() {
        JPanel readout = new JPanel(new FlowLayout(FlowLayout.LEFT, 6, 0));
        readout.setOpaque(false);
        readout.add(muted(new JLabel("Time")));
        readout.add(timeLabel);
        readout.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        readout.add(muted(new JLabel("Angle")));
        readout.add(angleLabel);
        readout.add(Box.createHorizontalStrut(JoinedButtons.ROW_GAP));
        readout.add(muted(new JLabel("RPM")));
        readout.add(rpmLabel);

        JPanel legend = new JPanel(new FlowLayout(FlowLayout.RIGHT, 14, 0));
        legend.setOpaque(false);
        legend.add(legendItem(ENGINE_CYCLE_LEGEND, engineCycleIcon));
        legend.add(legendItem(TIME_SCALE_LEGEND, timeScaleIcon));
        this.legend = legend;

        infoPanel.setBorder(BorderFactory.createEmptyBorder(7, 4, 6, JoinedButtons.ROW_END_INSET - 14));
        infoPanel.add(readout, BorderLayout.CENTER);
        infoPanel.add(legend, BorderLayout.EAST);
    }

    public void setTimeAxisTranslator(TimeAxisTranslator translator) {
        this.translator = translator;
    }

    public void setRevolutions(StringBuilder revolutions) {
        time2rpm = RevolutionLog.parseRevolutions(revolutions);
    }

    /** The legend swatches follow the chart's light or dark plot colours. */
    void setLegendColors(Color engineCycle, Color timeScale) {
        engineCycleIcon.color = engineCycle;
        timeScaleIcon.color = timeScale;
        legend.repaint();
    }

    /** A mono value label, wide enough for {@code prototype} so the strip does not jump while the mouse moves. */
    private static JLabel value(String prototype) {
        JLabel label = new JLabel(prototype);
        label.setFont(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize()));
        label.setPreferredSize(label.getPreferredSize());
        label.setText(NO_VALUE);
        return label;
    }

    private static JLabel muted(JLabel label) {
        label.setForeground(UIManager.getColor("Label.disabledForeground"));
        return label;
    }

    private static JLabel legendItem(String text, DashedLineIcon line) {
        JLabel label = muted(new JLabel(text, line, SwingConstants.LEADING));
        label.setIconTextGap(6);
        return label;
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    /** A short dashed vertical line, drawn like the chart's own engine cycle and time scale lines. */
    private static final class DashedLineIcon implements Icon {
        private static final int WIDTH = 6;
        private static final int HEIGHT = 14;
        private static final Stroke DASH = new BasicStroke(2, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 10f,
            new float[]{3, 2}, 0);
        private Color color;

        DashedLineIcon(Color color) {
            this.color = color;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setColor(color);
                g2.setStroke(DASH);
                g2.drawLine(x + WIDTH / 2, y, x + WIDTH / 2, y + HEIGHT);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return WIDTH;
        }

        @Override
        public int getIconHeight() {
            return HEIGHT;
        }
    }
}
