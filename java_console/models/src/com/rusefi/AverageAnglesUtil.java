package com.rusefi;

import com.rusefi.FileLog;
import org.apache.commons.math3.stat.descriptive.moment.Mean;
import org.apache.commons.math3.stat.descriptive.moment.StandardDeviation;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

/**
 * 2/15/2015
 */
public class AverageAnglesUtil {

    public static final String ANALOG_CHART = "analog_chart,";

    private static int currentRpm = -1;

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader("a.csv"));

        String line;

        AverageAngles aa = new AverageAngles();

        while ((line = br.readLine()) != null) {
            int index = line.indexOf(FileLog.END_OF_TIMESTAND_TAG);
            if (index == -1)
                continue;
            line = line.substring(index + FileLog.END_OF_TIMESTAND_TAG.length());

            if (line.startsWith("time")) {
                String[] f = line.split(",");
                if (f.length > 3 && f[2].equals("rpm")) {

                    currentRpm = Integer.parseInt(f[3]);
                    System.out.println("New rpm " + currentRpm);
                }
                continue;
            }

            if (!line.startsWith(ANALOG_CHART))
                continue;
            line = line.substring(ANALOG_CHART.length());

            String p[] = line.split(",");
            line = p[0];

            aa.add(currentRpm, line);
            System.out.println(line);
        }

        aa.printReport(System.out);

   }
}