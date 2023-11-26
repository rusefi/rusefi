package com.rusefi;

import org.apache.commons.math3.stat.descriptive.moment.Mean;
import org.apache.commons.math3.stat.descriptive.moment.StandardDeviation;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

/**
 * 2/15/2015
 */
public class AverageAngles {
    static final String PRIMARY = "T_PRIMARY";
    static final String SECONDARY = "T_SECONDARY";
    private int count;

    enum trigger_event_e {
        SHAFT_PRIMARY_FALLING(PRIMARY),
        SHAFT_PRIMARY_RISING(PRIMARY),
        SHAFT_SECONDARY_FALLING(SECONDARY),
        SHAFT_SECONDARY_RISING(SECONDARY);

        private final String channel;

        trigger_event_e(String channel) {
            this.channel = channel;
        }
    }

    private static final int MAX_RPM_CHANGE = 20;
    private int rpmAtPrevChart;
    final Map<Integer, List<AngleEvent>> angleData = new TreeMap<>();

    public AverageAngles() {
        clear();
    }

    public String getReport() {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        try {
            printReport(new PrintStream(byteArrayOutputStream));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
        return byteArrayOutputStream.toString();
    }

    public void clear() {
        rpmAtPrevChart = -1;
        angleData.clear();
        count = 0;
    }

    public void add(int rpm, String line) {
        if (rpmAtPrevChart != -1 && Math.abs(rpmAtPrevChart - rpm) > MAX_RPM_CHANGE) {
            System.out.println("Skipping due to rpm change: was " + rpmAtPrevChart + " now " + rpm);
            rpmAtPrevChart = rpm;
            return;
        }
        count ++;
        rpmAtPrevChart = rpm;

        String[] v = line.split("\\|");
        System.out.println("rpm " + rpm + ": " + v.length + " values");

        List<AngleEvent> current = new ArrayList<>();

        for (int i = 0; i < v.length / 2; i++) {
            double angle = Double.parseDouble(v[2 * i]);
            int signal = (int) Double.parseDouble(v[2 * i + 1]);
            if (Double.isNaN(angle)) {
                System.out.println("Skipping due to NaN");
                return;
            }
            current.add(new AngleEvent(angle, signal));
        }

        for (int i = 0; i < current.size(); i++) {
            List<AngleEvent> list = angleData.get(i);
            if (list == null) {
                list = new ArrayList<>();
                angleData.put(i, list);
            }
            list.add(current.get(i));
        }
    }

    public void printReport(Appendable stream) throws IOException {
        List<AngleEvent> angles = new ArrayList<>();

        stream.append("Based on " + count + " charts\r\n");

        stream.append("index,average,stdev,diff\r\n");

        double prev = 0;

        for (Map.Entry<Integer, List<AngleEvent>> e : angleData.entrySet()) {
            int k = e.getKey();
            List<AngleEvent> v = e.getValue();
            double[] values = new double[v.size()];
            for (int i = 0; i < v.size(); i++)
                values[i] = v.get(i).getAngle();

            Mean m = new Mean();
            double mean = m.evaluate(values);

            StandardDeviation sd = new StandardDeviation();
            double sdv = sd.evaluate(values, mean);

            angles.add(new AngleEvent(mean, v.get(0).signal));

            double diff = mean - prev;
            prev = mean;
            stream.append(k + "," + mean + "," + sdv + "," + diff + "\r\n");
        }
        if (angleData.isEmpty())
            return;
        double lastValue = angles.get(angles.size() - 1).angle;
        stream.append("Last value = " + lastValue + ", using it to offset...\r\n");
        double delta = 720 - lastValue;
        stream.append("And the " + angles.size() + " angles are:\r\n");
        for (AngleEvent v : angles) {
            stream.append(range720(delta + v.angle) + "\r\n");
        }

        stream.append("And the " + angles.size() + " lines of code are:\r\n");
        for (AngleEvent v : angles) {
            int ckpSignalType = v.signal / 1000;
            trigger_event_e event = trigger_event_e.values()[ckpSignalType];
            String signal = ckpSignalType % 2 == 0 ? "TV_FALL" : "TV_RISE";
            stream.append("s->addEvent2(" + range720(delta + v.angle) + ", " +
                    event.channel +
                    ", " + signal + " PASS_ENGINE_PARAMETER);\r\n");
        }
    }

    private double range720(double v) {
        while (v > 720)
            v -= 702;
        return v;
    }

    private static class AngleEvent {
        private final double angle;
        private final int signal;

        public AngleEvent(double angle, int signal) {
            this.angle = angle;
            this.signal = signal;
        }

        public double getAngle() {
            return angle;
        }
    }
}
