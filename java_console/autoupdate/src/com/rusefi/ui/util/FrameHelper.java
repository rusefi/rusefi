package com.rusefi.ui.util;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

/**
 * Date: 3/24/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class FrameHelper {
    private final JFrame frame = new JFrame();

    public JFrame getFrame() {
        return frame;
    }

    public void showFrame(JComponent component) {
        showFrame(component, true);
    }

    public void showFrame(JComponent component, final boolean maximizeOnStart) {
        initFrame(component, maximizeOnStart);
        frame.setVisible(true);
    }

    public void initFrame(JComponent component, final boolean maximizeOnStart) {
        frame.setSize(800, 500);
        frame.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowOpened(WindowEvent e) {
                onWindowOpened();
                if (maximizeOnStart)
                    frame.setExtendedState(frame.getExtendedState() | JFrame.MAXIMIZED_BOTH);
            }

            @Override
            public void windowClosed(WindowEvent ev) {
                onWindowClosed();
            }
        });
        frame.add(component);
    }

    protected void onWindowOpened() {
    }

    protected void onWindowClosed() {
    }
}
