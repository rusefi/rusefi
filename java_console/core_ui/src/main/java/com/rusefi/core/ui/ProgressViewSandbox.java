package com.rusefi.core.ui;

import javax.swing.*;

/**
 * Visual sandbox that pops up a {@link ProgressView} dialog.
 */
public class ProgressViewSandbox {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                ProgressView.create("test");
            }
        });
    }
}
