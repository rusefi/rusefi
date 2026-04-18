package com.rusefi.ui.wizard;

import javax.swing.*;
import java.util.function.Consumer;

public interface WizardStep {
    String getTitle();

    JComponent getPanel();

    /**
     * INI field name for the wizard progress flag bit,
     * e.g. "wizardNumberOfCylinders"
     */
    String getWizardFlagFieldName();

    void setOnStepCompleted(Consumer<WizardStepResult> callback);

    /** Called each time this step becomes visible — useful for refreshing against the live ECU config. */
    default void onShow() {}
}
