package com.opensr5.ini;

public class CurveModel {
    private final String curveId;
    private final String title;
    private final AxisModel xAxis;
    private final AxisModel yAxis;
    private final String xBins;
    private final String yBins;

    public CurveModel(String curveId, String title, AxisModel xAxis, AxisModel yAxis, String xBins, String yBins) {
        this.curveId = curveId;
        this.title = title;
        this.xAxis = xAxis;
        this.yAxis = yAxis;
        this.xBins = xBins;
        this.yBins = yBins;
    }

    public String getCurveId() {
        return curveId;
    }

    public String getTitle() {
        return title;
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
                ", xAxis=" + xAxis +
                ", yAxis=" + yAxis +
                ", xBins='" + xBins + '\'' +
                ", yBins='" + yBins + '\'' +
                '}';
    }
}
