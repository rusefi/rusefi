package com.rusefi;

import org.apache.commons.math3.stat.descriptive.moment.Mean;
import org.apache.commons.math3.stat.descriptive.moment.StandardDeviation;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

/**
 * 2/15/2015
 */
public class AverageAngles {

    private static final int MAX_RPM_CHANGE = 20;
    private int rpmAtPrevChart;
    Map<Integer, List<Double>> angleData = new TreeMap<>();

    public AverageAngles() {
        clear();
    }

    public void clear() {
        rpmAtPrevChart = -1;
        angleData.clear();
    }

    public void add(int rpm, String line) {
        if (rpmAtPrevChart != -1 && Math.abs(rpmAtPrevChart - rpm) > MAX_RPM_CHANGE) {
            System.out.println("Skipping due to rpm change: was " + rpmAtPrevChart + " now " + rpm);
            rpmAtPrevChart = rpm;
            return;
        }
        rpmAtPrevChart = rpm;


        String v[] = line.split("\\|");
        System.out.println("rpm " + rpm + ": " + v.length + " values");

        for (int i = 0; i < v.length / 2; i++) {

            List<Double> list = angleData.get(i);
            if (list == null) {
                list = new ArrayList<>();
                angleData.put(i, list);
            }
            list.add(Double.parseDouble(v[2 * i]));
        }
    }

    public void printReport() {
        List<Double> angles = new ArrayList<>();


        for (Map.Entry<Integer, List<Double>> e : angleData.entrySet()) {
            int k = e.getKey();
            List<Double> v = e.getValue();
            double values[] = new double[v.size()];
            for (int i = 0; i < v.size(); i++)
                values[i] = v.get(i);

            Mean m = new Mean();
            double mean = m.evaluate(values);

            StandardDeviation sd = new StandardDeviation();
            double sdv = sd.evaluate(values, mean);

            angles.add(mean);

            System.out.println("i " + k + " average=" + mean + " dev=" + sdv);
        }
        if (angleData.isEmpty())
            return;
        Double lastValue = angles.get(angles.size() - 1);
        System.out.println("Last value = " + lastValue);
        double delta = 720 - lastValue;
        for (double v : angles) {
            System.out.println((delta + v));
        }

    }
}
