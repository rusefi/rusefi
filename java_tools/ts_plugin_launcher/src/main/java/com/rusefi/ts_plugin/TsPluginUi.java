package com.rusefi.ts_plugin;

import javax.swing.*;

public interface TsPluginUi {
    String PLUGIN_UI_IMPL_CLASS = "com.rusefi.ts_plugin.TsPluginUiImpl";

    JComponent getContent();
}
