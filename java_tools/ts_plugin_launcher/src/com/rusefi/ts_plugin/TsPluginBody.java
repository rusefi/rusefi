package com.rusefi.ts_plugin;

import javax.swing.*;

public interface TsPluginBody {
    JComponent getContent();

    void close();
}
