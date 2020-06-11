package com.rusefi.models;

import com.rusefi.FileLog;

import java.util.Set;

/**
 * 7/18/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class AverageData {
    public static XYData average(XYData data, int divider) {

        double minX = data.getMinXValue();
        double xWidth = data.getMaxXValue() - minX;

        FileLog.MAIN.logLine("From x" + minX + " w=" + xWidth);

        XYData result = new XYData();

        for (int i = 0; i < divider; i++) {
            double fromX_ = minX + (xWidth * i) / divider;
            double toX_ = minX + (xWidth * (i + 1)) / divider;

            FileLog.MAIN.logLine("from " + fromX_ + " to " + toX_);

//            double fromX = data.findXfromSet(fromX_);
//            double toX = data.findXfromSet(toX_);
//            System.out.println("from internal " + fromX + " to internal " + toX);

            average(data, result, fromX_, toX_, divider);
        }
        return result;
    }

    private static void average(XYData data, XYData result, double fromX, double toX, int divider) {
        double minY = data.getMinYValue();
        double yWidth = data.getMaxYValue() - minY;

        for (int i = 0; i < divider; i++) {
            double fromY_ = minY + (yWidth * i) / divider;
            double toY_ = minY + (yWidth * (i + 1)) / divider;

            Set<Double> xRange = data.getXSet().tailSet(fromX).headSet(toX);

            int counter = 0;
            double acc = 0;
            for (Double x : xRange) {
                YAxisData yData = data.getYAxis(x);

                Set<Double> yRange = yData.getYs().tailSet(fromY_).headSet(toY_);

                for (double y : yRange) {
                    counter++;
                    acc += yData.getValue(y);
                }
            }

            if (counter == 0)
                result.addPoint(new Point3D(fromX, fromY_, Float.NaN));
            else
                result.addPoint(new Point3D(fromX, fromY_, (float) (acc / counter)));
        }
    }
}