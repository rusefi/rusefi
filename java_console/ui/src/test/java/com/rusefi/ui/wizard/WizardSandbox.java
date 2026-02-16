package com.rusefi.ui.wizard;

import com.rusefi.core.ui.FrameHelper;

import javax.swing.*;

import static com.rusefi.core.ui.FrameHelper.createFrame;

public class WizardSandbox {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                FrameHelper fh = createFrame("Sand");
                fh.getFrame().setSize(600, 400);
                fh.getFrame().setVisible(true);
            }
        });
    }
}
