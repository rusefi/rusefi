package com.rusefi.ui;

import com.irnems.FileLog;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

/**
 * Date: 3/24/13
 * (c) Andrey Belomutskiy
 */
public class FrameHelper {
    protected final JFrame frame = new JFrame();

    protected void showFrame(JComponent container) {
        frame.setSize(800, 500);
        frame.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowOpened(WindowEvent e) {
                onWindowOpened();
                frame.setExtendedState(frame.getExtendedState() | JFrame.MAXIMIZED_BOTH);
            }

            @Override
            public void windowClosed(WindowEvent ev) {
                onWindowClosed();
            }
        });
        frame.add(container);
        frame.setVisible(true);
    }

    protected void onWindowOpened() {
        FileLog.rlog("onWindowOpened");
    }

    protected void onWindowClosed() {
        FileLog.rlog("onWindowClosed");
        FileLog.MAIN.close();
    }
}
