package com.rusefi.ui;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ini.IniFileState;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.sensor_logs.SensorLogger;
import org.jetbrains.annotations.NotNull;

/**
 * key frontend app state singleton
 */
public class UIContext {
    private final LinkManager linkManager = new LinkManager();

    public SensorLogger sensorLogger = new SensorLogger(this);

    public IniFileState iniFileState = new IniFileState(this);
    public GaugesPanel.DetachedRepository DetachedRepositoryINSTANCE = new GaugesPanel.DetachedRepository(this);


    @NotNull
    public LinkManager getLinkManager() {
        return linkManager;
    }

    // shortcut method considering how often it's needed
    public BinaryProtocol getBinaryProtocol() {
        return linkManager.getBinaryProtocol();
    }

    public CommandQueue getCommandQueue() {
        return linkManager.getCommandQueue();
    }
}
