package com.rusefi.models;

import com.rusefi.FileLog;

import java.io.*;

/**
 * 6/30/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class XYDataReader {
    public static XYData readFile(String fileName) {
        if (!new File(fileName).exists())
            throw new IllegalArgumentException("No file: " + fileName);
        try {
            return doReadFile(fileName);
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    private static XYData doReadFile(String fileName) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(fileName));
        String line;
        XYData data = new XYData();
        while ((line = reader.readLine()) != null) {
            //process each line in some way
            Point3D xyz = Point3D.parseLine(line);
            data.addPoint(xyz);
        }
        FileLog.MAIN.logLine("x range: " + data.getMinXValue() + " to " + data.getMaxXValue());
        FileLog.MAIN.logLine("y range: " + data.getMinYValue() + " to " + data.getMaxYValue());
        return data;
    }
}
