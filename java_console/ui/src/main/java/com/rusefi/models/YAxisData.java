package com.rusefi.models;

import com.rusefi.FileLog;

import java.io.IOException;
import java.io.Writer;
import java.util.Map;
import java.util.NavigableSet;
import java.util.TreeMap;

/**
 * Y>Z mapping for the same X
 * <p/>
 * Date: 3/24/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class YAxisData {
    private final TreeMap<Double, ValuesHolder> values = new TreeMap<>();
    private double maxYValue = Double.MIN_VALUE;
    private double minYValue = Double.MAX_VALUE;
    private final double x;

    public YAxisData(double x) {
        this.x = x;
    }

    public double getX() {
        return x;
    }

    public NavigableSet<Double> getYs() {
        return values.navigableKeySet();
    }

    public float findZ(double y) {
        Map.Entry<Double, ValuesHolder> entry = values.floorEntry(y);
        if (entry != null)
            return entry.getValue().get();
        return values.ceilingEntry(y).getValue().get();
    }

    public void setValue(double y, float value) {
        ValuesHolder holder = getHolder(y);
        holder.set(value);
    }

    public void addValue(double y, float value) {
        ValuesHolder holder = getHolder(y);

        holder.add(value);

        float newAvg = holder.get();
        if (newAvg != value && !Float.isNaN(newAvg)) {
            FileLog.MAIN.logLine("new " + value + " avg " + newAvg + " for x=" + x + "/y=" + y);
        }
    }

    private ValuesHolder getHolder(double y) {
        minYValue = Math.min(minYValue, y);
        maxYValue = Math.max(maxYValue, y);

        ValuesHolder holder = values.get(y);
        if (holder == null)
            values.put(y, holder = new ValuesHolder());
        return holder;
    }

    public double getValue(double y) {
        return values.get(y).get();
    }

    @Override
    public String toString() {
        return "YAxisData{" +
                "size=" + values.size() +
                ", maxYValue=" + maxYValue +
                ", minYValue=" + minYValue +
                ", x=" + x +
                '}';
    }

    public void write(Writer w) throws IOException {
        for (Map.Entry<Double, ValuesHolder> e : values.entrySet())
            w.write("rpm," + x + ",key," + e.getKey() + ",value," + e.getValue().get() + "\r\n");
    }

    private static class ValuesHolder {

        private float total;
        private int count;

        private ValuesHolder() {
        }

        public float get() {
            return total / count;
        }

        public void add(float value) {
            total += value;
            count++;
        }

        public void set(float value) {
            total = value;
            count = 1;
        }
    }
}
