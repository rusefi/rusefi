package com.rusefi;

import javax.swing.*;

public class Standalone {
    public static void main(String[] args) {
        JFrame frame = new JFrame();
        frame.setSize(800, 500);
        frame.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

        frame.add(new TsPlugin().getPluginPanel());

        frame.setVisible(true);
    }
}
