package com.rusefi.stream;

import com.rusefi.composite.CompositeEvent;

import java.util.ArrayList;
import java.util.List;

public class LogicdataStreamFileSandbox {
    public static void main(String[] args) {
        List<CompositeEvent> events = new ArrayList<>();
        events.add(new CompositeEvent(10, false, false, false, false, false, false));
        events.add(new CompositeEvent(20, true, false, true, false, false, false));
        events.add(new CompositeEvent(30, false, false, false, false, false, false));
        events.add(new CompositeEvent(1000030, false, false, false, false, true, false));
        events.add(new CompositeEvent(2000030, false, false, true, false, false, true));

        LogicdataStreamFile file = new LogicdataStreamFile("sandbox.logicdata");
        file.append(events);
        file.close();
    }
}
