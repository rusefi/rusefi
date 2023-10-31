package com.rusefi.waves;

import com.rusefi.config.generated.Fields;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static com.rusefi.config.generated.Fields.PROTOCOL_ES_DOWN;
import static com.rusefi.config.generated.Fields.PROTOCOL_ES_UP;

/**
 * A model of a digital signal represented as a sequence of {@link UpDown}
 *
 * Date: 6/23/13
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see SensorSnifferPane
 */
public class EngineReport {
    public static final String ENGINE_CHART = Fields.PROTOCOL_ENGINE_SNIFFER;
    public static final EngineReport MOCK = new EngineReport(Collections.singletonList(new UpDown(0, -1, 1, -1, Double.NaN, Double.NaN)));
    /**
     * number of Engine Sniffer ticks per ms
     */
    public static final double ENGINE_SNIFFER_TICKS_PER_MS = 1000.0 / Fields.ENGINE_SNIFFER_UNIT_US;
    public static final double RATIO = 0.05;

    private final List<UpDown> list;
    private int maxTime;
    /**
     * min timestamp on this chart, in Engine Sniffer ticks
     */
    private int minTime;

    public EngineReport(String report) {
        this(parse(report));
    }

    public EngineReport(List<UpDown> list) {
        this.list = list;
        if (!list.isEmpty()) {
            minTime = list.get(0).upTime;
            maxTime = list.get(list.size() - 1).downTime;
        }
    }

    public EngineReport(List<UpDown> list, int minTime, int maxTime) {
        this.list = list;
        this.minTime = minTime;
        this.maxTime = maxTime;
    }

    public static boolean isCloseEnough(double v1, double v2) {
        return isCloseEnough(v1, v2, RATIO);
    }

    public static boolean isCloseEnough(double v1, double v2, double th) {
        if (v2 == 0)
            return v1 == 0;
        double ratio = v1 / v2;
        return Math.abs(1 - ratio) < th;
    }

    public List<UpDown> getList() {
        return list;
    }

    private final TimeAxisTranslator timeAxisTranslator = new TimeAxisTranslator() {
        @Override
        public int getMaxTime() {
            return maxTime;
        }

        @Override
        public int getMinTime() {
            return minTime;
        }

        @Override
        public int timeToScreen(double time, int width) {
            // 0 = left side
            // 1 = right side
            double fraction = (time - minTime) / getDuration();

            // Space to leave on the left side to avoid overlap
            int offset = 150;

            return (int) (offset + (width - offset) * fraction);
        }

        @Override
        public double screenToTime(int screenX, int screenWidth) {
            double time = 1.0 * screenX * getDuration() / screenWidth + minTime;
            return (int) time;
        }
    };

    public TimeAxisTranslator getTimeAxisTranslator() {
        return timeAxisTranslator;
    }

    /**
     * @see EngineChartParser#unpackToMap(String)
     */
    @NotNull
    public static List<UpDown> parse(String report) {
        String[] array = report.split("!");

        List<UpDown> times = new ArrayList<>();

        int index = 0;
        if (array[0].equals(PROTOCOL_ES_DOWN))
            index += 2;

        while (index + 3 < array.length) {
            if (!array[index].startsWith(PROTOCOL_ES_UP)) {
                index += 2;
                continue;
            }
            if (!array[index + 2].startsWith(PROTOCOL_ES_DOWN)) {
                index += 2;
                continue;
            }

            String upString[] = array[index].split("_");
            String downString[] = array[index + 2].split("_");
            try {
                int upTime = Integer.parseInt(array[index + 1]);
                int downTime = Integer.parseInt(array[index + 3]);

                int upEventIndex = upString.length > 1 ? Integer.parseInt(upString[1]) : -1;
                int downEventIndex = downString.length > 1 ? Integer.parseInt(downString[1]) : -1;


                times.add(new UpDown(upTime, upEventIndex, downTime, downEventIndex, Double.NaN, Double.NaN));
            } catch (NumberFormatException e) {
                System.err.println("Invalid? [" + Arrays.toString(upString) + "][" + Arrays.toString(downString) + "]");
            }

            index += 4;
        }
        return times;
    }

    /**
     * @return Length of this chart in Engine Sniffer ticks
     */
    public int getDuration() {
        return maxTime - minTime;
    }

    @Override
    public String toString() {
        return "EngineReport{" +
                "size=" + list.size() +
                ", maxTime=" + maxTime +
                ", minTime=" + minTime +
                '}';
    }

    public static class UpDown {
        // times in sys ticks
        public final int upTime;
        /**
         * Only trigger channels have these
         */
        public final int upTriggerCycleIndex;
        public final int downTime;
        public final int downTriggerCycleIndex;
        public final double prevGap;
        public final double gap;

        public UpDown(int upTime, int upIndex, int downTime, int downIndex, double prevGap, double gap) {
            this.upTime = upTime;
            this.upTriggerCycleIndex = upIndex;
            this.downTime = downTime;
            this.downTriggerCycleIndex = downIndex;
            this.prevGap = prevGap;
            this.gap = gap;
        }

        public int getDuration() {
            return downTime - upTime;
        }

        @Override
        public String toString() {
            return "UpDown{" +
                    "upTime=" + upTime +
                    ", downTime=" + downTime +
                    ", d=" + getDuration() +
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
