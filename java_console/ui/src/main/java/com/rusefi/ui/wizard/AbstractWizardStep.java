package com.rusefi.ui.wizard;

import com.opensr5.ini.field.EnumIniField;

import javax.management.ObjectName;
import javax.swing.*;
import java.awt.*;
import java.util.function.Consumer;

/**
 * Common scaffolding for wizard steps: title/flag storage, completion callback wiring,
 * and shared static helpers for font scaling and INI-style quote stripping.
 * <p>
 * Subclasses still own their own root component ({@link #getPanel()}) — some return a
 * plain {@link JPanel}, others return a {@link JLayeredPane}.
 */
public abstract class AbstractWizardStep implements WizardStep {
    protected static final String INVALID_SENTINEL = "INVALID";

    private final String title;
    private final String wizardFlagFieldName;

    protected Consumer<WizardStepResult> onStepCompleted;

    protected AbstractWizardStep(String title, String wizardFlagFieldName) {
        this.title = title;
        this.wizardFlagFieldName = wizardFlagFieldName;
    }

    @Override
    public String getTitle() {
        return title;
    }

    @Override
    public String getWizardFlagFieldName() {
        return wizardFlagFieldName;
    }

    @Override
    public void setOnStepCompleted(Consumer<WizardStepResult> callback) {
        this.onStepCompleted = callback;
    }

    /** Safe no-op when no completion listener is wired up. */
    protected final void fireCompleted(WizardStepResult result) {
        if (onStepCompleted != null) onStepCompleted.accept(result);
    }

    /** Multiplies a component's current font size by {@code factor}. */
    protected static void scale(JComponent c, float factor) {
        Font f = c.getFont();
        c.setFont(f.deriveFont(f.getSize() * factor));
    }

    /** Returns {@code value} with surrounding INI quotes removed, or unchanged if not quoted. */
    protected static String stripQuotes(String value) {
        return EnumIniField.isQuoted(value) ? ObjectName.unquote(value) : value;
    }
}
