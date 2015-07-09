package com.rusefi.ui.widgets;

import com.rusefi.core.Sensor;

/**
 * 7/11/13
 * (c) Andrey Belomutskiy
 */
public class AdcDebugControl extends BooleanFlagControlPanel {
    public AdcDebugControl() {
        super("Adc Debug", "");
        installStatusReader(Sensor.ADC_STATUS);
        installCommand("adcDebug ");
    }
}
