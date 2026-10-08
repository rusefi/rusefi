package com.rusefi.ui;

import com.rusefi.io.LinkManager;
import com.rusefi.ui.llm.LLMTab;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.nio.file.Path;
import java.nio.file.Paths;

/** Auth/UI development launcher. Live troubleshooting uses the connected Console's tab. */
public final class LLMTabSandbox {
    public static void main(String[] args) {
        Path storage = args.length == 0 ? Paths.get(System.getProperty("user.home"), ".rusefi", "llm-access") : Paths.get(args[0]);
        SwingUtilities.invokeLater(() -> {
            LLMTab tab = new LLMTab(storage, new LinkManager());
            JFrame frame = new JFrame("rusEFI - Troubleshooting Sandbox");
            frame.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
            frame.addWindowListener(new WindowAdapter() {
                @Override public void windowClosed(WindowEvent e) { tab.close(); }
            });
            frame.setContentPane(tab.getContent());
            frame.pack();
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
        });
    }
}
