package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;

import javax.swing.*;
import java.awt.*;

public class MapSensorTypePanel extends AbstractWizardStep {
    private static final String DIALOG_KEY = "mapSettings";

    private final JPanel content = new JPanel(new BorderLayout());
    private final UIContext uiContext;
    private final CalibrationDialogWidget dialogWidget;
    private final ConfigurationImage[] workingImage = {null};

    public MapSensorTypePanel(UIContext uiContext) {
        super("MAP Sensor Settings", "wizardMapSensorType");
        this.uiContext = uiContext;
        this.dialogWidget = new CalibrationDialogWidget(uiContext);

        JLabel title = new JLabel(getTitle());
        title.setHorizontalAlignment(SwingConstants.CENTER);
        title.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        scale(title, 2);
        content.add(title, BorderLayout.NORTH);

        // Wrap the dialog content so it takes its preferred size and centers horizontally
        // instead of having West/East panels pushed to the window edges.
        JPanel centerWrapper = new JPanel(new GridBagLayout());
        GridBagConstraints cgbc = new GridBagConstraints();
        cgbc.anchor = GridBagConstraints.CENTER;
        cgbc.fill = GridBagConstraints.NONE;
        centerWrapper.add(dialogWidget.getContentPane(), cgbc);
        content.add(new JScrollPane(centerWrapper), BorderLayout.CENTER);

        JButton continueButton = new JButton("Continue");
        scale(continueButton, 1.5f);
        continueButton.addActionListener(e -> fireCompleted(new WizardStepResult(workingImage[0])));

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        buttonPanel.add(continueButton);
        content.add(buttonPanel, BorderLayout.SOUTH);

        // Track edits so the final working image can be burned when Continue is clicked
        dialogWidget.setOnConfigChange(image -> workingImage[0] = image.clone());

        loadDialog();
    }

    private void loadDialog() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) return;

        DialogModel dialog = cfg.ini.getDialogs().get(DIALOG_KEY);
        if (dialog == null) return;

        workingImage[0] = cfg.image.clone();
        dialogWidget.update(dialog, cfg.ini, cfg.image);
    }

    @Override
    public void onShow() {
        // Reload against the latest ECU image — earlier wizard steps may have burned changes
        loadDialog();
    }

    @Override
    public JComponent getPanel() {
        return content;
    }
}
