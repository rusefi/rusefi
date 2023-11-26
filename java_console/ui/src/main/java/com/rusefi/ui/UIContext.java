package com.rusefi.ui;

import com.rusefi.SensorSnifferCentral;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.NotNull;

public class UIContext {
    private final LinkManager linkManager = new LinkManager();

    public final SensorSnifferCentral sensorSnifferCentral = new SensorSnifferCentral(linkManager);

    @NotNull
    public LinkManager getLinkManager() {
        return linkManager;
    }

    public CommandQueue getCommandQueue() {
        return linkManager.getCommandQueue();
    }
}
