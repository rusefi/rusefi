package com.rusefi.ui;

import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.sensor_logs.SensorLogger;
import org.jetbrains.annotations.NotNull;

public class UIContext {
    private final LinkManager linkManager = new LinkManager();

    public SensorLogger sensorLogger = new SensorLogger(this);
    public GaugesPanel.DetachedRepository DetachedRepositoryINSTANCE = new GaugesPanel.DetachedRepository(this);


    @NotNull
    public LinkManager getLinkManager() {
        return linkManager;
    }

    public CommandQueue getCommandQueue() {
        return linkManager.getCommandQueue();
    }
}
