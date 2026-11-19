package com.opensr5.ini;

public class CurveModel {
    private final String curveId;
    private final String title;
    private final String xLabel;
    private final String yLabel;
    private final AxisModel xAxis;
    private final AxisModel yAxis;
    private final String xBins;
    private final String yBins;
    private final String topicHelp;
    private final String xBinsChannel;

    public CurveModel(String curveId, String title, String xLabel, String yLabel,
                      AxisModel xAxis, AxisModel yAxis, String xBins, String yBins) {
        this(curveId, title, xLabel, yLabel, xAxis, yAxis, xBins, yBins, null);
    }

    public CurveModel(String curveId, String title, String xLabel, String yLabel,
                      AxisModel xAxis, AxisModel yAxis, String xBins, String yBins, String topicHelp) {
        this(curveId, title, xLabel, yLabel, xAxis, yAxis, xBins, yBins, topicHelp, null);
    }

    /**
     * @param xBinsChannel output channel of the X axis, from {@code xBins = bins, channel}; null when
     *                     the INI names none
     */
    public CurveModel(String curveId, String title, String xLabel, String yLabel,
                      AxisModel xAxis, AxisModel yAxis, String xBins, String yBins, String topicHelp,
                      String xBinsChannel) {
        this.curveId = curveId;
        this.title = title;
        this.xLabel = xLabel;
        this.yLabel = yLabel;
        this.xAxis = xAxis;
        this.yAxis = yAxis;
        this.xBins = xBins;
        this.yBins = yBins;
        this.topicHelp = topicHelp;
        this.xBinsChannel = xBinsChannel;
    }

    public String getXBinsChannel() {
        return xBinsChannel;
    }

    public String getTopicHelp() {
        return topicHelp;
    }

    public String getCurveId() {
        return curveId;
    }

    public String getTitle() {
        return title;
    }

    public String getXLabel() {
        return xLabel;
    }

    public String getYLabel() {
        return yLabel;
    }

    public AxisModel getxAxis() {
        return xAxis;
    }

    public AxisModel getyAxis() {
        return yAxis;
    }

    public String getxBins() {
        return xBins;
    }

    public String getyBins() {
        return yBins;
    }

    @Override
    public String toString() {
        return "CurveModel{" +
                "curveId='" + curveId + '\'' +
                ", title='" + title + '\'' +
                ", xLabel='" + xLabel + '\'' +
                ", yLabel='" + yLabel + '\'' +
                ", xAxis=" + xAxis +
                ", yAxis=" + yAxis +
                ", xBins='" + xBins + '\'' +
                ", yBins='" + yBins + '\'' +
                '}';
    }
}
