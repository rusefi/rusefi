package com.rusefi.waves;

import com.irnems.waves.TimeAxisTranslator;
import com.irnems.waves.ZoomProvider;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * A model of a digital signal represented as a sequence of {@link UpDown}
 *
 * Date: 6/23/13
 * (c) Andrey Belomutskiy
 *
 * @see com.rusefi.AnalogChartPanel.AnalogChart
 */
public class WaveReport implements TimeAxisTranslator {
    public static final String WAVE_CHART = "wave_chart";
    public static final WaveReport MOCK = new WaveReport(Collections.singletonList(new UpDown(0, -1, 1, -1)));
    /**
     * number of ChibiOS systicks per ms
     */
    public static final double SYS_TICKS_PER_MS = 100;
    public static final int mult = (int) (100 * SYS_TICKS_PER_MS); // 100ms
    private static final String WC_DOWN = "d";
    private static final String WC_UP = "u";

    private final List<UpDown> list;
    private int maxTime;
    /**
     * min timestamp on this chart, in systicks
     */
    private int minTime;

    public WaveReport(String report) {
        this(parse(report));
    }

    public WaveReport(List<UpDown> list) {
        this.list = list;
        if (!list.isEmpty()) {
            minTime = list.get(0).upTime;
            maxTime = list.get(list.size() - 1).downTime;
        }
    }

    public static boolean isCloseEnough(double v1, double v2) {
        if (v2 == 0)
            return v1 == 0;
        double ratio = v1 / v2;
        return Math.abs(1 - ratio) < 0.05;
    }

    public List<UpDown> getList() {
        return list;
    }

    @Override
    public int getMaxTime() {
        return maxTime;
    }

    @Override
    public int getMinTime() {
        return minTime;
    }

    @NotNull
    public static List<UpDown> parse(String report) {
        String[] array = report.split("!");
//        if (array.length % 4 != 0)
//            throw new IllegalArgumentException("Unexpected length " + array.length);

        List<UpDown> times = new ArrayList<UpDown>();

        int index = 0;
        if (array[0].equals(WC_DOWN))
            index += 2;

        while (index + 3 < array.length) {
            if (!array[index].equals(WC_UP)) {
                index += 2;
                continue;
            }
            if (!array[index + 2].equals(WC_DOWN)) {
                index += 2;
                continue;
            }

            String upString[] = array[index + 1].split("_");
            String downString[] = array[index + 3].split("_");
            try {
                int upTime = Integer.parseInt(upString[0]);
                int downTime = Integer.parseInt(downString[0]);

                int upIndex = upString.length > 1 ? Integer.parseInt(upString[1]) : -1;
                int downIndex = downString.length > 1 ? Integer.parseInt(downString[1]) : -1;


                times.add(new UpDown(upTime, upIndex, downTime, downIndex));
            } catch (NumberFormatException e) {
                System.err.println("Invalid? [" + upString + "][" + downString + "]");
            }

            index += 4;
        }
        return times;
    }

    @Override
    public int timeToScreen(int time, int width, ZoomProvider zoomProvider) {
        double translated = (time - minTime) * zoomProvider.getZoomValue() / getDuration();
        return (int) (width * translated);
    }

    @Override
    public double screenToTime(int screen, int width, ZoomProvider zoomProvider) {
        //  / SYS_TICKS_PER_MS / 1000
        double time = 1.0 * screen * getDuration() / width / zoomProvider.getZoomValue() + minTime;
        int x2 = timeToScreen((int) time, width, zoomProvider);
//        FileLog.rlog("screenToTime " + (screen - x2));
        return (int) time;
    }

    /**
     * @return Length of this chart ini systicks
     */
    public int getDuration() {
        return maxTime - minTime;
    }

    @Override
    public String toString() {
        return "WaveReport{" +
                "size=" + list.size() +
                ", maxTime=" + maxTime +
                ", minTime=" + minTime +
                '}';
    }

    public static class UpDown {
        // times in sys ticks
        public final int upTime;
        public final int upIndex;
        public final int downTime;
        public final int downIndex;

        UpDown(int upTime, int upIndex, int downTime, int downIndex) {
            this.upTime = upTime;
            this.upIndex = upIndex;
            this.downTime = downTime;
            this.downIndex = downIndex;
        }

        public int getDuration() {
            return downTime - upTime;
        }

        @Override
        public String toString() {
            return "UpDown{" +
                    "upTime=" + upTime +
                    ", downTime=" + downTime +
                    '}';
        }

        public double getDutyCycle(RevolutionLog rl) {
            double angleDuration = (rl.getCrankAngleByTime(downTime) + 720 - rl.getCrankAngleByTime(upTime)) % 360;
            if (angleDuration < 0)
                throw new IllegalStateException("Negative width: " + angleDuration);
            return angleDuration / 360;
        }
    }
}
