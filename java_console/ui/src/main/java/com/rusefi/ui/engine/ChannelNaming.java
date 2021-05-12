package com.rusefi.ui.engine;

import java.util.HashMap;

public enum ChannelNaming {
    INSTANCE;

    public final HashMap<String, String> channelName2PhysicalPin = new HashMap<>();
}
