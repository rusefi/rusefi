package com.rusefi.ui;

import java.util.ArrayList;
import java.util.List;

/**
 * 7/27/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class ChartRepository {
    private static final ChartRepository instance = new ChartRepository();

    private final List<String> charts = new ArrayList<>();

    private ChartRepository() {
    }

    public static ChartRepository getInstance() {
        return instance;
    }

    public String getChart(int index) {
        if (index < 0 || index >= charts.size())
            throw new IllegalArgumentException("No chart by index " + index);
        return charts.get(index);
    }

    public int getSize() {
        return charts.size();
    }

    public void addChart(String value) {
        charts.add(value);
    }

    public void clear() {
        charts.clear();
    }

    public interface ChartRepositoryListener {
        void onDigitalChart(String chart);
    }

    public ChartScrollControl createControls(final ChartRepositoryListener listener) {
        return new ChartScrollControl(listener);
    }
}
