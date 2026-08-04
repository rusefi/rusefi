package com.rusefi.ui;

import javax.swing.*;

/**
 * Manual Swing sandbox showing {@link SlcanTab} in a standalone frame, no console needed.
 * Requires a rusEFI board exposing the CAN sniffer SLCAN channel as a secondary USB CDC port.
 *
 * @see com.rusefi.binaryprotocol.test.SlcanSandbox console variant
 */
public class SlcanUiSandbox {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            JFrame frame = new JFrame("SLCAN Sandbox");
            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            frame.getContentPane().add(new SlcanTab().getContent());
            frame.setSize(900, 600);
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
        });
    }
}
