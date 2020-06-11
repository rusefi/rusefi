package com.rusefi;

import com.rusefi.ui.storage.Node;

public class PaneSettings {
    private static final String SHOW_ETB = "show_etb_pane";
    private static final String SHOW_FUEL_TUNE = "show_fuel_tune_pane";
    public boolean showEtbPane;
    public boolean showFuelTunePane;
    public boolean showTriggerShapePane;
    public boolean showStimulatorPane;


    public PaneSettings(Node config) {
        showEtbPane = Boolean.getBoolean(SHOW_ETB) ;//config.getBoolProperty(SHOW_ETB, false);
        showFuelTunePane = config.getBoolProperty(SHOW_FUEL_TUNE, false);
    }
}
