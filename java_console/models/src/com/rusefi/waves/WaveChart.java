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
    public static final String INJECTOR_1 = "inj1";
    public static final String INJECTOR_2 = "inj2";
    public static final String INJECTOR_3 = "inj3";
    public static final String INJECTOR_4 = "inj4";
    public static final String INJECTOR_5 = "inj5";
    public static final String INJECTOR_6 = "inj6";
    public static final String INJECTOR_7 = "inj7";
    public static final String INJECTOR_8 = "inj8";
    public static final String SPARK_1 = "spa1";
    public static final String SPARK_2 = "spa2";
    public static final String SPARK_3 = "spa3";
    public static final String SPARK_4 = "spa4";

    public static final String TRIGGER_1 = "c1";
    public static final String TRIGGER_2 = "c2";

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
}
