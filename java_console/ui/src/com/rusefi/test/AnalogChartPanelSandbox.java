package com.rusefi.test;

import com.rusefi.SensorSnifferPane;
import com.rusefi.ui.util.FrameHelper;

/**
 * 2/16/15
 * (c) Andrey Belomutskiy
 */

public class AnalogChartPanelSandbox extends FrameHelper {
    public static void main(String[] args) {
        new EcuStimulatorSandbox().showFrame(new SensorSnifferPane().getPanel());
    }
}