package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;
import org.jetbrains.annotations.NotNull;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class LogicdataStreamFileCsvSandbox {

    // 1 second
    private static final int maxDelayTime = 1_000_000;

    public static void main(String[] args) {
        if (args.length == 0)
            throw new IllegalArgumentException("I want file name parameter please");

        String filename = args[0];
        List<CompositeEvent> events = readCsvFile(filename);


        LogicdataStreamFile file = new LogicdataStreamFile(args[1]);
        file.append(events);
        file.close();
    }

    @NotNull
    private static List<CompositeEvent> readCsvFile(String filename) {
        List<CompositeEvent> events = new ArrayList<>();

        try {
            System.out.println("Reading " + filename);
            BufferedReader reader = new BufferedReader(new FileReader(filename));
            // skip header
            reader.readLine();
            reader.readLine();
            reader.readLine();
            // read records
            String line;
            int cur_time = 0;
            while ((line = reader.readLine()) != null) {
                //System.out.println("* " + line);
                String[] tokens = line.split(",");
                int v1 = Integer.parseInt(tokens[0]);
                int v2 = Integer.parseInt(tokens[1]);
                int v3 = Integer.parseInt(tokens[2]);
                int v4 = Integer.parseInt(tokens[3]);
                int time = (int) (Double.parseDouble(tokens[4]) * 1000.0);
                int dt = (int) (Double.parseDouble(tokens[5]) * 1000.0);
                int v5 = Integer.parseInt(tokens[6]);
                int v6 = Integer.parseInt(tokens[7]);

                // we allow time to increment only in small amounts.
                // for time discontinuities we jump 1 sec
                if (dt < 0 || dt > maxDelayTime)
                    dt = maxDelayTime;
                // we want to catch integer overflow here
                cur_time = Math.addExact(cur_time, dt);

                System.out.println("* " + cur_time);
                events.add(new CompositeEvent(cur_time, v1 == 1, v2 == 1, v3 == 1, v4 == 1, v5 == 1, v6 == 1));
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
        System.out.println("Found " + events.size() + " records");
        return events;
    }
}
