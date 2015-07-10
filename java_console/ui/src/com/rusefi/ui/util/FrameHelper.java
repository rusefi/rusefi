package com.rusefi.ui.util;

import com.rusefi.FileLog;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

/**
 * Date: 3/24/13
 * (c) Andrey Belomutskiy
 */
public class FrameHelper {
    protected final JFrame frame = new JFrame();
    public static int defaultFontSize;

    public JFrame getFrame() {
        return frame;
    }

    public void showFrame(JComponent component) {
        showFrame(component, true);
    }

    public void showFrame(JComponent component, final boolean maximizeOnStart) {
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
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowOpened(WindowEvent e) {
                defaultFontSize = frame.getFont().getSize();
            }
        });
        frame.setVisible(true);
    }

    protected void onWindowOpened() {
        FileLog.MAIN.logLine("onWindowOpened");
    }

    protected void onWindowClosed() {
        FileLog.MAIN.logLine("onWindowClosed");
        FileLog.MAIN.close();
    }

    public int getDefaultFontSize() {
        return defaultFontSize;
    }
}
