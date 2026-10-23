package com.rusefi.ui.wizard;

import com.formdev.flatlaf.FlatClientProperties;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.OrdinalOutOfRangeException;

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
    static final String CONTINUE_TEXT = "Continue";
    static final String SKIP_TEXT = "Skip";

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

    protected static void styleTitle(JLabel label) {
        label.setForeground(WizardStyle.text());
        label.setFont(label.getFont().deriveFont(Font.BOLD, label.getFont().getSize() * 1.6f));
        label.setBorder(BorderFactory.createEmptyBorder(
            WizardStyle.LARGE_GAP, WizardStyle.GAP, WizardStyle.LARGE_GAP, WizardStyle.GAP));
    }

    protected static void styleButton(AbstractButton button) {
        button.putClientProperty("JButton.buttonType", "square");
    }

    protected static void stylePrimaryAction(JButton button) {
        button.setMargin(new Insets(9, 18, 9, 18));
        button.setFont(button.getFont().deriveFont(Font.BOLD));
    }

    /** Neutral button in the wizard footer, e.g. Skip or Exit Wizard; footer buttons share one size. */
    static void styleFooterAction(JButton button) {
        button.setMargin(new Insets(7, 16, 7, 16));
    }

    /** The footer's Continue: the console's blue primary button, worded the same on every step. */
    protected static void styleContinueAction(JButton button) {
        button.setText(CONTINUE_TEXT);
        styleFooterAction(button);
        button.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        button.setIcon(WizardStyle.ActionIcon.NEXT);
        button.setHorizontalTextPosition(SwingConstants.LEADING);
    }

    /**
     * A choice that saves and advances on a single click, e.g. a cylinder count: outlined in the accent
     * colour so it reads as an action. The successor of #10481's green confirmation for these buttons.
     */
    protected static void styleChoiceAction(JButton button) {
        stylePrimaryAction(button);
        button.putClientProperty(FlatClientProperties.STYLE_CLASS, "wizardChoice");
    }

    /** A choice the wizard cannot save (it only explains why), greyed out but still clickable. */
    protected static void styleUnsupportedChoice(JButton button, String tooltip) {
        stylePrimaryAction(button);
        button.setFont(button.getFont().deriveFont(Font.PLAIN));
        button.putClientProperty(FlatClientProperties.STYLE_CLASS, "wizardChoiceMuted");
        button.setToolTipText(tooltip);
    }

    protected static void styleSkipAction(JButton button, String tooltip) {
        button.setText(SKIP_TEXT);
        button.setToolTipText(tooltip);
        styleFooterAction(button);
    }

    protected static void styleConfirmAction(JButton button) {
        stylePrimaryAction(button);
        button.putClientProperty(FlatClientProperties.STYLE_CLASS, "wizardConfirm");
        button.setIcon(WizardStyle.ActionIcon.CONFIRM);
    }

    protected static void styleNegativeAction(JButton button) {
        stylePrimaryAction(button);
        button.putClientProperty(FlatClientProperties.STYLE_CLASS, "wizardNegative");
        button.setIcon(WizardStyle.ActionIcon.CANCEL);
    }

    /** Returns {@code value} with surrounding INI quotes removed, or unchanged if not quoted. */
    protected static String stripQuotes(String value) {
        return EnumIniField.isQuoted(value) ? ObjectName.unquote(value) : value;
    }

    /** Returns null when a persisted enum value is not exposed by the current board's INI. */
    protected static String readValue(IniField field, ConfigurationImage image) {
        try {
            return stripQuotes(ConfigurationImageGetterSetter.getStringValue(field, image));
        } catch (OrdinalOutOfRangeException e) {
            return null;
        }
    }
}
