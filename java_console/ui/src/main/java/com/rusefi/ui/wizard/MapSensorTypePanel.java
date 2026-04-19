package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;

import javax.swing.*;
import java.awt.*;
import java.util.function.Consumer;

public class MapSensorTypePanel implements WizardStep {
    private static final String DIALOG_KEY = "mapSettings";

    private final JPanel content = new JPanel(new BorderLayout());
    private final UIContext uiContext;
    private final CalibrationDialogWidget dialogWidget;
    private final ConfigurationImage[] workingImage = {null};

    private Consumer<WizardStepResult> onStepCompleted;

    public MapSensorTypePanel(UIContext uiContext) {
        this.uiContext = uiContext;
        this.dialogWidget = new CalibrationDialogWidget(uiContext);

        JLabel title = new JLabel("MAP Sensor Settings");
        title.setHorizontalAlignment(SwingConstants.CENTER);
        title.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        scaleComponent(title, 2);
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
        scaleComponent(continueButton, 1.5f);
        continueButton.addActionListener(e -> {
            if (onStepCompleted != null) {
                onStepCompleted.accept(new WizardStepResult(workingImage[0]));
            }
        });

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        buttonPanel.add(continueButton);
        content.add(buttonPanel, BorderLayout.SOUTH);

        // Track edits so the final working image can be burned when Continue is clicked
        dialogWidget.setOnConfigChange(image -> workingImage[0] = image.clone());

        loadDialog();
    }

    private void loadDialog() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (bp == null || ini == null) return;
        ConfigurationImage image = bp.getControllerConfiguration();
        if (image == null) return;

        DialogModel dialog = ini.getDialogs().get(DIALOG_KEY);
        if (dialog == null) return;

        workingImage[0] = image.clone();
        dialogWidget.update(dialog, ini, image);
    }

    @Override
    public void onShow() {
        // Reload against the latest ECU image — earlier wizard steps may have burned changes
        loadDialog();
    }

    @Override
    public String getTitle() {
        return "MAP Sensor Settings";
    }

    @Override
    public JComponent getPanel() {
        return content;
    }

    @Override
    public String getWizardFlagFieldName() {
        return "wizardMapSensorType";
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
