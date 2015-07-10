package com.rusefi.ui;

import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;

/**
 * Create(c) Andrey Belomutskiy 2013-2015
 * 3/7/2015
 */
public class StatusWindow {
    private final JTextArea log = new JTextArea();
    private final JScrollPane messagesScroll = new JScrollPane(log, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
        @Override
        public Dimension getPreferredSize() {
            return new Dimension(400, 400);
        }
    };

    public StatusWindow() {
        log.setLineWrap(true);
    }

    public void showFrame(String title) {
        FrameHelper f = new FrameHelper();
        f.getFrame().setTitle(title);
        f.showFrame(messagesScroll, false);
        UiUtils.centerWindow(f.getFrame());
        log.setText(""); // let's remove stuff from previous invocation
    }

    public void appendMsg(final String s) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                log.append(s + "\r\n");
                UiUtils.trueLayout(log);
            }
        });
    }
}
