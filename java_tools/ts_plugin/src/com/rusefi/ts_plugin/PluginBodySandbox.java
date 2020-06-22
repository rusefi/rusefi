package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.ui.util.FrameHelper;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * @see PluginLauncherSandbox
 */
public class PluginBodySandbox {
    public static void main(String[] args) {
        new FrameHelper().showFrame(new PluginEntry(() -> {
            ControllerAccess controllerAccess = mock(ControllerAccess.class);
            when(controllerAccess.getEcuConfigurationNames()).thenReturn(new String[]{"dev"});
            return controllerAccess;
        }).getContent());
    }
}
