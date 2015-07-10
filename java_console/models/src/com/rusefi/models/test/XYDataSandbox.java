package com.rusefi.models.test;

import com.rusefi.models.XYData;

/**
 * 7/24/13
 * (c) Andrey Belomutskiy
 */
public class XYDataSandbox {
    public static void main(String[] args) {
        XYData d = new XYData();

        d.addPoint(600, 3, 11);
        d.addPoint(600, 3.1, 11);

        d.addPoint(1600, 3.1, 11);

        d.saveToFile("_mult.dat");
    }
}
