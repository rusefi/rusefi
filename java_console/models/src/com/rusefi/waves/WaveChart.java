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
    public static final String INJECTOR_1 = "Injector 1";
    public static final String INJECTOR_2 = "Injector 2";
    public static final String INJECTOR_3 = "Injector 3";
    public static final String INJECTOR_4 = "Injector 4";
    public static final String INJECTOR_5 = "Injector 5";
    public static final String INJECTOR_6 = "Injector 6";
    public static final String SPARK_1 = "Spark 1";
    public static final String SPARK_2 = "Spark 2";
    public static final String SPARK_3 = "Spark 3";
    public static final String SPARK_4 = "Spark 4";

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
