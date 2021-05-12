package com.rusefi.waves;

import com.rusefi.config.generated.Fields;

import java.util.Map;

/**
 * A collection of {@link EngineReport}
 * <p/>
 * <p/>
 *
 * @see EngineChartParser
 * <p/>
 * Date: 3/17/14
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class EngineChart {
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

    public static final String TRIGGER_2 = "t2";
    public static final String MAP_AVERAGING = "map";

    public final Map<String, StringBuilder> map;

    public EngineChart(Map<String, StringBuilder> map) {
        this.map = map;
    }

    public RevolutionLog getRevolutionsLog() {
        StringBuilder revolutions = get(Fields.TOP_DEAD_CENTER_MESSAGE);
        return RevolutionLog.parseRevolutions(revolutions);
    }

    public StringBuilder get(String key) {
        return map.get(key);
    }

    @Override
    public String toString() {
        return "EngineChart{" +
                "size=" + map.size() +
                '}';
    }

    public Map<String, StringBuilder> getMap() {
        return map;
    }
}
