package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.ScrollablePanel;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;

import javax.swing.*;
import java.awt.*;

public class CamTriggerPanel extends AbstractWizardStep {
    private static final int CARD_WIDTH = 960;
    private static final String DIALOG_KEY = "trigger_cams";

    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel card = new JPanel(new GridBagLayout()) {
        @Override
        public Dimension getPreferredSize() {
            Dimension preferred = super.getPreferredSize();
            preferred.width = CARD_WIDTH;
            return preferred;
        }

        @Override
        public Dimension getMaximumSize() {
            Dimension preferred = super.getPreferredSize();
            return new Dimension(CARD_WIDTH, preferred.height);
        }
    };
    private final UIContext uiContext;
    private final JButton saveButton = new JButton();
    private final JButton skipButton = new JButton();
    private final CalibrationDialogWidget dialogWidget;
    private ConfigurationImage workingImage;

    public CamTriggerPanel(UIContext uiContext) {
        super("Cam Trigger", "wizardCamTrigger");
        this.uiContext = uiContext;
        dialogWidget = new CalibrationDialogWidget(uiContext);
        dialogWidget.setFieldRowVerticalMargin(3);
        dialogWidget.getContentPane().setBorder(BorderFactory.createEmptyBorder(8, 8, 8, 8));

        WizardStyle.styleCard(card, 22, 26);

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.WEST;
        gbc.insets = new Insets(0, 0, 5, 0);

        JLabel eyebrow = WizardStyle.sectionLabel("CAM TRIGGER SETUP");
        card.add(eyebrow, gbc);

        gbc.gridy++;
        JLabel title = WizardStyle.cardTitle(getTitle());
        card.add(title, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 18, 0);
        card.add(WizardStyle.description("Configure the first cam/VVT input and verify its wheel pattern."), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 16, 0);
        card.add(dialogWidget.getContentPane(), gbc);

        styleContinueAction(saveButton);
        saveButton.addActionListener(e -> {
            if (workingImage != null) {
                fireCompleted(new WizardStepResult(workingImage.clone()));
            }
        });
        styleSkipAction(skipButton, "Keep the current trigger settings and continue");
        skipButton.addActionListener(e -> fireCompleted(new WizardStepResult(null, null)));

        JPanel wrapper = new ScrollablePanel();
        wrapper.setLayout(new BoxLayout(wrapper, BoxLayout.Y_AXIS));
        wrapper.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        card.setAlignmentX(Component.CENTER_ALIGNMENT);
        wrapper.add(card);
        content.add(WizardStyle.pageScrollPane(wrapper), BorderLayout.CENTER);

        dialogWidget.setOnConfigChange(image -> workingImage = image.clone());
    }

    @Override
    public void onShow() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        workingImage = null;
        if (cfg == null) {
            dialogWidget.reset();
            return;
        }
        DialogModel dialog = cfg.ini.getDialogs().get(DIALOG_KEY);
        if (dialog == null) {
            dialogWidget.reset();
            return;
        }
        workingImage = cfg.image.clone();
        dialogWidget.update(dialog, cfg.ini, cfg.image);
    }

    @Override
    public JButton getContinueButton() {
        return saveButton;
    }

    @Override
    public JButton getSkipButton() {
        return skipButton;
    }

    @Override
    public JComponent getPanel() {
        return content;
    }
}
