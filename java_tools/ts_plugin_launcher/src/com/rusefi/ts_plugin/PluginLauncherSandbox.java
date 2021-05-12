package com.rusefi.ts_plugin;

import javax.swing.*;

/**
 * Sandbox for {@link TsPluginLauncher}
 */
public class PluginLauncherSandbox {
    public static void main(String[] args) {
        JFrame frame = new JFrame();
        frame.setSize(800, 500);
        frame.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

        frame.add(new TsPluginLauncher().getPluginPanel());

        frame.setVisible(true);
    }
}
