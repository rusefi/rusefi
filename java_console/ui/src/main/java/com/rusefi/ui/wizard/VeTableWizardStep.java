package com.rusefi.ui.wizard;

import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.tune.VeTableGeneratorPanel;

import javax.swing.*;
import java.awt.*;

/**
 * Wizard step that embeds the VE table generator.
 * Apply fires step completion with the patched image; Skip fires it with no image change.
 * Uses wizardDisplacement as the progress flag — silently skipped on older firmware.
 */
public class VeTableWizardStep extends AbstractWizardStep {
    private final UIContext uiContext;
    private final JPanel panel;

    public VeTableWizardStep(UIContext uiContext) {
        super("Base VE Table", "wizardDisplacement");
        this.uiContext = uiContext;
        panel = new JPanel(new BorderLayout());
        panel.add(new JLabel("Loading...", SwingConstants.CENTER), BorderLayout.CENTER);
    }

    @Override
    public void onShow() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) {
            panel.removeAll();
            panel.add(new JLabel("ECU not connected — click Skip to continue.", SwingConstants.CENTER),
                BorderLayout.CENTER);
            panel.revalidate();
            panel.repaint();
            return;
        }

        // ponytail: one-element array so both lambdas share a mutable fired flag in the closure
        final boolean[] fired = {false};

        VeTableGeneratorPanel inner = new VeTableGeneratorPanel(
            cfg.ini,
            cfg.image,
            patched -> { if (!fired[0]) { fired[0] = true; fireCompleted(new WizardStepResult(patched)); } },
            ()      -> { if (!fired[0]) { fired[0] = true; fireCompleted(new WizardStepResult(null, null)); } },
            "Apply and Continue",
            "Skip"
        );

        panel.removeAll();
        panel.add(inner, BorderLayout.CENTER);
        panel.revalidate();
        panel.repaint();
    }

    @Override
    public JComponent getPanel() {
        return panel;
    }
}
