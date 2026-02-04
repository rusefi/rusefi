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
        leftPanel.setBorder(BorderFactory.createTitledBorder("Left Panel"));
        leftPanel.add(new JCheckBox("Sample Checkbox"));
        panel.add(leftPanel, new GridBagConstraints(0, 0, 1, 1, 1, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(5, 5, 5, 5), 0, 0));

        JPanel rightPanel = new JPanel();
        rightPanel.setBorder(BorderFactory.createTitledBorder("Right Panel"));
        rightPanel.add(new JButton("Sample Button"));
        panel.add(rightPanel, new GridBagConstraints(1, 0, 1, 1, 1, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(5, 5, 5, 5), 0, 0));

        frameHelper.showFrame(panel);
    }
}
