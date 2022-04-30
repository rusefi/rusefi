package com.rusefi;

import com.rusefi.models.Factory;
import com.rusefi.models.MafValue;
import com.rusefi.models.RpmValue;
import com.rusefi.models.Utils;

import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Date: 1/29/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class ReportReader {
    private static final Pattern LINE_PATTERN = Pattern.compile("\\D*(\\d*);a0;(\\d*);a1;-1;rpm;(\\d*);wave;(\\d*).*");
    private static final int INVALID_RPM_DIFF = 200;

    public static void main(String[] args) {
        //List<ReportLine> lines = read("unfiltered.txt");
        readMap("200ohm.txt");

    }

    public static TreeMap<Integer, TreeMap<Integer, ReportLine>> readMap(String filename) {
        if (!new File(filename).exists()) {
            FileLog.MAIN.logLine("Error: not found " + filename);
            return new TreeMap<>();
        }
        List<ReportLine> lines = read(filename);
        FileLog.MAIN.logLine("Got " + lines.size() + " lines");

        lines = filter(Collections.unmodifiableList(lines));

        findMinMax(lines);

        return asMap(lines);
    }

    private static TreeMap<Integer, TreeMap<Integer, ReportLine>> asMap(List<ReportLine> lines) {
        /**
         * map of maps by RPM. inner map is by MAF
         */
        TreeMap<Integer, TreeMap<Integer, ReportLine>> rpm2mapByMaf = new TreeMap<>();
        for (ReportLine cur : lines) {
            int rpmKey = cur.getRpm().getValue();

            // round to 100
            rpmKey -= (rpmKey % 100);

            TreeMap<Integer, ReportLine> maf2line = Utils.getOrCreate(rpm2mapByMaf, rpmKey, new Factory<Integer, TreeMap<Integer, ReportLine>>() {
                public TreeMap<Integer, ReportLine> create(Integer key) {
                    return new TreeMap<>();
                }
            });
            maf2line.put(cur.getMaf().getValue(), cur);
        }
        return rpm2mapByMaf;
    }

    private static void findMinMax(List<ReportLine> lines) {
        int minMaf = 100000;
        int maxMaf = 0;

        int minRpm = 100000;
        int maxRpm = 0;
        for (ReportLine cur : lines) {
            minMaf = Math.min(minMaf, cur.getMaf().getValue());
            maxMaf = Math.max(maxMaf, cur.getMaf().getValue());

            minRpm = Math.min(minRpm, cur.getRpm().getValue());
            maxRpm = Math.max(maxRpm, cur.getRpm().getValue());
        }
        FileLog.MAIN.logLine("MAF range from " + minMaf + " to " + maxMaf);
        FileLog.MAIN.logLine("RPM range from " + minRpm + " to " + maxRpm);
    }

    private static List<ReportLine> filter(List<ReportLine> lines) {
        List<ReportLine> result = new ArrayList<>();
        int maxValidDiff = 0;
        int originalCount = lines.size();
        int removedCount = 0;

        result.add(lines.get(0));

        for (int i = 1; i < lines.size(); i++) {
            ReportLine prev = result.get(result.size() - 1);
            ReportLine cur = lines.get(i);

            int rpmDiff = cur.getRpm().getValue() - prev.getRpm().getValue();
            int timeDiff = cur.getTime() - prev.getTime();

            if (Math.abs(rpmDiff) > INVALID_RPM_DIFF) {
                FileLog.MAIN.logLine("Invalid diff: " + cur);
                removedCount++;
                continue;
            }
            result.add(cur);
            // maximum valid diff
            maxValidDiff = Math.max(maxValidDiff, Math.abs(rpmDiff));
//            System.out.println("current rpm: " + cur + ", rpm diff=" + rpmDiff + " td=" + timeDiff);
//            System.out.println("value," + cur.getRpm().getValue() + "," + cur.getMaf().getValue() + "," + cur.getWave());
        }
        double percent = 100.0 * removedCount / originalCount;
        FileLog.MAIN.logLine(removedCount + " out of " + originalCount + " record(s) removed. " + percent + "%");
        FileLog.MAIN.logLine("Max valid diff: " + maxValidDiff);
        return result;
    }

    private static List<ReportLine> read(String filename) {
        List<ReportLine> result = new LinkedList<>();
        try {
            BufferedReader reader = new BufferedReader(new FileReader(filename));
            String line;
            while ((line = reader.readLine()) != null) {
                //process each line in some way
                ReportLine rl = handleOneLine(line);
                if (rl != null)
                    result.add(rl);
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
        return result;
    }

    private static int prevMaf = -1;
    private static int prevRpm = -1;
    private static int prevWave = -1;

    private static ReportLine handleOneLine(String line) {
//        System.out.println(line);
        Matcher m = LINE_PATTERN.matcher(line);
        if (m.matches()) {
            int time = Integer.parseInt(m.group(1));
            MafValue maf = MafValue.valueOf(m.group(2));
            RpmValue rpm = RpmValue.valueOf(m.group(3));
            int wave = Integer.parseInt(m.group(4));

            if (prevMaf == maf.getValue() && prevRpm == rpm.getValue() && prevWave == wave) {
//                System.out.println("All the same...");
                return null;
            }

            //System.out.println(time + " m=" + maf + " r=" + rpm + " w=" + wave);
            prevMaf = maf.getValue();
            prevRpm = rpm.getValue();
            prevWave = wave;
            return new ReportLine(time, maf, rpm, wave);
        }
        return null;
    }
}
