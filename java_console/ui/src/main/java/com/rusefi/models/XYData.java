package com.rusefi.models;

import com.rusefi.FileLog;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.Map;
import java.util.NavigableSet;
import java.util.TreeMap;

/**
 * Date: 3/24/13
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see com.rusefi.file.TableGenerator
 */
public class XYData {
    @NotNull
    private final TreeMap<Double, YAxisData> yDatas = new TreeMap<>();
    private double maxXValue;
    private double minXValue;

    private double maxYValue;
    private double minYValue;

    String date = FileLog.getDate();

    public XYData() {
        clear();
    }

    public float getValue(double x, double y) {
        YAxisData yAxis = findYAxis(x);
        if (yAxis == null)
            return Float.NaN; // empty map?
        return yAxis.findZ(y);
    }

    public double getMaxXValue() {
        return maxXValue;
    }

    public double getMinXValue() {
        return minXValue;
    }

    public double getMaxYValue() {
        return maxYValue;
    }

    public double getMinYValue() {
        return minYValue;
    }

    public void addPoint(int rpm, double key, float value) {
        addPoint(new Point3D(rpm, key, value));
    }

    public void addPoint(Point3D xyz) {
        YAxisData yAxis = getYdata(xyz);
        yAxis.addValue(xyz.getY(), xyz.getZ());
    }

    public void setPoint(Point3D xyz) {
        YAxisData yAxis = getYdata(xyz);
        yAxis.setValue(xyz.getY(), xyz.getZ());
    }

    private YAxisData getYdata(Point3D xyz) {
        minYValue = Math.min(minYValue, xyz.getY());
        maxYValue = Math.max(maxYValue, xyz.getY());
        return getYAxis(xyz.getX());
    }

    @NotNull
    public NavigableSet<Double> getXSet() {
        return yDatas.navigableKeySet();
    }

    @NotNull
    public YAxisData getYAxis(double x) {
        YAxisData result = yDatas.get(x);
        if (result == null) {
            result = new YAxisData(x);
            maxXValue = Math.max(maxXValue, x);
            minXValue = Math.min(minXValue, x);
            yDatas.put(x, result);
        }
        return result;
    }

    @Nullable
    public YAxisData findYAxis(double x) {
        double xfromSet = findXfromSet(x);
        if (Double.isNaN(xfromSet))
            return null;
        return yDatas.get(xfromSet);
    }

    public double findXfromSet(double x) {
        Map.Entry<Double, YAxisData> floorEntry = yDatas.floorEntry(x);
        if (floorEntry != null)
            return floorEntry.getKey();
        Map.Entry<Double, YAxisData> ceilingEntry = yDatas.ceilingEntry(x);
        if (ceilingEntry == null)
            return Double.NaN;
        return ceilingEntry.getKey();
    }

    public void clear() {
        maxXValue = Double.MIN_VALUE;
        minXValue = Double.MAX_VALUE;
        maxYValue = Double.MIN_VALUE;
        minYValue = Double.MAX_VALUE;
        yDatas.clear();
    }

    @Override
    public String toString() {
        return "XYData{" +
                "yDatas.size()=" + yDatas.size() +
                '}';
    }

    public void saveToFile(String filename) {
        try {
            String name = date + filename;
            FileLog.MAIN.logLine("Writing data to " + name);
            Writer w = new FileWriter(name);
            for (YAxisData yAxisData : yDatas.values())
                yAxisData.write(w);

            w.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void fill(Range rpmRange, Range keyRange, int count, float value) {
        clear();
        for (int i = 0; i < count; i++)
            for (int j = 0; j < count; j++) {

                float rpm = rpmRange.getMin() + (rpmRange.getWidth() * i / count);
                float key = keyRange.getMin() + (keyRange.getWidth() * j / count);


                addPoint(new Point3D(rpm, key, value));


            }
    }
}
