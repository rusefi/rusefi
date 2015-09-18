package com.rusefi.test;

import com.rusefi.SensorSnifferPane;
import com.rusefi.ui.storage.Node;
import com.rusefi.ui.util.FrameHelper;

/**
 * 2/16/15
 * (c) Andrey Belomutskiy
 */

public class SensorSnifferPaneSandbox extends FrameHelper {
    public static void main(String[] args) {
        Node config = null;//getConfig().getRoot().getChild("sensor_sniffer");
        new EcuStimulatorSandbox().showFrame(new SensorSnifferPane(config).getPanel());
    }
}