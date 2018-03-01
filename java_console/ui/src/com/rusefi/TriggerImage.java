package com.rusefi;

import com.rusefi.trigger.WaveState;
import com.rusefi.ui.engine.UpDownImage;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.waves.EngineReport;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * This utility produces images of trigger signals supported by rusEfi
 *
 * 06/23/15
 * (c) Andrey Belomutskiy 2013-2017
 */
public class TriggerImage {
    private static final String TRIGGERTYPE = "TRIGGERTYPE";
    private static final String OUTPUT_FOLDER = "triggers";
    private static final String INPUT_FILE_NAME = "triggers.txt";
    private static final String TOP_MESSAGE = "(c) rusEfi 2013-2017";
    private static final String DEFAULT_WORK_FOLDER = ".." + File.separator + "unit_tests";
    /**
     * number of extra frames
     */
    public static int EXTRA_COUNT = 1;

    public static void main(String[] args) throws IOException, InvocationTargetException, InterruptedException {
        final String workingFolder;
        if (args.length != 1) {
            workingFolder = DEFAULT_WORK_FOLDER;
        } else {
            workingFolder = args[0];
        }

        FrameHelper f = new FrameHelper();

        final TriggerPanel triggerPanel = new TriggerPanel() {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension((1 + EXTRA_COUNT) * WIDTH, 480);
            }
        };

        f.showFrame(triggerPanel);

        SwingUtilities.invokeAndWait(new Runnable() {
            @Override
            public void run() {
                try {
                    generateImages(workingFolder, triggerPanel);
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
        System.exit(-1);
    }

    private static void generateImages(String workingFolder, TriggerPanel trigger) throws IOException {
        String fileName = workingFolder + File.separator + INPUT_FILE_NAME;
        BufferedReader br = new BufferedReader(new FileReader(fileName));

        System.out.println("Reading " + fileName);
        String line;
        while ((line = br.readLine()) != null) {
            if (line.trim().startsWith("#")) {
                // skipping a comment
                continue;
            }

            if (line.startsWith(TRIGGERTYPE)) {
                readTrigger(br, line, trigger);
            }
        }
    }

    private static void readTrigger(BufferedReader reader, String line, TriggerPanel triggerPanel) throws IOException {
        String[] tokens = line.split(" ");
        String idStr = tokens[1];
        String eventCountStr = tokens[2];
        String triggerName = tokens[3];
        System.out.println("Processing " + line + " " + idStr);
        triggerPanel.tdcPosition = Double.parseDouble(tokens[4]);
        int eventCount = Integer.parseInt(eventCountStr);
        int id = Integer.parseInt(idStr);

//        if (id != 20)
//            return;

        System.out.println("id=" + id + ", count=" + eventCount + ", name=" + triggerName);

        List<WaveState> waves = readTrigger(reader, eventCount);

        EngineReport re0 = new EngineReport(waves.get(0).list, 720, 720 * (1 + EXTRA_COUNT));
        System.out.println(re0);
        EngineReport re1 = new EngineReport(waves.get(1).list, 720, 720 * (1 + EXTRA_COUNT));
        EngineReport re2 = new EngineReport(waves.get(2).list, 720, 720 * (1 + EXTRA_COUNT));

        triggerPanel.removeAll();
        UpDownImage upDownImage0 = new UpDownImage(re0, "trigger");
        upDownImage0.showMouseOverText = false;
        triggerPanel.add(upDownImage0);

        UpDownImage upDownImage1 = new UpDownImage(re1, "trigger");
        upDownImage1.showMouseOverText = false;

        UpDownImage upDownImage2 = new UpDownImage(re2, "trigger");
        upDownImage2.showMouseOverText = false;

        boolean isSingleSensor = re1.getList().isEmpty();
        boolean isThirdVisible = !re2.getList().isEmpty();

        int height;
        if (isSingleSensor) {
            height = 1;
        } else if (isThirdVisible) {
            height = 3;
        } else {
            height = 2;
        }

        triggerPanel.setLayout(new GridLayout(height, 1));

        if (!isSingleSensor)
            triggerPanel.add(upDownImage1);
        if (isThirdVisible)
            triggerPanel.add(upDownImage2);

        triggerPanel.name = triggerName;
        triggerPanel.id = id;

        UiUtils.trueLayout(triggerPanel);
        UiUtils.trueRepaint(triggerPanel);
        new File(OUTPUT_FOLDER).mkdir();
        UiUtils.saveImage(OUTPUT_FOLDER + File.separator + "trigger_" + id + ".png", triggerPanel);
    }

    @NotNull
    private static List<WaveState> readTrigger(BufferedReader reader, int count) throws IOException {
        String line;
        String[] tokens;
        List<Signal> signals = new ArrayList<>();

        int index = 0;
        while (index < count) {
            line = reader.readLine();
            if (line.trim().startsWith("#"))
                continue;
            tokens = line.split(" ");
            if (tokens.length < 4)
                throw new IllegalStateException("Unexpected [" + line + "]");
            String signalStr = tokens[2];
            int signal = Integer.parseInt(signalStr);
            String angleStr = tokens[3];
            double angle = Double.parseDouble(angleStr);

            Signal s = new Signal(signal, angle);
//            System.out.println(s);
            signals.add(s);
            index++;
        }

        List<Signal> toShow = new ArrayList<>(signals);
        for (int i = 1; i <= 2 + EXTRA_COUNT; i++) {
            for (Signal s : signals)
                toShow.add(new Signal(s.signal, s.angle + i * 720));
        }

        List<WaveState> waves = new ArrayList<>();
        waves.add(new WaveState());
        waves.add(new WaveState());
        waves.add(new WaveState());

        for (Signal s : toShow) {
            int waveIndex = s.signal / 1000;
            WaveState.trigger_value_e signal = (s.signal % 1000 == 0) ? WaveState.trigger_value_e.TV_LOW : WaveState.trigger_value_e.TV_HIGH;

            WaveState waveState = waves.get(waveIndex);
            waveState.handle(signal, s.angle);
        }
        for (WaveState wave : waves)
            wave.wrap();

        return waves;
    }

    public static int angleToTime(double prevUp) {
        return (int) (prevUp);
    }

    private static class Signal {
        private final int signal;
        private final double angle;

        public Signal(int signal, double angle) {
            this.signal = signal;
            this.angle = angle;
        }

        @Override
        public String toString() {
            return "Signal{" +
                    "signal=" + signal +
                    ", angle=" + angle +
                    '}';
        }
    }

    private static class TriggerPanel extends JPanel {
        public String name = "";
        public int id;
        public double tdcPosition;

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

            g.drawString(name, 0, (int) (h * 0.75));
            g.drawString("#" + id, 0, (int) (h * 0.9));

            g.setColor(Color.green);
            int tdcFontSize = (int) (f.getSize() * 1.5);
            g.setFont(new Font(f.getName(), Font.BOLD, tdcFontSize));
            g.drawString("tdcPosition " + formatTdcPosition(), 0, tdcFontSize);

            int tdcX = (int) (w / 720.0 * tdcPosition);
            g.drawLine(tdcX, 0, tdcX, h);
        }

        private String formatTdcPosition() {
            if ((int) tdcPosition == tdcPosition)
                return Integer.toString((int) tdcPosition);
            return Double.toString(tdcPosition);
        }
    }
}