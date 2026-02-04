package com.rusefi.ui.laf;

import com.rusefi.core.ui.FrameHelper;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.ui.basic.UiHelper.commonUiStartup;

public class LafSandbox {
    public static void main(String[] args) {
        commonUiStartup();

        SwingUtilities.invokeLater(() -> runAwt());
    }

    private static void runAwt() {
        FrameHelper frameHelper = new FrameHelper(JDialog.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new GridBagLayout());

        JPanel leftPanel = new JPanel();
        GradientTitleBorder.installBorder("Left Panel", leftPanel);
        leftPanel.add(new JCheckBox("Sample Checkbox"));
        JCheckBox disabledCheckBox = new JCheckBox("Disabled Checkbox");
        disabledCheckBox.setEnabled(false);
        leftPanel.add(disabledCheckBox);
        panel.add(leftPanel, new GridBagConstraints(0, 0, 1, 1, 1, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(5, 5, 5, 5), 0, 0));

        JPanel rightPanel = new JPanel();
        GradientTitleBorder.installBorder("Right Panel", rightPanel);
        rightPanel.add(new JButton("Sample Button"));
        rightPanel.add(new JComboBox<>(new String[]{"Enabled Combo"}));
        JComboBox<String> disabledCombo = new JComboBox<>(new String[]{"Disabled Combo"});
        disabledCombo.setEnabled(false);
        rightPanel.add(disabledCombo);
        panel.add(rightPanel, new GridBagConstraints(1, 0, 1, 1, 1, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(5, 5, 5, 5), 0, 0));

        frameHelper.showFrame(panel);
    }
}
