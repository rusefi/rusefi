package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;

/**
 * Result from a wizard step.
 * <p>
 * Steps may report their result in two ways:
 * <ul>
 *   <li>A single field edit: set configFieldName + value</li>
 *   <li>A full modified image (e.g., multi-field dialogs): set modifiedImage</li>
 * </ul>
 * For placeholder steps with no config changes, all fields may be null.
 */
public class WizardStepResult {
    public final String configFieldName;
    public final String value;
    public final ConfigurationImage modifiedImage;

    public WizardStepResult(String configFieldName, String value) {
        this(configFieldName, value, null);
    }

    public WizardStepResult(ConfigurationImage modifiedImage) {
        this(null, null, modifiedImage);
    }

    public WizardStepResult(String configFieldName, String value, ConfigurationImage modifiedImage) {
        this.configFieldName = configFieldName;
        this.value = value;
        this.modifiedImage = modifiedImage;
    }
}
