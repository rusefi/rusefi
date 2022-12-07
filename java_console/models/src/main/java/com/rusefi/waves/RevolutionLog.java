package com.rusefi.waves;

import java.util.*;

import static com.rusefi.models.Utils.parseIntWithReason;

/**
 * 1/11/14.
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class RevolutionLog {
    private final TreeMap<Integer, Integer> time2rpm;

    public RevolutionLog(TreeMap<Integer, Integer> time2rpm) {
        this.time2rpm = time2rpm;
    }

    public static RevolutionLog parseRevolutions(CharSequence revolutions) {
        TreeMap<Integer, Integer> time2rpm = new TreeMap<>();
        if (revolutions == null)
            return new RevolutionLog(time2rpm);

        String[] r = revolutions.toString().split("!");
        for (int i = 0; i < r.length - 1; i += 2) {
            int rpm = parseIntWithReason(r[i], "RL rpm");
            int time = parseIntWithReason(r[i + 1], "RL time");
            time2rpm.put(time, rpm);
        }
        return new RevolutionLog(time2rpm);
    }

    public int getSize() {
        return time2rpm.size();
    }

    public String getCrankAngleByTimeString(double time) {
        return angle2string(getCrankAngleByTime(time));
    }

    public static String angle2string(double angle) {
        return Double.isNaN(angle) ? "n/a" : String.format("%.1f", angle);
    }

    public double getCrankAngleByTime(double time) {
        return doGetAngle(time, true);
    }

    private double doGetAngle(double time, boolean tryNextRevolution) {
        Map.Entry<Integer, Integer> entry = getTimeAndRpm(time);
        if (entry == null) {
            if (tryNextRevolution && time2rpm.size() >= 2) {
                // we are here if the value is below the first revolution point
                List<Map.Entry<Integer, Integer>> element = new ArrayList<>(time2rpm.entrySet());
                Map.Entry<Integer, Integer> first = element.get(0);
                Map.Entry<Integer, Integer> second = element.get(1);

                int oneRevolutionDuration = second.getKey() - first.getKey();
                return doGetAngle(time + oneRevolutionDuration, false);
            } else {
                return Double.NaN;
            }
        } else {
            double diff = time - entry.getKey();

            Integer rpm = entry.getValue();
            double timeForRevolution = 60000 * EngineReport.ENGINE_SNIFFER_TICKS_PER_MS / rpm;

            return 360.0 * diff / timeForRevolution;
        }
    }

    public Map.Entry<Integer, Integer> getTimeAndRpm(double time) {
        return time2rpm.floorEntry((int) time);
    }

    public Set<Integer> keySet() {
        return time2rpm.keySet();
    }
}
