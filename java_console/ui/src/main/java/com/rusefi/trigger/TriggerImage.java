package com.rusefi.trigger;

import com.rusefi.StartupFrame;
import com.rusefi.enums.trigger_type_e;
import com.rusefi.ui.engine.UpDownImage;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.waves.EngineReport;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * This utility produces images of trigger signals supported by rusEFI
 * <p>
 * 06/23/15
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class TriggerImage {
    private static final String OUTPUT_FOLDER = "triggers";
    private static final String TOP_MESSAGE = StartupFrame.LINK_TEXT;

    private static final int WHEEL_BORDER = 20;
    private static final int WHEEL_DIAMETER = 500;
    private static final int SMALL_DIAMETER = 420;
    private static final int _180 = 180;
    public static final int MIN_TIME = 720;

    /**
     * number of extra frames
     */
    public static int EXTRA_COUNT = 1;
    private static int sleepAtEnd;
    private static trigger_type_e onlyOneTrigger = null;

    /**
     * todo: https://github.com/rusefi/rusefi/issues/2077
     * @see TriggerWheelInfo#isCrankBased
     */
    private static String getTriggerName(TriggerWheelInfo triggerName) {
        switch (findByOrdinal(triggerName.getId())) {
            case TT_FORD_ASPIRE:
                return "Ford Aspire";
            case TT_VVT_BOSCH_QUICK_START:
                return "Bosch Quick Start";
            case TT_MAZDA_MIATA_NA:
                return "Miata NA";
            case TT_MAZDA_MIATA_NB1:
                return "Miata NB";
            case TT_SUBARU_SVX:
                return "Subaru SVX";
            case TT_HONDA_K_12_1:
                return "Honda K 1/12";
            case TT_SUBARU_7_6:
                return "Subaru 7/6";
            case TT_GM_24x:
                return "GM 24x";
            case TT_GM_24x_2:
                return "GM 24x 2";
            case TT_SKODA_FAVORIT:
                return "Skoda Favorit";
            case TT_GM_7X:
                return "GM 7x";
            case TT_CHRYSLER_NGC_36_2_2:
                return "Chrysler NGC 36/2/2";
            case TT_ONE:
                return "Single Tooth";
            case TT_2JZ_1_12:
                return "2JZ 1/12";
            case TT_JEEP_4_CYL:
                return "Jeep 4 cylinder";
            case TT_JEEP_18_2_2_2:
                return "18/2/2/2";
            case TT_RENIX_44_2_2:
                return "44/2/2";
            case TT_RENIX_66_2_2_2:
                return "66/2/2/2";
            case TT_TOOTHED_WHEEL_36_1:
                return "36/1";
            case TT_TOOTHED_WHEEL_36_2:
                return "36/2";
            case TT_TRI_TACH:
                return "TriTach";
            case TT_TOOTHED_WHEEL_60_2:
                return "60/2";
            case TT_GM_60_2_2_2:
                return "GM 60/2/2/2";
        }
        return triggerName.getTriggerName();
    }

    public static void main(String[] args) throws InvocationTargetException, InterruptedException {
        final String workingFolder;
        if (args.length < 1) {
            workingFolder = TriggerWheelInfo.DEFAULT_WORK_FOLDER;
        } else {
            workingFolder = args[0];
        }

        if (args.length > 1)
            onlyOneTrigger = findByOrdinal(Integer.parseInt(args[1]));

        if (args.length > 2)
            sleepAtEnd = Integer.parseInt(args[2]);

        FrameHelper f = new FrameHelper();

        JPanel content = new JPanel(new BorderLayout());
        final TriggerPanel triggerPanel = new TriggerPanel() {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension((1 + EXTRA_COUNT) * WIDTH, 480);
            }
        };

        JPanel topPanel = new JPanel(new FlowLayout());

        SwingUtilities.invokeAndWait(() -> {
            content.add(topPanel, BorderLayout.NORTH);
            content.add(triggerPanel, BorderLayout.CENTER);

            f.showFrame(content);
            f.getFrame().setSize(900, 700);

            UiUtils.trueRepaint(content);
        });

        SwingUtilities.invokeAndWait(() -> {
            TriggerWheelInfo.readWheels(workingFolder, wheelInfo -> onWheel(triggerPanel, topPanel, content, wheelInfo));
        });
        Thread.sleep(1000L * sleepAtEnd);
        System.exit(-1);
    }

    private static void onWheel(TriggerPanel triggerPanel, JPanel topPanel, JPanel content, TriggerWheelInfo triggerWheelInfo) {
        if (onlyOneTrigger != null && findByOrdinal(triggerWheelInfo.getId()) != onlyOneTrigger)
            return;

        topPanel.removeAll();

        JPanel firstWheelControl = createWheelPanel(triggerWheelInfo.getFirstWheeTriggerSignals(), true,
                triggerWheelInfo);

        topPanel.add(firstWheelControl);
        topPanel.add(StartupFrame.createLogoLabel());

        List<WaveState> waves = TriggerImage.convertSignalsToWaves(triggerWheelInfo.getSignals());

        if (!waves.get(1).list.isEmpty()) {
            JPanel secondWheelControl = createWheelPanel(triggerWheelInfo.getSecondWheeTriggerSignals(), false,
                    triggerWheelInfo);
            topPanel.add(secondWheelControl);
        }

        UiUtils.trueLayout(topPanel);
        UiUtils.trueLayout(content);

        triggerPanel.tdcPosition = triggerWheelInfo.getTdcPosition();
        triggerPanel.gaps = triggerWheelInfo.getGaps();

        EngineReport re0 = new EngineReport(waves.get(0).list, MIN_TIME, 720 * (1 + EXTRA_COUNT));
        System.out.println(re0);
        EngineReport re1 = new EngineReport(waves.get(1).list, MIN_TIME, 720 * (1 + EXTRA_COUNT));

        triggerPanel.removeAll();
        UpDownImage upDownImage0 = new UpDownImage(re0, "trigger");
        upDownImage0.setRenderText(false);
        triggerPanel.image = upDownImage0;

        UpDownImage upDownImage1 = new UpDownImage(re1, "trigger");
        upDownImage1.setRenderText(false);

        boolean isSingleSensor = re1.getList().isEmpty();

        int height;
        if (isSingleSensor) {
            height = 1;
        } else {
            height = 2;
        }

        triggerPanel.setLayout(new GridLayout(height, 1));

        // always render the first channel
        triggerPanel.add(upDownImage0);

        if (!isSingleSensor)
            triggerPanel.add(upDownImage1);

        triggerPanel.name = getTriggerName(triggerWheelInfo);
//        triggerPanel.id = "#" + triggerWheelInfo.id;

        UiUtils.trueLayout(triggerPanel);
        UiUtils.trueRepaint(triggerPanel);
        content.paintImmediately(content.getVisibleRect());
        new File(OUTPUT_FOLDER).mkdir();
        UiUtils.saveImage(OUTPUT_FOLDER + File.separator + "trigger_" + findByOrdinal(triggerWheelInfo.getId()) + ".png", content);
    }

    @NotNull
    private static JPanel createWheelPanel(List<TriggerSignal> wheel, boolean showTdc,
                                           TriggerWheelInfo shape) {
        return new JPanel() {
            @Override
            public void paint(Graphics g) {
                super.paint(g);

                int middle = WHEEL_BORDER + WHEEL_DIAMETER / 2;
                if (showTdc) {
                    double tdcAngle = Math.toRadians(_180 + shape.getTdcPositionIn360());

                    int smallX = (int) (WHEEL_DIAMETER / 2 * Math.sin(tdcAngle));
                    int smallY = (int) (WHEEL_DIAMETER / 2 * Math.cos(tdcAngle));

                    int tdcMarkRadius = 8;
                    g.setColor(UpDownImage.ENGINE_CYCLE_COLOR);
                    // draw TDC mark and text on the round wheel
                    g.fillOval(middle + smallX - tdcMarkRadius, middle + smallY - tdcMarkRadius,
                            2 * tdcMarkRadius,
                            2 * tdcMarkRadius);

                    g.drawString("TDC", middle + smallX + tdcMarkRadius * 2, middle + smallY);
                }
                g.setColor(Color.black);

                for (int i = 0; i < wheel.size(); i++) {
                    TriggerSignal current = wheel.get(i);

                    drawRadialLine(g, current.getAngle());
                    /**
                     * java arc API is
                     *      * Angles are interpreted such that 0&nbsp;degrees
                     *      * is at the 3'clock position.
                     *      * A positive value indicates a counter-clockwise rotation
                     *      * while a negative value indicates a clockwise rotation.
                     *
                     * we want zero to be at 12'clock position and clockwise rotation
                     */

                    double nextAngle = i == wheel.size() - 1 ? 360 + wheel.get(0).getAngle() : wheel.get(i + 1).getAngle();
                    int arcDuration = (int) (current.getAngle() - nextAngle);
                    int arcStart = (int) arcToRusEFI(nextAngle);
                    if (current.getState() == 1) {
                        g.drawArc(WHEEL_BORDER, WHEEL_BORDER, WHEEL_DIAMETER, WHEEL_DIAMETER, arcStart, arcDuration);
                    } else {
                        int corner = WHEEL_BORDER + (WHEEL_DIAMETER - SMALL_DIAMETER) / 2;
                        g.drawArc(corner, corner, SMALL_DIAMETER, SMALL_DIAMETER, arcStart, arcDuration);
                    }
                }

                int dirArrow = 40;
                g.drawArc(middle - dirArrow, middle - dirArrow, 2 * dirArrow, 2 * dirArrow, 0, 180);
                g.drawLine(middle + dirArrow + 5, middle - 15, middle + dirArrow, middle);

            }

            @Override
            public Dimension getPreferredSize() {
                return new Dimension(WHEEL_DIAMETER + 2 * WHEEL_BORDER, WHEEL_DIAMETER + 2 * WHEEL_BORDER);
            }
        };
    }

    private static double arcToRusEFI(double angle) {
        return angle + _180 - 90;
    }

    private static void drawRadialLine(Graphics g, double angle) {
        int center = WHEEL_BORDER + WHEEL_DIAMETER / 2;

        double radianAngle = Math.toRadians(_180 + angle);

        int smallX = (int) (SMALL_DIAMETER / 2 * Math.sin(radianAngle));
        int smallY = (int) (SMALL_DIAMETER / 2 * Math.cos(radianAngle));
        int largeX = (int) (WHEEL_DIAMETER / 2 * Math.sin(radianAngle));
        int largeY = (int) (WHEEL_DIAMETER / 2 * Math.cos(radianAngle));

        g.drawLine(center + smallX, center + smallY, center + largeX, center + largeY);
    }

    @NotNull
    static List<WaveState> convertSignalsToWaves(List<TriggerSignal> signals) {
        /**
         * todo: what does this code do? does this work?
         * looks to be repeating trigger share couple of times? but not visible on images somehow?
         */
        List<TriggerSignal> toShow = new ArrayList<>(signals);
        for (int i = 1; i <= 2 + EXTRA_COUNT; i++) {
            for (TriggerSignal s : signals)
                toShow.add(new TriggerSignal(s.getWaveIndex(), s.getState(), s.getAngle() + i * 720));
        }

        List<WaveState> waves = new ArrayList<>();
        waves.add(new WaveState());
        waves.add(new WaveState());
        waves.add(new WaveState());

        for (TriggerSignal s : toShow) {
            WaveState.trigger_value_e signal = (s.getState() == 0) ? WaveState.trigger_value_e.TV_LOW : WaveState.trigger_value_e.TV_HIGH;

            WaveState waveState = waves.get(s.getWaveIndex());
            waveState.handle(signal, s.getAngle());
        }
        for (WaveState wave : waves)
            wave.wrap();

        return waves;
    }

    public static trigger_type_e findByOrdinal(int id) {
        // todo: do we care about quicker implementation? probably not
        for (trigger_type_e type : trigger_type_e.values()) {
            if (type.ordinal() == id)
                return type;
        }
        throw new IllegalArgumentException("No type for " + id);
    }

    private static class TriggerPanel extends JPanel {
        public String name = "";
        public String id;
        // angle
        public double tdcPosition;
        public UpDownImage image;
        public TriggerWheelInfo.TriggerGaps gaps;

        @Override
        public void paint(Graphics g) {
            super.paint(g);

            g.setColor(Color.black);

            int w = getWidth();

            int off = g.getFontMetrics().stringWidth(TOP_MESSAGE);
            g.drawString(TOP_MESSAGE, w - off, g.getFont().getSize());

            String line = new Date().toString();
            off = g.getFontMetrics().stringWidth(line);
            g.drawString(line, w - off, 2 * g.getFont().getSize());

            Font f = g.getFont();
            g.setFont(new Font(f.getName(), Font.BOLD, f.getSize() * 3));

            int h = getHeight();

            g.drawString(name, 50, (int) (h * 0.75));
            if (id != null)
                g.drawString(id, 0, (int) (h * 0.9));

            g.setColor(UpDownImage.ENGINE_CYCLE_COLOR);
            int tdcFontSize = (int) (f.getSize() * 1.5);
            g.setFont(new Font(f.getName(), Font.BOLD, tdcFontSize));
            String tdcMessage;
            if (tdcPosition != 0) {
                tdcMessage = "TDC " + formatTdcPosition() + " degree from synchronization point";
            } else {
                tdcMessage = "TDC at synchronization point";
            }
            g.drawString("     " + tdcMessage, 0, tdcFontSize);
            g.setColor(Color.darkGray);
            for (int i = 0; i < gaps.gapFrom.length; i++) {
                String message = "Sync " + (i + 1) + ": From " + gaps.gapFrom[i] + " to " + gaps.gapTo[i];
                g.drawString("            " + message, 0, tdcFontSize * (2 + i));
            }


            if (image == null)
                return;
            int tdcX = image.engineReport.getTimeAxisTranslator().timeToScreen(MIN_TIME + tdcPosition, w);
            g.drawLine(tdcX, 0, tdcX, h);
            Graphics2D g2 = (Graphics2D) g;
            g2.rotate(Math.PI / 2);
            g2.drawString("TDC", 160, -tdcX - 3);
            g2.rotate(-Math.PI / 2);
        }

        private String formatTdcPosition() {
            if ((int) tdcPosition == tdcPosition)
                return Integer.toString((int) tdcPosition);
            return Double.toString(tdcPosition);
        }
    }
}