package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.tune.xml.Msq;

public class UploadQueue {
    public static void enqueue(ControllerAccess controllerAccess, String configurationName) {
        Msq msq = TuneUploder.grabTune(controllerAccess, configurationName);
    }
}
