package com.rusefi.waves;

import java.util.Map;

/**
 * A collection of {@link WaveReport}
 * <p/>
 * <p/>
 *
 * @see WaveChartParser
 * <p/>
 * Date: 3/17/14
 * (c) Andrey Belomutskiy
 */
public class WaveChart {
    // todo: merge with NameUtil
    public static final String INJECTOR_1 = "i1";
    public static final String INJECTOR_2 = "i2";
    public static final String INJECTOR_3 = "i3";
    public static final String INJECTOR_4 = "i4";
    public static final String INJECTOR_5 = "i5";
    public static final String INJECTOR_6 = "i6";
    public static final String INJECTOR_7 = "i7";
    public static final String INJECTOR_8 = "i8";
    public static final String SPARK_1 = "c1";
    public static final String SPARK_2 = "c2";
    public static final String SPARK_3 = "c3";
    public static final String SPARK_4 = "c4";

    public static final String TRIGGER_1 = "t1";
    public static final String TRIGGER_2 = "t2";

    public final Map<String, StringBuilder> map;

    public WaveChart(Map<String, StringBuilder> map) {
        this.map = map;
    }

    public RevolutionLog getRevolutionsLog() {
        StringBuilder revolutions = get(RevolutionLog.TOP_DEAD_CENTER_MESSAGE);
        return RevolutionLog.parseRevolutions(revolutions);
    }

    public StringBuilder get(String key) {
        return map.get(key);
    }

    @Override
    public String toString() {
        return "WaveChart{" +
                "size=" + map.size() +
                '}';
    }

    public Map<String, StringBuilder> getMap() {
        return map;
    }
}
