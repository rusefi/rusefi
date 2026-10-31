package com.rusefi.ui.plugins;

import com.rusefi.ui.UIContext;

import javax.swing.*;

/**
 * Adds an optional tab after Update Firmware, Manage Tunes and Connect on the startup screen.
 * Register public implementations with a public no-argument constructor in
 * {@code META-INF/services/com.rusefi.ui.plugins.StartupTabProvider}.
 */
public interface StartupTabProvider {
    String getTitle();

    /**
     * Called on the Swing event dispatch thread once per startup frame.
     * The context is shared with the console, but an ECU connection and INI may not be available yet.
     */
    JComponent createTab(UIContext uiContext);
}
