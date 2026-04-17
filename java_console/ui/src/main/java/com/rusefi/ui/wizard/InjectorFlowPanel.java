package com.rusefi.ui.wizard;

import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.util.function.Consumer;

public class InjectorFlowPanel implements WizardStep {
    private final JPanel content = new JPanel(new GridBagLayout());
    private Consumer<WizardStepResult> onStepCompleted;

    public InjectorFlowPanel(UIContext uiContext) {
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.anchor = GridBagConstraints.CENTER;
        gbc.insets = new Insets(20, 20, 20, 20);

        JLabel title = new JLabel("Injector Flow");
        scaleComponent(title, 3);
        content.add(title, gbc);

        gbc.gridy++;
        JLabel placeholder = new JLabel("Injector flow configuration");
        scaleComponent(placeholder, 1.5f);
        content.add(placeholder, gbc);

        gbc.gridy++;
        JButton skipButton = new JButton("Skip / Use Default");
        scaleComponent(skipButton, 2);
        skipButton.addActionListener(e -> {
            if (onStepCompleted != null) {
                onStepCompleted.accept(new WizardStepResult(null, null));
            }
        });
        content.add(skipButton, gbc);
    }

    @Override
    public String getTitle() {
        return "Injector Flow";
    }

    @Override
    public JComponent getPanel() {
        return content;
    }

    @Override
    public String getWizardFlagFieldName() {
        return "wizardInjectorFlow";
    }

    @Override
    public void setOnStepCompleted(Consumer<WizardStepResult> callback) {
        this.onStepCompleted = callback;
    }

    private void scaleComponent(JComponent c, float factor) {
        Font f = c.getFont();
        c.setFont(f.deriveFont(f.getSize() * factor));
    }
}
