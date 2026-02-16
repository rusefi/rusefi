package com.rusefi.ui.wizard;

import com.formdev.flatlaf.FlatDarkLaf;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ui.basic.UiHelper;

import javax.swing.*;
import java.awt.*;

import static com.rusefi.core.ui.FrameHelper.createFrame;

public class WizardSandbox {
    public static void main(String[] args) {
        UiHelper.configureLaf();

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                FrameHelper fh = createFrame("Wizard Sandbox");

                JPanel content = new JPanel(new BorderLayout());
                content.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));
                content.add(new JLabel("This is a modern Wizard Sandbox"), BorderLayout.NORTH);

                JPanel buttons = new JPanel();
                buttons.add(new JButton("Previous"));
                buttons.add(new JButton("Next"));
                buttons.add(new JButton("Finish"));
                content.add(buttons, BorderLayout.SOUTH);

                fh.getFrame().add(content);

                fh.getFrame().setSize(800, 600);
                fh.getFrame().setLocationRelativeTo(null);
                fh.getFrame().setVisible(true);
            }
        });
    }
}
