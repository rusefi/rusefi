package com.rusefi.ui.wizard;

import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;

public class CamTriggerPanel extends AbstractWizardStep {
    private final JPanel content = new JPanel(new GridBagLayout());

    public CamTriggerPanel(UIContext uiContext) {
        super("Cam Trigger", "wizardCamTrigger");

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.anchor = GridBagConstraints.CENTER;
        gbc.insets = new Insets(20, 20, 20, 20);

        JLabel title = new JLabel(getTitle());
        styleTitle(title);
        content.add(title, gbc);

        gbc.gridy++;
        JLabel placeholder = new JLabel("Cam trigger configuration - placeholder");
        scale(placeholder, 1.5f);
        content.add(placeholder, gbc);

        gbc.gridy++;
        JButton skipButton = new JButton("Skip / Use Default");
        stylePrimaryAction(skipButton);
        skipButton.addActionListener(e -> fireCompleted(new WizardStepResult(null, null)));
        content.add(skipButton, gbc);
    }

    @Override
    public JComponent getPanel() {
        return content;
    }
}
