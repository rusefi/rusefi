package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;

public class DisplacementPanel extends AbstractWizardStep {
    private final UIContext uiContext;
    private final JPanel content = new JPanel(new GridBagLayout());
    private final JTextField displacement = new JTextField(10);
    private final JLabel error = new JLabel(" ");
    private final JButton save = new JButton("Save and Continue");
    private boolean available;

    public DisplacementPanel(UIContext uiContext) {
        super("Engine Displacement", "wizardDisplacement");
        this.uiContext = uiContext;

        JPanel card = new JPanel(new GridBagLayout());
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(WizardStyle.border()),
            BorderFactory.createEmptyBorder(24, 28, 24, 28)));
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = 0;
        c.gridy = 0;
        c.insets = new Insets(8, 8, 8, 8);
        c.anchor = GridBagConstraints.WEST;
        c.fill = GridBagConstraints.HORIZONTAL;
        JLabel title = new JLabel(getTitle());
        styleTitle(title);
        card.add(title, c);
        c.gridy++;
        card.add(new JLabel("<html>engine total displacement.<br>" +
            "This value scales fueling for all cylinders combined.</html>"), c);
        c.gridy++;
        JLabel label = new JLabel("Total displacement (litres)");
        label.setLabelFor(displacement);
        card.add(label, c);
        c.gridy++;
        displacement.setName("displacement");
        scale(displacement, 1.5f);
        card.add(displacement, c);
        c.gridy++;
        card.add(new JLabel("Example: 1998 cc = 1.998 L"), c);
        c.gridy++;
        error.setForeground(Color.RED);
        card.add(error, c);
        c.gridy++;
        stylePrimaryAction(save);
        save.setEnabled(false);
        card.add(save, c);
        content.add(card);

        displacement.getDocument().addDocumentListener(new DocumentListener() {
            @Override public void insertUpdate(DocumentEvent e) { updateValidity(); }
            @Override public void removeUpdate(DocumentEvent e) { updateValidity(); }
            @Override public void changedUpdate(DocumentEvent e) { updateValidity(); }
        });
        save.addActionListener(e -> save());
        displacement.addActionListener(e -> {
            if (save.isEnabled()) {
                save();
            }
        });
    }

    private void updateValidity() {
        String message = validationError(displacement.getText());
        save.setEnabled(available && message == null);
        error.setText(!available ? "Displacement configuration is unavailable." : message == null ? " " : message);
    }

    static String validationError(String text) {
        try {
            double value = Double.parseDouble(text.trim());
            if (Double.isFinite(value) && value > 0 && value <= 65) {
                return null;
            }
        } catch (NumberFormatException ignored) {
        }
        return "Enter a displacement greater than 0 and no more than 65 litres.";
    }

    static WizardStepResult createResult(IniFileModel ini, ConfigurationImage image, String text) {
        String message = validationError(text);
        if (message != null) {
            throw new IllegalArgumentException(message);
        }
        IniField field = ini.findIniField("displacement").orElse(null);
        if (!(field instanceof ScalarIniField)) {
            throw new IllegalArgumentException("Displacement configuration is unavailable.");
        }
        ConfigurationImage modified = image.clone();
        ConfigurationImageGetterSetter.setValue2(field, modified, "displacement", text.trim());
        if (validationError(ConfigurationImageGetterSetter.getStringValue(field, modified)) != null) {
            throw new IllegalArgumentException("Displacement cannot be represented by this configuration.");
        }
        return new WizardStepResult(modified);
    }

    private void save() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) {
            available = false;
            updateValidity();
            return;
        }
        try {
            fireCompleted(createResult(cfg.ini, cfg.image, displacement.getText()));
        } catch (IllegalArgumentException e) {
            error.setText(e.getMessage());
        }
    }

    @Override
    public void onShow() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        IniField field = cfg == null ? null : cfg.ini.findIniField("displacement").orElse(null);
        available = field instanceof ScalarIniField;
        displacement.setText(available ? ConfigurationImageGetterSetter.getStringValue(field, cfg.image) : "");
        displacement.setEnabled(available);
        updateValidity();
    }

    @Override
    public JComponent getPanel() {
        return content;
    }
}
