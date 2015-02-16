package com.rusefi.test;

import com.rusefi.EcuStimulator;
import com.rusefi.ui.util.FrameHelper;

/**
 * 8/13/14
 * (c) Andrey Belomutskiy
 */
public class EcuStimulatorSandbox extends FrameHelper {
    public static void main(String[] args) {
        new EcuStimulatorSandbox().showFrame(EcuStimulator.getInstance().getPanel());
    }
}
