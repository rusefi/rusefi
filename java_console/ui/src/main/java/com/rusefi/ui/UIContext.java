package com.rusefi.ui;

import com.rusefi.io.LinkManager;
import com.rusefi.sensor_logs.SensorLogger;

public class UIContext {
    private final LinkManager linkManager = new LinkManager();

    public SensorLogger sensorLogger = new SensorLogger();

    public LinkManager getLinkManager() {
        return linkManager;
    }
}
