package com.rusefi.ui.wizard;

/**
 * Result from a wizard step: the config field to set and its value.
 * For placeholder steps that don't set an actual config field,
 * configFieldName and value may be null.
 */
public class WizardStepResult {
    public final String configFieldName;
    public final String value;

    public WizardStepResult(String configFieldName, String value) {
        this.configFieldName = configFieldName;
        this.value = value;
    }
}
