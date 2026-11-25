package com.rusefi.ini.reader;

import com.opensr5.ini.AxisModel;
import com.opensr5.ini.CurveModel;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class CurveBuilder {
    // TunerStudio treats the axis step (grid divisions) as optional, e.g. "yAxis = -5, 5,"
    private static final int DEFAULT_AXIS_STEP = 10;

    private String curveId;
    private String title;
    private String xLabel;
    private String yLabel;
    private AxisModel xAxis;
    private AxisModel yAxis;
    private String xBins;
    private String xBinsChannel;
    private String yBins;
    private final List<String> yBinsSeries = new ArrayList<>();
    private final List<String> yColumnLabels = new ArrayList<>();
    private final List<String> lineLabels = new ArrayList<>();
    private String topicHelp;

    public void setCurveDefinition(String curveId, String title) {
        this.curveId = curveId;
        this.title = title;
    }

    public void setColumnLabels(String xLabel, String yLabel) {
        this.xLabel = xLabel;
        this.yLabel = yLabel;
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
        yBinsSeries.add(yBins);
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
        List<String> labels = new ArrayList<>(yBinsSeries.size());
        for (int i = 0; i < yBinsSeries.size(); i++) {
            String label = i < lineLabels.size() ? lineLabels.get(i) : "";
            if (label.trim().isEmpty() && i < yColumnLabels.size()) {
                label = yColumnLabels.get(i);
            }
            labels.add(label);
        }
        return new CurveModel(curveId, title, xLabel, yLabel, xAxis, yAxis, xBins, yBins, topicHelp,
                xBinsChannel, yBinsSeries, labels);
    }

    public void reset() {
        curveId = null;
        title = null;
        xLabel = null;
        yLabel = null;
        xAxis = null;
        yAxis = null;
        xBins = null;
        xBinsChannel = null;
        yBins = null;
        yBinsSeries.clear();
        yColumnLabels.clear();
        lineLabels.clear();
        topicHelp = null;
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

        if (first.equalsIgnoreCase("topicHelp")) {
            list.removeFirst();
            topicHelp = list.isEmpty() ? null : list.removeFirst();
            return true;
        } else if (first.equalsIgnoreCase("columnLabel")) {
            list.removeFirst();
            setColumnLabels(list.removeFirst(), list.removeFirst());
            yColumnLabels.add(yLabel);
            yColumnLabels.addAll(list);
            return true;
        } else if (first.equalsIgnoreCase("lineLabel")) {
            lineLabels.add(list.size() >= 2 ? list.get(1) : "");
            return true;
        } else if (first.equalsIgnoreCase("xAxis")) {
            list.removeFirst();
            String min = list.removeFirst();
            String max = list.removeFirst();
            int step = list.isEmpty() ? DEFAULT_AXIS_STEP : (int) com.opensr5.ini.field.IniField.parseDouble(list.removeFirst());
            xAxis = new AxisModel(min, max, step);
            return true;
        } else if (first.equalsIgnoreCase("yAxis")) {
            list.removeFirst();
            String min = list.removeFirst();
            String max = list.removeFirst();
            int step = list.isEmpty() ? DEFAULT_AXIS_STEP : (int) com.opensr5.ini.field.IniField.parseDouble(list.removeFirst());
            yAxis = new AxisModel(min, max, step);
            return true;
        } else if (first.equalsIgnoreCase("xBins")) {
            list.removeFirst();
            String bins = list.removeFirst();
            setXBins(bins);
            // "xBins = fan1TempBins, coolantTemperature": the output channel shows the live point
            String channel = list.isEmpty() ? null : list.removeFirst();
            xBinsChannel = channel == null || channel.equalsIgnoreCase("readonly") ? null : channel;
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
