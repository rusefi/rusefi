package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;

import java.util.ArrayList;
import java.util.List;

public class LogicdataStreamFileSandbox {
    public static void main(String[] args) {
        List<CompositeEvent> events = new ArrayList<>();

        long timestamp = 10;

        events.add(new CompositeEvent(timestamp, false, false, false, false, false, false));

        events.add(new CompositeEvent(20, true, false, true, false, false, false));
        events.add(new CompositeEvent(30, false, false, false, false, false, false));

        /**
         * works with 31000
         * does not work with 63000
         */
        for (int i = 0; i < 31000; i++) {

            timestamp = 100 + i * 1000;

            boolean primary = (i % 2) == 0;

            events.add(new CompositeEvent(timestamp, primary, false, false, false, false, false));
        }

        events.add(new CompositeEvent(timestamp + 1000, false, false, false, false, true, false));
        events.add(new CompositeEvent(timestamp + 2030, false, false, true, false, false, true));

        LogicdataStreamFile file = new LogicdataStreamFile("sandbox.logicdata");
        file.append(events);
        file.close();
    }
}
