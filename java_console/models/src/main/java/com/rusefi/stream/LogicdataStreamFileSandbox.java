package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;

import java.util.ArrayList;
import java.util.List;

public class LogicdataStreamFileSandbox {
    public static void main(String[] args) {
        List<CompositeEvent> events = new ArrayList<>();

        long timestamp = 10;

        //events.add(new CompositeEvent(timestamp, false, false, false, false, false, false));

        //events.add(new CompositeEvent(20, true, false, true, false, false, false));
        //events.add(new CompositeEvent(30, false, false, false, false, false, false));
/*
        int signals[][] = {
                { 43, 1 },
                { 18512, 0 },
                { 24848, 1 },
                { 51350, 0 },
                { 70521, 1 },
                { 86247, 0 },
                { 97727, 1 },
                { 127087, 0 },
        };
        for (int i = 0; i < signals.length; i++) {
            events.add(new CompositeEvent(signals[i][0],
                    signals[i][1] == 1, signals[i][2] == 1, signals[i][3] == 1, signals[i][4] == 1,
                    new boolean[]{ signals[i][5] == 1, signals[i][6] == 1, signals[i][7] == 1, signals[i][8] == 1, signals[i][9] == 1, signals[i][10] == 1 },
                    new boolean[]{ signals[i][11] == 1, signals[i][12] == 1, signals[i][13] == 1, signals[i][14] == 1, signals[i][15] == 1, signals[i][16] == 1}
            ));
        }
*/

        /**
         * works with 31000
         * does not work with 63000
         */
        for (int i = 0; i < 31000; i++) {

            timestamp = i * 100;

            boolean primary = (i % 2) == 0;

            events.add(new CompositeEvent(timestamp, primary, false, false, false, false, false));
        }

        //events.add(new CompositeEvent(timestamp + 1000, false, false, false, false, true, false));
        //events.add(new CompositeEvent(timestamp + 2030, false, false, true, false, false, true));

        LogicdataStreamFile file = new LogicdataStreamFile("sandbox.logicdata");
        file.append(events);
        file.close();
    }
}
