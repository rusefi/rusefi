package com.rusefi.ts_plugin;

import com.rusefi.ui.util.FrameHelper;

/**
 * @see PluginLauncherSandbox
 */
public class PluginBodySandbox {
    public static void main(String[] args) {
        new FrameHelper().showFrame(new PluginEntry().getContent());
    }
}
