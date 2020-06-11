package com.rusefi.ui.widgets;

/**
 * Date: 1/14/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class WaveInfoPanel extends BooleanFlagControlPanel {
    public WaveInfoPanel(final int index) {
        super("wave" + index, "active on low");
        installCommand("set_logic_input_mode " + index + " ");
    }
}
