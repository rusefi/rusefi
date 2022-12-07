package com.rusefi;

import com.rusefi.core.preferences.storage.Node;

public class PaneSettings {
    private static final String SHOW_ETB = "show_etb_pane";
    public boolean showEtbPane;
    public boolean showTriggerShapePane;
    public boolean showStimulatorPane;

    public PaneSettings(Node config) {
        showEtbPane = Boolean.getBoolean(SHOW_ETB) ;//config.getBoolProperty(SHOW_ETB, false);
    }
}
