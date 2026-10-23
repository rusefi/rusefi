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

    /** Called before this step stops being visible. */
    default void onHide() {}

    /**
     * The step's Continue button, shown at the right of the wizard footer so every step advances from the
     * same place. The step keeps its listener and enabled state; null when the step completes from a choice
     * in its card instead (see {@link #getFooterHint()}). Read after {@link #onShow()}.
     */
    default JButton getContinueButton() { return null; }

    /** The step's Skip button, shown left of Continue in the footer; null when the step cannot be skipped. */
    default JButton getSkipButton() { return null; }

    /** Footer text for a step without a Continue button, telling the user how to advance. */
    default String getFooterHint() { return null; }
}
