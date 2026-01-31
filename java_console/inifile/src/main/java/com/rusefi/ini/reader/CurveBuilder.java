package com.rusefi.ini.reader;

import com.opensr5.ini.AxisModel;
import com.opensr5.ini.CurveModel;

import java.util.LinkedList;

public class CurveBuilder {
    private String curveId;
    private String title;
    private AxisModel xAxis;
    private AxisModel yAxis;
    private String xBins;
    private String yBins;

    public void setCurveDefinition(String curveId, String title) {
        this.curveId = curveId;
        this.title = title;
    }

    public void setXAxis(double min, double max, int step) {
        this.xAxis = new AxisModel(min, max, step);
    }

    public void setYAxis(double min, double max, int step) {
        this.yAxis = new AxisModel(min, max, step);
    }

    public void setXBins(String xBins) {
        this.xBins = xBins;
    }

    public void setYBins(String yBins) {
        this.yBins = yBins;
    }

    public boolean isComplete() {
        return curveId != null && xAxis != null && yAxis != null;
    }

    public boolean isActive() {
        return curveId != null;
    }

    public String getCurveId() {
        return curveId;
    }

    public CurveModel build() {
        return new CurveModel(curveId, title, xAxis, yAxis, xBins, yBins);
    }

    public void reset() {
        curveId = null;
        title = null;
        xAxis = null;
        yAxis = null;
        xBins = null;
        yBins = null;
    }

    public boolean handleLine(LinkedList<String> list, java.util.function.Consumer<String> addField, Runnable finishCurrentCurve) {
        String first = list.getFirst();
        if (first.equalsIgnoreCase("curve")) {
            finishCurrentCurve.run();
            list.removeFirst();
            String curveId = list.removeFirst();
            String title = list.isEmpty() ? "" : list.removeFirst();
            setCurveDefinition(curveId, title);
            return true;
        }

        if (!isActive()) {
            return false;
        }

        if (first.equalsIgnoreCase("xAxis")) {
            list.removeFirst();
            double min = com.opensr5.ini.field.IniField.parseDouble(list.removeFirst());
            double max = com.opensr5.ini.field.IniField.parseDouble(list.removeFirst());
            int step = (int) com.opensr5.ini.field.IniField.parseDouble(list.removeFirst());
            setXAxis(min, max, step);
            return true;
        } else if (first.equalsIgnoreCase("yAxis")) {
            list.removeFirst();
            double min = com.opensr5.ini.field.IniField.parseDouble(list.removeFirst());
            double max = com.opensr5.ini.field.IniField.parseDouble(list.removeFirst());
            int step = (int) com.opensr5.ini.field.IniField.parseDouble(list.removeFirst());
            setYAxis(min, max, step);
            return true;
        } else if (first.equalsIgnoreCase("xBins")) {
            list.removeFirst();
            String bins = list.removeFirst();
            setXBins(bins);
            addField.accept(bins);
            return true;
        } else if (first.equalsIgnoreCase("yBins")) {
            list.removeFirst();
            String bins = list.removeFirst();
            setYBins(bins);
            addField.accept(bins);
            return true;
        }

        return false;
    }
}
