package com.rusefi.ui;

import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ui.widgets.MainMenuTreeWidget;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.ui.basic.UiHelper.commonUiStartup;

public class TopLevelMenuSandbox {
    public static void main(String[] args) {
        commonUiStartup();

        

        SwingUtilities.invokeLater(TopLevelMenuSandbox::runAwt);
    }

    private static void runAwt() {
        FrameHelper frameHelper = new FrameHelper(JDialog.EXIT_ON_CLOSE);

        JPanel panel = new JPanel(new GridBagLayout());

        MainMenuTreeWidget left = new MainMenuTreeWidget();

        panel.add(left.getContentPane(), new GridBagConstraints(0, 0, 1, 1, 1, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));

        JPanel right = new JPanel();
        right.setBackground(Color.BLUE);
        panel.add(right, new GridBagConstraints(1, 0, 1, 1, 2, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));

        frameHelper.showFrame(panel);
    }
}
