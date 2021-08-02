package com.rusefi;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Andrey Belomutskiy, (c) 2012-2015
 * 6/29/2015
 */
public class OBD2C {
    // 1 Psi = 6.89475729 Kilopascals
    static double PSI_2_KPA = 6.89475729;

    static int RPM_COUNT = 16;
    static int LOAD_COUNT = 16;

    static Bucket[][] buckets = new Bucket[RPM_COUNT][LOAD_COUNT];


    public static void main(String[] args) throws IOException {
        for (int rpmIndex = 0; rpmIndex < RPM_COUNT; rpmIndex++)
            for (int loadIndex = 0; loadIndex < LOAD_COUNT; loadIndex++) {
                buckets[rpmIndex][loadIndex] = new Bucket();
            }

        fillLoadBins(TS2C.loadBins);
        loadRpmBins(TS2C.rpmBins);

        String fileName = "datalog.csv";

        BufferedReader reader = new BufferedReader(new FileReader(fileName));

        String line;

        while ((line = reader.readLine()) != null)
            processLine(line);

        BufferedWriter w = new BufferedWriter(new FileWriter("output.c"));

        TS2C.writeTable(w, new TS2C.ValueSource() {
            @Override
            public float getValue(int loadIndex, int rpmIndex) {
                return buckets[rpmIndex][loadIndex].getValue();
            }
        }, "OBD2C");
    }

    private static void loadRpmBins(float[] rpmBins) {
        for (int i =0;i< rpmBins.length;i++) {
            rpmBins[i] = 800 + (7000 - 800) * i / (rpmBins.length - 1);
        }
        System.out.println("rpm bins: " + Arrays.toString(rpmBins));
    }

    private static void fillLoadBins(float[] loadBins) {
        for (int i = 0; i < loadBins.length; i++)
            loadBins[i] = (float) (100.0 * i / (loadBins.length - 1));
        System.out.println("load bins: " + Arrays.toString(loadBins));
    }

    private static void processLine(String line) {
        String[] values = line.split(",");

        double map = Double.parseDouble(values[1]) * PSI_2_KPA;
        double advance = Double.parseDouble(values[3]);
        double rpm = Double.parseDouble(values[5]);
        System.out.println("map=" + map + ", advance=" + advance + ", rpm=" + rpm);

        int rpmIndex = (int) ((rpm - 800) / ((7000 - 800) / 16));
        int loadIndex = (int) (map / (100 / 16));

        if (rpmIndex >= RPM_COUNT)
            rpmIndex = RPM_COUNT - 1;
        if (loadIndex >= LOAD_COUNT)
            loadIndex = LOAD_COUNT - 1;

        System.out.println("rpm index: " + rpmIndex + ", loadIndex=" + loadIndex);


        buckets[rpmIndex][loadIndex].addValue(advance);

    }

    static class Bucket {
        List<Double> values = new ArrayList<Double>();

        public void addValue(double advance) {
            values.add(advance);
        }

        public float getValue() {
            if (values.isEmpty())
                return -100;
            double total = 0;
            for (double v : values)
                total += v;

            return (float) (total / values.size());
        }
    }
}

