package com.rusefi.ui.widgets;

import com.irnems.core.Sensor;

/**
 * 7/13/13
 * (c) Andrey Belomutskiy
 */
public class InjectorControl extends BooleanFlagControlPanel {
    public InjectorControl(int id, Sensor sensor) {
        super("Injector " + id, "");
        installStatusReader(sensor);
        installCommand("injector " + id + " ");
    }
}
