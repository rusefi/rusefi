package com.rusefi.core.ui;

import javax.swing.*;

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
