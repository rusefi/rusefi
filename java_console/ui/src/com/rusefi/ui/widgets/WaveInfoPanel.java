package com.rusefi.ui.widgets;

/**
 * Date: 1/14/13
 * (c) Andrey Belomutskiy
 */
public class WaveInfoPanel extends BooleanFlagControlPanel {
    public WaveInfoPanel(final int index) {
        super("wave" + index, "active on low");
        installCommand("wm " + index + " ");
    }
}
