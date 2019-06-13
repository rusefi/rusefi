package com.rusefi.ui.test;

import com.rusefi.ui.fsio.FlexibleControls;
import com.rusefi.ui.util.FrameHelper;

/**
 * (c) Andrey Belomutskiy
 * 12/4/14
 */
public class FlexibleControlsSandbox {
    public static void main(String[] args) {
        new FrameHelper().showFrame(new FlexibleControls().getPanel());
    }
}
