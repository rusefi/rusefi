package com.rusefi.ui.plugins;

import com.rusefi.ui.UIContext;

import javax.swing.*;

public interface ConsoleTabProvider {
    String getTitle();

    JComponent createTab(UIContext uiContext);
}
